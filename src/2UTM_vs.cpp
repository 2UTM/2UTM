#define _CRT_SECURE_NO_WARNINGS

#include "framework.h"
#include "2UTM_vs.h"
#include "ControlReaders.h"
#include "ControlService.h"

#define MAX_LOADSTRING 100
#define TRAY_ICON WM_USER + 1
#define TIMER_CHECK_SERVICE WM_USER + 2

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

    HANDLE hFile = CreateFile(FileName, GENERIC_WRITE, FILE_SHARE_READ, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
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
"Что нового в версии 1.4:\n"
"1. Добавлена вкладка службы УТМ со статусами служб УТМ, обновление статусов раз в минуту.\n"
"2. Переделан ListBox на ListView.\n"
"3. Добавлены иконки для красоты.\n"
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

NOTIFYICONDATA pnid; // структура для трея
bool flagShowMessageTray = false; // для сообщения в трее
bool flagServiceRun = false; // флаг - запущен ли из под службы или нет
bool flagAutostartUTM = false; // флаг - идет автозапуск утм или нет
bool flagStartUTM = false; // флаг - идет запуск утм
bool flagStopUTM = false; // флаг - идет остановка утм
bool flagCheckServices = false; // флаг, что идет получение статусов служб
std::map<DWORD, DWORD> mapSessionIDProcessID; // для контроля входа выхода пользователей и для завершения процессов
UINT WM_TASKBARCREATED; // для пользовательского сообщения о создании панели задач от explorer.exe
UINT_PTR checkServiceTimer = 0; // таймер для сбора инфы о службах
std::vector<std::string> vecPathService; // для путей до исполняющих файлов служб
std::vector<int> vecUTMError; // для ошибок УТМ при проверке поля rsaError , 0 - не запущен, 1 - ошибок нет, 2 - ошибки есть

// Иконки для листвью
HICON hIconToken = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON6));
HICON hIconServiceRun = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON4));
HICON hIconServiceStop = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON5));
HICON hIconServiceWarning = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1));

// Хендлы виджетов
HWND hWndMain, hListBoxTokensGlobal, hEditInfoTokens, hStatusBar, hProgressBar, hDlgReadersNoContext, hDlgInstallUTM, hTabControl, hListBoxUTMServiceGlobal;

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
SERVICE_STATUS servicestatus;
SERVICE_STATUS_HANDLE hstatus;

DWORD WINAPI ServiceHandlerEx(DWORD dwControl, DWORD dwEventType, LPVOID lpEventData, LPVOID lpContext)
{
    HANDLE hProcess;
    switch (dwControl)
    {
    case SERVICE_CONTROL_STOP:
    {
        servicestatus.dwWin32ExitCode = 0;
        servicestatus.dwCurrentState = SERVICE_STOPPED;
        SetServiceStatus(hstatus, &servicestatus);

        std::map<DWORD, DWORD>::iterator it;
        for (it = mapSessionIDProcessID.begin(); it != mapSessionIDProcessID.end(); ++it)
        {
            hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, it->second);
            TerminateProcess(hProcess, 0);
        }

        logger("Если идет запуск УТМ, после остановки службы могут пропасть ридеры (рутокены)", "INFO");
        logger("Служба остановлена SERVICE_CONTROL_STOP", "INFO");

        break;
    }
    case SERVICE_CONTROL_SHUTDOWN:
    {
        servicestatus.dwWin32ExitCode = 0;
        servicestatus.dwCurrentState = SERVICE_STOPPED;
        SetServiceStatus(hstatus, &servicestatus);

        std::map<DWORD, DWORD>::iterator it;
        for (it = mapSessionIDProcessID.begin(); it != mapSessionIDProcessID.end(); ++it)
        {
            hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, it->second);
            TerminateProcess(hProcess, 0);
        }

        logger("Если идет запуск УТМ, после остановки службы могут пропасть ридеры (рутокены)", "INFO");
        logger("Служба остановлена SERVICE_CONTROL_SHUTDOWN", "INFO");

        break;
    }
    case SERVICE_CONTROL_SESSIONCHANGE: // контроль входа/выхода пользователя
    {
        if (dwEventType == WTS_SESSION_LOGOFF)
        {
            WTSSESSION_NOTIFICATION* pSessionNotification = (WTSSESSION_NOTIFICATION*)lpEventData;
            logger("Служба сообщение SERVICE_CONTROL_SESSIONCHANGE - WTS_SESSION_LOGOFF - sessionID - " + std::to_string(pSessionNotification->dwSessionId), "INFO");
            logger("Процесс в сессии " + std::to_string(pSessionNotification->dwSessionId) + " завершен", "INFO");
        }
        if (dwEventType == WTS_SESSION_LOGON)
        {
            WTSSESSION_NOTIFICATION* pSessionNotification = (WTSSESSION_NOTIFICATION*)lpEventData;
            logger("Служба сообщение SERVICE_CONTROL_SESSIONCHANGE - WTS_SESSION_LOGON - sessionID - " + std::to_string(pSessionNotification->dwSessionId), "INFO");

            // ждем запуска explorer.exe, и запускаем свое приложение
            int result = 0;
            while (true)
            {
                result = checkExplorerExe();
                if (result == 2)
                {
                    logger("Ошибка WTSEnumerateProcesses - " + std::to_string(GetLastError()), "ERROR");
                    break;
                }
                if (result == 1)
                {
                    Sleep(3000); // чтобы explorer.exe отдуплился
                    if (startProcessTokenServiceEnvUser(pSessionNotification->dwSessionId) == 1)
                    {
                        logger("Процесс в сессии " + std::to_string(pSessionNotification->dwSessionId) + " не запущен!", "ERROR");
                        break;
                    }
                    logger("Процесс в сессии " + std::to_string(pSessionNotification->dwSessionId) + " запущен", "INFO");
                    break;
                }
                Sleep(1000);
            }
        }

        break;
    }
    default:
    {
        break;
    }
    }

    SetServiceStatus(hstatus, &servicestatus);

    return 0;
}

