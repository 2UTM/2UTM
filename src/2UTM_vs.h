#pragma once

#include "Resource.h"
#include <string>
#include <vector>
#include <map>
#include <thread>
#include "dbt.h" // ��� �������� �� �������
#include "Logger.h"
#include "CommCtrl.h"
#include "commdlg.h"
#include "config.h"
#include "installUTM.h"
#include "GetPathExe.h"
#include "shellapi.h"
#include "objbase.h"
#include "shobjidl_core.h"
#include "wtsapi32.h"
#include "userenv.h"
#pragma comment(lib, "Winscard.lib") // ��� ���������
#pragma comment(lib, "Wtsapi32.lib") // ��� ��������� ������ ������������
#pragma comment(lib, "Userenv.lib") // ��� ��������� ������������

// ��� ������������
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

void getVersionOS(DWORD& dwMinorVersion); // �������� ������ ��
void setStatusBar(std::string status); // ��������� ������ � ������ ���
void setInfoRutokens(std::string info); // ��������� ������ � ���� �������� ��������

void wraperReadDevice(int index); // ������� ��� ������� � ���������� (����� �������� ������ ������� �� ����)
int fillListBoxRutokens(std::vector<std::string> vecReader); // ���������� ���������
int workCollectDevice(); // �������, ������������ ��������� ���������

int fillListBoxRutokensNoContext(std::vector<std::string> vecReaderNoContext); // ���������� ��������� ��������� ��� ���������
int workCollectDeviceNoContext(); // �������, ������������ ��������� ��������� ��� ���������

int FileOpenDlg(CHAR* fileName); // �����, ����� �����

int checkUTM(); // �������� �� ��� ������������� ���

int deleteUTM(); // ������� ����
int deleteFolderUTM(std::string src); // ������� ����� ���

// ��������� ����
int startUTM();

// ����� ��������� ��� ������� ���
int backChangeStartUTM();

// ���������� ����
int stopUTM();

// �������� ������ � ����
int createTrayWindow(HWND hwnd);

// �������� ��������� � ����
void showMessageInTray();

// ���������� ������ 2UTM
int InstallService2UTM();

// ������� ������ 2UTM
int RemoveService2UTM();

// ������ ������� �� ������ � ������� ������ � ��������� ������������
int startProcessTokenServiceEnvUser(DWORD sessionID);

// ������ ������ 2UTM
int startService2UTM();

// ��������� ������ 2UTM
int stopService2UTM();

// �������� �� ������������� �������� explorer.exe
int checkExplorerExe();

// ��� ��������� ������� �� ��������
enum MainWidgetId
{
	LISTBOX_TOKENS,
	TRAY_SHOW2UTM,
	TRAY_INSTALL_SERVICE,
	TRAY_DELETE_SERVICE,
	TRAY_EXIT
};
