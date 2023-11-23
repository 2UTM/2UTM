#pragma once
#include "windows.h"
#include "Resource.h"
#include "Commctrl.h"
#include "Logger.h"
#include "config.h"
#include "winscard.h"
#include "TlHelp32.h"
#include "GetPathExe.h"
#include <fstream>

void installUTM(HWND dlg, int countUTM, std::string fileUTM, SCARDCONTEXT ctx, std::vector<std::string> vecTokens,
	std::vector<std::string> vecAttr, HWND btnCancel, HWND hDlg); // ����� �������, ��������� ���
void changeStaticText(HWND dlg); // ������� ���������� � �������, ������ ���, ��� �� ������
int startProcessInstallUTM(std::string fileUTM); // ������ � �������� ���������� ��������, ��������� ���

// �������� ����� ���
int CopyFolder(std::string src, std::string dst);
int copyFolderUTM(int numberUTM);

// ����������� �������� � ���
int copyBatFiles(int numberUTM);

// ����������� �������� ��� (������ ������)
int changePortConfigUTM(int numberUTM, std::string port);

// ���������� ������ � ������ ���, ����� ������ �� ���������� ����
int changeInstallBatUTM();

// ��������� ������� javaw.exe
int killProcessJava();

// ������� �� ������������
int deleteFileUTMlnk();

// ����� ��������� ��� ������ ��� ������
int backChange(std::vector<std::string> vecRutokens,
	std::vector<std::string> vecAttrRutokens,
	SCARDCONTEXT ctx,
	HWND hDlg);

// ��������� � �������� ����� � �����
int deleteUTMBackChange(HWND hDlg);

// �������� ����� ���
int deleteFolderUTMBackChange(std::string src);