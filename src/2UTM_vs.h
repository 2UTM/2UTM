#pragma once

#include "Resource.h"
#include <string>
#include <vector>
#include <thread>
#include "dbt.h" // ��� �������� �� �������
#include "Logger.h"
#include "CommCtrl.h"
#include "commdlg.h"
#include "config.h"
#include "installUTM.h"
#include "GetPathExe.h"
#pragma comment(lib, "Winscard.lib") // ��� ���������

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

// ��� ��������� ������� �� ��������
enum MainWidgetId
{
	LISTBOX_TOKENS
};
