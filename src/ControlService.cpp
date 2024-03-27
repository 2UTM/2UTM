#include "ControlService.h"
#include "Logger.h"
#include "wininet.h"

#pragma comment(lib, "Wininet.lib")

extern std::string pathExe;
bool flagInstallUTM = false; // флаг для синхронизации потоков

// остановка
int stopServiceUTM(std::string nameService)
{
    SC_HANDLE schSCManager, schService;
    SERVICE_STATUS_PROCESS ssStatus;
    DWORD dwBytesNeeded;

    // Открываем менеджер служб
    schSCManager = OpenSCManager(0, 0, SC_MANAGER_ALL_ACCESS);

    // Открываем службу
    schService = OpenService(schSCManager, nameService.c_str(), SERVICE_ALL_ACCESS);

    // Получаем статус службы
    if (QueryServiceStatusEx(schService, SC_STATUS_PROCESS_INFO, (LPBYTE)&ssStatus, sizeof(SERVICE_STATUS_PROCESS), &dwBytesNeeded) == 0)
    {
        CloseServiceHandle(schService);
        CloseServiceHandle(schSCManager);
        return 2;
    }
    // Если служба остановлена, возвращаем 0
    if (ssStatus.dwCurrentState == SERVICE_STOPPED)
    {
        CloseServiceHandle(schService);
        CloseServiceHandle(schSCManager);
        return 0;
    }

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
        return 2;
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
            return 3;
        }
    }
    // Если служба остановлена, возвращаем 0
    if (ssStatus.dwCurrentState == SERVICE_STOPPED)
    {
        CloseServiceHandle(schService);
        CloseServiceHandle(schSCManager);
        return 0;
    }
    CloseServiceHandle(schService);
    CloseServiceHandle(schSCManager);
	return 4;
}

// старт
int startServiceUTM(std::string nameService)
{
    SC_HANDLE schSCManager, schService;
    SERVICE_STATUS_PROCESS ssStatus;
    DWORD dwBytesNeeded;

    // Открываем менеджер служб
    schSCManager = OpenSCManager(0, 0, SC_MANAGER_ALL_ACCESS);

    // Открываем службу
    schService = OpenService(schSCManager, nameService.c_str(), SERVICE_ALL_ACCESS);

    // Обновляем статус в структуре
    if (QueryServiceStatusEx(schService, SC_STATUS_PROCESS_INFO, (LPBYTE)&ssStatus, sizeof(SERVICE_STATUS_PROCESS), &dwBytesNeeded) == 0)
    {
        CloseServiceHandle(schService);
        CloseServiceHandle(schSCManager);
        return 2;
    }
    // Если служба запущена, возвращаем 0
    if (ssStatus.dwCurrentState == SERVICE_RUNNING)
    {
        CloseServiceHandle(schService);
        CloseServiceHandle(schSCManager);
        return 0;
    }

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
        return 2;
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
            return 3;
        }
    }

    // Если служба запущена, возвращаем 0
    if (ssStatus.dwCurrentState == SERVICE_RUNNING)
    {
        CloseServiceHandle(schService);
        CloseServiceHandle(schSCManager);
        return 0;
    }
    CloseServiceHandle(schService);
    CloseServiceHandle(schSCManager);
    return 4;
}

// установка (обязательно через батник из утм, иначе будет работать с настройками по умолчанию)
int installServiceUTM(int numberUTM)
{
    std::string result;
    if (numberUTM == 1)
    {
        int err = exec("C:\\UTM\\transporter\\bin\\install.bat", result);
        if (err == 1)
        {
            return 1;
        }
    }
    else
    {
        int err = exec("C:\\UTM_" + std::to_string(numberUTM) + "\\transporter\\bin\\install.bat Transport" + std::to_string(numberUTM), result);
        if (err == 1)
        {
            return 1;
        }
    }

    return 0;
}

// удаление службы
int deleteServiceUTM(int numberUTM)
{
    std::string result;
    if (numberUTM == 1)
    {
        int err = exec("C:\\UTM\\transporter\\bin\\delete.bat", result);
        if (err == 1)
        {
            return 1;
        }
    }
    else
    {
        int err = exec("C:\\UTM_" + std::to_string(numberUTM) + "\\transporter\\bin\\delete.bat Transport" + std::to_string(numberUTM), result);
        if (err == 1)
        {
            return 1;
        }
    }

    return 0;
}

