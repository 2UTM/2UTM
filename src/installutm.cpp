#define _CRT_SECURE_NO_WARNINGS
#include "installUTM.h"
#include "ControlReaders.h"
#include "ControlService.h"


extern std::string pathExe;
extern bool flagInstallUTM;
bool flagChangeStatic = true;
HANDLE evt = CreateEvent(
	NULL, // �������������, ������ ���� NULL
	TRUE, // �������������� ����� ������ (TRUE - ��������, ����� ������������ ResetEvent)
	FALSE, // ��������� ��������� (FALSE - �� ����������)
	NULL); // ��� ������

// ��������� ���
void installUTM(HWND dlg, int countUTM, std::string fileUTM, SCARDCONTEXT ctx, std::vector<std::string> vecTokens,
	std::vector<std::string> vecAttr, HWND btnCancel, HWND hDlg)
{
	flagInstallUTM = true;
	flagChangeStatic = true;
	bool flagStartUTM = false;

	// ������ �� �������� ������ ������
	EnableWindow(btnCancel, FALSE);

	// ������������� ��� ������������
	SendMessage(GetDlgItem(dlg, IDC_PROGRESS1), PBM_SETSTEP, 1, 0);

	std::string str;
	int err = 0;
	std::string error;

	str = "��������� ��� 1";
	SetDlgItemText(dlg, IDC_STATIC_PROGRESSBAR2, str.c_str());
	logger(str, "INFO");
	SendMessage(GetDlgItem(dlg, IDC_PROGRESS1), PBM_STEPIT, 0, 0);

	// ��������� ���
	err = startProcessInstallUTM(fileUTM);
	if (err == 1)
	{
		flagChangeStatic = false;
		error = "��������� ��� ����������� �������! ��� ������ - " + std::to_string(GetLastError());
		MessageBox(dlg, error.c_str(), "������", MB_ICONERROR);
		logger(error, "ERROR");
		EndDialog(dlg, 0);
		return;
	}
	str = "��� 1 ������� ����������";
	logger(str, "INFO");
	SendMessage(GetDlgItem(dlg, IDC_PROGRESS1), PBM_STEPIT, 0, 0);

	// ������� ������ ���
	if (deleteServiceUTM(1) == 1)
	{
		flagChangeStatic = false;
		error = "�� ������� ������� ������ ���! ��� ������ - " + std::to_string(GetLastError()) + "\n����� ��������� ������ ���������";
		MessageBox(dlg, error.c_str(), "������", MB_ICONERROR);
		logger(error, "ERROR");
		if (backChange(vecTokens, vecAttr, ctx, hDlg) != 0)
		{
			logger("������ ��������� ����������� ��������", "ERROR");
		}
		EndDialog(dlg, 0);
		return;
	}
	str = "������ ��� �������";
	logger(str, "INFO");
	SendMessage(GetDlgItem(dlg, IDC_PROGRESS1), PBM_STEPIT, 0, 0);

	// ���������� ������ � ������ ���, ����� ������ �� ���������� ����
	if (changeInstallBatUTM() == 1)
	{
		flagChangeStatic = false;
		error = "�� ������� �������� install.bat � ���! ��� ������ - " + std::to_string(GetLastError()) + "\n����� ��������� ������ ���������";
		MessageBox(dlg, error.c_str(), "������", MB_ICONERROR);
		logger(error, "ERROR");
		if (backChange(vecTokens, vecAttr, ctx, hDlg) != 0)
		{
			logger("������ ��������� ����������� ��������", "ERROR");
		}
		EndDialog(dlg, 0);
		return;
	}
	str = "������ ��� ���������";
	logger(str, "INFO");
	SendMessage(GetDlgItem(dlg, IDC_PROGRESS1), PBM_STEPIT, 0, 0);

	// ������������� ������ ������� ��� ��� ��� �����������
	if (installServiceUTM(1) == 1)
	{
		flagChangeStatic = false;
		error = "�� ������� ���������� ������ ���! ��� ������ - " + std::to_string(GetLastError()) + "\n����� ��������� ������ ���������";
		MessageBox(dlg, error.c_str(), "������", MB_ICONERROR);
		logger(error, "ERROR");
		if (backChange(vecTokens, vecAttr, ctx, hDlg) != 0)
		{
			logger("������ ��������� ����������� ��������", "ERROR");
		}
		EndDialog(dlg, 0);
		return;
	}
	str = "������ ��� �����������";
	logger(str, "INFO");
	SendMessage(GetDlgItem(dlg, IDC_PROGRESS1), PBM_STEPIT, 0, 0);

	// ������ �������� ������ ������
	EnableWindow(btnCancel, TRUE);

	// �������� �����
	if (flagInstallUTM == false)
	{
		logger("����� ��������� ������� �������������", "INFO");
		SetEvent(evt);
		return;
	}

	// ������ ������
	SetDlgItemText(dlg, IDC_STATIC_PROGRESSBAR2, "������ �������");
	logger("������ �������", "INFO");
	SendMessage(GetDlgItem(dlg, IDC_PROGRESS1), PBM_STEPIT, 0, 0);
	std::vector<std::string> vectorNameReaders, vectorAttrReaders, ports, serialNumbers;
	if (readConfigNameAndReader(countUTM, vectorNameReaders, vectorAttrReaders, ports, serialNumbers) == 1)
	{
		error = "������ ������� ����������� �������! ��� ������ - " + std::to_string(GetLastError()) + "\n����� ��������� ������ ���������";
		MessageBox(dlg, error.c_str(), "������", MB_ICONERROR);
		logger(error, "ERROR");
		// ������ �� �������� ������ ������
		EnableWindow(btnCancel, FALSE);
		if (backChange(vecTokens, vecAttr, ctx, hDlg) != 0)
		{
			logger("������ ��������� ����������� ��������", "ERROR");
		}
		EndDialog(dlg, 0);
		return;
	}
	logger("������ ������� ������� ���������", "INFO");

	// �������� �����
	if (flagInstallUTM == false)
	{
		logger("����� ��������� ������� �������������", "INFO");
		SetEvent(evt);
		return;
	}

	// ���� �������� 1 ���, �� ���������
	if (ports.size() == 1)
	{
		// ������ ������ ���
		SetDlgItemText(dlg, IDC_STATIC_PROGRESSBAR2, "������ ������ Transport");
		logger("������ ������ Transport", "INFO");
		SendMessage(GetDlgItem(dlg, IDC_PROGRESS1), PBM_STEPIT, 0, 0);
		err = startServiceUTM("Transport");
		if (err != 0)
		{
			error = "������ ������� ������ Trasport! ��� ������ - " + std::to_string(GetLastError()) + "\n����� ��������� ������ ���������";
			MessageBox(dlg, error.c_str(), "������", MB_ICONERROR);
			logger(error, "ERROR");
			// ������ �� �������� ������ ������
			EnableWindow(btnCancel, FALSE);
			if (backChange(vecTokens, vecAttr, ctx, hDlg) != 0)
			{
				logger("������ ��������� ����������� ��������", "ERROR");
			}
			EndDialog(dlg, 0);
			return;
		}
		logger("������ ������ Transport ������� ���������", "INFO");

		// �������� �����
		if (flagInstallUTM == false)
		{
			logger("����� ��������� ������� �������������", "INFO");
			SetEvent(evt);
			return;
		}

		// �������� ������ ��� (������ �� ������� ��������)
		str = "�������� ������ ��� 1 (������ �� ������� ��������)";
		SetDlgItemText(dlg, IDC_STATIC_PROGRESSBAR2, str.c_str());
		logger(str, "INFO");
		SendMessage(GetDlgItem(dlg, IDC_PROGRESS1), PBM_STEPIT, 0, 0);
		err = checkHomePageUTM(ports[0], flagStartUTM);
		if (err == 2)
		{
			logger("����� ��������� ������� �������������", "INFO");
			return;
		}
		if (err != 0)
		{
			error = "�� ������� ��������� ������ ���! ��� ������ - " + std::to_string(err) + "\n������� ����������� �� ��������\n����� ��������� ������ ���������";
			MessageBox(dlg, error.c_str(), "������", MB_ICONERROR);
			logger(error, "ERROR");
			// ������ �� �������� ������ ������
			EnableWindow(btnCancel, FALSE);
			if (backChange(vecTokens, vecAttr, ctx, hDlg) != 0)
			{
				logger("������ ��������� ����������� ��������", "ERROR");
			}
			EndDialog(dlg, 0);
			return;
		}
		str = "�������� ������ ��� 1 ������� ���������";
		logger(str, "INFO");
	}
	else // ����� ����������
	{
		int countPort = 0; // ������ ��� ������ ������ ��������� � ������ �������

		// ��������� ���� ���
		for (int i = countUTM; i > 1; --i)
		{
			// �������� ����
			str = "�������� ��� " + std::to_string(i);
			SetDlgItemText(dlg, IDC_STATIC_PROGRESSBAR2, str.c_str());
			logger(str, "INFO");
			SendMessage(GetDlgItem(dlg, IDC_PROGRESS1), PBM_STEPIT, 0, 0);
			if (copyFolderUTM(i) == 1)
			{
				error = "�� ������� ����������� ����� ���! ��� ������ - " + std::to_string(GetLastError()) + "\n����� ��������� ������ ���������";
				MessageBox(dlg, error.c_str(), "������", MB_ICONERROR);
				logger(error, "ERROR");
				// ������ �� �������� ������ ������
				EnableWindow(btnCancel, FALSE);
				if (backChange(vecTokens, vecAttr, ctx, hDlg) != 0)
				{
					logger("������ ��������� ����������� ��������", "ERROR");
				}
				EndDialog(dlg, 0);
				return;
			}
			str = "����������� ��� " + std::to_string(i) + " ������� ���������";
			logger(str, "INFO");

			// �������� �����
			if (flagInstallUTM == false)
			{
				logger("����� ��������� ������� �������������", "INFO");
				SetEvent(evt);
				return;
			}

			// �������� �������
			str = "�������� ������� � ��� " + std::to_string(i);
			SetDlgItemText(dlg, IDC_STATIC_PROGRESSBAR2, str.c_str());
			logger(str, "INFO");
			SendMessage(GetDlgItem(dlg, IDC_PROGRESS1), PBM_STEPIT, 0, 0);
			if (copyBatFiles(i) == 1)
			{
				error = "�� ������� ����������� ������� � ���! ��� ������ - " + std::to_string(GetLastError()) + "\n����� ��������� ������ ���������";
				MessageBox(dlg, error.c_str(), "������", MB_ICONERROR);
				logger(error, "ERROR");
				// ������ �� �������� ������ ������
				EnableWindow(btnCancel, FALSE);
				if (backChange(vecTokens, vecAttr, ctx, hDlg) != 0)
				{
					logger("������ ��������� ����������� ��������", "ERROR");
				}
				EndDialog(dlg, 0);
				return;
			}
			str = "����������� �������� � ��� " + std::to_string(i) + " ������� ���������";
			logger(str, "INFO");

			// �������� �����
			if (flagInstallUTM == false)
			{
				logger("����� ��������� ������� �������������", "INFO");
				SetEvent(evt);
				return;
			}

			// ���������� ������� ���
			str = "���������� ������� � ��� " + std::to_string(i);
			SetDlgItemText(dlg, IDC_STATIC_PROGRESSBAR2, str.c_str());
			logger(str, "INFO");
			if (changePortConfigUTM(i, ports[countPort]) == 1)
			{
				error = "�� ������� �������� ������ � ���! ��� ������ - " + std::to_string(GetLastError()) + "\n����� ��������� ������ ���������";
				MessageBox(dlg, error.c_str(), "������", MB_ICONERROR);
				logger(error, "ERROR");
				// ������ �� �������� ������ ������
				EnableWindow(btnCancel, FALSE);
				if (backChange(vecTokens, vecAttr, ctx, hDlg) != 0)
				{
					logger("������ ��������� ����������� ��������", "ERROR");
				}
				EndDialog(dlg, 0);
				return;
			}
			++countPort;
			str = "����������� �������� � ��� " + std::to_string(i) + " ������� ���������";
			logger(str, "INFO");

			// �������� �����
			if (flagInstallUTM == false)
			{
				logger("����� ��������� ������� �������������", "INFO");
				SetEvent(evt);
				return;
			}

			// ������������� ������ ���
			str = "������������� ������ ��� " + std::to_string(i);
			SetDlgItemText(dlg, IDC_STATIC_PROGRESSBAR2, str.c_str());
			logger(str, "INFO");
			SendMessage(GetDlgItem(dlg, IDC_PROGRESS1), PBM_STEPIT, 0, 0);
			if (installServiceUTM(i) == 1)
			{
				error = "�� ������� ���������� ������ ���! ��� ������ - " + std::to_string(GetLastError()) + "\n����� ��������� ������ ���������";
				MessageBox(dlg, error.c_str(), "������", MB_ICONERROR);
				logger(error, "ERROR");
				// ������ �� �������� ������ ������
				EnableWindow(btnCancel, FALSE);
				if (backChange(vecTokens, vecAttr, ctx, hDlg) != 0)
				{
					logger("������ ��������� ����������� ��������", "ERROR");
				}
				EndDialog(dlg, 0);
				return;
			}
			str = "��������� ������ ��� " + std::to_string(i) + " ������� ���������";
			logger(str, "INFO");

			// �������� �����
			if (flagInstallUTM == false)
			{
				logger("����� ��������� ������� �������������", "INFO");
				SetEvent(evt);
				return;
			}
		}

		// ������ ���� ���
		// �������� �� ����������� ������
		std::vector<std::string> vecTokensNoChoose, vecAttrNoChoose;
		if (vecTokens.size() != vectorNameReaders.size())
		{
			std::vector<std::string>::iterator it;
			for (unsigned int i = 0; i < vecTokens.size(); ++i)
			{
				it = std::find(vectorNameReaders.begin(), vectorNameReaders.end(), vecTokens[i]);
				if (it == vectorNameReaders.end())
				{
					vecTokensNoChoose.push_back(vecTokens[i]);
					vecAttrNoChoose.push_back(vecAttr[i]);
				}
			}
		}

		// �������� �����
		if (flagInstallUTM == false)
		{
			logger("����� ��������� ������� �������������", "INFO");
			SetEvent(evt);
			return;
		}

		// ������� ��� ������
		str = "�������� ���� �������";
		SetDlgItemText(dlg, IDC_STATIC_PROGRESSBAR2, str.c_str());
		logger(str, "INFO");
		SendMessage(GetDlgItem(dlg, IDC_PROGRESS1), PBM_STEPIT, 0, 0);
		for (std::string elem : vecTokens)
		{
			err = deleteReader(elem, ctx);
			if (err != 0)
			{
				error = "�������� ���� ������� ����������� �������! ��� ������ - " + std::to_string(err) + "\n����� ��������� ������ ���������";
				MessageBox(dlg, error.c_str(), "������", MB_ICONERROR);
				logger(error, "ERROR");
				// ������ �� �������� ������ ������
				EnableWindow(btnCancel, FALSE);
				if (backChange(vecTokens, vecAttr, ctx, hDlg) != 0)
				{
					logger("������ ��������� ����������� ��������", "ERROR");
				}
				EndDialog(dlg, 0);
				return;
			}
		}
		logger("�������� ���� ������� ������� ���������", "INFO");

		// �������� �����
		if (flagInstallUTM == false)
		{
			logger("����� ��������� ������� �������������", "INFO");
			SetEvent(evt);
			return;
		}

		// ��������� ������, ������� � �����, � ����� �� ��������� ������, ����� ��������� � �����
		// � ��������� ����
		str = "��������� ������ ���";
		SetDlgItemText(dlg, IDC_STATIC_PROGRESSBAR2, str.c_str());
		logger(str, "INFO");
		SendMessage(GetDlgItem(dlg, IDC_PROGRESS1), PBM_STEPIT, 0, 0);

		// ���������� ������� � ������ ����� ���
		// ������� ��������� �� ��������� ������
		std::string nameReader, numberService;
		int countReader = 1;
		for (unsigned int i = 0; i < vecTokensNoChoose.size(); ++i, ++countReader)
		{
			nameReader = "Aktiv Rutoken ECP " + std::to_string(vecTokens.size() - countReader);
			str = "��������� ����� " + nameReader;
			SetDlgItemText(dlg, IDC_STATIC_PROGRESSBAR2, str.c_str());
			logger(str, "INFO");
			SendMessage(GetDlgItem(dlg, IDC_PROGRESS1), PBM_STEPIT, 0, 0);
			err = addNameReader(nameReader, ctx, vecAttrNoChoose[i]);
			if (err != 0)
			{
				error = "���������� ������ " + nameReader + " ����������� �������! ��� ������ - " + std::to_string(err) + "\n����� ��������� ������ ���������";
				MessageBox(dlg, error.c_str(), "������", MB_ICONERROR);
				logger(error, "ERROR");
				// ������ �� �������� ������ ������
				EnableWindow(btnCancel, FALSE);
				if (backChange(vecTokens, vecAttr, ctx, hDlg) != 0)
				{
					logger("������ ��������� ����������� ��������", "ERROR");
				}
				EndDialog(dlg, 0);
				return;
			}

			// �������� �����
			if (flagInstallUTM == false)
			{
				logger("����� ��������� ������� �������������", "INFO");
				SetEvent(evt);
				return;
			}
		}
		logger("���������� �� ��������� ������� ������� ���������", "INFO");
		// ����� ��������� ��������� ������ � ����� � ��������� ����
		for (unsigned int i = 0; i < vectorNameReaders.size(); ++i, ++countReader)
		{
			if (i == 1) // ���� ������� �� ����� �����������, ������� ��������� �� ������ ����
			{
				// ��������� ������� javaw.exe
				if (killProcessJava() == 1)
				{
					error = "�� ������� ��������� ������� javaw.exe! ��� ������ - " + std::to_string(GetLastError()) + "\n����� ��������� ������ ���������";
					MessageBox(dlg, error.c_str(), "������", MB_ICONERROR);
					logger(error, "ERROR");
					// ������ �� �������� ������ ������
					EnableWindow(btnCancel, FALSE);
					if (backChange(vecTokens, vecAttr, ctx, hDlg) != 0)
					{
						logger("������ ��������� ����������� ��������", "ERROR");
					}
					EndDialog(dlg, 0);
					return;
				}
				logger("��������� ������� javaw.exe", "INFO");

				// �������� �����
				if (flagInstallUTM == false)
				{
					logger("����� ��������� ������� �������������", "INFO");
					SetEvent(evt);
					return;
				}

				// ������� ��� �� ������������
				if (deleteFileUTMlnk() == 1)
				{
					error = "�� ������� ������� UTM.lnk! ��� ������ - " + std::to_string(GetLastError()) + "\n����� ��������� ������ ���������";
					MessageBox(dlg, error.c_str(), "������", MB_ICONERROR);
					logger(error, "ERROR");
					// ������ �� �������� ������ ������
					EnableWindow(btnCancel, FALSE);
					if (backChange(vecTokens, vecAttr, ctx, hDlg) != 0)
					{
						logger("������ ��������� ����������� ��������", "ERROR");
					}
					EndDialog(dlg, 0);
					return;
				}
				logger("������� UTM.lnk �� ������������", "INFO");
			}

			// �������� �����
			if (flagInstallUTM == false)
			{
				logger("����� ��������� ������� �������������", "INFO");
				SetEvent(evt);
				return;
			}

			// ���������� ������
			nameReader = "Aktiv Rutoken ECP " + std::to_string(vecTokens.size() - countReader);
			str = "��������� ����� " + nameReader;
			SetDlgItemText(dlg, IDC_STATIC_PROGRESSBAR2, str.c_str());
			logger(str, "INFO");
			SendMessage(GetDlgItem(dlg, IDC_PROGRESS1), PBM_STEPIT, 0, 0);
			err = addNameReader(nameReader, ctx, vectorAttrReaders[i]);
			if (err != 0)
			{
				error = "���������� ������ " + nameReader + " ����������� �������! ��� ������ - " + std::to_string(err) + "\n����� ��������� ������ ���������";
				MessageBox(dlg, error.c_str(), "������", MB_ICONERROR);
				logger(error, "ERROR");
				// ������ �� �������� ������ ������
				EnableWindow(btnCancel, FALSE);
				if (backChange(vecTokens, vecAttr, ctx, hDlg) != 0)
				{
					logger("������ ��������� ����������� ��������", "ERROR");
				}
				EndDialog(dlg, 0);
				return;
			}
			logger("���������� ������ " + nameReader + " ������� ���������", "INFO");

			// �������� �����
			if (flagInstallUTM == false)
			{
				logger("����� ��������� ������� �������������", "INFO");
				SetEvent(evt);
				return;
			}

			// ������ ������ ���
			numberService = std::to_string(vecTokens.size() - countReader + 1);
			if (numberService == "1")
			{
				str = "������ ������ Transport";
				numberService = "";
			}
			else
			{
				str = "������ ������ Transport " + numberService;
			}
			SetDlgItemText(dlg, IDC_STATIC_PROGRESSBAR2, str.c_str());
			logger(str, "INFO");
			SendMessage(GetDlgItem(dlg, IDC_PROGRESS1), PBM_STEPIT, 0, 0);
			err = startServiceUTM("Transport" + numberService);
			if (err != 0)
			{
				error = "������ ������� ������ Transport" + numberService + "! ��� ������ - " + std::to_string(GetLastError()) + "\n����� ��������� ������ ���������";
				MessageBox(dlg, error.c_str(), "������", MB_ICONERROR);
				logger(error, "ERROR");
				// ������ �� �������� ������ ������
				EnableWindow(btnCancel, FALSE);
				if (backChange(vecTokens, vecAttr, ctx, hDlg) != 0)
				{
					logger("������ ��������� ����������� ��������", "ERROR");
				}
				EndDialog(dlg, 0);
				return;
			}
			str = "������ ������ Transport " + numberService + " ������� ���������";
			logger(str, "INFO");
			Sleep(500);

			// �������� �����
			if (flagInstallUTM == false)
			{
				logger("����� ��������� ������� �������������", "INFO");
				SetEvent(evt);
				return;
			}

			// �������� ������ ���
			str = "�������� ������ ��� " + numberService + " (������ �� ������� ��������)";
			SetDlgItemText(dlg, IDC_STATIC_PROGRESSBAR2, str.c_str());
			logger(str, "INFO");
			SendMessage(GetDlgItem(dlg, IDC_PROGRESS1), PBM_STEPIT, 0, 0);
			err = checkHomePageUTM(ports[i], flagStartUTM);
			if (err == 2)
			{
				logger("����� ��������� ������� �������������", "INFO");
				SetEvent(evt);
				return;
			}
			if (err != 0)
			{
				error = "�� ������� ��������� ������ ��� " + numberService + "! ��� ������ - " + std::to_string(err) + "\n������� ����������� �� ��������\n����� ��������� ������ ���������";
				MessageBox(dlg, error.c_str(), "������", MB_ICONERROR);
				logger(error, "ERROR");
				// ������ �� �������� ������ ������
				EnableWindow(btnCancel, FALSE);
				if (backChange(vecTokens, vecAttr, ctx, hDlg) != 0)
				{
					logger("������ ��������� ����������� ��������", "ERROR");
				}
				EndDialog(dlg, 0);
				return;
			}
			str = "�������� ������ ��� " + numberService + " ������� ���������";
			logger(str, "INFO");
		}

		str = "������ ����� ��� ������� ���������";
		logger(str, "INFO");
	}

	SendMessage(GetDlgItem(dlg, IDC_PROGRESS1), PBM_SETPOS, 100, 0);
	flagChangeStatic = false;
	MessageBox(dlg, "��������� ��� ������� ���������!", "�������", MB_ICONINFORMATION);
	logger("��������� ��� ������� ���������!", "INFO");
	EndDialog(dlg, 0);
}

