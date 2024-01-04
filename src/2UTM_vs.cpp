#define _CRT_SECURE_NO_WARNINGS

#include "framework.h"
#include "2UTM_vs.h"
#include "ControlReaders.h"
#include "ControlService.h"

#define MAX_LOADSTRING 100

///////////////////////////////////////////////////////////////////////////////////////
// Создание дампа в случае аварийного завершения
#include "minidumpapiset.h"
#pragma comment(lib,"Dbghelp.lib")

void CreateDump(_EXCEPTION_POINTERS* ExceptionInfo)
{

    char FileName[32];
    SYSTEMTIME LocalTime;    GetLocalTime(&LocalTime);
    sprintf_s(FileName, _countof(FileName),
        "Memory %04hu.%02hu.%02hu %02hu %02hu %02hu.dmp",
        LocalTime.wYear, LocalTime.wMonth, LocalTime.wDay,
        LocalTime.wHour, LocalTime.wMinute, LocalTime.wSecond);

    HANDLE hFile = CreateFileA(FileName, GENERIC_WRITE, FILE_SHARE_READ, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
    if (INVALID_HANDLE_VALUE != hFile)
    {
        MINIDUMP_EXCEPTION_INFORMATION ExInfo;
        ExInfo.ThreadId = GetCurrentThreadId();
        ExInfo.ExceptionPointers = ExceptionInfo;
        ExInfo.ClientPointers = 0;
        MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hFile, MiniDumpWithFullMemory, &ExInfo, 0, 0);
        CloseHandle(hFile);
    }
}

LONG WINAPI MyUnhandledExceptionFilter(_EXCEPTION_POINTERS* ExceptionInfo)
{
    CreateDump(ExceptionInfo);
    return EXCEPTION_CONTINUE_SEARCH;
}

class INITILIZE
{
public:
    INITILIZE()
    {
        SetUnhandledExceptionFilter(MyUnhandledExceptionFilter);
    }
} Initilize;
///////////////////////////////////////////////////////////////////////////////////////

extern std::string pathExe;
extern bool flagInstallUTM;
extern HANDLE evt;

// О программе
std::string about = 
"Заметьте, что у программы нет никакой лицензии, то есть это, по сути, экспериментальное ПО. "
"Cтабильность никто не гарантирует, если вам кто то сказал, что все работает стабильно, можете пнуть его ногой в живот.\n"
"Так же вы можете использовать это ПО, изменять и внедрять где и куда угодно, но на свой страх и риск.\n"
"\n"
"У УТМ есть еще два служебных порта - 8192 и 8194. Программа меняет их для каждого УТМ - первый уменьшает на 1, второй увеличивает на 1. "
"Если вы хотите поменять порты для веб интерфейса, то учтите, что при установке 10 УТМ порты 8183 - 8192 и 8194 - 8203 будут заняты.\n"
"\n"
"Программа не контролирует работу УТМ, она создает условия, чтобы несколько УТМ смогли работать.\n"
"Ограничение в 10 УТМ, потому что в ОС Windows, начиная с Windows 8, одновременно может быть только 10 ридеров для смарт карт.\n"
"\n"
"Что нового в версии 1.2.1:\n"
"1. Исправлена ошибка выделения токена в листбоксе.\n"
"2. Исправлена ошибка заполнения листбокса.\n"
"3. Исправлена ошибка обращения к несуществующим токенам при установке УТМ, если токены отвалилсь, то окно установки обновляется с новыми данными.\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"Разработчик: Королев Сергей.\n"
"Контакты:\n"
"GitHub - https://github.com/2UTM/2UTM/issues \n"
"Почта - 2UTM.dev@gmail.com \n"
"Telegram - @regnorr \n";

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
CHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
CHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна
DWORD dwMinorVersionOS;                         // версия ос
int countUTM = 0;                               // кол во УТМ
int indexToken = -1;                             // индекс выделенного токена, чтобы не выделять одно и тоже
HANDLE handleThrInstallUTM;

// Хендлы виджетов
HWND hWndMain, hListBoxTokensGlobal, hEditInfoTokens, hStatusBar, hProgressBar, hDlgReadersNoContext, hDlgInstallUTM;

// создание векторов с ид виджетов в окне установить утм
std::vector<int> vecStaticUTM = { IDC_STATIC3, IDC_STATIC4, IDC_STATIC5, IDC_STATIC6, IDC_STATIC7, IDC_STATIC8,
IDC_STATIC9, IDC_STATIC10, IDC_STATIC11, IDC_STATIC12 };
std::vector<int> vecComboBoxToken = { IDC_COMBO_UTM1 ,IDC_COMBO_UTM2, IDC_COMBO_UTM3, IDC_COMBO_UTM4, IDC_COMBO_UTM5,
IDC_COMBO_UTM6, IDC_COMBO_UTM7, IDC_COMBO_UTM8, IDC_COMBO_UTM9, IDC_COMBO_UTM10 };
std::vector<int> vecEditPort = { IDC_EDIT_PORT1, IDC_EDIT_PORT2, IDC_EDIT_PORT3, IDC_EDIT_PORT4, IDC_EDIT_PORT5,
IDC_EDIT_PORT6, IDC_EDIT_PORT7, IDC_EDIT_PORT8, IDC_EDIT_PORT9, IDC_EDIT_PORT10 };

// Вектора с ридерами, атрибутами и серийными номера
std::vector<std::string> vecRutokens, vecAttrRutokens, vecRutokensNoContext, vecRutokenSerialNumber;

// Контекст смарт карт
SCARDCONTEXT ctxCard;

// GUID устройства WUDF для подписки на событие WM_DEVICECHANGE
GUID GuidWUDF = { 0x50dd5230, 0xba8a, 0x11d1, 0xbf,0x5d,0x00,0x00,0xf8,0x05,0xf5,0x30};

// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    WndInstallUTM(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK    EditCountInstallUTMProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    WndNoContextProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // Инициализация глобальных строк
    LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadString(hInstance, IDC_MY2UTMVS, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY2UTMVS));

    MSG msg;

    // Цикл основного сообщения:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  ФУНКЦИЯ: MyRegisterClass()
//
//  ЦЕЛЬ: Регистрирует класс окна.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY2UTMVS));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW);
    wcex.lpszMenuName   = MAKEINTRESOURCE(IDC_MY2UTMVS);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassEx(&wcex);
}

//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   ЦЕЛЬ: Сохраняет маркер экземпляра и создает главное окно
//
//   КОММЕНТАРИИ:
//
//        В этой функции маркер экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    if (openLogFile())
    {
        MessageBoxA(hWndMain, ("Не удалось открыть лог файл! Код ошибки - " + std::to_string(GetLastError())).c_str(), "Ошибка", MB_ICONERROR);
        return FALSE;
    }

    hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

    // Расположение окна
    int x = (GetSystemMetrics(SM_CXSCREEN) >> 1) - (500 >> 1) - 200;
    int y = (GetSystemMetrics(SM_CYSCREEN) >> 1) - (200 >> 1) - 200;

    // Получаем версию ос
    getVersionOS(dwMinorVersionOS);

    hWndMain = CreateWindowEx(0, szWindowClass, szTitle, WS_SYSMENU, x, y, 390, 560, nullptr, nullptr, hInstance, nullptr);
    HWND hSeparator;

    // Если минор равен 1, это вин 7
    if (dwMinorVersionOS == 1)
    {
        // Создаем листбокс с токенами
        hListBoxTokensGlobal = CreateWindowEx(0, "LISTBOX", "Tokens ListBox", WS_CHILD | LBS_STANDARD | WS_VISIBLE,
            1, 1, 381, 240, hWndMain, (HMENU)MainWidgetId::LISTBOX_TOKENS, NULL, NULL);

        // Сепаратор
        hSeparator = CreateWindowEx(0, "Static", "", WS_CHILD | WS_VISIBLE | SS_ETCHEDHORZ, 1, 235, 381, 0, hWndMain, NULL, NULL, NULL);

        // Поле для информации о токене
        hEditInfoTokens = CreateWindowEx(0, "Edit", 0, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY | ES_MULTILINE | WS_VSCROLL, 1, 245, 381, 250, hWndMain, NULL, NULL, NULL);

        // Статусбар
        hStatusBar = CreateWindowEx(0, "Static", " ", WS_CHILD | WS_VISIBLE | SS_LEFT, 1, 498, 381, 20, hWndMain, NULL, NULL, NULL);

        // Прогрессбар
        hProgressBar = CreateWindowEx(0, PROGRESS_CLASS, NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | PBS_MARQUEE, 6, 215, 370, 30, hWndMain, NULL, NULL, NULL);
    }
    else // иначе вин 10
    {
        // Создаем листбокс с токенами
        hListBoxTokensGlobal = CreateWindowEx(0, "LISTBOX", "Tokens ListBox", WS_CHILD | LBS_STANDARD | WS_VISIBLE,
            1, 1, 371, 240, hWndMain, (HMENU)MainWidgetId::LISTBOX_TOKENS, NULL, NULL);

        // Сепаратор
        hSeparator = CreateWindowEx(0, "Static", "", WS_CHILD | WS_VISIBLE | SS_ETCHEDHORZ, 1, 235, 371, 0, hWndMain, NULL, NULL, NULL);

        // Поле для информации о токене
        hEditInfoTokens = CreateWindowEx(0, "Edit", 0, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY | ES_MULTILINE | WS_VSCROLL, 1, 245, 371, 238, hWndMain, NULL, NULL, NULL);

        // Статусбар
        hStatusBar = CreateWindowEx(0, "Static", " ", WS_CHILD | WS_VISIBLE | SS_LEFT, 1, 484, 371, 20, hWndMain, NULL, NULL, NULL);

        // Прогрессбар
        hProgressBar = CreateWindowEx(0, PROGRESS_CLASS, NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | PBS_MARQUEE, 6, 215, 360, 30, hWndMain, NULL, NULL, NULL);
    }

    if (!hWndMain || !hListBoxTokensGlobal || !hEditInfoTokens || !hSeparator || !hStatusBar || !hProgressBar)
    {
        logger("Не удалось создать окно!", "ERROR");
        return FALSE;
    }

    ShowWindow(hProgressBar, SW_HIDE);

    // Установка шрифта
    HFONT font = CreateFont(
        16,						// nHeight
        0,						// nWidth
        0,						// nEscapement
        0,						// nOrientation
        FW_NORMAL,				// nWeight
        FALSE,					// bItalic
        FALSE,					// bUnderline
        FALSE,					// cStrikeOut
        DEFAULT_CHARSET,		    // nCharSet
        OUT_DEFAULT_PRECIS,		// nOutPrecision
        CLIP_DEFAULT_PRECIS,	    // nClipPrecision
        DEFAULT_QUALITY,		    // nQuality
        VARIABLE_PITCH,			// nPitchAndFamily
        "Tahoma");				// lpszFacename

    SendMessage(hWndMain, WM_SETFONT, (WPARAM)font, TRUE);
    SendMessage(hListBoxTokensGlobal, WM_SETFONT, (WPARAM)font, TRUE);
    SendMessage(hEditInfoTokens, WM_SETFONT, (WPARAM)font, TRUE);
    SendMessage(hStatusBar, WM_SETFONT, (WPARAM)font, TRUE);

    // Сбор данных
    workCollectDevice();

    ShowWindow(hWndMain, SW_SHOW);
    UpdateWindow(hWndMain);
    
    return TRUE;
}