void WINAPI ServiceMain(int argc, char** argv)
{
    if (openLogFile())
    {
        return;
    }

    servicestatus.dwServiceType = SERVICE_WIN32;
    servicestatus.dwCurrentState = SERVICE_START_PENDING;
    servicestatus.dwControlsAccepted = SERVICE_ACCEPT_SHUTDOWN | SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SESSIONCHANGE;
    servicestatus.dwWin32ExitCode = 0;
    servicestatus.dwServiceSpecificExitCode = 0;
    servicestatus.dwCheckPoint = 0;
    servicestatus.dwWaitHint = 0;

    hstatus = RegisterServiceCtrlHandlerEx("2UTM_service", ServiceHandlerEx, 0);

    if (hstatus == 0)
    {
        logger("Ошибка RegisterServiceCtrlHandler - " + std::to_string(GetLastError()), "ERROR");
        return;
    }
    logger("Успешно RegisterServiceCtrlHandler", "INFO");

    // Сообщаем о состоянии выполнения в SCM
    servicestatus.dwCurrentState = SERVICE_RUNNING;
    SetServiceStatus(hstatus, &servicestatus);

    // если есть активный сеанс, то запускаем процесс
    WTS_SESSION_INFO* pSessionInfo = NULL;
    DWORD count = 0;
    DWORD users = 0;

    if (WTSEnumerateSessions(WTS_CURRENT_SERVER_HANDLE, 0, 1, &pSessionInfo, &count) == 0)
    {
        logger("Ошибка WTSEnumerateSessions - " + std::to_string(GetLastError()), "ERROR");
        return;
    }
    for (DWORD i = 0; i < count; ++i)
    {
        if (pSessionInfo[i].State == WTSActive)
        {
            if (startProcessTokenServiceEnvUser(pSessionInfo[i].SessionId) == 1)
            {
                logger("Процесс в сессии " + std::to_string(pSessionInfo[i].SessionId) + " не запущен!", "ERROR");
                break;
            }
            logger("Процесс в сессии " + std::to_string(pSessionInfo[i].SessionId) + " запущен", "INFO");
            break;
        }
    }

    WTSFreeMemory(pSessionInfo);
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // запуск службы
    if (wcscmp(lpCmdLine, L"-service") == 0)
    {
        SERVICE_TABLE_ENTRYA entrytable[2];

        entrytable[0].lpServiceName = (LPSTR)"2UTM_service";
        entrytable[0].lpServiceProc = (LPSERVICE_MAIN_FUNCTIONA)ServiceMain;
        entrytable[1].lpServiceName = NULL;
        entrytable[1].lpServiceProc = NULL;

        StartServiceCtrlDispatcherA(entrytable);

        return 0;
    }

    // запуск приложения из под службы
    if (wcscmp(lpCmdLine, L"-service_run") == 0)
    {
        flagServiceRun = true;
    }
    
    // Инициализация глобальных строк
    LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadString(hInstance, IDC_MY2UTMVS, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance(hInstance, nCmdShow))
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

    return (int)msg.wParam;
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

    // Если минор равен 1, это вин 7
    if (dwMinorVersionOS == 1)
    {
        // Создаем листбокс с токенами
        hListBoxTokensGlobal = CreateWindowEx(0, WC_LISTVIEW, "Tokens ListBox", WS_VISIBLE | WS_BORDER | WS_CHILD | LVS_REPORT |
            LVS_NOCOLUMNHEADER | LVS_SINGLESEL | LVS_SORTASCENDING, 1, 26, 381, 215, hWndMain, (HMENU)MainWidgetId::LISTVIEW_TOKENS, NULL, NULL);

        // Создаем листбокс с утм и службами
        hListBoxUTMServiceGlobal = CreateWindowEx(0, WC_LISTVIEW, "Tokens UTM Service", WS_VISIBLE | WS_BORDER | WS_CHILD | LVS_REPORT |
            LVS_NOCOLUMNHEADER | LVS_SINGLESEL | LVS_SORTASCENDING, 1, 26, 381, 472, hWndMain, (HMENU)MainWidgetId::LISTVIEW_UTM_SERVICE, NULL, NULL);

        // Поле для информации о токене
        hEditInfoTokens = CreateWindowEx(0, "Edit", 0, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY | ES_MULTILINE | WS_VSCROLL, 1, 245, 381, 252, hWndMain, NULL, NULL, NULL);

        // Статусбар
        hStatusBar = CreateWindowEx(0, "Static", " ", WS_CHILD | WS_VISIBLE | SS_LEFT, 1, 498, 381, 20, hWndMain, NULL, NULL, NULL);

        // Прогрессбар
        hProgressBar = CreateWindowEx(0, PROGRESS_CLASS, NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | PBS_MARQUEE, 6, 215, 370, 30, hWndMain, NULL, NULL, NULL);
    }
    else // иначе вин 10
    {
        // Создаем листбокс с токенами
        hListBoxTokensGlobal = CreateWindowEx(0, WC_LISTVIEW, "Tokens ListBox", WS_VISIBLE | WS_BORDER | WS_CHILD | LVS_REPORT |
            LVS_NOCOLUMNHEADER | LVS_SINGLESEL | LVS_SORTASCENDING, 1, 26, 371, 210, hWndMain, (HMENU)MainWidgetId::LISTVIEW_TOKENS, NULL, NULL);

        // Создаем листбокс с утм и службами
        hListBoxUTMServiceGlobal = CreateWindowEx(0, WC_LISTVIEW, "Tokens UTM Service", WS_VISIBLE | WS_BORDER | WS_CHILD | LVS_REPORT |
            LVS_NOCOLUMNHEADER | LVS_SINGLESEL | LVS_SORTASCENDING, 1, 26, 371, 457, hWndMain, (HMENU)MainWidgetId::LISTVIEW_UTM_SERVICE, NULL, NULL);

        // Поле для информации о токене
        hEditInfoTokens = CreateWindowEx(0, "Edit", 0, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY | ES_MULTILINE | WS_VSCROLL, 1, 245, 371, 238, hWndMain, NULL, NULL, NULL);

        // Статусбар
        hStatusBar = CreateWindowEx(0, "Static", " ", WS_CHILD | WS_VISIBLE | SS_LEFT, 1, 484, 371, 20, hWndMain, NULL, NULL, NULL);

        // Прогрессбар
        hProgressBar = CreateWindowEx(0, PROGRESS_CLASS, NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | PBS_MARQUEE, 6, 215, 360, 30, hWndMain, NULL, NULL, NULL);
    }

    // Иконки для листвью
    HIMAGELIST hSmall = ImageList_Create(16, 16, ILC_COLOR32, 4, 0);
    ImageList_AddIcon(hSmall, hIconToken);
    ImageList_AddIcon(hSmall, hIconServiceRun);
    ImageList_AddIcon(hSmall, hIconServiceStop);
    ImageList_AddIcon(hSmall, hIconServiceWarning);
    ListView_SetImageList(hListBoxTokensGlobal, hSmall, LVSIL_SMALL);
    ListView_SetImageList(hListBoxUTMServiceGlobal, hSmall, LVSIL_SMALL);

    CreateColumn(hListBoxTokensGlobal, 1, 0, 370);
    CreateColumn(hListBoxUTMServiceGlobal, 1, 0, 370);
    ListView_SetExtendedListViewStyle(hListBoxTokensGlobal, LVS_EX_FULLROWSELECT);
    ListView_SetExtendedListViewStyle(hListBoxUTMServiceGlobal, LVS_EX_FULLROWSELECT | LVS_EX_INFOTIP);

    checkServiceUTM(); // получаем инфу о утм и службах, заполняем листбокс
    checkServiceTimer = SetTimer(hWndMain, TIMER_CHECK_SERVICE, 60000, NULL);

    if (!hWndMain || !hListBoxTokensGlobal || !hEditInfoTokens || !hStatusBar || !hProgressBar || !hListBoxUTMServiceGlobal || !checkServiceTimer)
    {
        logger("Не удалось создать окно!", "ERROR");
        return FALSE;
    }

    ShowWindow(hProgressBar, SW_HIDE);
    ShowWindow(hListBoxUTMServiceGlobal, SW_HIDE);

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
    SendMessage(hListBoxUTMServiceGlobal, WM_SETFONT, (WPARAM)font, TRUE);
    SendMessage(hEditInfoTokens, WM_SETFONT, (WPARAM)font, TRUE);
    SendMessage(hStatusBar, WM_SETFONT, (WPARAM)font, TRUE);

    if (!flagServiceRun)
    {
        ShowWindow(hWndMain, SW_SHOW);
        UpdateWindow(hWndMain);
    }
    
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
    // Это сообщение отправляеться при перезапуске explorer.exe
    if (message == WM_TASKBARCREATED)
    {
        Shell_NotifyIcon(NIM_ADD, &pnid); // отправка сообщения - добавить иконку в трей
    }

    // Скрываем каретку в полях ввода
    HideCaret(hEditInfoTokens);

    switch (message)
    {
        case WM_TIMER:
        {
            if (wParam == TIMER_CHECK_SERVICE)
            {
                std::thread thr(&checkServiceUTM);
                thr.detach();
            }

            break;
        }

        case WM_CREATE:
        {
            // Регистрация пользовательского сообщения о создании панели задач от explorer.exe
            WM_TASKBARCREATED = RegisterWindowMessage("TaskbarCreated");

            OnCreate(hWnd, (CREATESTRUCT*)lParam); // создаем вкладки

            // Сбор данных
            workCollectDevice();

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
                break;
            }

            createTrayWindow(hWnd);

            std::string autostart;
            int err = readConfigAutoStart(autostart);
            if (err == 1)
            {
                logger("Не удалось прочитать конфиг! Нет поля autostart. Код ошибки - " + std::to_string(GetLastError()), "ERROR");
            }
            if (autostart == "true")
            {
                // ставим галочку на пункт меню
                MENUITEMINFO lmii = { 0 };
                lmii.cbSize = sizeof(MENUITEMINFO);
                lmii.fMask = MIIM_STATE;
                HMENU hmenu = GetMenu(hWnd);
                GetMenuItemInfo(hmenu, ID_AUTOSTART_UTM, FALSE, &lmii);
                lmii.fState = MF_CHECKED;
                SetMenuItemInfo(hmenu, ID_AUTOSTART_UTM, FALSE, &lmii);

                // автозапуск утм через сервис
                if (flagServiceRun)
                {
                    // Проверка на существование конфига
                    if (configExists() == false)
                    {
                        std::string error = "Файл конфига не найден!";
                        logger(error, "ERROR");
                        setStatusBar(error);
                        break;
                    }

                    // проверка на уже установленные утм
                    int err = checkUTM();
                    if (err == 1)
                    {
                        std::string error = "Установленные УТМы не найдены!";
                        logger(error, "ERROR");
                        setStatusBar(error);
                        break;
                    }

                    // запускаем утмы
                    std::thread thr(&startUTM);
                    thr.detach();

                    flagAutostartUTM = true;
                }
            }
            else
            {
                // убираем галочку с пункта меню
                MENUITEMINFO lmii = { 0 };
                lmii.cbSize = sizeof(MENUITEMINFO);
                lmii.fMask = MIIM_STATE;
                HMENU hmenu = GetMenu(hWnd);
                GetMenuItemInfo(hmenu, ID_AUTOSTART_UTM, FALSE, &lmii);
                lmii.fState = MF_UNCHECKED;
                SetMenuItemInfo(hmenu, ID_AUTOSTART_UTM, FALSE, &lmii);
            }
        }
        case WM_DEVICECHANGE: // событие изменения устройств
        {
            PDEV_BROADCAST_DEVICEINTERFACE b = (PDEV_BROADCAST_DEVICEINTERFACE)lParam;

            switch (wParam)
            {
                // события подключение и отключения устройств
                case 0:
                case DBT_DEVICEARRIVAL:
                case DBT_DEVICEREMOVECOMPLETE:
                {
                    if (flagAutostartUTM == false) // если не идет автозапуск УТМов, обновляем данные
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

        case TRAY_ICON: // сообщение от иконки в трее
        {
            switch (lParam)
            {

            case WM_LBUTTONDOWN: // клик левой кнопкой мыши в трее
            {
                ShowWindow(hWnd, SW_NORMAL); // показать главное окно
                SetForegroundWindow(hWnd);
            }
            break;

            case WM_RBUTTONDOWN: //клик правой кнопкой мыши в трее
            {
                // Извлекаем координаты курсора мыши из lParam
                POINT pt;
                GetCursorPos(&pt);
                POINT ptLocal = pt;

                // Если не сделать окно трея активным, меню не пропадет
                SetForegroundWindow(pnid.hWnd);

                // Создаем меню
                HMENU hPopupMenu = CreatePopupMenu();
                AppendMenu(hPopupMenu, MF_STRING, MainWidgetId::TRAY_SHOW2UTM, "Показать 2UTM");
                AppendMenu(hPopupMenu, MF_SEPARATOR, 0, 0);
                AppendMenu(hPopupMenu, MF_STRING, MainWidgetId::TRAY_INSTALL_SERVICE, "Установить службу");
                AppendMenu(hPopupMenu, MF_STRING, MainWidgetId::TRAY_DELETE_SERVICE, "Удалить службу");
                AppendMenu(hPopupMenu, MF_SEPARATOR, 0, 0);
                AppendMenu(hPopupMenu, MF_STRING, MainWidgetId::TRAY_EXIT, "Выход");

                // Отображаем меню
                TrackPopupMenu(hPopupMenu, TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y, 0, hWnd, NULL);

                // Уничтожаем меню
                DestroyMenu(hPopupMenu);
            }
            break;

            }

            break;
        }

        case WM_COMMAND:
        {
            // События трея и листбокса
            switch ((MainWidgetId)LOWORD(wParam))
            {
                case MainWidgetId::TRAY_SHOW2UTM:
                {
                    ShowWindow(hWnd, SW_NORMAL);

                    break;
                }

                case MainWidgetId::TRAY_INSTALL_SERVICE:
                {
                    if (flagAutostartUTM)
                    {
                        MessageBox(hWnd, "Идет запуск УТМ, пожалуйста подождите...", "Внимание", MB_ICONINFORMATION);
                        break;
                    }

                    if (InstallService2UTM() == -2)
                    {
                        MessageBox(hWnd, "Служба уже установлена", "Внимание", MB_ICONINFORMATION);
                        break;
                    }

                    MessageBox(hWnd, "Будет установлена служба 2UTM_service и перезапущена программа", "Внимание", MB_ICONINFORMATION);

                    SCardReleaseContext(ctxCard);
                    Shell_NotifyIcon(NIM_DELETE, &pnid); // отправка сообщения - убрать иконку из трея

                    startService2UTM();

                    PostQuitMessage(0);

                    break;
                }

                case MainWidgetId::TRAY_DELETE_SERVICE:
                {
                    if (flagAutostartUTM)
                    {
                        MessageBox(hWnd, "Идет запуск УТМ, пожалуйста подождите...", "Внимание", MB_ICONINFORMATION);
                        break;
                    }

                    if (RemoveService2UTM() == -2)
                    {
                        MessageBox(hWnd, "Служба уже удалена", "Внимание", MB_ICONINFORMATION);
                        break;
                    }

                    MessageBox(hWnd, "Будет удалена служба 2UTM_service и завершена программа", "Внимание", MB_ICONINFORMATION);

                    SCardReleaseContext(ctxCard);
                    Shell_NotifyIcon(NIM_DELETE, &pnid); // отправка сообщения - убрать иконку из трея
                    PostQuitMessage(0);

                    break;
                }

                case MainWidgetId::TRAY_EXIT:
                {
                    if (flagAutostartUTM)
                    {
                        MessageBox(hWnd, "Идет запуск УТМ, пожалуйста подождите...", "Внимание", MB_ICONINFORMATION);
                        break;
                    }

                    SCardReleaseContext(ctxCard);
                    Shell_NotifyIcon(NIM_DELETE, &pnid); // отправка сообщения - убрать иконку из трея
                    if (flagServiceRun)
                    {
                        stopService2UTM();
                    }
                    else
                    {
                        PostQuitMessage(0);
                    }

                    break;
                }
            }

            int wmId = LOWORD(wParam);

            // Выбор в меню:
            switch (wmId)
            {
                case ID_AUTOSTART_UTM:
                {
                    std::string autostart;
                    int err = readConfigAutoStart(autostart);
                    if (err == 1)
                    {
                        err = GetLastError();
                        if (err == 2)
                        {
                            autostart = "false";
                        }
                        else
                        {
                            logger("Не удалось прочитать конфиг! Код ошибки - " + std::to_string(err), "ERROR");
                            break;
                        }
                    }
                    if (autostart == "true")
                    {
                        err = writeConfigAutoStart("false");
                        if (err == 1)
                        {
                            logger("Не удалось записать конфиг! Код ошибки - " + std::to_string(GetLastError()), "ERROR");
                            break;
                        }

                        // убираем галочку с пункта меню
                        MENUITEMINFO lmii = { 0 };
                        lmii.cbSize = sizeof(MENUITEMINFO);
                        lmii.fMask = MIIM_STATE;
                        HMENU hmenu = GetMenu(hWnd);
                        GetMenuItemInfo(hmenu, ID_AUTOSTART_UTM, FALSE, &lmii);
                        lmii.fState = MF_UNCHECKED;
                        SetMenuItemInfo(hmenu, ID_AUTOSTART_UTM, FALSE, &lmii);
                    }
                    else
                    {
                        err = writeConfigAutoStart("true");
                        if (err == 1)
                        {
                            logger("Не удалось записать конфиг! Код ошибки - " + std::to_string(GetLastError()), "ERROR");
                            break;
                        }

                        // ставим галочку на пункта меню
                        MENUITEMINFO lmii = { 0 };
                        lmii.cbSize = sizeof(MENUITEMINFO);
                        lmii.fMask = MIIM_STATE;
                        HMENU hmenu = GetMenu(hWnd);
                        GetMenuItemInfo(hmenu, ID_AUTOSTART_UTM, FALSE, &lmii);
                        lmii.fState = MF_CHECKED;
                        SetMenuItemInfo(hmenu, ID_AUTOSTART_UTM, FALSE, &lmii);
                    }

                    break;
                }

                case IDM_ABOUT:
                {
                    DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                    break;
                }

                case ID_REREAD_SERVICES: // перечитать службы УТМ
                {
                    if (flagCheckServices == true) // если проверка уже идет прерываем
                    {
                        break;
                    }

                    flagCheckServices = true;

                    // Открываем прогрессбар и блокируем главное окно
                    ShowWindow(hProgressBar, SW_SHOW);
                    SendMessage(hProgressBar, (UINT)PBM_SETMARQUEE, (WPARAM)1, NULL);
                    EnableWindow(hWnd, FALSE);

                    // Перечитываем службы УТМ
                    std::thread thr(&checkServiceUTM);
                    thr.detach();

                    break;
                }

                case ID_REREAD_DEVICES: // перечитать устройства
                {
                    if (flagAutostartUTM)
                    {
                        MessageBox(hWnd, "Идет запуск УТМ, пожалуйста подождите...", "Внимание", MB_ICONINFORMATION);
                        break;
                    }

                    // Открываем прогрессбар и блокируем главное окно
                    ShowWindow(hProgressBar, SW_SHOW);
                    SendMessage(hProgressBar, (UINT)PBM_SETMARQUEE, (WPARAM)1, NULL);
                    EnableWindow(hWnd, FALSE);

                    // Сбор данных
                    indexToken = -1;
                    setStatusBar("Выполняется обновление устройств...");
                    std::thread thr(&workCollectDevice);
                    thr.detach();
                    SetWindowTextA(hEditInfoTokens, ""); // очищаем поле информации
                    break;
                }

                case ID_INSTALL_UTM: // установить УТМы
                {
                    if (flagAutostartUTM)
                    {
                        MessageBox(hWnd, "Идет запуск УТМ, пожалуйста подождите...", "Внимание", MB_ICONINFORMATION);
                        break;
                    }

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
                    if (flagAutostartUTM)
                    {
                        MessageBox(hWnd, "Идет запуск УТМ, пожалуйста подождите...", "Внимание", MB_ICONINFORMATION);
                        break;
                    }

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
                    if (flagAutostartUTM)
                    {
                        MessageBox(hWnd, "Идет запуск УТМ, пожалуйста подождите...", "Внимание", MB_ICONINFORMATION);
                        break;
                    }

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
                    flagStartUTM = true;
                    std::thread thr(&startUTM);
                    thr.detach();

                    break;
                }

                case ID_STOP_UTM: // остановить УТМы
                {
                    if (flagAutostartUTM)
                    {
                        MessageBox(hWnd, "Идет запуск УТМ, пожалуйста подождите...", "Внимание", MB_ICONINFORMATION);
                        break;
                    }

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
                    flagStopUTM = true;
                    std::thread thr(&stopUTM);
                    thr.detach();

                    break;
                }

                case ID_INSTALL_SERVICE:
                {
                    if (flagAutostartUTM)
                    {
                        MessageBox(hWnd, "Идет запуск УТМ, пожалуйста подождите...", "Внимание", MB_ICONINFORMATION);
                        break;
                    }

                    if (InstallService2UTM() == -2)
                    {
                        MessageBox(hWnd, "Служба уже установлена", "Внимание", MB_ICONINFORMATION);
                        break;
                    }

                    MessageBox(hWnd, "Будет установлена служба 2UTM_service и перезапущена программа", "Внимание", MB_ICONINFORMATION);

                    SCardReleaseContext(ctxCard);
                    Shell_NotifyIcon(NIM_DELETE, &pnid); // отправка сообщения - убрать иконку из трея

                    startService2UTM();

                    PostQuitMessage(0);

                    break;
                }

                case ID_DELETE_SERVICE:
                {
                    if (flagAutostartUTM)
                    {
                        MessageBox(hWnd, "Идет запуск УТМ, пожалуйста подождите...", "Внимание", MB_ICONINFORMATION);
                        break;
                    }

                    if (RemoveService2UTM() == -2)
                    {
                        MessageBox(hWnd, "Служба уже удалена", "Внимание", MB_ICONINFORMATION);
                        break;
                    }

                    MessageBox(hWnd, "Будет удалена служба 2UTM_service и завершена программа", "Внимание", MB_ICONINFORMATION);

                    SCardReleaseContext(ctxCard);
                    Shell_NotifyIcon(NIM_DELETE, &pnid); // отправка сообщения - убрать иконку из трея
                    PostQuitMessage(0);

                    break;
                }

                case ID_SHOW_DEVICES_NO_CONTEXT: // показать устройства вне контекста
                {
                    if (flagAutostartUTM)
                    {
                        MessageBox(hWnd, "Идет запуск УТМ, пожалуйста подождите...", "Внимание", MB_ICONINFORMATION);
                        break;
                    }

                    DialogBox(hInst, MAKEINTRESOURCE(IDD_READERS_NON_CONTEXT), hWnd, WndNoContextProc);
                    break;
                }

                case IDM_EXIT:
                {
                    if (flagAutostartUTM)
                    {
                        MessageBox(hWnd, "Идет запуск УТМ, пожалуйста подождите...", "Внимание", MB_ICONINFORMATION);
                        break;
                    }

                    SCardReleaseContext(ctxCard);
                    Shell_NotifyIcon(NIM_DELETE, &pnid); // отправка сообщения - убрать иконку из трея
                    if (flagServiceRun)
                    {
                        stopService2UTM();
                    }
                    else
                    {
                        PostQuitMessage(0);
                    }

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
        case WM_CLOSE:
        {
            ShowWindow(hWnd, SW_HIDE);

            // Показать сообщение в трее только один раз (первый)
            if (flagShowMessageTray == false)
            {
                Shell_NotifyIcon(NIM_DELETE, &pnid); // убрать иконку из трея
                showMessageInTray(); // сообщение в трее
                Shell_NotifyIcon(NIM_ADD, &pnid); // добавить иконку в трей
                flagShowMessageTray = true;
            }

            break;
        }

        // регулируем расположение вкладок
        case WM_SIZE:
        {
            OnSize(hWnd, LOWORD(lParam), HIWORD(lParam), (UINT)wParam);
            break;
        }

        // Обрабатываем переключение вкладки
        // break в конце не нужен
        case WM_NOTIFY:
        {
            if (((LPNMHDR)lParam)->hwndFrom == hListBoxTokensGlobal)
            {
                switch (((LPNMHDR)lParam)->code)
                {
                    case NM_CLICK:
                    {
                        if (flagAutostartUTM)
                        {
                            MessageBox(hWnd, "Идет запуск УТМ, пожалуйста подождите...", "Внимание", MB_ICONINFORMATION);
                            break;
                        }

                        NM_LISTVIEW* lst = (NM_LISTVIEW*)lParam;

                        // Если кликнули просто так, то ниче не делаем
                        std::vector<CHAR> vec(MAX_PATH);
                        ListView_GetItemText(hListBoxTokensGlobal, lst->iItem, lst->iSubItem, &vec.front(), MAX_PATH);
                        std::string buff(&vec.front());
                        if (buff.empty())
                        {
                            break;
                        }

                        // Открываем прогрессбар и блокируем главное окно
                        ShowWindow(hProgressBar, SW_SHOW);
                        SendMessage(hProgressBar, (UINT)PBM_SETMARQUEE, (WPARAM)1, NULL);
                        EnableWindow(hWnd, FALSE);

                        // читаем выбранный токен
                        std::thread thr(&wraperReadDevice, lst->iItem);
                        thr.detach();

                        break;
                    }
                }
            }

            switch (((LPNMHDR)lParam)->code)
            {
                // подсказка в листвью с путем
                case LVN_GETINFOTIP:
                {
                    LPNMLVGETINFOTIP GetInfoTip = (LPNMLVGETINFOTIP)lParam;
                    strcpy(GetInfoTip->pszText, vecPathService[GetInfoTip->iItem].c_str());

                    break;
                }

                // break в конце не нужен
                case TCN_SELCHANGE:
                {
                    int tab = TabCtrl_GetCurSel(((LPNMHDR)lParam)->hwndFrom); // получаем номер активной вкладки
                    // В зависимости от вкладки показываем нужные контролы
                    if (tab == 0) // рутокены
                    {
                        showOrHideOption(0);
                    }
                    if (tab == 1) // службы утм
                    {
                        showOrHideOption(1);
                    }
                }
            }
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

    // Удаляем все строки в листвью контактов
    ListView_DeleteAllItems(hListBoxTokensGlobal);

    // Заполняем
    for (int i = 0; i < vecReader.size(); ++i)
    {
        std::string s = "      " + vecReader[i]; // пробелы для красоты
        CreateItem(hListBoxTokensGlobal, (CHAR*)s.c_str(), i, 0);
    }

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
        // Удаляем все строки в листвью контактов
        ListView_DeleteAllItems(hListBoxTokensGlobal);
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
        // Удаляем все строки в листвью контактов
        ListView_DeleteAllItems(hListBoxTokensGlobal);
        // Убираем прогрессбар и разблокируем главное окно
        ShowWindow(hProgressBar, SW_HIDE);
        EnableWindow(hWndMain, TRUE);
        return 2;
    }

    // Получаем список ридеров
    err = getListDevice(vecRutokens, ctxCard);
    if (err == 1)
    {
        error = "Не найдено подключенных смарткарт";
        setStatusBar(error);
        logger(error, "ERROR");
        // Удаляем все строки в листвью контактов
        ListView_DeleteAllItems(hListBoxTokensGlobal);
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
        // Удаляем все строки в листвью контактов
        ListView_DeleteAllItems(hListBoxTokensGlobal);
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
        // Удаляем все строки в листвью контактов
        ListView_DeleteAllItems(hListBoxTokensGlobal);
        // Убираем прогрессбар и разблокируем главное окно
        ShowWindow(hProgressBar, SW_HIDE);
        EnableWindow(hWndMain, TRUE);
        return 5;
    }

    // пишем в лог найденные токены
    std::string log = "Найденные токены: ";
    for (std::string elem : vecRutokens)
    {
        log += elem + ", ";
    }
    logger(log, "INFO");

    if (fillListBoxRutokens(vecRutokens) != 0)
    {
        error = "Не удалось заполнить листбокс";
        setStatusBar(error);
        logger(error, "ERROR");
        // Удаляем все строки в листвью контактов
        ListView_DeleteAllItems(hListBoxTokensGlobal);
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

    hFind = FindFirstFile("c:\\UTM", &FindFileData);
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

    std::string countUTMstr;
    if (readConfigCountUTM(countUTMstr) == 1)
    {
        flagAutostartUTM = false;
        flagStartUTM = false;
        error = "Чтение конфига завершилась ошибкой! Код ошибки - " + std::to_string(GetLastError());
        logger(error, "ERROR");
        MessageBox(hWndMain, error.c_str(), "Ошибка", MB_ICONERROR);
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
        flagAutostartUTM = false;
        flagStartUTM = false;
        error = "Чтение конфига завершилась ошибкой! Код ошибки - " + std::to_string(GetLastError());
        logger(error, "ERROR");
        MessageBox(hWndMain, error.c_str(), "Ошибка", MB_ICONERROR);
        setStatusBar(error);
        // Скрываем прогрессбар и разблокируем главное окно
        ShowWindow(hProgressBar, SW_HIDE);
        EnableWindow(hWndMain, TRUE);
        return 1;
    }

    if (vecRutokens.size() < vectorNameReaders.size())
    {
        flagAutostartUTM = false;
        flagStartUTM = false;
        error = "Активных токенов меньше, чем в конфиге.\nПереустановите УТМы, или добавьте активных токенов";
        logger(error, "ERROR");
        MessageBox(hWndMain, error.c_str(), "Ошибка", MB_ICONERROR);
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
        flagAutostartUTM = false;
        flagStartUTM = false;
        error = "Некоторые активные токены отсутсвуют в конфиге.\nПереустановите УТМы, или исправьте активные токены";
        logger(error, "ERROR");
        MessageBox(hWndMain, error.c_str(), "Ошибка", MB_ICONERROR);
        setStatusBar(error);
        // Скрываем прогрессбар и разблокируем главное окно
        ShowWindow(hProgressBar, SW_HIDE);
        EnableWindow(hWndMain, TRUE);
        return 1;
    }

    // Если 1 УТМ
    if (ports.size() == 1)
    {
        setStatusBar("Запуск Transport");
        err = startServiceUTM("Transport");
        if (err != 0)
        {
            flagAutostartUTM = false;
            flagStartUTM = false;
            error = "Ошибка запуска службы Trasport! Код ошибки - " + std::to_string(GetLastError()) + "\nБудет выполнена отмена изменений";
            logger(error, "ERROR");
            MessageBox(hWndMain, error.c_str(), "Ошибка", MB_ICONERROR);
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
        err = checkHomePageUTM(ports[0], flagStartUTM, DEFAULT_TIMEOUT);
        if (err != 0)
        {
            flagAutostartUTM = false;
            flagStartUTM = false;
            error = "Не удалось проверить работу УТМ! Код ошибки - " + std::to_string(err) + "\nФункция завершилась по таймауту" + "\nБудет выполнена отмена изменений";
            logger(error, "ERROR");
            MessageBox(hWndMain, error.c_str(), "Ошибка", MB_ICONERROR);
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

        unsigned int vecRutokensSize = vecRutokens.size(); // запоминаем кол во токенов, можеты быть гонка
        for (std::string elem : vecRutokens)
        {
            err = deleteReader(elem, ctxCard);
            if (err != 0)
            {
                flagAutostartUTM = false;
                flagStartUTM = false;
                error = "Удаление всех ридеров завершилось ошибкой! Код ошибки - " + std::to_string(err) + "\nБудет выполнена отмена изменений";
                logger(error, "ERROR");
                MessageBox(hWndMain, error.c_str(), "Ошибка", MB_ICONERROR);
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
        unsigned int countReader = 1;
        for (unsigned int i = 0; i < vecTokensNoChoose.size(); ++i, ++countReader)
        {
            nameReader = "Aktiv Rutoken ECP " + std::to_string(vecRutokensSize - countReader);
            logger("Добавляем ридер " + nameReader, "INFO");
            err = addNameReader(nameReader, ctxCard, vecAttrNoChoose[i]);
            if (err != 0)
            {
                flagAutostartUTM = false;
                flagStartUTM = false;
                error = "Добавление ридера " + nameReader + " завершилось ошибкой! Код ошибки - " + std::to_string(err) + "\nБудет выполнена отмена изменений";
                logger(error, "ERROR");
                MessageBox(hWndMain, error.c_str(), "Ошибка", MB_ICONERROR);
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
            logger("vecRutokens.size() - " + std::to_string(vecRutokensSize), "INFO");
            logger("countReader - " + std::to_string(countReader), "INFO");

            // добавление ридера
            nameReader = "Aktiv Rutoken ECP " + std::to_string(vecRutokensSize - countReader);
            logger("Добавляем ридер " + nameReader, "INFO");
            err = addNameReader(nameReader, ctxCard, vecActualAttrReader[i]);
            if (err != 0)
            {
                flagAutostartUTM = false;
                flagStartUTM = false;
                error = "Добавление ридера " + nameReader + " завершилось ошибкой! Код ошибки - " + std::to_string(err) + "\nБудет выполнена отмена изменений";
                logger(error, "ERROR");
                MessageBox(hWndMain, error.c_str(), "Ошибка", MB_ICONERROR);
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
            numberService = std::to_string(vecRutokensSize - countReader + 1);
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
                flagAutostartUTM = false;
                flagStartUTM = false;
                error = "Ошибка запуска службы Transport" + numberService + "! Код ошибки - " + std::to_string(GetLastError()) + "\nБудет выполнена отмена изменений";
                logger(error, "ERROR");
                MessageBox(hWndMain, error.c_str(), "Ошибка", MB_ICONERROR);
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
            err = checkHomePageUTM(ports[i], flagStartUTM, DEFAULT_TIMEOUT);
            if (err != 0)
            {
                flagAutostartUTM = false;
                flagStartUTM = false;
                error = "Не удалось проверить работу УТМ " + numberService + "! Код ошибки - " + std::to_string(err) + "\nФункция завершилась по таймауту" + "\nБудет выполнена отмена изменений";
                logger(error, "ERROR");
                MessageBox(hWndMain, error.c_str(), "Ошибка", MB_ICONERROR);
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

    flagAutostartUTM = false;
    flagStartUTM = false;

    // Сбор данных
    workCollectDevice();
    checkServiceUTM();

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
        flagStopUTM = false;
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
                flagStopUTM = false;
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
            flagStopUTM = false;
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

    flagStopUTM = false;

    checkServiceUTM();

    // Скрываем прогрессбар и разблокируем главное окно
    ShowWindow(hProgressBar, SW_HIDE);
    EnableWindow(hWndMain, TRUE);

    setStatusBar("Выполнено");
    logger("Остановка УТМ успешно завершено", "INFO");

    return 0;
}

// создания иконки в трее
int createTrayWindow(HWND hwnd)
{
    memset(&pnid, 0, sizeof(NOTIFYICONDATA)); // выделяем память на структуру
    pnid.hWnd = hwnd; // указываем хендл окна, куда будут приходить сообщения от трея
    pnid.hIcon = (HICON)LoadImage(GetModuleHandleW(NULL), MAKEINTRESOURCE(IDI_MY2UTMVS), //
        IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR | LR_DEFAULTSIZE);                     // указываем иконку
    pnid.uID = 1; // идетификатор трея
    pnid.uFlags = NIF_ICON | NIF_TIP | NIF_MESSAGE;
    pnid.uCallbackMessage = TRAY_ICON; // сообщение, которое будет прилетать в обработчик событий окна hwnd
    memcpy(pnid.szTip, "2UTM", 5); // имя

    Shell_NotifyIcon(NIM_ADD, &pnid); // отправка сообщения - добавить иконку в трей

    return 0;
}

// показать сообщение в трее
void showMessageInTray()
{
    CoInitialize(NULL);
    IUserNotification* un = NULL;
    if (CoCreateInstance(CLSID_UserNotification, 0, CLSCTX_ALL, IID_IUserNotification, (void**)&un) == S_OK)
    {
        un->SetBalloonInfo(L"Внимание!", L"2UTM все еще работает!", NIIF_INFO);
        un->SetIconInfo(pnid.hIcon, L""); // иконка
        un->SetBalloonRetry(0, 0, 0); // иначе бесконечно появляеться
        un->Show(NULL, 0);
        un->Release();
    }
    CoUninitialize();
}

int InstallService2UTM()
{
    SC_HANDLE hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_CREATE_SERVICE);
    if (!hSCManager)
    {
        logger("Ошибка открытия Service Control Manager - " + std::to_string(GetLastError()), "ERROR");
        return -1;
    }

    // Открываем службу
    HANDLE schService = OpenService(hSCManager, "2UTM_service", SERVICE_ALL_ACCESS);
    if (schService)
    {
        logger("Служба уже установлена", "INFO");
        return -2;
    }

    //Формируем путь до exe с ключем -service для установки и запуска службы
    CHAR PathService[MAX_PATH];
    GetModuleFileName(NULL, PathService, MAX_PATH);
    std::string strPath(PathService);
    strPath.append(" -service");

    SC_HANDLE hService = CreateService(
        hSCManager,
        "2UTM_service",
        "2UTM_service",
        SERVICE_ALL_ACCESS,
        SERVICE_WIN32_OWN_PROCESS,
        SERVICE_AUTO_START,
        SERVICE_ERROR_NORMAL,
        strPath.c_str(),
        NULL, NULL, NULL, NULL, NULL
    );

    if (!hService) {
        int err = GetLastError();
        switch (err) {
        case ERROR_ACCESS_DENIED:
            logger("Ошибка установки службы ERROR_ACCESS_DENIED", "ERROR");
            break;
        case ERROR_CIRCULAR_DEPENDENCY:
            logger("Ошибка установки службы ERROR_CIRCULAR_DEPENDENCY", "ERROR");
            break;
        case ERROR_DUPLICATE_SERVICE_NAME:
            logger("Ошибка установки службы ERROR_DUPLICATE_SERVICE_NAME", "ERROR");
            break;
        case ERROR_INVALID_HANDLE:
            logger("Ошибка установки службы ERROR_INVALID_HANDLE", "ERROR");
            break;
        case ERROR_INVALID_NAME:
            logger("Ошибка установки службы ERROR_INVALID_NAME", "ERROR");
            break;
        case ERROR_INVALID_PARAMETER:
            logger("Ошибка установки службы ERROR_INVALID_PARAMETER", "ERROR");
            break;
        case ERROR_INVALID_SERVICE_ACCOUNT:
            logger("Ошибка установки службы ERROR_INVALID_SERVICE_ACCOUNT", "ERROR");
            break;
        case ERROR_SERVICE_EXISTS:
            logger("Ошибка установки службы ERROR_SERVICE_EXISTS", "ERROR");
            break;
        default:
            logger("Ошибка установки службы Undefined", "ERROR");
        }
        CloseServiceHandle(hSCManager);
        return -1;
    }
    CloseServiceHandle(hService);

    CloseServiceHandle(hSCManager);
    logger("Установка службы успешно завершена", "INFO");
    return 0;
}

int RemoveService2UTM()
{
    SC_HANDLE hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    if (!hSCManager)
    {
        logger("Ошибка открытия Service Control Manager - " + std::to_string(GetLastError()), "ERROR");
        return -1;
    }
    SC_HANDLE hService = OpenService(hSCManager, "2UTM_service", SERVICE_STOP | DELETE);
    if (!hService)
    {
        logger("Ошибка открытия службы - " + std::to_string(GetLastError()), "ERROR");
        CloseServiceHandle(hSCManager);
        return -2;
    }

    DeleteService(hService);
    CloseServiceHandle(hService);
    CloseServiceHandle(hSCManager);
    logger("Удаление службы прошло успешно", "INFO"); // ТЕСТ ЛОГА ЧЕРЕЗ fprintf
    return 0;
}

// Запуск процеса из службы с токеном службы в окружении пользователя
int startProcessTokenServiceEnvUser(DWORD sessionID)
{
    DWORD SessionId = 0;
    HANDLE phTokenService = NULL;
    HANDLE phDuplicateToken = NULL;
    HANDLE phTokenUser = NULL;
    VOID* pEnvBlock = NULL;
    SessionId = WTSGetActiveConsoleSessionId();  // Получаем ИД сессии пользователя
    if (WTSQueryUserToken(SessionId, &phTokenUser) == 0)  // Получаем токен пользователя
    {
        logger("Ошибка WTSQueryUserToken - " + std::to_string(GetLastError()), "ERROR");
        DestroyEnvironmentBlock(pEnvBlock);
        CloseHandle(phTokenService);
        CloseHandle(phDuplicateToken);
        CloseHandle(phTokenUser);
        return 1;
    }

    if (OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY | TOKEN_DUPLICATE | TOKEN_ADJUST_SESSIONID, &phTokenService) == 0)  // Открываем токен нашей службы
    {
        logger("Ошибка OpenProcessToken - " + std::to_string(GetLastError()), "ERROR");
        DestroyEnvironmentBlock(pEnvBlock);
        CloseHandle(phTokenService);
        CloseHandle(phDuplicateToken);
        CloseHandle(phTokenUser);
        return 1;
    }

    if (DuplicateTokenEx(phTokenService, MAXIMUM_ALLOWED, 0, SECURITY_IMPERSONATION_LEVEL::SecurityImpersonation, TOKEN_TYPE::TokenPrimary, &phDuplicateToken) == 0)  // Дублируем токен
    {
        logger("Ошибка DuplicateTokenEx - " + std::to_string(GetLastError()), "ERROR");
        DestroyEnvironmentBlock(pEnvBlock);
        CloseHandle(phTokenService);
        CloseHandle(phDuplicateToken);
        CloseHandle(phTokenUser);
        return 1;
    }

    if (SetTokenInformation(phDuplicateToken, TOKEN_INFORMATION_CLASS::TokenSessionId, &SessionId, sizeof(SessionId)) == 0)  // Устанавливаем дубликат в сессию пользователя
    {
        logger("Ошибка SetTokenInformation - " + std::to_string(GetLastError()), "ERROR");
        DestroyEnvironmentBlock(pEnvBlock);
        CloseHandle(phTokenService);
        CloseHandle(phDuplicateToken);
        CloseHandle(phTokenUser);
        return 1;
    }

    if (CreateEnvironmentBlock(&pEnvBlock, phTokenUser, FALSE) == FALSE)  // Получаем окружение пользователя из токена
    {
        logger("Ошибка CreateEnvironmentBlock - " + std::to_string(GetLastError()), "ERROR");
        DestroyEnvironmentBlock(pEnvBlock);
        CloseHandle(phTokenService);
        CloseHandle(phDuplicateToken);
        CloseHandle(phTokenUser);
        return 1;
    }

    STARTUPINFO StartInfo;  // Структура определяет оконную станцию, рабочий стол, стандартные дескрипторы и внешний вид главного окна для процесса во время создания.
    GetStartupInfo(&StartInfo);  // Извлекает содержимое структуры
    StartInfo.lpDesktop = (LPSTR)"WinSta0\\Default";  // Устанавливаем оконную станцию и рабочий стол
    PROCESS_INFORMATION ProcInfo;  // Структура содержит информацию о вновь созданном процессе и его основном потоке

    bool result;
    CHAR PathExe[MAX_PATH];
    GetModuleFileName(NULL, PathExe, MAX_PATH);
    std::string strExe(PathExe);
    strExe.append(" -service_run");
    // Создаем процесс
    result = CreateProcessAsUser(
        phDuplicateToken, // Токен
        NULL, // Имя приложения.
        (LPSTR)strExe.c_str(), // Путь до исполняемого файла
        NULL, // Атрибуты безопасности процесса.
        NULL, // Атрибуты безопасности потоков.
        FALSE, // Наследовать дескрипторы или нет.
        CREATE_NEW_CONSOLE | CREATE_UNICODE_ENVIRONMENT, // Флаги создания
        pEnvBlock, // Окружение
        NULL, // Текущий каталог.
        &StartInfo,
        &ProcInfo
    );
    
    if (!result)
    {
        logger("Не удалось запустить экземпляр 2UTM - " + std::to_string(GetLastError()), "ERROR");
        CloseHandle(ProcInfo.hProcess);
        CloseHandle(ProcInfo.hThread);
        DestroyEnvironmentBlock(pEnvBlock);
        CloseHandle(phTokenService);
        CloseHandle(phDuplicateToken);
        CloseHandle(phTokenUser);
        return 1;
    }

    mapSessionIDProcessID[sessionID] = ProcInfo.dwProcessId; // запоминаем хендл процесса для завершения при завершении службы

    // Освобождаем память закрываем токены дискрипторы структуры
    CloseHandle(ProcInfo.hProcess);
    CloseHandle(ProcInfo.hThread);
    DestroyEnvironmentBlock(pEnvBlock);
    CloseHandle(phTokenService);
    CloseHandle(phDuplicateToken);
    CloseHandle(phTokenUser);

    return 0;
}

// Запуск службы 2UTM
int startService2UTM()
{
    SC_HANDLE schSCManager, schService;
    SERVICE_STATUS_PROCESS ssStatus;
    DWORD dwBytesNeeded;

    // Открываем менеджер служб
    schSCManager = OpenSCManager(0, 0, SC_MANAGER_ALL_ACCESS);

    // Открываем службу
    schService = OpenService(schSCManager, "2UTM_service", SERVICE_ALL_ACCESS);

    // Запуск службы
    if (StartService(schService, 0, NULL) == 0)
    {
        CloseServiceHandle(schService);
        CloseServiceHandle(schSCManager);
        return 1;
    }

    // Обновляем статус в структуре
    if (QueryServiceStatusEx(schService, SC_STATUS_PROCESS_INFO, (LPBYTE)&ssStatus, sizeof(SERVICE_STATUS_PROCESS), &dwBytesNeeded) == 0)
    {
        CloseServiceHandle(schService);
        CloseServiceHandle(schSCManager);
        return 1;
    }

    // Ждем пока служба запустится
    while (ssStatus.dwCurrentState == SERVICE_START_PENDING)
    {
        Sleep(1000);

        // Обновляем статус в структуре
        if (QueryServiceStatusEx(schService, SC_STATUS_PROCESS_INFO, (LPBYTE)&ssStatus, sizeof(SERVICE_STATUS_PROCESS), &dwBytesNeeded) == 0)
        {
            CloseServiceHandle(schService);
            CloseServiceHandle(schSCManager);
            return 1;
        }
    }

    // Если служба запущена, возвращаем 0, иначе 1
    if (ssStatus.dwCurrentState == SERVICE_RUNNING)
    {
        CloseServiceHandle(schService);
        CloseServiceHandle(schSCManager);
        return 0;
    }
    CloseServiceHandle(schService);
    CloseServiceHandle(schSCManager);
    return 1;
}

// Остановка службы 2UTM
int stopService2UTM()
{
    SC_HANDLE schSCManager, schService;
    SERVICE_STATUS_PROCESS ssStatus;
    DWORD dwBytesNeeded;

    // Открываем менеджер служб
    schSCManager = OpenSCManager(0, 0, SC_MANAGER_ALL_ACCESS);

    // Открываем службу
    schService = OpenService(schSCManager, "2UTM_service", SERVICE_ALL_ACCESS);

    // Останавливаем службу
    SERVICE_STATUS ServiceSt;
    if (ControlService(schService, SERVICE_CONTROL_STOP, &ServiceSt) == 0)
    {
        CloseServiceHandle(schSCManager);
        CloseServiceHandle(schService);
        return 1;
    }

    // Получаем статус службы
    if (QueryServiceStatusEx(schService, SC_STATUS_PROCESS_INFO, (LPBYTE)&ssStatus, sizeof(SERVICE_STATUS_PROCESS), &dwBytesNeeded) == 0)
    {
        CloseServiceHandle(schService);
        CloseServiceHandle(schSCManager);
        return 1;
    }

    // Ждем пока служба остановится
    while (ssStatus.dwCurrentState == SERVICE_STOP_PENDING)
    {
        Sleep(1000);

        // Обновляем статус в структуре
        if (QueryServiceStatusEx(schService, SC_STATUS_PROCESS_INFO, (LPBYTE)&ssStatus, sizeof(SERVICE_STATUS_PROCESS), &dwBytesNeeded) == 0)
        {
            CloseServiceHandle(schService);
            CloseServiceHandle(schSCManager);
            return 1;
        }
    }

    // Если служба остановлена, возвращаем 0, иначе 1
    if (ssStatus.dwCurrentState == SERVICE_STOPPED)
    {
        CloseServiceHandle(schService);
        CloseServiceHandle(schSCManager);
        return 0;
    }
    CloseServiceHandle(schService);
    CloseServiceHandle(schSCManager);
    return 1;
}

// Проверка на существование процесса explorer.exe
int checkExplorerExe()
{
    WTS_PROCESS_INFO* pProcessInfo;
    DWORD NumProcesses;

    if (WTSEnumerateProcesses(WTS_CURRENT_SERVER_HANDLE, 0, 1, &pProcessInfo, &NumProcesses) == 0)
    {
        return 2;
    }

    for (DWORD i = 0; i < NumProcesses; ++i)
    {
        if (std::string(pProcessInfo[i].pProcessName) == "explorer.exe")
        {
            WTSFreeMemory(pProcessInfo);
            return 1;
        }
    }

    WTSFreeMemory(pProcessInfo);
    return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
// Создание вкладок
//////////////////////////////////////////////////////////////////////////////////////////////////////////
// создание окружения для вкладок
void StartCommonControls(DWORD flags)
{
    INITCOMMONCONTROLSEX iccx;
    iccx.dwSize = sizeof(INITCOMMONCONTROLSEX);
    iccx.dwICC = flags;
    InitCommonControlsEx(&iccx);
}

// Создание окна вкладок
HWND CreateTab(const HWND hParent, const HINSTANCE hInst, DWORD dwStyle, const RECT& rc, const int id)
{
    dwStyle |= WS_CHILD | WS_VISIBLE;

    return CreateWindowEx(0,                  //extended styles
        WC_TABCONTROL,     //control 'class' name
        0,                  //control caption
        dwStyle,            //wnd style
        rc.left,            //position: left
        rc.top,             //position: top
        rc.right,           //width
        rc.bottom,          //height
        hParent,            //parent window handle
        (HMENU)id,          //control's ID
        hInst,              //instance
        0);                 //user defined info
}

// Добавление вкладок
int InsertItem(HWND hTc, std::string txt, int item_index, int image_index, UINT mask)
{
    TCITEM tabPage = { 0 };

    tabPage.mask = mask;
    tabPage.pszText = (LPSTR)txt.c_str();
    tabPage.cchTextMax = txt.length();
    tabPage.iImage = image_index;
    SendMessage(hTc, TCM_INSERTITEM, item_index, (LPARAM)&tabPage);

    return 0;
}

// создание окна с вкладками
int OnCreate(const HWND hwnd, CREATESTRUCT* cs)
{
    RECT rc = { 0,0,0,0 };
    StartCommonControls(ICC_TAB_CLASSES);

    hTabControl = CreateTab(hwnd, cs->hInstance, TCS_MULTILINE, rc, MainWidgetId::TAB_CONTOL);

    // Сохраняем дескриптор управления вкладкой как пользовательские данные, связанные с
    // родительским окном, чтобы его можно было извлечь для последующего использования
    SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)hTabControl);

    InsertItem(hTabControl, "Рутокены", 0, 0);
    InsertItem(hTabControl, "УТМы и службы", 1, 1);

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

    SendMessage(hTabControl, WM_SETFONT, (WPARAM)font, TRUE);

    return 0;
}

// регулировка размера и расположения вкладок
void OnSize(const HWND hwnd, int cx, int cy, UINT flags)
{
    // Получаем дескриптор управления заголовком, который ранее был сохранен в пользовательском
    // данные, связанные с родительским окном.
    HWND hTabCntrl = (HWND)(LONG_PTR)GetWindowLongPtr(hwnd, GWLP_USERDATA);

    MoveWindow(hTabCntrl, 1, 1, cx - 1, cy - 18, TRUE);
}

// Показать/скрыть виджеты в определенной влкадке
int showOrHideOption(int tab)
{
    switch (tab)
    {
    case 0: // токены
    {
        ShowWindow(hListBoxTokensGlobal, SW_NORMAL);
        ShowWindow(hEditInfoTokens, SW_NORMAL);
        
        ShowWindow(hListBoxUTMServiceGlobal, SW_HIDE);

        break;
    }
    case 1: // утм и службы
    {
        ShowWindow(hListBoxUTMServiceGlobal, SW_NORMAL);

        ShowWindow(hListBoxTokensGlobal, SW_HIDE);
        ShowWindow(hEditInfoTokens, SW_HIDE);

        break;
    }
    default:
        break;
    }

    return 0;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////

// проверка на наличие служб утм по кол ву из конфига, и запрос на главную страницу
int checkServiceUTM()
{
    logger("Получение информации о службах УТМ", "INFO");

    // получаем кол во утм из конфига
    std::string countUTM;
    if (readConfigCountUTM(countUTM) == 1)
    {
        logger("Ошибка чтения конфига при проверке наличия служб УТМ - " + std::to_string(GetLastError()), "ERROR");
        return 1;
    }
    std::vector<std::string> vecUTMService;
    int intCountUTM = atoi(countUTM.c_str());
    for (int i = 1; i <= intCountUTM; ++i)
    {
        if (i == 1)
        {
            vecUTMService.push_back("Transport");
        }
        else
        {
            vecUTMService.push_back("Transport" + std::to_string(i));
        }
    }

    // получаем порты УТМ из конфига
    std::vector<std::string> ports;
    if (readConfigPorts(intCountUTM, ports) == 1)
    {
        logger("Ошибка чтения конфига при проверке наличия служб УТМ - " + std::to_string(GetLastError()), "ERROR");
        return 1;
    }

    // проверка наличие служб УТМ и их статус
    std::vector<std::string> vecServiceStatus;
    if (getServiceList(vecUTMService, vecServiceStatus) == 1)
    {
        logger("Ошибка получения статуса служб при проверке наличия служб УТМ - " + std::to_string(GetLastError()), "ERROR");
        return 1;
    }

    // проверка ошибок УТМ
    // 0 - не запущен, 1 - ошибок нет, 2 - ошибки есть
    std::vector<int> vecUTMError;
    int UTMError = 0;
    int countPort = ports.size() - 1; // потому что массив портов развернут в другую сторону
    for (int i = 0; i < vecServiceStatus.size(); ++i)
    {
        if (vecServiceStatus[i].find("Запущена") != std::string::npos)
        {
            if (checkUTMError(UTMError, ports[countPort]) != 0)
            {
                logger("Ошибка проверки ошибок УТМ при проверке наличия служб УТМ - " + std::to_string(GetLastError()), "ERROR");
                return 1;
            }
            vecUTMError.push_back(UTMError);
        }
        else
        {
            UTMError = 0;
            vecUTMError.push_back(UTMError);
        }
        --countPort;
    }


    // заполняем листбокс утм и службы
    if (fillListBoxUTMService(vecServiceStatus, vecUTMError) == 1)
    {
        logger("Ошибка заполнения листбокса при проверке наличия служб УТМ - вектор vecServiceStatus пуст", "ERROR");
        // Удаляем все строки в листвью контактов
        ListView_DeleteAllItems(hListBoxUTMServiceGlobal);

        return 1;
    }

    logger("Получение информации о службах УТМ успешно завершено", "INFO");

    if (flagAutostartUTM == false && flagStartUTM == false && flagStopUTM == false) // при запуске и остановке утмов прогрессбар не убирать
    {
        // Убираем прогрессбар и разблокируем главное окно
        ShowWindow(hProgressBar, SW_HIDE);
        EnableWindow(hWndMain, TRUE);
    }

    flagCheckServices = false;

    return 0;
}

// получение списка всех служб
int getServiceList(std::vector<std::string> vecUTMService, std::vector<std::string>& vecServiceStatus)
{
    SC_HANDLE hHandle = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    if (!hHandle)
    {
        logger("Ошибка OpenSCManager - " + std::to_string(GetLastError()), "ERROR");
        return 1;
    }

    vecPathService.clear();

    DWORD dwBytesNeeded = 0;
    DWORD dwServicesReturned = 0;
    DWORD dwResumedHandle = 0;

    // Выясняем, какой нужен буффер для информации
    BOOL retVal = EnumServicesStatus(hHandle, SERVICE_WIN32, SERVICE_STATE_ALL,
        NULL, 0, &dwBytesNeeded, &dwServicesReturned, &dwResumedHandle);
    if (retVal == 0)
    {
        if (GetLastError() != ERROR_MORE_DATA)
        {
            logger("Ошибка EnumServicesStatus с NULL- " + std::to_string(GetLastError()), "ERROR");
        }
        // Делаем буффер и получаем информацию из структур
        else
        {
            DWORD dwBytesNeeded2 = 0;
            LPQUERY_SERVICE_CONFIG lpsc = { 0 };
            LPENUM_SERVICE_STATUS pServices = { 0 };
            pServices = new ENUM_SERVICE_STATUS[dwBytesNeeded];
            if (EnumServicesStatus(hHandle, SERVICE_WIN32, SERVICE_STATE_ALL,
                pServices, dwBytesNeeded, &dwBytesNeeded2, &dwServicesReturned, &dwResumedHandle) == 0)
            {
                logger("Ошибка EnumServicesStatus - " + std::to_string(GetLastError()), "ERROR");
            }
            else
            {
                for (std::string elem : vecUTMService)
                {
                    for (DWORD i = 0; i < dwServicesReturned; ++i)
                    {
                        if (strcmp(elem.c_str(), (pServices + i)->lpDisplayName) == 0)
                        {
                            std::string str;

                            SC_HANDLE service = OpenService(hHandle, (pServices + i)->lpDisplayName, SC_MANAGER_ALL_ACCESS);
                            if (service == NULL)
                            {
                                logger("Ошибка OpenService - " + std::to_string(GetLastError()), "ERROR");
                                vecPathService.push_back("Путь не доступен");
                            }

                            // Выясняем, какой нужен буффер для информации
                            dwBytesNeeded = 0;
                            if (QueryServiceConfig(service, NULL, 0, &dwBytesNeeded) == 0)
                            {
                                if (GetLastError() != ERROR_INSUFFICIENT_BUFFER)
                                {
                                    logger("Ошибка QueryServiceConfig с NULL- " + std::to_string(GetLastError()), "ERROR");
                                    vecPathService.push_back("Путь не доступен");
                                }
                            }
                            // Делаем буффер и получаем информацию из структур
                            lpsc = new QUERY_SERVICE_CONFIG[sizeof(QUERY_SERVICE_CONFIG) + dwBytesNeeded];
                            dwBytesNeeded2 = 0;
                            if (QueryServiceConfig(service, lpsc, dwBytesNeeded, &dwBytesNeeded2) == 0)
                            {
                                logger("Ошибка QueryServiceConfig - " + std::to_string(GetLastError()), "ERROR");
                                str += "Путь не доступен - ";
                            }
                            vecPathService.push_back(std::string(lpsc->lpBinaryPathName));

                            str += (pServices + i)->lpDisplayName;
                            if ((pServices + i)->ServiceStatus.dwCurrentState == 4)
                            {
                                str += " - Запущена";
                            }
                            else
                            {
                                str += " - Остановлена";
                            }
                            vecServiceStatus.push_back(str);
                            break;
                        }
                    }
                }
            }
            delete[] lpsc;
            delete[] pServices;
            pServices = NULL;
        }
    }
    if (!CloseServiceHandle(hHandle))
    {
        logger("Ошибка CloseServiceHandle - " + std::to_string(GetLastError()), "ERROR");
        return 1;
    }
    
    return 0;
}

// заполнение листбокса утм и службы
int fillListBoxUTMService(std::vector<std::string> vecServiceStatus, std::vector<int> vecUTMError)
{
    if (vecServiceStatus.empty())
    {
        return 1;
    }

    //EnableWindow(hListBoxUTMServiceGlobal, TRUE);

    // Удаляем все строки в листвью контактов
    ListView_DeleteAllItems(hListBoxUTMServiceGlobal);

    // Заполняем
    std::string str;
    for (int i = 0; i < vecServiceStatus.size(); ++i)
    {
        if (vecServiceStatus[i].find("Запущена") != std::string::npos) // для подбора картинок
        {
            if (vecUTMError[i] == 1)
            {
                str = "      " + vecServiceStatus[i] + " - Ошибок нет"; // пробелы для красоты
                CreateItem(hListBoxUTMServiceGlobal, (CHAR*)str.c_str(), i, 1);
            }
            else
            {
                str = "      " + vecServiceStatus[i] + " - Ошибка ключа"; // пробелы для красоты
                CreateItem(hListBoxUTMServiceGlobal, (CHAR*)str.c_str(), i, 3);
            }
        }
        else
        {
            str = "      " + vecServiceStatus[i]; // пробелы для красоты
            CreateItem(hListBoxUTMServiceGlobal, (CHAR*)str.c_str(), i, 2);
        }
    }

    //EnableWindow(hListBoxUTMServiceGlobal, FALSE);

    return 0;
}

// создание колонки в листвью
int CreateColumn(HWND hwndLV, int iCol, CHAR* Text, int iWidth)
{
    LVCOLUMN lvc;

    lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
    lvc.fmt = LVCFMT_LEFT;
    lvc.cx = iWidth;
    lvc.pszText = Text;
    lvc.iSubItem = iCol;
    return ListView_InsertColumn(hwndLV, iCol, &lvc);
}

// создание элемента в листвью
int CreateItem(HWND hwndList, CHAR* text, int index, int status)
{
    LVITEM lvi = { 0 };
    int res;

    // Initialize LVITEM members that are common to all items. 
    lvi.mask = LVIF_TEXT | LVIF_PARAM | LVIF_IMAGE;
    lvi.iItem = index;
    lvi.pszText = text;
    lvi.iImage = status; // индекс картинки в списке hSmall
    lvi.lParam = 0;
    res = ListView_InsertItem(hwndList, &lvi);

    if (res >= 0)
    {
        ListView_SetItemText(hwndList, res, 0, text);
    }

    return res;
}