// проверка работы УТМ (запрос на главную страницу УТМ)
int checkHomePageUTM(std::string port, bool flagStartUTM, int timeout)
{
    // если пользователь прервал, выходим
    if (flagStartUTM == false)
    {
        if (flagInstallUTM == false)
        {
            return 2;
        }
    }

    HINTERNET hSession, hConnect, hRequest;

    // инициализация
    hSession = InternetOpen("connect", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, NULL);
    if (hSession == NULL)
    {
        InternetCloseHandle(hSession);
        return GetLastError();
    }

    // подключение
    hConnect = InternetConnect(hSession, "localhost", atoi(port.c_str()), NULL, NULL, INTERNET_SERVICE_HTTP, NULL, NULL);
    if (hConnect == NULL)
    {
        InternetCloseHandle(hSession);
        InternetCloseHandle(hConnect);
        return GetLastError();
    }

    // объявление типа запроса
    hRequest = HttpOpenRequest(hConnect, "POST", NULL, NULL, NULL, NULL, NULL, NULL);
    if (hRequest == NULL)
    {
        InternetCloseHandle(hSession);
        InternetCloseHandle(hConnect);
        InternetCloseHandle(hRequest);
        return GetLastError();
    }

    // в течении, примерно, 5ти минут пытаемся отправить запрос
    // (в HttpSendRequest таймаут от 2 до 5 секунд, в зависимости от версии ОС)
    // установка таймаута через InternetSetOption почему то не работает
    BOOL check;
    int count = 0;
    int err = 0;
    while (count < timeout)
    {
        // если пользователь прервал, выходим
        if (flagStartUTM == false)
        {
            if (flagInstallUTM == false)
            {
                return 2;
            }
        }

        check = HttpSendRequest(hRequest, NULL, NULL, NULL, NULL);

        if (check == FALSE)
        {
            err = GetLastError();
            if (err == ERROR_INTERNET_CANNOT_CONNECT)
            {
                ++count;
                Sleep(1000);
                continue;
            }
            InternetCloseHandle(hSession);
            InternetCloseHandle(hConnect);
            InternetCloseHandle(hRequest);
            return err;
        }
        else
        {
            break;
        }
    }

    InternetCloseHandle(hSession);
    InternetCloseHandle(hConnect);
    InternetCloseHandle(hRequest);

    if (check == FALSE)
    {
        return err;
    }
    return 0;
}

// запрос у утм инфы через апи, смотрим поле rsaError
int checkUTMError(int& error, std::string port)
{
    HINTERNET hSession, hConnect;

    // инициализация
    hSession = InternetOpen("HTTPGET", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, NULL);
    if (hSession == NULL)
    {
        return GetLastError();
    }

    // открываем URL
    std::string url = "http://localhost:" + port + "/api/info/list";
    hConnect = InternetOpenUrl(hSession, url.c_str(), NULL, 0, INTERNET_FLAG_RELOAD, 0);

    if (hConnect == NULL)
    {
        InternetCloseHandle(hSession);
        return GetLastError();
    }

    // читаем данные
    DWORD bytesRead;
    std::string data;
    char buffer[MAX_PATH];
    while (InternetReadFile(hConnect, buffer, sizeof(buffer), &bytesRead) && bytesRead > 0)
    {
        data.append(buffer, bytesRead);
    }

    // разбор json (ну типо разбор)))
    int index = data.find("rsaError");
    if (index != std::string::npos)
    {
        data = data.substr(index + 10, 4);
        if (data == "null")
        {
            error = 1;
        }
        else
        {
            error = 2;
        }
    }

    InternetCloseHandle(hSession);
    InternetCloseHandle(hConnect);

    return 0;
}

// выполнение команды в консоли и чтение ее вывода
int exec(std::string cmd, std::string& result)
{
    DWORD error = 0;
    result = "";
    HANDLE hPipeRead, hPipeWrite;

    // Объявляем и создаем пайп (пайп - это канал связи между родительским и дочерним процессами)
    logger("Объявляем и создаем пайп", "INFO");
    SECURITY_ATTRIBUTES saAttr = { sizeof(SECURITY_ATTRIBUTES) };
    saAttr.bInheritHandle = TRUE;
    saAttr.lpSecurityDescriptor = NULL;
    if (!CreatePipe(&hPipeRead, &hPipeWrite, &saAttr, 0))
    {
        return 1;
    }

    // Объявляем и создаем процесс
    logger("Объявляем и создаем процесс", "INFO");
    STARTUPINFOA si = { sizeof(STARTUPINFO) };
    si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
    si.hStdOutput = hPipeWrite;
    si.hStdError = hPipeWrite;
    si.wShowWindow = SW_HIDE; // не показывать окно, нужно STARTF_USESHOWWINDOW в dwFlags
    PROCESS_INFORMATION pi = { 0 };
    BOOL fSuccess = CreateProcessA(NULL, (LPSTR)cmd.c_str(), NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);
    if (!fSuccess)
    {
        CloseHandle(hPipeWrite);
        CloseHandle(hPipeRead);
        return 1;
    }

    bool bProcessEnded = false;
    while (!bProcessEnded)
    {
        // Ждем завершения процесса
        logger("Ждем завершения процесса", "INFO");
        bProcessEnded = WaitForSingleObject(pi.hProcess, 50) == WAIT_OBJECT_0;

        // Читаем вывод
        while (true)
        {
            DWORD dwRead = 0;
            DWORD dwAvail = 0;

            // Получаем размер буффера из пайпа не очищая его
            logger("Получаем размер буффера из пайпа не очищая его", "INFO");
            if (!PeekNamedPipe(hPipeRead, NULL, 0, NULL, &dwAvail, NULL))
            {
                break;
            }

            if (!dwAvail) // если нет данных
            {
                break;
            }

            // Читаем из пайпа
            logger("Читаем из пайпа", "INFO");
            std::vector<char> buf(dwAvail);
            if (!ReadFile(hPipeRead, &buf.front(), dwAvail, &dwRead, NULL) || !dwRead)
            {
                break;
            }
            std::string tmp(buf.begin(), buf.end());
            result += tmp;
        }
    }

    // Чистка
    logger("Чистка", "INFO");
    if (GetExitCodeProcess(pi.hProcess, &error) == 0)
    {
        error = GetLastError();
    }
    CloseHandle(hPipeWrite);
    CloseHandle(hPipeRead);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return 0;
}