//
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ЦЕЛЬ: Обрабатывает сообщения в главном окне.
//
//  WM_COMMAND  - обработать меню приложения
//  WM_PAINT    - Отрисовка главного окна
//  WM_DESTROY  - отправить сообщение о выходе и вернуться
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    // Скрываем каретку в полях ввода
    HideCaret(hEditInfoTokens);

    switch (message)
    {
        case WM_CREATE:
        {
            DEV_BROADCAST_DEVICEINTERFACE Flt = { 0 };
            ZeroMemory(&Flt, sizeof(Flt));
            Flt.dbcc_size = sizeof(DEV_BROADCAST_DEVICEINTERFACE);
            Flt.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;
            Flt.dbcc_classguid = GuidWUDF; // указываем, какое устройство нужно отслеживать

            HDEVNOTIFY g_hNotify = RegisterDeviceNotificationA(hWnd, &Flt, DEVICE_NOTIFY_WINDOW_HANDLE);
            if (!g_hNotify)
            {
                std::string error = "Не удалось подписатья на событие! Код ошибки " + std::to_string(GetLastError());
                logger(error, "ERROR");
                exit(1);
            }
        }
        case WM_DEVICECHANGE: // событие изменения устройств
        {
            PDEV_BROADCAST_DEVICEINTERFACE b = (PDEV_BROADCAST_DEVICEINTERFACE)lParam;
            switch (wParam)
            {
                // события подключение и отключения устройств
                case DBT_DEVICEARRIVAL:
                case DBT_DEVICEREMOVECOMPLETE:
                {
                    Sleep(500); // чтобы успела система определить

                    // Открываем прогрессбар и блокируем главное окно
                    ShowWindow(hProgressBar, SW_SHOW);
                    SendMessage(hProgressBar, (UINT)PBM_SETMARQUEE, (WPARAM)1, NULL);
                    EnableWindow(hWnd, FALSE);

                    // Сбор данных
                    setStatusBar("Выполняется обновление устройств...");
                    std::thread thr(&workCollectDevice);
                    thr.detach();
                    SetWindowTextA(hEditInfoTokens, ""); // очищаем поле информации
                    indexToken = -1; // сбрасываем выделение токена
                }
                break;
            }
        }
        break;
        case WM_CTLCOLORSTATIC: // чтобы не активные поля не закрашивалось серым
        {
            if ((HWND)lParam == hEditInfoTokens)
            {
                return (LRESULT)GetSysColorBrush(COLOR_WINDOW);
            }
            if ((HWND)lParam == hStatusBar)
            {
                SetBkMode((HDC)wParam, TRANSPARENT);
                return (LONG)CreateSolidBrush(GetSysColor(COLOR_3DFACE));
            }
        }
        break;
        case WM_COMMAND:
        {
            // События листбокса
            switch ((MainWidgetId)LOWORD(wParam))
            {
                case MainWidgetId::LISTBOX_TOKENS:
                {
                    if (HIWORD(wParam) == LBN_SELCHANGE) // если выбрали токен
                    {
                        // Получаем индекс токена
                        int index = SendMessage(hListBoxTokensGlobal, LB_GETCURSEL, 0, 0); // получаем индекс выделенной строки
                        if (indexToken == index) // если выделение одно и того же токена, не обрабатываем дальше
                        {
                            break;
                        }
                        indexToken = index;
                        if (index == LB_ERR)
                        {
                            setStatusBar("Не удалось выделить токен!");
                            logger("Не удалось выделить токен!", "ERROR");
                            break;
                        }

                        // Открываем прогрессбар и блокируем главное окно
                        ShowWindow(hProgressBar, SW_SHOW);
                        SendMessage(hProgressBar, (UINT)PBM_SETMARQUEE, (WPARAM)1, NULL);
                        EnableWindow(hWnd, FALSE);

                        // читаем выбранный токен
                        std::thread thr(&wraperReadDevice, index);
                        thr.detach();
                    }
                }
                break;
            }

            int wmId = LOWORD(wParam);

            // Выбор в меню:
            switch (wmId)
            {
                case IDM_ABOUT:
                {
                    DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                    break;
                }

                case ID_REREAD_DEVICES: // перечитать устройства
                {
                    // Открываем прогрессбар и блокируем главное окно
                    ShowWindow(hProgressBar, SW_SHOW);
                    SendMessage(hProgressBar, (UINT)PBM_SETMARQUEE, (WPARAM)1, NULL);
                    EnableWindow(hWnd, FALSE);

                    // Сбор данных
                    setStatusBar("Выполняется обновление устройств...");
                    workCollectDevice();
                    SetWindowTextA(hEditInfoTokens, ""); // очищаем поле информации
                    break;
                }

                case ID_INSTALL_UTM: // установить УТМы
                {
                    // проверка на уже установленные утм
                    int err = checkUTM();
                    if (err == 0)
                    {
                        std::string error = "Удалите самостоятельно уже установленные УТМ!";
                        MessageBox(hWnd, error.c_str(), "Ошибка", MB_ICONERROR);
                        logger(error, "ERROR");
                        setStatusBar(error);
                        break;
                    }

                    DialogBox(hInst, MAKEINTRESOURCE(IDD_INSTALL_UTM), hWnd, WndInstallUTM);

                    break;
                }

                case ID_DELETE_UTM: // удалить УТМы
                {
                    // Проверка на существование конфига
                    if (configExists() == false)
                    {
                        std::string error = "Файл конфига не найден!";
                        MessageBox(hWnd, error.c_str(), "Ошибка", MB_ICONERROR);
                        logger(error, "ERROR");
                        setStatusBar(error);
                        break;
                    }

                    // проверка на уже установленные утм
                    int err = checkUTM();
                    if (err == 1)
                    {
                        std::string error = "Установленные УТМы не найдены!";
                        MessageBox(hWnd, error.c_str(), "Ошибка", MB_ICONERROR);
                        logger(error, "ERROR");
                        setStatusBar(error);
                        break;
                    }

                    // Открываем прогрессбар и блокируем главное окно
                    ShowWindow(hProgressBar, SW_SHOW);
                    SendMessage(hProgressBar, (UINT)PBM_SETMARQUEE, (WPARAM)1, NULL);
                    EnableWindow(hWnd, FALSE);

                    // удаляем утмы
                    std::thread thr(&deleteUTM);
                    thr.detach();

                    break;
                }

                case ID_START_UTM: // запустить УТМы
                {
                    // Проверка на существование конфига
                    if (configExists() == false)
                    {
                        std::string error = "Файл конфига не найден!";
                        MessageBox(hWnd, error.c_str(), "Ошибка", MB_ICONERROR);
                        logger(error, "ERROR");
                        setStatusBar(error);
                        break;
                    }

                    // проверка на уже установленные утм
                    int err = checkUTM();
                    if (err == 1)
                    {
                        std::string error = "Установленные УТМы не найдены!";
                        MessageBox(hWnd, error.c_str(), "Ошибка", MB_ICONERROR);
                        logger(error, "ERROR");
                        setStatusBar(error);
                        break;
                    }

                    // Открываем прогрессбар и блокируем главное окно
                    ShowWindow(hProgressBar, SW_SHOW);
                    SendMessage(hProgressBar, (UINT)PBM_SETMARQUEE, (WPARAM)1, NULL);
                    EnableWindow(hWnd, FALSE);

                    // запускаем утмы
                    std::thread thr(&startUTM);
                    thr.detach();

                    break;
                }

                case ID_STOP_UTM: // остановить УТМы
                {
                    // Проверка на существование конфига
                    if (configExists() == false)
                    {
                        std::string error = "Файл конфига не найден!";
                        MessageBox(hWnd, error.c_str(), "Ошибка", MB_ICONERROR);
                        logger(error, "ERROR");
                        setStatusBar(error);
                        break;
                    }

                    // проверка на уже установленные утм
                    int err = checkUTM();
                    if (err == 1)
                    {
                        std::string error = "Установленные УТМы не найдены!";
                        MessageBox(hWnd, error.c_str(), "Ошибка", MB_ICONERROR);
                        logger(error, "ERROR");
                        setStatusBar(error);
                        break;
                    }

                    // Открываем прогрессбар и блокируем главное окно
                    ShowWindow(hProgressBar, SW_SHOW);
                    SendMessage(hProgressBar, (UINT)PBM_SETMARQUEE, (WPARAM)1, NULL);
                    EnableWindow(hWnd, FALSE);

                    // запускаем утмы
                    std::thread thr(&stopUTM);
                    thr.detach();

                    break;
                }

                case ID_SHOW_DEVICES_NO_CONTEXT: // показать устройства вне контекста
                {
                    DialogBox(hInst, MAKEINTRESOURCE(IDD_READERS_NON_CONTEXT), hWnd, WndNoContextProc);
                    break;
                }

                case IDM_EXIT:
                {
                    DestroyWindow(hWnd);
                    break;
                }
            }
        }
        break;
        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            EndPaint(hWnd, &ps);
        }
        break;
        case WM_DESTROY:
        {
            SCardReleaseContext(ctxCard);
            PostQuitMessage(0);
            break;
        }
        default:
        {
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    return 0;
}