// ������� ���������� � �������, ������ ���, ��� �� ������
void changeStaticText(HWND dlg)
{
	while (flagChangeStatic)
	{
		SetDlgItemText(dlg, IDC_STATIC_POINT, "");
		Sleep(500);
		SetDlgItemText(dlg, IDC_STATIC_POINT, ".");
		Sleep(500);
		SetDlgItemText(dlg, IDC_STATIC_POINT, "..");
		Sleep(500);
		SetDlgItemText(dlg, IDC_STATIC_POINT, "...");
		Sleep(500);
	}
}

// ������ � �������� ���������� ��������, ��������� ���
int startProcessInstallUTM(std::string fileUTM)
{
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	int err = CreateProcess(fileUTM.c_str(), NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
	if (err == 0)
	{
		return 1;
	}
	WaitForSingleObject(pi.hProcess, INFINITE);

	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);

	return 0;
}

// �������� ����� ���
int CopyFolder(std::string src, std::string dst)
{
	WIN32_FIND_DATA FindFileData; // ��������� ��� ������ ������
	HANDLE hFind;

	CreateDirectory(dst.c_str(), NULL); // ������ �����, ���� ����������
	std::string filter = src + "\\*"; // ������ ������
	
	hFind = FindFirstFile(filter.c_str(), &FindFileData); // ����� ������
	if (hFind == NULL)
	{
		return 1;
	}

	do
	{
		if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) // ���� ����� �����
		{
			if (strcmp(FindFileData.cFileName, ".") && strcmp(FindFileData.cFileName, ".."))
			{
				// ���� ���� ��������, ���� ������ ��� ����� ��������
				std::string srs2 = src + "\\" + std::string(FindFileData.cFileName) + "\\";
				std::string dst2 = dst + "\\" + std::string(FindFileData.cFileName);
				CopyFolder(srs2, dst2);
			}
		}
		else // ����� ����
		{
			std::string srs2 = src + "\\" + std::string(FindFileData.cFileName);
			std::string dst2 = dst + "\\" + std::string(FindFileData.cFileName);
			if (CopyFile(srs2.c_str(), dst2.c_str(), TRUE) == 0) // �������� ����
			{
				return 1;
			}
		}
	} while (FindNextFile(hFind, &FindFileData)); // ���� ���� ��� ����������

	FindClose(hFind); // �������
	return 0;
}

