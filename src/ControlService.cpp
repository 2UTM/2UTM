#include "ControlService.h"
#include "Logger.h"
#include "wininet.h"

#pragma comment(lib, "Wininet.lib")

extern std::string pathExe;
bool flagInstallUTM = false; // ���� ��� ������������� �������

// ���������
int stopServiceUTM(std::string nameService)
{
    SC_HANDLE schSCManager, schService;
    SERVICE_STATUS_PROCESS ssStatus;
    DWORD dwBytesNeeded;

    // ��������� �������� �����
    schSCManager = OpenSCManager(0, 0, SC_MANAGER_ALL_ACCESS);

    // ��������� ������
    schService = OpenService(schSCManager, nameService.c_str(), SERVICE_ALL_ACCESS);

    // �������� ������ ������
    if (QueryServiceStatusEx(schService, SC_STATUS_PROCESS_INFO, (LPBYTE)&ssStatus, sizeof(SERVICE_STATUS_PROCESS), &dwBytesNeeded) == 0)
    {
        CloseServiceHandle(schService);
        CloseServiceHandle(schSCManager);
        return 2;
    }
    // ���� ������ �����������, ���������� 0
    if (ssStatus.dwCurrentState == SERVICE_STOPPED)
    {
        CloseServiceHandle(schService);
        CloseServiceHandle(schSCManager);
        return 0;
    }

    // ������������� ������
    SERVICE_STATUS ServiceSt;
    if (ControlService(schService, SERVICE_CONTROL_STOP, &ServiceSt) == 0)
    {
        CloseServiceHandle(schSCManager);
        CloseServiceHandle(schService);
        return 1;
    }

    // �������� ������ ������
    if (QueryServiceStatusEx(schService, SC_STATUS_PROCESS_INFO, (LPBYTE)&ssStatus, sizeof(SERVICE_STATUS_PROCESS), &dwBytesNeeded) == 0)
    {
        CloseServiceHandle(schService);
        CloseServiceHandle(schSCManager);
        return 2;
    }

    // ���� ���� ������ �����������
    while (ssStatus.dwCurrentState == SERVICE_STOP_PENDING)
    {
        Sleep(1000);

        // ��������� ������ � ���������
        if (QueryServiceStatusEx(schService, SC_STATUS_PROCESS_INFO, (LPBYTE)&ssStatus, sizeof(SERVICE_STATUS_PROCESS), &dwBytesNeeded) == 0)
        {
            CloseServiceHandle(schService);
            CloseServiceHandle(schSCManager);
            return 3;
        }
    }
    // ���� ������ �����������, ���������� 0
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

// �����
int startServiceUTM(std::string nameService)
{
    SC_HANDLE schSCManager, schService;
    SERVICE_STATUS_PROCESS ssStatus;
    DWORD dwBytesNeeded;

    // ��������� �������� �����
    schSCManager = OpenSCManager(0, 0, SC_MANAGER_ALL_ACCESS);

    // ��������� ������
    schService = OpenService(schSCManager, nameService.c_str(), SERVICE_ALL_ACCESS);

    // ��������� ������ � ���������
    if (QueryServiceStatusEx(schService, SC_STATUS_PROCESS_INFO, (LPBYTE)&ssStatus, sizeof(SERVICE_STATUS_PROCESS), &dwBytesNeeded) == 0)
    {
        CloseServiceHandle(schService);
        CloseServiceHandle(schSCManager);
        return 2;
    }
    // ���� ������ ��������, ���������� 0
    if (ssStatus.dwCurrentState == SERVICE_RUNNING)
    {
        CloseServiceHandle(schService);
        CloseServiceHandle(schSCManager);
        return 0;
    }

    // ������ ������
    if (StartService(schService, 0, NULL) == 0)
    {
        CloseServiceHandle(schService);
        CloseServiceHandle(schSCManager);
        return 1;
    }

    // ��������� ������ � ���������
    if (QueryServiceStatusEx(schService, SC_STATUS_PROCESS_INFO, (LPBYTE)&ssStatus, sizeof(SERVICE_STATUS_PROCESS), &dwBytesNeeded) == 0)
    {
        CloseServiceHandle(schService);
        CloseServiceHandle(schSCManager);
        return 2;
    }

    // ���� ���� ������ ����������
    while (ssStatus.dwCurrentState == SERVICE_START_PENDING)
    {
        Sleep(1000);

        // ��������� ������ � ���������
        if (QueryServiceStatusEx(schService, SC_STATUS_PROCESS_INFO, (LPBYTE)&ssStatus, sizeof(SERVICE_STATUS_PROCESS), &dwBytesNeeded) == 0)
        {
            CloseServiceHandle(schService);
            CloseServiceHandle(schSCManager);
            return 3;
        }
    }

    // ���� ������ ��������, ���������� 0
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

// ��������� (����������� ����� ������ �� ���, ����� ����� �������� � ����������� �� ���������)
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

// �������� ������
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

// �������� ������ ��� (������ �� ������� �������� ���)
int checkHomePageUTM(std::string port, bool flagStartUTM, int timeout)
{
    // ���� ������������ �������, �������
    if (flagStartUTM == false)
    {
        if (flagInstallUTM == false)
        {
            return 2;
        }
    }

    HINTERNET hSession, hConnect, hRequest;

    // �������������
    hSession = InternetOpen("connect", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, NULL);
    if (hSession == NULL)
    {
        InternetCloseHandle(hSession);
        return GetLastError();
    }

    // �����������
    hConnect = InternetConnect(hSession, "localhost", atoi(port.c_str()), NULL, NULL, INTERNET_SERVICE_HTTP, NULL, NULL);
    if (hConnect == NULL)
    {
        InternetCloseHandle(hSession);
        InternetCloseHandle(hConnect);
        return GetLastError();
    }

    // ���������� ���� �������
    hRequest = HttpOpenRequest(hConnect, "POST", NULL, NULL, NULL, NULL, NULL, NULL);
    if (hRequest == NULL)
    {
        InternetCloseHandle(hSession);
        InternetCloseHandle(hConnect);
        InternetCloseHandle(hRequest);
        return GetLastError();
    }

    // � �������, ��������, 5�� ����� �������� ��������� ������
    // (� HttpSendRequest ������� �� 2 �� 5 ������, � ����������� �� ������ ��)
    // ��������� �������� ����� InternetSetOption ������ �� �� ��������
    BOOL check;
    int count = 0;
    int err = 0;
    while (count < timeout)
    {
        // ���� ������������ �������, �������
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

// ������ � ��� ���� ����� ���, ������� ���� rsaError
int checkUTMError(int& error, std::string port)
{
    HINTERNET hSession, hConnect;

    // �������������
    hSession = InternetOpen("HTTPGET", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, NULL);
    if (hSession == NULL)
    {
        return GetLastError();
    }

    // ��������� URL
    std::string url = "http://localhost:" + port + "/api/info/list";
    hConnect = InternetOpenUrl(hSession, url.c_str(), NULL, 0, INTERNET_FLAG_RELOAD, 0);

    if (hConnect == NULL)
    {
        InternetCloseHandle(hSession);
        return GetLastError();
    }

    // ������ ������
    DWORD bytesRead;
    std::string data;
    char buffer[MAX_PATH];
    while (InternetReadFile(hConnect, buffer, sizeof(buffer), &bytesRead) && bytesRead > 0)
    {
        data.append(buffer, bytesRead);
    }

    // ������ json (�� ���� ������)))
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

// ���������� ������� � ������� � ������ �� ������
int exec(std::string cmd, std::string& result)
{
    DWORD error = 0;
    result = "";
    HANDLE hPipeRead, hPipeWrite;

    // ��������� � ������� ���� (���� - ��� ����� ����� ����� ������������ � �������� ����������)
    logger("��������� � ������� ����", "INFO");
    SECURITY_ATTRIBUTES saAttr = { sizeof(SECURITY_ATTRIBUTES) };
    saAttr.bInheritHandle = TRUE;
    saAttr.lpSecurityDescriptor = NULL;
    if (!CreatePipe(&hPipeRead, &hPipeWrite, &saAttr, 0))
    {
        return 1;
    }

    // ��������� � ������� �������
    logger("��������� � ������� �������", "INFO");
    STARTUPINFOA si = { sizeof(STARTUPINFO) };
    si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
    si.hStdOutput = hPipeWrite;
    si.hStdError = hPipeWrite;
    si.wShowWindow = SW_HIDE; // �� ���������� ����, ����� STARTF_USESHOWWINDOW � dwFlags
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
        // ���� ���������� ��������
        logger("���� ���������� ��������", "INFO");
        bProcessEnded = WaitForSingleObject(pi.hProcess, 50) == WAIT_OBJECT_0;

        // ������ �����
        while (true)
        {
            DWORD dwRead = 0;
            DWORD dwAvail = 0;

            // �������� ������ ������� �� ����� �� ������ ���
            logger("�������� ������ ������� �� ����� �� ������ ���", "INFO");
            if (!PeekNamedPipe(hPipeRead, NULL, 0, NULL, &dwAvail, NULL))
            {
                break;
            }

            if (!dwAvail) // ���� ��� ������
            {
                break;
            }

            // ������ �� �����
            logger("������ �� �����", "INFO");
            std::vector<char> buf(dwAvail);
            if (!ReadFile(hPipeRead, &buf.front(), dwAvail, &dwRead, NULL) || !dwRead)
            {
                break;
            }
            std::string tmp(buf.begin(), buf.end());
            result += tmp;
        }
    }

    // ������
    logger("������", "INFO");
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