// Обработчик сообщений для окна ридеры вне контекста
INT_PTR CALLBACK WndNoContextProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        case WM_INITDIALOG:
        {
            hDlgReadersNoContext = hDlg; // передаем хендл в глобальную область для заполнения листбокса

            // Заполняем диалог
            workCollectDeviceNoContext();

            return (INT_PTR)TRUE;
        }

        case WM_COMMAND:
        {
            // кнопки удалить ридер и закрыть
            switch (LOWORD(wParam))
            {
                case IDOK_DELETE_ALL_READER: // удалить все ридеры
                {
                    int answer = MessageBox(NULL, "Вы уверены, что хотите удалить все ридеры?", "Внимание", MB_OKCANCEL | MB_ICONQUESTION);
                    if (answer == 1)
                    {
                        for (std::string elem : vecRutokensNoContext)
                        {
                            int err = deleteReader(elem, ctxCard);
                            if (err != 0)
                            {
                                std::string error = "Не удалось удалить выделенный ридер! Код ошибки: " + std::to_string(err);
                                setStatusBar(error);
                                logger(error, "ERROR");
                                break;
                            }
                        }
                        workCollectDeviceNoContext();
                    }

                    break;
                }

                case IDOK_DELETE_READER:
                {
                    // Получаем имя выделенного ридера
                    // Получаем индекс токена
                    int index = SendDlgItemMessage(hDlgReadersNoContext, IDC_LIST_READERS_NON_CONTEXT, LB_GETCURSEL, 0, 0); // получаем индекс выделенной строки
                    if (index == LB_ERR)
                    {
                        setStatusBar("Не удалось выделить токен!");
                        logger("Не удалось выделить токен!", "ERROR");
                        break;
                    }

                    std::string question = "Вы уверены, что хотите удалить ридер " + vecRutokensNoContext[index] + "?";
                    int answer = MessageBox(NULL, question.c_str(), "Внимание", MB_OKCANCEL | MB_ICONQUESTION);
                    if (answer == 1)
                    {
                        int err = deleteReader(vecRutokensNoContext[index], ctxCard);
                        if (err != 0)
                        {
                            std::string error = "Не удалось удалить выделенный ридер! Код ошибки: " + std::to_string(err);
                            setStatusBar(error);
                            logger(error, "ERROR");
                            break;
                        }
                        workCollectDeviceNoContext();
                    }

                    break;
                }

                case ID_CANCEL_READERS_NON_CONTEXT:
                {
                    EndDialog(hDlg, LOWORD(wParam));
                    return (INT_PTR)TRUE;
                }
            }
            break;
        }
    }
    return (INT_PTR)FALSE;
}