// �������� ����� ���
int copyFolderUTM(int numberUTM)
{
	std::string src = "C:\\UTM";
	std::string dst = "C:\\UTM_" + std::to_string(numberUTM);

	if (CopyFolder(src, dst) == 1)
	{
		return 1;
	}

	return 0;
}

// ����������� �������� � ���
int copyBatFiles(int numberUTM)
{
	std::string srcPath = pathExe + "transport_bat\\";
	std::string dstPath = "C:\\UTM_" + std::to_string(numberUTM) + "\\transporter\\bin\\";
	std::vector<std::string> vecBat = { "delete.bat", "install.bat", "run.bat", "stop.bat", "terminal.bat", "terminal-prod.bat" };

	for (std::string elem : vecBat)
	{
		if (CopyFile((srcPath + elem).c_str(), (dstPath + elem).c_str(), FALSE) == 0) // �������� ����
		{
			return 1;
		}
	}

	return 0;
}

// ����������� �������� ��� (������ ������)
int changePortConfigUTM(int numberUTM, std::string port)
{
	std::string config = "C:\\UTM_" + std::to_string(numberUTM) + "\\transporter\\conf\\transport.properties";

	std::ifstream fin;
	fin.open(config);
	if (fin.is_open())
	{
		std::string word_source_web = "8080";
		std::string word_source_transport = "8192";
		std::string word_source_agent = "8194";
		std::string word_destinate_transport = std::to_string(atoi(word_source_transport.c_str()) - numberUTM);
		std::string word_destinate_agent = std::to_string(atoi(word_source_agent.c_str()) + numberUTM);
		std::string temp;

		// ������ ���������
		while (fin)
		{
			std::string str;
			getline(fin, str, '\n');
			if (!fin) break;

			size_t position = 0;

			while (position != std::string::npos)
			{
				size_t position_start = position;

				position = str.find(word_source_web, position);
				if (position != std::string::npos)
				{   
					temp += str.substr(position_start, position - position_start); // ����� �� ����������
					temp += port; // ������ �����
					position += word_source_web.size(); // ������� ������� �� ����� ����������� �����
					break;
				}

				position = 0;
				position = str.find(word_source_transport, position);
				if (position != std::string::npos)
				{
					temp += str.substr(position_start, position - position_start); // ����� �� ����������
					temp += word_destinate_transport; // ������ �����
					position += word_source_transport.size(); // ������� ������� �� ����� ����������� �����
					break;
				}

				position = 0;
				position = str.find(word_source_agent, position);
				if (position != std::string::npos)
				{
					temp += str.substr(position_start, position - position_start); // ����� �� ����������
					temp += word_destinate_agent; // ������ �����
					position += word_source_agent.size(); // ������� ������� �� ����� ����������� �����
					break;
				}

				temp += str.substr(position_start, position - position_start);
			}
			temp += "\n";
		}
		fin.close();

		
		// �������������� ����
		std::ofstream fout(config);
		fout << temp.substr(0, temp.size());
		fout.close();

		return 0;
	}

	return 1;
}

// ���������� ������ � ������ ���, ����� ������ �� ���������� ����
int changeInstallBatUTM()
{
	std::string config = "C:\\UTM\\transporter\\bin\\install.bat";

	std::ifstream fin;
	fin.open(config);
	if (fin.is_open())
	{
		std::string word_source = "--Startup auto";
		std::string word_destinate = "";
		std::string temp;

		// ������ ���������
		while (fin)
		{
			std::string str;
			getline(fin, str, '\n');
			if (!fin) break;

			size_t position = 0;

			while (position != std::string::npos)
			{
				size_t position_start = position;

				position = str.find(word_source, position);
				if (position != std::string::npos)
				{
					temp += str.substr(position_start, position - position_start); // ����� �� ����������
					temp += word_destinate; // ������ �����
					position += word_source.size(); // ������� ������� �� ����� ����������� �����
					break;
				}
				else
				{
					temp += str.substr(position_start, position - position_start);
				}
			}
			temp += "\n";
		}
		fin.close();


		// �������������� ����
		std::ofstream fout(config);
		fout << temp.substr(0, temp.size());
		fout.close();

		return 0;
	}

	return 1;
}

// ��������� ������� javaw.exe
int killProcessJava()
{
	HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, NULL);
	PROCESSENTRY32 pEntry;
	pEntry.dwSize = sizeof(pEntry);
	BOOL hRes = Process32First(hSnapShot, &pEntry);
	while (hRes)
	{
		if (!strcmp(pEntry.szExeFile, "javaw.exe"))
		{
			HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, 0, (DWORD)pEntry.th32ProcessID);
			if (hProcess != NULL)
			{
				TerminateProcess(hProcess, 9);
				CloseHandle(hProcess);
			}
			else
			{
				return 1;
			}
		}
		hRes = Process32Next(hSnapShot, &pEntry);
	}
	CloseHandle(hSnapShot);

	return 0;
}