// Обработчик сообщений для окна установки УТМ
WNDPROC DefEditCountInstallUTMProc;
INT_PTR CALLBACK WndInstallUTM(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        case WM_INITDIALOG:
        {
            // Скрываем прогрессбар
            ShowWindow(GetDlgItem(hDlg, IDC_STATIC_NAME_INSTALL), SW_HIDE);
            ShowWindow(GetDlgItem(hDlg, IDC_STATIC_POINT), SW_HIDE);
            ShowWindow(GetDlgItem(hDlg, IDC_STATIC_PROGRESSBAR), SW_HIDE);
            ShowWindow(GetDlgItem(hDlg, IDC_PROGRESS1), SW_HIDE);
            ShowWindow(GetDlgItem(hDlg, IDC_STATIC_PROGRESSBAR2), SW_HIDE);

            hDlgInstallUTM = hDlg; // передаем в глобальную область хендл окна для обработчика поля ввода колво утмов

            // Переоопределяем обработчик событий для поля кол во УТМ
            DefEditCountInstallUTMProc = (WNDPROC)GetWindowLongPtr(GetDlgItem(hDlg, IDC_EDIT_COUNT_UTM), GWLP_WNDPROC);
            SetWindowLongPtr(GetDlgItem(hDlg, IDC_EDIT_COUNT_UTM), GWLP_WNDPROC, (LPARAM)EditCountInstallUTMProc);

            // Заполняем комбобоксы, колво УТМ, порты
            SetWindowText(GetDlgItem(hDlg, IDC_EDIT_COUNT_UTM), std::to_string(vecRutokens.size()).c_str());
            for (std::string elem : vecRutokens)
            {
                for (unsigned int i = 0; i < vecComboBoxToken.size(); ++i)
                {
                    SendDlgItemMessage(hDlgInstallUTM, vecComboBoxToken[i], CB_ADDSTRING, 0, (LPARAM)elem.c_str());
                    SendDlgItemMessage(hDlgInstallUTM, vecComboBoxToken[i], CB_SETCURSEL, 0, 0);
                }
            }
            int portUTM = 8080; // для заполнения портов
            for (int i : vecEditPort)
            {
                SetWindowText(GetDlgItem(hDlgInstallUTM, i), std::to_string(portUTM).c_str());
                portUTM += 1;
            }

            // Отправляем WM_KEYUP, чтобы отработало активация полей в EditCountInstallUTMProc
            SendMessage(GetDlgItem(hDlgInstallUTM, IDC_EDIT_COUNT_UTM), WM_KEYUP, 0, 1);

            return (INT_PTR)TRUE;
        }

        case WM_COMMAND:
        {
            switch (LOWORD(wParam))
            {
                case IDC_BUTTON_BROWSE_PATH_UTM:
                {
                    CHAR fileName[MAX_PATH];
                    FileOpenDlg(fileName);

                    if (strcmp(fileName, "") != 0)
                    {
                        SetWindowText(GetDlgItem(hDlgInstallUTM, IDC_EDIT_PATH_UTM), fileName);
                    }

                    break;
                }

                case IDOK_INSTALL_UTM:
                {
                    // проверка данных и запись в лог
                    CHAR countUTM[3];
                    GetDlgItemText(hDlg, IDC_EDIT_COUNT_UTM, countUTM, 3);
                    int err = writeConfigCountUTM(std::string(countUTM));
                    if (err == 1)
                    {
                        logger("Не удалось записать конфиг! Код ошибки - 1", "ERROR");
                        setStatusBar("Не удалось записать конфиг! Код ошибки - 1");
                        EndDialog(hDlg, LOWORD(wParam));
                        return (INT_PTR)TRUE;
                    }
                    int intCountUTM = atoi(countUTM);
                    std::string attrToken, serialNumber;
                    std::vector<std::string> tokens, ports;
                    bool flagPort = false;
                    for (int i = 0; i < intCountUTM; ++i)
                    {
                        CHAR set[MAX_PATH];
                        GetDlgItemText(hDlg, vecComboBoxToken[i], set, MAX_PATH);
                        tokens.push_back(std::string(set));
                        GetDlgItemText(hDlg, vecEditPort[i], set, MAX_PATH);
                        if (atoi(set) < 0 || atoi(set) > 65535)
                        {
                            flagPort = true;
                            break;
                        }
                        ports.push_back(std::string(set));
                    }
                    if (flagPort)
                    {
                        MessageBox(hDlg, "Выбраны неверные порты для УТМ!", "Ошибка", MB_ICONERROR);
                        break;
                    }
                    if (std::set<std::string>(tokens.begin(), tokens.end()).size() != tokens.size())
                    {
                        MessageBox(hDlg, "Выбраны одинаковые токены для разных УТМ!", "Ошибка", MB_ICONERROR);
                        break;
                    }
                    if (std::set<std::string>(ports.begin(), ports.end()).size() != ports.size())
                    {
                        MessageBox(hDlg, "Выбраны одинаковые порты для разных УТМ!", "Ошибка", MB_ICONERROR);
                        break;
                    }
                    for (int i = 0; i < 10; ++i)
                    {
                        if (i < intCountUTM)
                        {
                            for (unsigned int ii = 0; ii < vecRutokens.size(); ++ii)
                            {
                                if (tokens[i] == vecRutokens[ii])
                                {
                                    attrToken = vecAttrRutokens[ii];
                                    serialNumber = vecRutokenSerialNumber[ii];
                                    break;
                                }
                            }
                            err = writeConfig("UTM_" + std::to_string(i + 1), tokens[i], attrToken, serialNumber, ports[i]);
                            if (err == 1)
                            {
                                logger("Не удалось записать конфиг! Код ошибки - 1", "ERROR");
                                setStatusBar("Не удалось записать конфиг! Код ошибки - 1");
                                EndDialog(hDlg, LOWORD(wParam));
                                return (INT_PTR)TRUE;
                            }
                        }
                        else // удаляем лишние записи в конфиге
                        {
                            err = writeConfig("UTM_" + std::to_string(i + 1), "", "", "", "");
                            if (err == 1)
                            {
                                logger("Не удалось записать конфиг! Код ошибки - 1", "ERROR");
                                setStatusBar("Не удалось записать конфиг! Код ошибки - 1");
                                EndDialog(hDlg, LOWORD(wParam));
                                return (INT_PTR)TRUE;
                            }
                        }
                    }
                    CHAR fileInstallUTM[MAX_PATH];
                    GetDlgItemText(hDlg, IDC_EDIT_PATH_UTM, fileInstallUTM, MAX_PATH);
                    std::string strFileInstallUTM(fileInstallUTM);
                    if (strFileInstallUTM == "")
                    {
                        MessageBox(hDlg, "Не выбран установочный файл для разных УТМ!", "Ошибка", MB_ICONERROR);
                        break;
                    }

                    // Скрывам все и показываем прогрессбар
                    ShowWindow(GetDlgItem(hDlg, IDC_STATIC), SW_HIDE);
                    ShowWindow(GetDlgItem(hDlg, IDC_EDIT_COUNT_UTM), SW_HIDE);
                    for (unsigned int i = 0; i < vecComboBoxToken.size(); ++i)
                    {
                        ShowWindow(GetDlgItem(hDlg, vecComboBoxToken[i]), SW_HIDE);
                        ShowWindow(GetDlgItem(hDlg, vecEditPort[i]), SW_HIDE);
                        ShowWindow(GetDlgItem(hDlg, vecStaticUTM[i]), SW_HIDE);
                    }
                    ShowWindow(GetDlgItem(hDlg, IDC_STATIC2), SW_HIDE);
                    ShowWindow(GetDlgItem(hDlg, IDC_EDIT_PATH_UTM), SW_HIDE);
                    ShowWindow(GetDlgItem(hDlg, IDC_BUTTON_BROWSE_PATH_UTM), SW_HIDE);
                    ShowWindow(GetDlgItem(hDlg, IDOK_INSTALL_UTM), SW_HIDE);

                    ShowWindow(GetDlgItem(hDlg, IDC_STATIC_NAME_INSTALL), SW_SHOW);
                    ShowWindow(GetDlgItem(hDlg, IDC_STATIC_POINT), SW_SHOW);
                    ShowWindow(GetDlgItem(hDlg, IDC_STATIC_PROGRESSBAR), SW_SHOW);
                    ShowWindow(GetDlgItem(hDlg, IDC_STATIC_PROGRESSBAR2), SW_SHOW);
                    ShowWindow(GetDlgItem(hDlg, IDC_PROGRESS1), SW_SHOW);

                    // установка УТМ
                    HWND btnCancel = GetDlgItem(hDlg, ID_CANCEL_INSTALL_UTM);
                    std::thread thrInstallUTM(&installUTM, hDlg, atoi(countUTM), strFileInstallUTM, ctxCard, vecRutokens, vecAttrRutokens, btnCancel, hDlg);
                    handleThrInstallUTM = thrInstallUTM.native_handle();
                    std::thread thrChangeStatic(&changeStaticText, hDlg);
                    thrInstallUTM.detach();
                    thrChangeStatic.detach();

                    break;
                }

                case ID_CANCEL_INSTALL_UTM:
                {
                    if (flagInstallUTM == false)
                    {
                        setStatusBar("Установка УТМ отменена");
                        EndDialog(hDlg, LOWORD(wParam));
                        return (INT_PTR)TRUE;
                    }
                    else
                    {
                        int result = MessageBox(NULL, "Будет произведена отмена изменений! Может занять продолжительное время.\nХотите отменить установку?",
                            "Внимание", MB_YESNO | MB_ICONQUESTION);

                        if (result == IDYES)
                        {
                            // Делаем неактивной кнопку отмены
                            HWND btnCancel = GetDlgItem(hDlg, ID_CANCEL_INSTALL_UTM);
                            EnableWindow(btnCancel, FALSE);

                            // ждем завершения потока установки 5 секунд, иначе завершаем принудительно
                            flagInstallUTM = false;
                            WaitForSingleObject(evt, 5000);
                            if (handleThrInstallUTM)
                            {
                                CloseHandle(handleThrInstallUTM);
                            }

                            // откат изменений
                            std::thread thrbackChange(&backChange, vecRutokens, vecAttrRutokens, ctxCard, hDlg);
                            thrbackChange.detach();
                            setStatusBar("Установка УТМ отменена");
                            break;
                        }
                        else
                        {
                            break;
                        }
                    }
                }
            }
            
            break;
        }
    }
    return 0;
}

// Обработчик сообщений для окна "О программе".
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        SetDlgItemText(hDlg, IDC_ABOUT_STATIC, about.c_str());
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDC_BUTTON_GIT)
        {
            int err = system("start https://github.com/2UTM/2UTM/issues");
            if (err == -1)
            {
                MessageBox(hDlg, "Не удалось перейти на GitHub!", "Ошибка", MB_ICONERROR);
                logger("Не удалось перейти на GitHub!", "ERROR");
                return (INT_PTR)TRUE;
            }
        }
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

///////////////////////////////////////////////////////////////////////////////////
// Переопреленный обработчик событий для поля кол во УТМ
LRESULT CALLBACK EditCountInstallUTMProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        // В зависимости сколько УТМ, активируем поля
        case WM_KEYUP:
        {
            CHAR temp[3];
            GetWindowText(GetDlgItem(hDlgInstallUTM, IDC_EDIT_COUNT_UTM), temp, 3);
            countUTM = atoi(temp); 
            if (vecRutokens.size() == 0) // если токенов нет
            {
                SetWindowText(GetDlgItem(hDlgInstallUTM, IDC_EDIT_COUNT_UTM), (LPCSTR)std::to_string(vecRutokens.size()).c_str());
                SendDlgItemMessage(hDlgInstallUTM, IDC_EDIT_COUNT_UTM, EM_SETSEL, -2, -2); // переместить каретку в конец
                for (unsigned int i = 0; i < vecComboBoxToken.size(); ++i)
                {
                    EnableWindow(GetDlgItem(hDlgInstallUTM, vecComboBoxToken[i]), FALSE);
                    EnableWindow(GetDlgItem(hDlgInstallUTM, vecEditPort[i]), FALSE);
                }

                break;
            }
            if (countUTM > (int)vecRutokens.size()) // если больше чем активные токены, ставим сколько активных токенов
            {
                countUTM = vecRutokens.size();
                SetWindowText(GetDlgItem(hDlgInstallUTM, IDC_EDIT_COUNT_UTM), (LPCSTR)std::to_string(vecRutokens.size()).c_str());
                SendDlgItemMessage(hDlgInstallUTM, IDC_EDIT_COUNT_UTM, EM_SETSEL, -2, -2); // переместить каретку в конец
            }
            if (countUTM > 10 || countUTM < 1) // если больше 10 или меньше 1, то ставим 1
            {
                countUTM = 1;
                SetWindowText(GetDlgItem(hDlgInstallUTM, IDC_EDIT_COUNT_UTM), (LPCSTR)std::to_string(countUTM).c_str());
                SendDlgItemMessage(hDlgInstallUTM, IDC_EDIT_COUNT_UTM, EM_SETSEL, -2, -2); // переместить каретку в конец
            }
            switch (countUTM)
            {
                case 1:
                {
                    for (unsigned int i = 0; i < vecComboBoxToken.size(); ++i)
                    {
                        if (i < 1)
                        {
                            EnableWindow(GetDlgItem(hDlgInstallUTM, vecComboBoxToken[i]), TRUE);
                            EnableWindow(GetDlgItem(hDlgInstallUTM, vecEditPort[i]), TRUE);
                        }
                        else
                        {
                            EnableWindow(GetDlgItem(hDlgInstallUTM, vecComboBoxToken[i]), FALSE);
                            EnableWindow(GetDlgItem(hDlgInstallUTM, vecEditPort[i]), FALSE);
                        }
                    }

                    break;
                }

                case 2:
                {
                    for (unsigned int i = 0; i < vecComboBoxToken.size(); ++i)
                    {
                        if (i < 2)
                        {
                            EnableWindow(GetDlgItem(hDlgInstallUTM, vecComboBoxToken[i]), TRUE);
                            EnableWindow(GetDlgItem(hDlgInstallUTM, vecEditPort[i]), TRUE);
                        }
                        else
                        {
                            EnableWindow(GetDlgItem(hDlgInstallUTM, vecComboBoxToken[i]), FALSE);
                            EnableWindow(GetDlgItem(hDlgInstallUTM, vecEditPort[i]), FALSE);
                        }
                    }

                    break;
                }

                case 3:
                {
                    for (unsigned int i = 0; i < vecComboBoxToken.size(); ++i)
                    {
                        if (i < 3)
                        {
                            EnableWindow(GetDlgItem(hDlgInstallUTM, vecComboBoxToken[i]), TRUE);
                            EnableWindow(GetDlgItem(hDlgInstallUTM, vecEditPort[i]), TRUE);
                        }
                        else
                        {
                            EnableWindow(GetDlgItem(hDlgInstallUTM, vecComboBoxToken[i]), FALSE);
                            EnableWindow(GetDlgItem(hDlgInstallUTM, vecEditPort[i]), FALSE);
                        }
                    }

                    break;
                }

                case 4:
                {
                    for (unsigned int i = 0; i < vecComboBoxToken.size(); ++i)
                    {
                        if (i < 4)
                        {
                            EnableWindow(GetDlgItem(hDlgInstallUTM, vecComboBoxToken[i]), TRUE);
                            EnableWindow(GetDlgItem(hDlgInstallUTM, vecEditPort[i]), TRUE);
                        }
                        else
                        {
                            EnableWindow(GetDlgItem(hDlgInstallUTM, vecComboBoxToken[i]), FALSE);
                            EnableWindow(GetDlgItem(hDlgInstallUTM, vecEditPort[i]), FALSE);
                        }
                    }

                    break;
                }

                case 5:
                {
                    for (unsigned int i = 0; i < vecComboBoxToken.size(); ++i)
                    {
                        if (i < 5)
                        {
                            EnableWindow(GetDlgItem(hDlgInstallUTM, vecComboBoxToken[i]), TRUE);
                            EnableWindow(GetDlgItem(hDlgInstallUTM, vecEditPort[i]), TRUE);
                        }
                        else
                        {
                            EnableWindow(GetDlgItem(hDlgInstallUTM, vecComboBoxToken[i]), FALSE);
                            EnableWindow(GetDlgItem(hDlgInstallUTM, vecEditPort[i]), FALSE);
                        }
                    }

                    break;
                }

                case 6:
                {
                    for (unsigned int i = 0; i < vecComboBoxToken.size(); ++i)
                    {
                        if (i < 6)
                        {
                            EnableWindow(GetDlgItem(hDlgInstallUTM, vecComboBoxToken[i]), TRUE);
                            EnableWindow(GetDlgItem(hDlgInstallUTM, vecEditPort[i]), TRUE);
                        }
                        else
                        {
                            EnableWindow(GetDlgItem(hDlgInstallUTM, vecComboBoxToken[i]), FALSE);
                            EnableWindow(GetDlgItem(hDlgInstallUTM, vecEditPort[i]), FALSE);
                        }
                    }

                    break;
                }

                case 7:
                {
                    for (unsigned int i = 0; i < vecComboBoxToken.size(); ++i)
                    {
                        if (i < 7)
                        {
                            EnableWindow(GetDlgItem(hDlgInstallUTM, vecComboBoxToken[i]), TRUE);
                            EnableWindow(GetDlgItem(hDlgInstallUTM, vecEditPort[i]), TRUE);
                        }
                        else
                        {
                            EnableWindow(GetDlgItem(hDlgInstallUTM, vecComboBoxToken[i]), FALSE);
                            EnableWindow(GetDlgItem(hDlgInstallUTM, vecEditPort[i]), FALSE);
                        }
                    }

                    break;
                }

                case 8:
                {
                    for (unsigned int i = 0; i < vecComboBoxToken.size(); ++i)
                    {
                        if (i < 8)
                        {
                            EnableWindow(GetDlgItem(hDlgInstallUTM, vecComboBoxToken[i]), TRUE);
                            EnableWindow(GetDlgItem(hDlgInstallUTM, vecEditPort[i]), TRUE);
                        }
                        else
                        {
                            EnableWindow(GetDlgItem(hDlgInstallUTM, vecComboBoxToken[i]), FALSE);
                            EnableWindow(GetDlgItem(hDlgInstallUTM, vecEditPort[i]), FALSE);
                        }
                    }

                    break;
                }

                case 9:
                {
                    for (unsigned int i = 0; i < vecComboBoxToken.size(); ++i)
                    {
                        if (i < 9)
                        {
                            EnableWindow(GetDlgItem(hDlgInstallUTM, vecComboBoxToken[i]), TRUE);
                            EnableWindow(GetDlgItem(hDlgInstallUTM, vecEditPort[i]), TRUE);
                        }
                        else
                        {
                            EnableWindow(GetDlgItem(hDlgInstallUTM, vecComboBoxToken[i]), FALSE);
                            EnableWindow(GetDlgItem(hDlgInstallUTM, vecEditPort[i]), FALSE);
                        }
                    }

                    break;
                }

                case 10:
                {
                    for (int i : vecComboBoxToken)
                    {
                        EnableWindow(GetDlgItem(hDlgInstallUTM, i), TRUE);
                        EnableWindow(GetDlgItem(hDlgInstallUTM, i), TRUE);
                    }

                    break;
                }
            }

            break;
        }

    }
    return((LRESULT)CallWindowProc((WNDPROC)DefEditCountInstallUTMProc, hwnd, uMsg, wParam, lParam));
}
///////////////////////////////////////////////////////////////////////////////////

// получаем версию ос
// Если минор равен 1, это вин 7
void getVersionOS(DWORD& dwMinorVersion)
{
    DWORD dwVersion = 0;
#pragma warning(suppress : 4996) // GetVersion deprecated
    dwVersion = GetVersion();
    dwMinorVersion = (DWORD)(HIBYTE(LOWORD(dwVersion)));
}

// установка текста в статус бар
void setStatusBar(std::string status)
{
    SetWindowTextA(hStatusBar, status.c_str());
}

// установка текста в поле описание рутокена
void setInfoRutokens(std::string info)
{
    SetWindowTextA(hEditInfoTokens, info.c_str());
}

// обертка над чтением с устройства (чтобы отделить логику ридеров от окна)
void wraperReadDevice(int index)
{
    setStatusBar("Выполняется сбор данных с устройства...");

    std::string s = "Имя ридера: " + vecRutokens[index] + "\r\nАтрибут ридера: " + vecAttrRutokens[index] + "\r\n\r\n";
    int err = readDevice(index, s);
    if (err != 0)
    {
        std::string error = "Ошибка сбора данных с устройства, код ошибки " + std::to_string(err);
        setStatusBar(error);
        logger(error, "ERROR");
    }

    // Скрываем прогрессбар и разблокируем главное окно
    ShowWindow(hProgressBar, SW_HIDE);
    EnableWindow(hWndMain, TRUE);

    SetWindowText(hEditInfoTokens, s.c_str());

    setStatusBar("Выполнено");
    logger("Сбор данных выполнен успешно", "INFO");
}

// заполнение листбокса
int fillListBoxRutokens(std::vector<std::string> vecReader)
{
    if (vecReader.empty())
    {
        return 1;
    }

    // получаем индекс первого элемента
    int topIndex = SendMessage(hListBoxTokensGlobal, LB_GETTOPINDEX, 0, 0);

    // Удаляем все строки в листбоксе контактов
    SendMessage(hListBoxTokensGlobal, LB_RESETCONTENT, 0, 0);

    // Заполняем
    for (std::string elem : vecReader)
    {
        std::string s = "      " + elem; // пробелы для красоты
        SendMessage(hListBoxTokensGlobal, LB_ADDSTRING, 0, (LPARAM)s.c_str());
    }

    // устанавливаем индекс первого элемента
    SendMessage(hListBoxTokensGlobal, LB_SETTOPINDEX, topIndex, 0);

    return 0;
}