// ������� �� ������������
int deleteFileUTMlnk()
{
	// ������� ����
	int resultDeleteFileA = DeleteFileA("C:\\ProgramData\\Microsoft\\Windows\\Start Menu\\Programs\\Startup\\UTM.lnk");
	switch (resultDeleteFileA)
	{
	case ERROR_FILE_NOT_FOUND:
		break;
	case ERROR_ACCESS_DENIED:
		return 1;
	case 0:
		return 1;
	default:
		break;
	}
	return 0;
}

// ����� ��������� ��� ������ ��� ������
int backChange(std::vector<std::string> vecRutokens,
	std::vector<std::string> vecAttrRutokens,
	SCARDCONTEXT ctx,
	HWND hDlg)
{
	logger("������ ���������", "INFO");
	SetDlgItemText(hDlg, IDC_STATIC_PROGRESSBAR, "������ ���������");

	int err;
	std::string error;

	// ������� ��� ����� ������
	for (std::string elem : vecRutokens)
	{
		err = deleteReader(elem, ctx);
		if (err == ERROR_FILE_NOT_FOUND) // ���� ��� ������ ������, ����������
		{
			continue;
		}
		if (err != 0)
		{
			flagChangeStatic = false;
			error = "������ ���������. �������� ���� ����� ������� ����������� �������! ��� ������ - " + std::to_string(err);
			logger(error, "ERROR");
			MessageBox(hDlg, error.c_str(), "������", MB_ICONERROR);
			EndDialog(hDlg, 0);
			return 1;
		}
	}
	logger("������ ���������. �������� ���� ����� ������� ������� ���������", "INFO");

	// ��������� ��� ������ ������
	for (unsigned int i = 0; i < vecRutokens.size(); ++i)
	{
		err = addNameReader(vecRutokens[i], ctx, vecAttrRutokens[i]);
		if (err != 0)
		{
			flagChangeStatic = false;
			error = "������ ���������. ���������� ���� ������ ������� ����������� �������! ��� ������ - " + std::to_string(err);
			logger(error, "ERROR");
			MessageBox(hDlg, error.c_str(), "������", MB_ICONERROR);
			EndDialog(hDlg, 0);
			return 1;
		}
	}
	logger("������ ���������. ���������� ���� ������ ������� ������� ���������", "INFO");

	// �������� ����� � ����� ���
	err = deleteUTMBackChange(hDlg);
	if (err != 0)
	{
		flagChangeStatic = false;
		error = "������ ���������. �� ������� �������� ����� � ����� ���, ��� ������ " + std::to_string(GetLastError());
		logger(error, "ERROR");
		MessageBox(hDlg, error.c_str(), "������", MB_ICONERROR);
		EndDialog(hDlg, 0);
		return 1;
	}
	logger("������ ���������. �������� ����� � ����� ��� ������� ���������", "INFO");

	return 0;
}