// функция, объединяющая получение устройств
int workCollectDevice()
{
    setStatusBar("Выполняется обновление устройств...");

    vecAttrRutokens.clear();
    vecRutokens.clear();
    vecRutokenSerialNumber.clear();

    std::string error;

    // Получаем контекст смарткарт
    int err = getContextCard(ctxCard);
    if (err != 0)
    {
        error = "Не удалось получить контекст смарткарт, код ошибки " + std::to_string(err);
        setStatusBar(error);
        logger(error, "ERROR");
        // Очищаем листбокс
        SendMessage(hListBoxTokensGlobal, LB_RESETCONTENT, 0, 0);
        // Убираем прогрессбар и разблокируем главное окно
        ShowWindow(hProgressBar, SW_HIDE);
        EnableWindow(hWndMain, TRUE);
        return 1;
    }

    // Получаем все серийные номера
    err = getSerialNumberAllDevices(vecRutokenSerialNumber);
    if (err != 0)
    {
        std::string error = "Не удалось получить серийные номера рутокенов, код ошибки " + std::to_string(err);
        setStatusBar(error);
        logger(error, "ERROR");
        // Очищаем листбокс
        SendMessage(hListBoxTokensGlobal, LB_RESETCONTENT, 0, 0);
        // Убираем прогрессбар и разблокируем главное окно
        ShowWindow(hProgressBar, SW_HIDE);
        EnableWindow(hWndMain, TRUE);
        return 2;
    }

    // Получаем список ридеров
    err = getListDevice(vecRutokens, ctxCard);
    if (err == 1)
    {
        // Удаляем все строки в листбоксе контактов
        SendMessage(hListBoxTokensGlobal, LB_RESETCONTENT, 0, 0);

        error = "Не найдено подключенных смарткарт";
        setStatusBar(error);
        logger(error, "ERROR");
        // Очищаем листбокс
        SendMessage(hListBoxTokensGlobal, LB_RESETCONTENT, 0, 0);
        // Убираем прогрессбар и разблокируем главное окно
        ShowWindow(hProgressBar, SW_HIDE);
        EnableWindow(hWndMain, TRUE);
        return 3;
    }
    if (err != 0)
    {
        error = "Не удалось получить список смарткарт, код ошибки " + std::to_string(err);
        setStatusBar(error);
        logger(error, "ERROR");
        // Очищаем листбокс
        SendMessage(hListBoxTokensGlobal, LB_RESETCONTENT, 0, 0);
        // Убираем прогрессбар и разблокируем главное окно
        ShowWindow(hProgressBar, SW_HIDE);
        EnableWindow(hWndMain, TRUE);
        return 4;
    }

    // Получаем атрибуты ридеров
    err = getAttrReader(vecAttrRutokens, ctxCard, vecRutokens);
    if (err != 0)
    {
        error = "Не удалось получить атрибуты смарткарт, код ошибки " + std::to_string(err);
        setStatusBar(error);
        logger(error, "ERROR");
        // Очищаем листбокс
        SendMessage(hListBoxTokensGlobal, LB_RESETCONTENT, 0, 0);
        // Убираем прогрессбар и разблокируем главное окно
        ShowWindow(hProgressBar, SW_HIDE);
        EnableWindow(hWndMain, TRUE);
        return 5;
    }

    if (fillListBoxRutokens(vecRutokens) != 0)
    {
        error = "Не удалось заполнить листбокс";
        setStatusBar(error);
        logger(error, "ERROR");
        // Очищаем листбокс
        SendMessage(hListBoxTokensGlobal, LB_RESETCONTENT, 0, 0);
    }

    // Убираем прогрессбар и разблокируем главное окно
    ShowWindow(hProgressBar, SW_HIDE);
    EnableWindow(hWndMain, TRUE);

    setStatusBar("Выполнено");
    logger("Обновление устройств выполнено успешно", "INFO");

    SendMessage(GetDlgItem(hDlgInstallUTM, IDC_EDIT_COUNT_UTM), WM_KEYUP, 0, 1); // для обновления данных, если открыто окно установки УТМ

    return 0;
}

// заполнение листбокса в окне вне контекста
int fillListBoxRutokensNoContext(std::vector<std::string> vecReaderNoContext)
{
    if (vecReaderNoContext.empty())
    {
        // Удаляем все строки в листбоксе контактов
        SendDlgItemMessage(hDlgReadersNoContext, IDC_LIST_READERS_NON_CONTEXT, LB_RESETCONTENT, 0, 0);
        return 1;
    }

    // получаем индекс первого элемента
    int topIndex = SendDlgItemMessage(hDlgReadersNoContext, IDC_LIST_READERS_NON_CONTEXT, LB_GETTOPINDEX, 0, 0);

    // Удаляем все строки в листбоксе контактов
    SendDlgItemMessage(hDlgReadersNoContext, IDC_LIST_READERS_NON_CONTEXT, LB_RESETCONTENT, 0, 0);

    // Заполняем
    for (std::string elem : vecReaderNoContext)
    {
        std::string s = "      " + elem; // пробелы для красоты
        SendDlgItemMessage(hDlgReadersNoContext, IDC_LIST_READERS_NON_CONTEXT, LB_ADDSTRING, 0, (LPARAM)s.c_str());
    }

    // устанавливаем индекс первого элемента
    SendDlgItemMessage(hDlgReadersNoContext, IDC_LIST_READERS_NON_CONTEXT, LB_SETTOPINDEX, topIndex, 0);

    return 0;
}

// функция, объединяющая получение устройств вне контекста
int workCollectDeviceNoContext()
{
    vecRutokensNoContext.clear();

    std::string error;

    // Получаем список ридеров вне конеткста (NULL)
    int err = getListDevice(vecRutokensNoContext, NULL);
    if (err == 1)
    {
        error = "Не найдено подключенных смарткарт";
        setStatusBar(error);
        logger(error, "ERROR");
        return 2;
    }
    if (err != 0)
    {
        error = "Не удалось получить список смарткарт, код ошибки " + std::to_string(err);
        setStatusBar(error);
        logger(error, "ERROR");
        return 3;
    }

    if (fillListBoxRutokensNoContext(vecRutokensNoContext) != 0)
    {
        error = "Не удалось заполнить листбокс";
        setStatusBar(error);
        logger(error, "ERROR");
        return 4;
    }

    logger("Обновление устройств вне контекста выполнено успешно", "INFO");
    return 0;
}

int FileOpenDlg(CHAR* fileName)
{
    OPENFILENAME ofn;       // структура диалога
    char szFile[MAX_PATH];       // буффер для имени файла
    HWND hwnd = 0;          // хендл окна родителя

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hwnd;
    ofn.lpstrFile = szFile;
    // Установите для lpstrFile[0] значение '\0', чтобы GetOpenFileName не
    // использовал содержимое szFile для инициализации самого себя.
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = "Файлы .exe\0*.exe\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_HIDEREADONLY | OFN_CREATEPROMPT;

    int result = GetOpenFileName(&ofn);

    if (result == 0)
    {
        strcpy(fileName, "");
        return 1;
    }
    if (result != 0)
    {
        strcpy(fileName, ofn.lpstrFile);
    }

    return 0;
}

// проверка на уже установленные утм
int checkUTM()
{
    WIN32_FIND_DATA FindFileData;
    HANDLE hFind;

    hFind = FindFirstFile("c:\\UTM*", &FindFileData);
    if (hFind == INVALID_HANDLE_VALUE)
    {
        return 1;
    }
    
        
    FindFileData.cFileName;
    FindClose(hFind);

    return 0;
}

// удалить УМТы
int deleteUTM()
{
    setStatusBar("Выполняеться удаление УТМ");
    logger("Выполняеться удаление УТМ", "INFO");

    std::string error;
    int err;
    std::string countUTM;

    // получаем кол-во утм из конфига
    err = readConfigCountUTM(countUTM);
    if (err != 0)
    {
        error = "Не удалось получить кол-во УТМ из конфига, код ошибки " + std::to_string(err);
        setStatusBar(error);
        logger(error, "ERROR");
        return 1;
    }

    // удаление первого утм чере uninstaller
    err = startProcessInstallUTM("C:\\UTM\\unins000.exe");
    if (err == 1)
    {
        error = "Удаление УТМ завершилась ошибкой! Код ошибки - " + std::to_string(GetLastError());
        setStatusBar(error);
        MessageBox(hWndMain, error.c_str(), "Ошибка", MB_ICONERROR);
        logger(error, "ERROR");
        // Скрываем прогрессбар и разблокируем главное окно
        ShowWindow(hProgressBar, SW_HIDE);
        EnableWindow(hWndMain, TRUE);
        return 1;
    }

    // остановка служб и удаление УТМ
    int intCountUTM = atoi(countUTM.c_str());
    for (int i = intCountUTM; i > 0; --i)
    {
        if (i == 1)
        {
            break;
        }
        setStatusBar("Остановка Transport" + std::to_string(i));
        err = stopServiceUTM("Transport" + std::to_string(i));
        if (err != 0)
        {
            error = "Не удалось остановить службу Transport" + std::to_string(i) + ", код ошибки " + std::to_string(err);
            setStatusBar(error);
            MessageBox(hWndMain, error.c_str(), "Ошибка", MB_ICONERROR);
            logger(error, "ERROR");
            // Скрываем прогрессбар и разблокируем главное окно
            ShowWindow(hProgressBar, SW_HIDE);
            EnableWindow(hWndMain, TRUE);
            return 1;
        }
        setStatusBar("Удаление Transport" + std::to_string(i));
        err = deleteServiceUTM(i);
        if (err != 0)
        {
            error = "Не удалось удалить службу Transport" + std::to_string(i) + ", код ошибки " + std::to_string(GetLastError());
            setStatusBar(error);
            MessageBox(hWndMain, error.c_str(), "Ошибка", MB_ICONERROR);
            logger(error, "ERROR");
            // Скрываем прогрессбар и разблокируем главное окно
            ShowWindow(hProgressBar, SW_HIDE);
            EnableWindow(hWndMain, TRUE);
            return 1;
        }
        setStatusBar("Удаление каталога C:\\UTM_" + std::to_string(i));
        err = deleteFolderUTM("C:\\UTM_" + std::to_string(i));
        if (err != 0)
        {
            error = "Не удалось удалить C:\\UTM_" + std::to_string(i) + ", код ошибки " + std::to_string(GetLastError());
            setStatusBar(error);
            MessageBox(hWndMain, error.c_str(), "Ошибка", MB_ICONERROR);
            logger(error, "ERROR");
            // Скрываем прогрессбар и разблокируем главное окно
            ShowWindow(hProgressBar, SW_HIDE);
            EnableWindow(hWndMain, TRUE);
            return 1;
        }
    }

    // Скрываем прогрессбар и разблокируем главное окно
    ShowWindow(hProgressBar, SW_HIDE);
    EnableWindow(hWndMain, TRUE);

    setStatusBar("Выполнено");
    logger("Удаление УТМ успешно завершено", "INFO");

    return 0;
}