// ��������� � �������� ����� � �����
int deleteUTMBackChange(HWND hDlg)
{
	logger("������ ���������. ������������ �������� ���", "INFO");

	std::string error;
	int err;
	std::string countUTM;

	// �������� ���-�� ��� �� �������
	err = readConfigCountUTM(countUTM);
	if (err != 0)
	{
		flagChangeStatic = false;
		error = "������ ���������. �� ������� �������� ���-�� ��� �� �������, ��� ������ " + std::to_string(err);
		logger(error, "ERROR");
		MessageBox(hDlg, error.c_str(), "������", MB_ICONERROR);
		EndDialog(hDlg, 0);
		return 1;
	}

	// �������� ������� ��� ���� uninstaller
	err = startProcessInstallUTM("C:\\UTM\\unins000.exe");
	if (err == 1)
	{
		flagChangeStatic = false;
		error = "������ ���������. �������� ��� ����������� �������! ��� ������ - " + std::to_string(GetLastError());
		MessageBox(hDlg, error.c_str(), "������", MB_ICONERROR);
		logger(error, "ERROR");
		EndDialog(hDlg, 0);
		return 1;
	}

	// ��������� ����� � �������� ���
	int intCountUTM = atoi(countUTM.c_str());
	for (int i = intCountUTM; i > 0; --i)
	{
		if (i == 1)
		{
			break;
		}
		err = stopServiceUTM("Transport" + std::to_string(i));
		if (err == ERROR_FILE_NOT_FOUND) // ���� ��� ������, ����������
		{
			continue;
		}
		if (err != 0)
		{
			flagChangeStatic = false;
			error = "������ ���������. �� ������� ���������� ������ Transport" + std::to_string(i) + ", ��� ������ " + std::to_string(err);
			MessageBox(hDlg, error.c_str(), "������", MB_ICONERROR);
			logger(error, "ERROR");
			EndDialog(hDlg, 0);
			return 1;
		}
		Sleep(500);
		err = deleteServiceUTM(i);
		if (err == ERROR_FILE_NOT_FOUND) // ���� ��� ������, ����������
		{
			continue;
		}
		if (err != 0)
		{
			flagChangeStatic = false;
			error = "������ ���������. �� ������� ������� ������ Transport" + std::to_string(i) + ", ��� ������ " + std::to_string(GetLastError());
			MessageBox(hDlg, error.c_str(), "������", MB_ICONERROR);
			logger(error, "ERROR");
			EndDialog(hDlg, 0);
			return 1;
		}
		Sleep(500);
		err = deleteFolderUTMBackChange("C:\\UTM_" + std::to_string(i));
		if (err == ERROR_FILE_NOT_FOUND) // ���� ��� ������, ����������
		{
			continue;
		}
		if (err != 0)
		{
			flagChangeStatic = false;
			error = "������ ���������. �� ������� ������� C:\\UTM_" + std::to_string(i) + ", ��� ������ " + std::to_string(GetLastError());
			MessageBox(hDlg, error.c_str(), "������", MB_ICONERROR);
			logger(error, "ERROR");
			EndDialog(hDlg, 0);
			return 1;
		}
	}

	flagChangeStatic = false;
	logger("������ ���������. �������� ��� ������� ���������", "INFO");
	MessageBox(hDlg, "������ ���������. �������� ��� ������� ���������", "�������", MB_ICONINFORMATION);
	EndDialog(hDlg, 0);

	return 0;
}