// удалить папку утм
int deleteFolderUTM(std::string src)
{
    WIN32_FIND_DATA FindFileData; // структура для поиска файлов
    HANDLE hFind;

    std::string filter = src + "\\*"; // фильтр поиска
    std::string src2;

    hFind = FindFirstFile(filter.c_str(), &FindFileData); // поиск файлов
    if (hFind == NULL)
    {
        return 1;
    }

    do
    {
        if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) // если нашли папку
        {
            if (strcmp(FindFileData.cFileName, ".") && strcmp(FindFileData.cFileName, ".."))
            {
                // если есть подпапки, ищем дальше там через рекурсию
                src2 = src + "\\" + std::string(FindFileData.cFileName) + "\\";
                deleteFolderUTM(src2); // рекурсия
            }
        }
        else // нашли файл
        {
            src2 = src + "\\" + std::string(FindFileData.cFileName);
            if (DeleteFile(src2.c_str()) == 0) // удаляем файл
            {
                return 1;
            }
        }
    } while (FindNextFile(hFind, &FindFileData)); // пока есть что удаляем

    // удаляем пустую папку
    if (RemoveDirectory(src.c_str()) == 0)
    {
        return 1;
    }

    FindClose(hFind); // очистка
    return 0;
}

// запустить УТМ
int startUTM()
{
    setStatusBar("Выполняеться запуск УТМ");
    logger("Выполняеться запуск УТМ", "INFO");

    std::string error;
    int err;
    bool flagStartUTM = true;

    std::string countUTMstr;
    if (readConfigCountUTM(countUTMstr) == 1)
    {
        error = "Чтение конфига завершилась ошибкой! Код ошибки - " + std::to_string(GetLastError());
        MessageBox(hWndMain, error.c_str(), "Ошибка", MB_ICONERROR);
        logger(error, "ERROR");
        setStatusBar(error);
        // Скрываем прогрессбар и разблокируем главное окно
        ShowWindow(hProgressBar, SW_HIDE);
        EnableWindow(hWndMain, TRUE);
        return 1;
    }
    int countUTM = atoi(countUTMstr.c_str());

    std::vector<std::string> vectorNameReaders, vectorAttrReaders, ports, serialNumbers;
    if (readConfigNameAndReader(countUTM, vectorNameReaders, vectorAttrReaders, ports, serialNumbers) == 1)
    {
        error = "Чтение конфига завершилась ошибкой! Код ошибки - " + std::to_string(GetLastError());
        MessageBox(hWndMain, error.c_str(), "Ошибка", MB_ICONERROR);
        logger(error, "ERROR");
        setStatusBar(error);
        // Скрываем прогрессбар и разблокируем главное окно
        ShowWindow(hProgressBar, SW_HIDE);
        EnableWindow(hWndMain, TRUE);
        return 1;
    }

    if (vecRutokens.size() < vectorNameReaders.size())
    {
        error = "Активных токенов меньше, чем в конфиге.\nПереустановите УТМы, или добавьте активных токенов";
        MessageBox(hWndMain, error.c_str(), "Ошибка", MB_ICONERROR);
        logger(error, "ERROR");
        setStatusBar(error);
        // Скрываем прогрессбар и разблокируем главное окно
        ShowWindow(hProgressBar, SW_HIDE);
        EnableWindow(hWndMain, TRUE);
        return 1;
    }

    // собираем актуальную инфу о токенах по серийному номеру
    std::vector<std::string> vecTempSerialNumber, vecActualNameReader, vecActualAttrReader;
    for (unsigned int i = 0; i < serialNumbers.size(); ++i)
    {
        std::vector<std::string>::iterator it;
        it = std::find(vecRutokenSerialNumber.begin(), vecRutokenSerialNumber.end(), serialNumbers[i]);
        if (it != vecRutokenSerialNumber.end())
        {
            vecTempSerialNumber.push_back(serialNumbers[i]);
            vecActualNameReader.push_back(vecRutokens[it - vecRutokenSerialNumber.begin()]);
            vecActualAttrReader.push_back(vecAttrRutokens[it - vecRutokenSerialNumber.begin()]);
        }
    }
    if (vecTempSerialNumber != serialNumbers)
    {
        error = "Некоторые активные токены отсутсвуют в конфиге.\nПереустановите УТМы, или исправьте активные токены";
        MessageBox(hWndMain, error.c_str(), "Ошибка", MB_ICONERROR);
        logger(error, "ERROR");
        setStatusBar(error);
        // Скрываем прогрессбар и разблокируем главное окно
        ShowWindow(hProgressBar, SW_HIDE);
        EnableWindow(hWndMain, TRUE);
        return 1;
    }

    logger("Чтение конфига успешно завершено", "INFO");

    // Если 1 УТМ
    if (ports.size() == 1)
    {
        setStatusBar("Запуск Transport");
        err = startServiceUTM("Transport");
        if (err != 0)
        {
            error = "Ошибка запуска службы Trasport! Код ошибки - " + std::to_string(GetLastError()) + "\nБудет выполнена отмена изменений";
            MessageBox(hWndMain, error.c_str(), "Ошибка", MB_ICONERROR);
            logger(error, "ERROR");
            setStatusBar(error);
            if (backChangeStartUTM() != 0)
            {
                logger("Отмена изменений завершилась неудачей", "ERROR");
            }
            // Скрываем прогрессбар и разблокируем главное окно
            ShowWindow(hProgressBar, SW_HIDE);
            EnableWindow(hWndMain, TRUE);
            return 1;
        }
        logger("Запуск службы Transport успешно завершено", "INFO");

        // Проверка работы УТМ (запрос на главную страницу)
        setStatusBar("Проверка работы УТМ 1 (запрос на главную страницу)");
        logger("Проверка работы УТМ 1 (запрос на главную страницу)", "INFO");
        err = checkHomePageUTM(ports[0], flagStartUTM);
        if (err != 0)
        {
            error = "Не удалось проверить работу УТМ! Код ошибки - " + std::to_string(err) + "\nФункция завершилась по таймауту" + "\nБудет выполнена отмена изменений";
            MessageBox(hWndMain, error.c_str(), "Ошибка", MB_ICONERROR);
            logger(error, "ERROR");
            setStatusBar(error);
            if (backChangeStartUTM() != 0)
            {
                logger("Отмена изменений завершилась неудачей", "ERROR");
            }
            // Скрываем прогрессбар и разблокируем главное окно
            ShowWindow(hProgressBar, SW_HIDE);
            EnableWindow(hWndMain, TRUE);
            return 1;
        }
        logger("Проверка работы УТМ 1 успешно завершена", "INFO");
    }
    else
    {
        int countPort = 0; // потому что массив портов развернут в другую сторону

        // проверка на невыбранные ридеры
        std::vector<std::string> vecTokensNoChoose, vecAttrNoChoose;
        if (vecRutokens.size() != vecActualNameReader.size())
        {
            std::vector<std::string>::iterator it;
            for (unsigned int i = 0; i < vecRutokens.size(); ++i)
            {
                it = std::find(vecActualNameReader.begin(), vecActualNameReader.end(), vecRutokens[i]);
                if (it == vecActualNameReader.end())
                {
                    vecTokensNoChoose.push_back(vecRutokens[i]);
                    vecAttrNoChoose.push_back(vecAttrRutokens[i]);
                }
            }
        }

        // Удаляем все ридеры
        setStatusBar("Удаление всех ридеров");
        logger("Удаление всех ридеров", "INFO");

        for (std::string elem : vecRutokens)
        {
            err = deleteReader(elem, ctxCard);
            if (err != 0)
            {
                error = "Удаление всех ридеров завершилось ошибкой! Код ошибки - " + std::to_string(err) + "\nБудет выполнена отмена изменений";
                MessageBox(hWndMain, error.c_str(), "Ошибка", MB_ICONERROR);
                logger(error, "ERROR");
                setStatusBar(error);
                if (backChangeStartUTM() != 0)
                {
                    logger("Отмена изменений завершилась неудачей", "ERROR");
                }
                // Скрываем прогрессбар и разблокируем главное окно
                ShowWindow(hProgressBar, SW_HIDE);
                EnableWindow(hWndMain, TRUE);
                return 1;
            }
        }
        logger("Удаление всех ридеров успешно завершено", "INFO");

        // Добавляем ридеры, начиная с конца, в конце не выбранные ридеры, далее выбранные с конца
        // И запускаем УТМы
        setStatusBar("Запускаем службы УТМ");
        logger("Запускаем службы УТМ", "INFO");

        // добавление ридеров и запуск служб УТМ
        // вначале добавляем не выбранные ридеры
        std::string nameReader, numberService;
        int countReader = 1;
        for (unsigned int i = 0; i < vecTokensNoChoose.size(); ++i, ++countReader)
        {
            nameReader = "Aktiv Rutoken ECP " + std::to_string(vecRutokens.size() - countReader);
            logger("Добавляем ридер " + nameReader, "INFO");
            err = addNameReader(nameReader, ctxCard, vecAttrNoChoose[i]);
            if (err != 0)
            {
                error = "Добавление ридера " + nameReader + " завершилось ошибкой! Код ошибки - " + std::to_string(err) + "\nБудет выполнена отмена изменений";
                MessageBox(hWndMain, error.c_str(), "Ошибка", MB_ICONERROR);
                logger(error, "ERROR");
                setStatusBar(error);
                if (backChangeStartUTM() != 0)
                {
                    logger("Отмена изменений завершилась неудачей", "ERROR");
                }
                // Скрываем прогрессбар и разблокируем главное окно
                ShowWindow(hProgressBar, SW_HIDE);
                EnableWindow(hWndMain, TRUE);
                return 1;
            }
        }
        logger("Добавление не выбранных ридеров успешно завершено", "INFO");
        // далее добавляем выбранные ридеры с конца и запускаем утмы
        for (unsigned int i = 0; i < vecActualNameReader.size(); ++i, ++countReader)
        {
            // добавление ридера
            nameReader = "Aktiv Rutoken ECP " + std::to_string(vecRutokens.size() - countReader);
            logger("Добавляем ридер " + nameReader, "INFO");
            err = addNameReader(nameReader, ctxCard, vecActualAttrReader[i]);
            if (err != 0)
            {
                error = "Добавление ридера " + nameReader + " завершилось ошибкой! Код ошибки - " + std::to_string(err) + "\nБудет выполнена отмена изменений";
                MessageBox(hWndMain, error.c_str(), "Ошибка", MB_ICONERROR);
                logger(error, "ERROR");
                setStatusBar(error);
                if (backChangeStartUTM() != 0)
                {
                    logger("Отмена изменений завершилась неудачей", "ERROR");
                }
                // Скрываем прогрессбар и разблокируем главное окно
                ShowWindow(hProgressBar, SW_HIDE);
                EnableWindow(hWndMain, TRUE);
                return 1;
            }
            logger("Добавление ридера " + nameReader + " успешно завершено", "INFO");

            // запуск службы утм
            numberService = std::to_string(vecRutokens.size() - countReader + 1);
            std::string str;
            if (numberService == "1")
            {
                str = "Запуск службы Transport";
                numberService = "";
            }
            else
            {
                str = "Запуск службы Transport " + numberService;
            }
            setStatusBar(str);
            logger(str, "INFO");
            err = startServiceUTM("Transport" + numberService);
            if (err != 0)
            {
                error = "Ошибка запуска службы Transport" + numberService + "! Код ошибки - " + std::to_string(GetLastError()) + "\nБудет выполнена отмена изменений";
                MessageBox(hWndMain, error.c_str(), "Ошибка", MB_ICONERROR);
                logger(error, "ERROR");
                setStatusBar(error);
                if (backChangeStartUTM() != 0)
                {
                    logger("Отмена изменений завершилась неудачей", "ERROR");
                }
                // Скрываем прогрессбар и разблокируем главное окно
                ShowWindow(hProgressBar, SW_HIDE);
                EnableWindow(hWndMain, TRUE);
                return 1;
            }
            logger("Запуск службы Transport " + numberService + " успешно завершена", "INFO");
            Sleep(500);

            // проверка работы утм
            setStatusBar("Проверка работы УТМ " + numberService + " (запрос на главную страницу)");
            logger("Проверка работы УТМ " + numberService + " (запрос на главную страницу)", "INFO");
            err = checkHomePageUTM(ports[i], flagStartUTM);
            if (err != 0)
            {
                error = "Не удалось проверить работу УТМ " + numberService + "! Код ошибки - " + std::to_string(err) + "\nФункция завершилась по таймауту" + "\nБудет выполнена отмена изменений";
                MessageBox(hWndMain, error.c_str(), "Ошибка", MB_ICONERROR);
                logger(error, "ERROR");
                setStatusBar(error);
                if (backChangeStartUTM() != 0)
                {
                    logger("Отмена изменений завершилась неудачей", "ERROR");
                }
                // Скрываем прогрессбар и разблокируем главное окно
                ShowWindow(hProgressBar, SW_HIDE);
                EnableWindow(hWndMain, TRUE);
                return 1;
            }
            logger("Проверка работы УТМ " + numberService + " успешно завершена", "INFO");
        }
    }

    // Скрываем прогрессбар и разблокируем главное окно
    ShowWindow(hProgressBar, SW_HIDE);
    EnableWindow(hWndMain, TRUE);

    setStatusBar("Выполнено");
    logger("Запуск служб УТМ успешно завершено", "INFO");

    return 0;
}