// ������� ����� ���
int deleteFolderUTMBackChange(std::string src)
{
	WIN32_FIND_DATA FindFileData; // ��������� ��� ������ ������
	HANDLE hFind;

	std::string filter = src + "\\*"; // ������ ������
	std::string src2;

	hFind = FindFirstFile(filter.c_str(), &FindFileData); // ����� ������
	if (hFind == NULL)
	{
		return 1;
	}

	do
	{
		if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) // ���� ����� �����
		{
			if (strcmp(FindFileData.cFileName, ".") && strcmp(FindFileData.cFileName, ".."))
			{
				// ���� ���� ��������, ���� ������ ��� ����� ��������
				src2 = src + "\\" + std::string(FindFileData.cFileName) + "\\";
				deleteFolderUTMBackChange(src2); // ��������
			}
		}
		else // ����� ����
		{
			src2 = src + "\\" + std::string(FindFileData.cFileName);
			if (DeleteFile(src2.c_str()) == 0) // ������� ����
			{
				return 1;
			}
		}
	} while (FindNextFile(hFind, &FindFileData)); // ���� ���� ��� �������

	// ������� ������ �����
	if (RemoveDirectory(src.c_str()) == 0)
	{
		return 1;
	}

	FindClose(hFind); // �������
	return 0;
}