// откат изменений при запуске утм
int backChangeStartUTM()
{
    logger("Отмена изменений", "INFO");

    int err;
    std::string error;

    // Удаляем все новый ридеры
    for (std::string elem : vecRutokens)
    {
        err = deleteReader(elem, ctxCard);
        if (err == ERROR_FILE_NOT_FOUND) // если нет такого ридера, продолжаем
        {
            continue;
        }
        if (err != 0)
        {
            error = "Отмена изменений. Удаление всех новых ридеров завершилось ошибкой! Код ошибки - " + std::to_string(err);
            logger(error, "ERROR");
            MessageBox(hWndMain, error.c_str(), "Ошибка", MB_ICONERROR);
            return 1;
        }
    }
    logger("Отмена изменений. Удаление всех новых ридеров успешно завершено", "INFO");

    // Добавляем все старые ридеры
    for (unsigned int i = 0; i < vecRutokens.size(); ++i)
    {
        err = addNameReader(vecRutokens[i], ctxCard, vecAttrRutokens[i]);
        if (err != 0)
        {
            error = "Отмена изменений. Добавление всех старых ридеров завершилось ошибкой! Код ошибки - " + std::to_string(err);
            logger(error, "ERROR");
            MessageBox(hWndMain, error.c_str(), "Ошибка", MB_ICONERROR);
            return 1;
        }
    }
    logger("Отмена изменений. Добавление всех старых ридеров успешно завершено", "INFO");

    return 0;
}

// остановить УТМы
int stopUTM()
{
    setStatusBar("Выполняеться остановка УТМ");
    logger("Выполняеться остановка УТМ", "INFO");
    std::string error, countUTMstr;

    // получаем кол-во утм из конфига
    int err = readConfigCountUTM(countUTMstr);
    if (err != 0)
    {
        error = "Не удалось получить кол-во УТМ из конфига, код ошибки " + std::to_string(err);
        setStatusBar(error);
        logger(error, "ERROR");
        setStatusBar(error);
        // Скрываем прогрессбар и разблокируем главное окно
        ShowWindow(hProgressBar, SW_HIDE);
        EnableWindow(hWndMain, TRUE);
        return 1;
    }


    int intCountUTM = atoi(countUTMstr.c_str());
    for (int i = intCountUTM; i > 0; --i)
    {
        if (i == 1)
        {
            setStatusBar("Остановка Transport");
            err = stopServiceUTM("Transport");
            if (err != 0)
            {
                error = "Не удалось остановить службу Transport, код ошибки " + std::to_string(err);
                setStatusBar(error);
                MessageBox(hWndMain, error.c_str(), "Ошибка", MB_ICONERROR);
                logger(error, "ERROR");
                setStatusBar(error);
                // Скрываем прогрессбар и разблокируем главное окно
                ShowWindow(hProgressBar, SW_HIDE);
                EnableWindow(hWndMain, TRUE);
                return 1;
            }
            break;
        }
        setStatusBar("Остановка Transport" + std::to_string(i));
        err = stopServiceUTM("Transport" + std::to_string(i));
        if (err != 0)
        {
            error = "Не удалось остановить службу Transport" + std::to_string(i) + ", код ошибки " + std::to_string(err);
            setStatusBar(error);
            MessageBox(hWndMain, error.c_str(), "Ошибка", MB_ICONERROR);
            logger(error, "ERROR");
            setStatusBar(error);
            // Скрываем прогрессбар и разблокируем главное окно
            ShowWindow(hProgressBar, SW_HIDE);
            EnableWindow(hWndMain, TRUE);
            return 1;
        }
    }

    // Скрываем прогрессбар и разблокируем главное окно
    ShowWindow(hProgressBar, SW_HIDE);
    EnableWindow(hWndMain, TRUE);

    setStatusBar("Выполнено");
    logger("Остановка УТМ успешно завершено", "INFO");

    return 0;
}