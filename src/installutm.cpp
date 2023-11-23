#define _CRT_SECURE_NO_WARNINGS
#include "installUTM.h"
#include "ControlReaders.h"
#include "ControlService.h"


extern std::string pathExe;
extern bool flagInstallUTM;
bool flagChangeStatic = true;
HANDLE evt = CreateEvent(
	NULL, // игнорируеться, должен быть NULL
	TRUE, // автоматический сброс ивента (TRUE - выключен, нужно использовать ResetEvent)
	FALSE, // начальное состояние (FALSE - не сигнальное)
	NULL); // имя ивента

// установка УТМ
void installUTM(HWND dlg, int countUTM, std::string fileUTM, SCARDCONTEXT ctx, std::vector<std::string> vecTokens,
	std::vector<std::string> vecAttr, HWND btnCancel, HWND hDlg)
{
	flagInstallUTM = true;
	flagChangeStatic = true;
	bool flagStartUTM = false;

	// Делаем не активной кнопку отмены
	EnableWindow(btnCancel, FALSE);

	// Устанавливаем шаг прогрессбара
	SendMessage(GetDlgItem(dlg, IDC_PROGRESS1), PBM_SETSTEP, 1, 0);

	std::string str;
	int err = 0;
	std::string error;

	str = "Установка УТМ 1";
	SetDlgItemText(dlg, IDC_STATIC_PROGRESSBAR2, str.c_str());
	logger(str, "INFO");
	SendMessage(GetDlgItem(dlg, IDC_PROGRESS1), PBM_STEPIT, 0, 0);

	// Установка УТМ
	err = startProcessInstallUTM(fileUTM);
	if (err == 1)
	{
		flagChangeStatic = false;
		error = "Установка УТМ завершилась ошибкой! Код ошибки - " + std::to_string(GetLastError());
		MessageBox(dlg, error.c_str(), "Ошибка", MB_ICONERROR);
		logger(error, "ERROR");
		EndDialog(dlg, 0);
		return;
	}
	str = "УТМ 1 успешно установлен";
	logger(str, "INFO");
	SendMessage(GetDlgItem(dlg, IDC_PROGRESS1), PBM_STEPIT, 0, 0);

	// Удаляем службу УТМ
	if (deleteServiceUTM(1) == 1)
	{
		flagChangeStatic = false;
		error = "Не удалось удалить службу УТМ! Код ошибки - " + std::to_string(GetLastError()) + "\nБудет выполнена отмена изменений";
		MessageBox(dlg, error.c_str(), "Ошибка", MB_ICONERROR);
		logger(error, "ERROR");
		if (backChange(vecTokens, vecAttr, ctx, hDlg) != 0)
		{
			logger("Отмена изменений завершилась неудачей", "ERROR");
		}
		EndDialog(dlg, 0);
		return;
	}
	str = "Служба УТМ удалена";
	logger(str, "INFO");
	SendMessage(GetDlgItem(dlg, IDC_PROGRESS1), PBM_STEPIT, 0, 0);

	// Исправляем батник в первом утм, чтобы служба не стартовала сама
	if (changeInstallBatUTM() == 1)
	{
		flagChangeStatic = false;
		error = "Не удалось изменить install.bat в УТМ! Код ошибки - " + std::to_string(GetLastError()) + "\nБудет выполнена отмена изменений";
		MessageBox(dlg, error.c_str(), "Ошибка", MB_ICONERROR);
		logger(error, "ERROR");
		if (backChange(vecTokens, vecAttr, ctx, hDlg) != 0)
		{
			logger("Отмена изменений завершилась неудачей", "ERROR");
		}
		EndDialog(dlg, 0);
		return;
	}
	str = "Батник УТМ исправлен";
	logger(str, "INFO");
	SendMessage(GetDlgItem(dlg, IDC_PROGRESS1), PBM_STEPIT, 0, 0);

	// Устанавливаем службу первого УТМ уже без автозапуска
	if (installServiceUTM(1) == 1)
	{
		flagChangeStatic = false;
		error = "Не удалось установить службу УТМ! Код ошибки - " + std::to_string(GetLastError()) + "\nБудет выполнена отмена изменений";
		MessageBox(dlg, error.c_str(), "Ошибка", MB_ICONERROR);
		logger(error, "ERROR");
		if (backChange(vecTokens, vecAttr, ctx, hDlg) != 0)
		{
			logger("Отмена изменений завершилась неудачей", "ERROR");
		}
		EndDialog(dlg, 0);
		return;
	}
	str = "Служба УТМ установлена";
	logger(str, "INFO");
	SendMessage(GetDlgItem(dlg, IDC_PROGRESS1), PBM_STEPIT, 0, 0);

	// Делаем активной кнопку отмены
	EnableWindow(btnCancel, TRUE);

	// проверка флага
	if (flagInstallUTM == false)
	{
		logger("Поток установки отменен пользователем", "INFO");
		SetEvent(evt);
		return;
	}

	// Читаем конфиг
	SetDlgItemText(dlg, IDC_STATIC_PROGRESSBAR2, "Чтение конфига");
	logger("Чтение конфига", "INFO");
	SendMessage(GetDlgItem(dlg, IDC_PROGRESS1), PBM_STEPIT, 0, 0);
	std::vector<std::string> vectorNameReaders, vectorAttrReaders, ports, serialNumbers;
	if (readConfigNameAndReader(countUTM, vectorNameReaders, vectorAttrReaders, ports, serialNumbers) == 1)
	{
		error = "Чтение конфига завершилась ошибкой! Код ошибки - " + std::to_string(GetLastError()) + "\nБудет выполнена отмена изменений";
		MessageBox(dlg, error.c_str(), "Ошибка", MB_ICONERROR);
		logger(error, "ERROR");
		// Делаем не активной кнопку отмены
		EnableWindow(btnCancel, FALSE);
		if (backChange(vecTokens, vecAttr, ctx, hDlg) != 0)
		{
			logger("Отмена изменений завершилась неудачей", "ERROR");
		}
		EndDialog(dlg, 0);
		return;
	}
	logger("Чтение конфига успешно завершено", "INFO");

	// проверка флага
	if (flagInstallUTM == false)
	{
		logger("Поток установки отменен пользователем", "INFO");
		SetEvent(evt);
		return;
	}

	// Если ставится 1 УТМ, то завершаем
	if (ports.size() == 1)
	{
		// запуск службы УТМ
		SetDlgItemText(dlg, IDC_STATIC_PROGRESSBAR2, "Запуск службы Transport");
		logger("Запуск службы Transport", "INFO");
		SendMessage(GetDlgItem(dlg, IDC_PROGRESS1), PBM_STEPIT, 0, 0);
		err = startServiceUTM("Transport");
		if (err != 0)
		{
			error = "Ошибка запуска службы Trasport! Код ошибки - " + std::to_string(GetLastError()) + "\nБудет выполнена отмена изменений";
			MessageBox(dlg, error.c_str(), "Ошибка", MB_ICONERROR);
			logger(error, "ERROR");
			// Делаем не активной кнопку отмены
			EnableWindow(btnCancel, FALSE);
			if (backChange(vecTokens, vecAttr, ctx, hDlg) != 0)
			{
				logger("Отмена изменений завершилась неудачей", "ERROR");
			}
			EndDialog(dlg, 0);
			return;
		}
		logger("Запуск службы Transport успешно завершено", "INFO");

		// проверка флага
		if (flagInstallUTM == false)
		{
			logger("Поток установки отменен пользователем", "INFO");
			SetEvent(evt);
			return;
		}

		// Проверка работы УТМ (запрос на главную страницу)
		str = "Проверка работы УТМ 1 (запрос на главную страницу)";
		SetDlgItemText(dlg, IDC_STATIC_PROGRESSBAR2, str.c_str());
		logger(str, "INFO");
		SendMessage(GetDlgItem(dlg, IDC_PROGRESS1), PBM_STEPIT, 0, 0);
		err = checkHomePageUTM(ports[0], flagStartUTM);
		if (err == 2)
		{
			logger("Поток установки отменен пользователем", "INFO");
			return;
		}
		if (err != 0)
		{
			error = "Не удалось проверить работу УТМ! Код ошибки - " + std::to_string(err) + "\nФункция завершилась по таймауту\nБудет выполнена отмена изменений";
			MessageBox(dlg, error.c_str(), "Ошибка", MB_ICONERROR);
			logger(error, "ERROR");
			// Делаем не активной кнопку отмены
			EnableWindow(btnCancel, FALSE);
			if (backChange(vecTokens, vecAttr, ctx, hDlg) != 0)
			{
				logger("Отмена изменений завершилась неудачей", "ERROR");
			}
			EndDialog(dlg, 0);
			return;
		}
		str = "Проверка работы УТМ 1 успешно завершена";
		logger(str, "INFO");
	}
	else // иначе продолжаем
	{
		int countPort = 0; // потому что массив портов развернут в другую сторону

		// УСТАНОВКА ВСЕХ УТМ
		for (int i = countUTM; i > 1; --i)
		{
			// копируем УТМы
			str = "Копируем УТМ " + std::to_string(i);
			SetDlgItemText(dlg, IDC_STATIC_PROGRESSBAR2, str.c_str());
			logger(str, "INFO");
			SendMessage(GetDlgItem(dlg, IDC_PROGRESS1), PBM_STEPIT, 0, 0);
			if (copyFolderUTM(i) == 1)
			{
				error = "Не удалось скопировать папку УТМ! Код ошибки - " + std::to_string(GetLastError()) + "\nБудет выполнена отмена изменений";
				MessageBox(dlg, error.c_str(), "Ошибка", MB_ICONERROR);
				logger(error, "ERROR");
				// Делаем не активной кнопку отмены
				EnableWindow(btnCancel, FALSE);
				if (backChange(vecTokens, vecAttr, ctx, hDlg) != 0)
				{
					logger("Отмена изменений завершилась неудачей", "ERROR");
				}
				EndDialog(dlg, 0);
				return;
			}
			str = "Копирование УТМ " + std::to_string(i) + " успешно завершена";
			logger(str, "INFO");

			// проверка флага
			if (flagInstallUTM == false)
			{
				logger("Поток установки отменен пользователем", "INFO");
				SetEvent(evt);
				return;
			}

			// копируем батники
			str = "Копируем батники в УТМ " + std::to_string(i);
			SetDlgItemText(dlg, IDC_STATIC_PROGRESSBAR2, str.c_str());
			logger(str, "INFO");
			SendMessage(GetDlgItem(dlg, IDC_PROGRESS1), PBM_STEPIT, 0, 0);
			if (copyBatFiles(i) == 1)
			{
				error = "Не удалось скопировать батники в УТМ! Код ошибки - " + std::to_string(GetLastError()) + "\nБудет выполнена отмена изменений";
				MessageBox(dlg, error.c_str(), "Ошибка", MB_ICONERROR);
				logger(error, "ERROR");
				// Делаем не активной кнопку отмены
				EnableWindow(btnCancel, FALSE);
				if (backChange(vecTokens, vecAttr, ctx, hDlg) != 0)
				{
					logger("Отмена изменений завершилась неудачей", "ERROR");
				}
				EndDialog(dlg, 0);
				return;
			}
			str = "Копирование батников в УТМ " + std::to_string(i) + " успешно завершена";
			logger(str, "INFO");

			// проверка флага
			if (flagInstallUTM == false)
			{
				logger("Поток установки отменен пользователем", "INFO");
				SetEvent(evt);
				return;
			}

			// исправляем конфиги УТМ
			str = "Исправляем конфиги в УТМ " + std::to_string(i);
			SetDlgItemText(dlg, IDC_STATIC_PROGRESSBAR2, str.c_str());
			logger(str, "INFO");
			if (changePortConfigUTM(i, ports[countPort]) == 1)
			{
				error = "Не удалось изменить конфиг в УТМ! Код ошибки - " + std::to_string(GetLastError()) + "\nБудет выполнена отмена изменений";
				MessageBox(dlg, error.c_str(), "Ошибка", MB_ICONERROR);
				logger(error, "ERROR");
				// Делаем не активной кнопку отмены
				EnableWindow(btnCancel, FALSE);
				if (backChange(vecTokens, vecAttr, ctx, hDlg) != 0)
				{
					logger("Отмена изменений завершилась неудачей", "ERROR");
				}
				EndDialog(dlg, 0);
				return;
			}
			++countPort;
			str = "Исправление конфигов в УТМ " + std::to_string(i) + " успешно завершена";
			logger(str, "INFO");

			// проверка флага
			if (flagInstallUTM == false)
			{
				logger("Поток установки отменен пользователем", "INFO");
				SetEvent(evt);
				return;
			}

			// Устанавливаем службу УТМ
			str = "Устанавливаем службу УТМ " + std::to_string(i);
			SetDlgItemText(dlg, IDC_STATIC_PROGRESSBAR2, str.c_str());
			logger(str, "INFO");
			SendMessage(GetDlgItem(dlg, IDC_PROGRESS1), PBM_STEPIT, 0, 0);
			if (installServiceUTM(i) == 1)
			{
				error = "Не удалось установить службу УТМ! Код ошибки - " + std::to_string(GetLastError()) + "\nБудет выполнена отмена изменений";
				MessageBox(dlg, error.c_str(), "Ошибка", MB_ICONERROR);
				logger(error, "ERROR");
				// Делаем не активной кнопку отмены
				EnableWindow(btnCancel, FALSE);
				if (backChange(vecTokens, vecAttr, ctx, hDlg) != 0)
				{
					logger("Отмена изменений завершилась неудачей", "ERROR");
				}
				EndDialog(dlg, 0);
				return;
			}
			str = "Установка службы УТМ " + std::to_string(i) + " успешно завершена";
			logger(str, "INFO");

			// проверка флага
			if (flagInstallUTM == false)
			{
				logger("Поток установки отменен пользователем", "INFO");
				SetEvent(evt);
				return;
			}
		}

		// ЗАПУСК ВСЕХ УТМ
		// проверка на невыбранные ридеры
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

		// проверка флага
		if (flagInstallUTM == false)
		{
			logger("Поток установки отменен пользователем", "INFO");
			SetEvent(evt);
			return;
		}

		// Удаляем все ридеры
		str = "Удаление всех ридеров";
		SetDlgItemText(dlg, IDC_STATIC_PROGRESSBAR2, str.c_str());
		logger(str, "INFO");
		SendMessage(GetDlgItem(dlg, IDC_PROGRESS1), PBM_STEPIT, 0, 0);
		for (std::string elem : vecTokens)
		{
			err = deleteReader(elem, ctx);
			if (err != 0)
			{
				error = "Удаление всех ридеров завершилось ошибкой! Код ошибки - " + std::to_string(err) + "\nБудет выполнена отмена изменений";
				MessageBox(dlg, error.c_str(), "Ошибка", MB_ICONERROR);
				logger(error, "ERROR");
				// Делаем не активной кнопку отмены
				EnableWindow(btnCancel, FALSE);
				if (backChange(vecTokens, vecAttr, ctx, hDlg) != 0)
				{
					logger("Отмена изменений завершилась неудачей", "ERROR");
				}
				EndDialog(dlg, 0);
				return;
			}
		}
		logger("Удаление всех ридеров успешно завершено", "INFO");

		// проверка флага
		if (flagInstallUTM == false)
		{
			logger("Поток установки отменен пользователем", "INFO");
			SetEvent(evt);
			return;
		}

		// Добавляем ридеры, начиная с конца, в конце не выбранные ридеры, далее выбранные с конца
		// И запускаем УТМы
		str = "Запускаем службы УТМ";
		SetDlgItemText(dlg, IDC_STATIC_PROGRESSBAR2, str.c_str());
		logger(str, "INFO");
		SendMessage(GetDlgItem(dlg, IDC_PROGRESS1), PBM_STEPIT, 0, 0);

		// добавление ридеров и запуск служб УТМ
		// вначале добавляем не выбранные ридеры
		std::string nameReader, numberService;
		int countReader = 1;
		for (unsigned int i = 0; i < vecTokensNoChoose.size(); ++i, ++countReader)
		{
			nameReader = "Aktiv Rutoken ECP " + std::to_string(vecTokens.size() - countReader);
			str = "Добавляем ридер " + nameReader;
			SetDlgItemText(dlg, IDC_STATIC_PROGRESSBAR2, str.c_str());
			logger(str, "INFO");
			SendMessage(GetDlgItem(dlg, IDC_PROGRESS1), PBM_STEPIT, 0, 0);
			err = addNameReader(nameReader, ctx, vecAttrNoChoose[i]);
			if (err != 0)
			{
				error = "Добавление ридера " + nameReader + " завершилось ошибкой! Код ошибки - " + std::to_string(err) + "\nБудет выполнена отмена изменений";
				MessageBox(dlg, error.c_str(), "Ошибка", MB_ICONERROR);
				logger(error, "ERROR");
				// Делаем не активной кнопку отмены
				EnableWindow(btnCancel, FALSE);
				if (backChange(vecTokens, vecAttr, ctx, hDlg) != 0)
				{
					logger("Отмена изменений завершилась неудачей", "ERROR");
				}
				EndDialog(dlg, 0);
				return;
			}

			// проверка флага
			if (flagInstallUTM == false)
			{
				logger("Поток установки отменен пользователем", "INFO");
				SetEvent(evt);
				return;
			}
		}
		logger("Добавление не выбранных ридеров успешно завершено", "INFO");
		// далее добавляем выбранные ридеры с конца и запускаем утмы
		for (unsigned int i = 0; i < vectorNameReaders.size(); ++i, ++countReader)
		{
			if (i == 1) // этот процесс не сразу появляеться, поэтому завершаем на втором утме
			{
				// Завершаем процесс javaw.exe
				if (killProcessJava() == 1)
				{
					error = "Не удалось завершить процесс javaw.exe! Код ошибки - " + std::to_string(GetLastError()) + "\nБудет выполнена отмена изменений";
					MessageBox(dlg, error.c_str(), "Ошибка", MB_ICONERROR);
					logger(error, "ERROR");
					// Делаем не активной кнопку отмены
					EnableWindow(btnCancel, FALSE);
					if (backChange(vecTokens, vecAttr, ctx, hDlg) != 0)
					{
						logger("Отмена изменений завершилась неудачей", "ERROR");
					}
					EndDialog(dlg, 0);
					return;
				}
				logger("Завершили процесс javaw.exe", "INFO");

				// проверка флага
				if (flagInstallUTM == false)
				{
					logger("Поток установки отменен пользователем", "INFO");
					SetEvent(evt);
					return;
				}

				// убираем его из автозагрузки
				if (deleteFileUTMlnk() == 1)
				{
					error = "Не удалось удалить UTM.lnk! Код ошибки - " + std::to_string(GetLastError()) + "\nБудет выполнена отмена изменений";
					MessageBox(dlg, error.c_str(), "Ошибка", MB_ICONERROR);
					logger(error, "ERROR");
					// Делаем не активной кнопку отмены
					EnableWindow(btnCancel, FALSE);
					if (backChange(vecTokens, vecAttr, ctx, hDlg) != 0)
					{
						logger("Отмена изменений завершилась неудачей", "ERROR");
					}
					EndDialog(dlg, 0);
					return;
				}
				logger("Удалили UTM.lnk из автозагрузки", "INFO");
			}

			// проверка флага
			if (flagInstallUTM == false)
			{
				logger("Поток установки отменен пользователем", "INFO");
				SetEvent(evt);
				return;
			}

			// добавление ридера
			nameReader = "Aktiv Rutoken ECP " + std::to_string(vecTokens.size() - countReader);
			str = "Добавляем ридер " + nameReader;
			SetDlgItemText(dlg, IDC_STATIC_PROGRESSBAR2, str.c_str());
			logger(str, "INFO");
			SendMessage(GetDlgItem(dlg, IDC_PROGRESS1), PBM_STEPIT, 0, 0);
			err = addNameReader(nameReader, ctx, vectorAttrReaders[i]);
			if (err != 0)
			{
				error = "Добавление ридера " + nameReader + " завершилось ошибкой! Код ошибки - " + std::to_string(err) + "\nБудет выполнена отмена изменений";
				MessageBox(dlg, error.c_str(), "Ошибка", MB_ICONERROR);
				logger(error, "ERROR");
				// Делаем не активной кнопку отмены
				EnableWindow(btnCancel, FALSE);
				if (backChange(vecTokens, vecAttr, ctx, hDlg) != 0)
				{
					logger("Отмена изменений завершилась неудачей", "ERROR");
				}
				EndDialog(dlg, 0);
				return;
			}
			logger("Добавление ридера " + nameReader + " успешно завершено", "INFO");

			// проверка флага
			if (flagInstallUTM == false)
			{
				logger("Поток установки отменен пользователем", "INFO");
				SetEvent(evt);
				return;
			}

			// запуск службы утм
			numberService = std::to_string(vecTokens.size() - countReader + 1);
			if (numberService == "1")
			{
				str = "Запуск службы Transport";
				numberService = "";
			}
			else
			{
				str = "Запуск службы Transport " + numberService;
			}
			SetDlgItemText(dlg, IDC_STATIC_PROGRESSBAR2, str.c_str());
			logger(str, "INFO");
			SendMessage(GetDlgItem(dlg, IDC_PROGRESS1), PBM_STEPIT, 0, 0);
			err = startServiceUTM("Transport" + numberService);
			if (err != 0)
			{
				error = "Ошибка запуска службы Transport" + numberService + "! Код ошибки - " + std::to_string(GetLastError()) + "\nБудет выполнена отмена изменений";
				MessageBox(dlg, error.c_str(), "Ошибка", MB_ICONERROR);
				logger(error, "ERROR");
				// Делаем не активной кнопку отмены
				EnableWindow(btnCancel, FALSE);
				if (backChange(vecTokens, vecAttr, ctx, hDlg) != 0)
				{
					logger("Отмена изменений завершилась неудачей", "ERROR");
				}
				EndDialog(dlg, 0);
				return;
			}
			str = "Запуск службы Transport " + numberService + " успешно завершена";
			logger(str, "INFO");
			Sleep(500);

			// проверка флага
			if (flagInstallUTM == false)
			{
				logger("Поток установки отменен пользователем", "INFO");
				SetEvent(evt);
				return;
			}

			// проверка работы утм
			str = "Проверка работы УТМ " + numberService + " (запрос на главную страницу)";
			SetDlgItemText(dlg, IDC_STATIC_PROGRESSBAR2, str.c_str());
			logger(str, "INFO");
			SendMessage(GetDlgItem(dlg, IDC_PROGRESS1), PBM_STEPIT, 0, 0);
			err = checkHomePageUTM(ports[i], flagStartUTM);
			if (err == 2)
			{
				logger("Поток установки отменен пользователем", "INFO");
				SetEvent(evt);
				return;
			}
			if (err != 0)
			{
				error = "Не удалось проверить работу УТМ " + numberService + "! Код ошибки - " + std::to_string(err) + "\nФункция завершилась по таймауту\nБудет выполнена отмена изменений";
				MessageBox(dlg, error.c_str(), "Ошибка", MB_ICONERROR);
				logger(error, "ERROR");
				// Делаем не активной кнопку отмены
				EnableWindow(btnCancel, FALSE);
				if (backChange(vecTokens, vecAttr, ctx, hDlg) != 0)
				{
					logger("Отмена изменений завершилась неудачей", "ERROR");
				}
				EndDialog(dlg, 0);
				return;
			}
			str = "Проверка работы УТМ " + numberService + " успешно завершена";
			logger(str, "INFO");
		}

		str = "Запуск служб УТМ успешно завершена";
		logger(str, "INFO");
	}

	SendMessage(GetDlgItem(dlg, IDC_PROGRESS1), PBM_SETPOS, 100, 0);
	flagChangeStatic = false;
	MessageBox(dlg, "Установка УТМ успешно завершена!", "Успешно", MB_ICONINFORMATION);
	logger("Установка УТМ успешно завершена!", "INFO");
	EndDialog(dlg, 0);
}

// перебор многоточия в статике, подаем вид, что то делаем
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

// запуск и ожидание завершения процесса, установка УТМ
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

// копируем папку УТМ
int CopyFolder(std::string src, std::string dst)
{
	WIN32_FIND_DATA FindFileData; // структура для поиска файлов
	HANDLE hFind;

	CreateDirectory(dst.c_str(), NULL); // создаём папку, куда копировать
	std::string filter = src + "\\*"; // фильтр поиска
	
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
				std::string srs2 = src + "\\" + std::string(FindFileData.cFileName) + "\\";
				std::string dst2 = dst + "\\" + std::string(FindFileData.cFileName);
				CopyFolder(srs2, dst2);
			}
		}
		else // нашли файл
		{
			std::string srs2 = src + "\\" + std::string(FindFileData.cFileName);
			std::string dst2 = dst + "\\" + std::string(FindFileData.cFileName);
			if (CopyFile(srs2.c_str(), dst2.c_str(), TRUE) == 0) // копируем файл
			{
				return 1;
			}
		}
	} while (FindNextFile(hFind, &FindFileData)); // пока есть что копировать

	FindClose(hFind); // очистка
	return 0;
}

// копируем папку УТМ
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

// копирование батников в УТМ
int copyBatFiles(int numberUTM)
{
	std::string srcPath = pathExe + "transport_bat\\";
	std::string dstPath = "C:\\UTM_" + std::to_string(numberUTM) + "\\transporter\\bin\\";
	std::vector<std::string> vecBat = { "delete.bat", "install.bat", "run.bat", "stop.bat", "terminal.bat", "terminal-prod.bat" };

	for (std::string elem : vecBat)
	{
		if (CopyFile((srcPath + elem).c_str(), (dstPath + elem).c_str(), FALSE) == 0) // копируем файл
		{
			return 1;
		}
	}

	return 0;
}

// исправление конфигов УТМ (замена портов)
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

		// читаем построчно
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
					temp += str.substr(position_start, position - position_start); // пишем до найденного
					temp += port; // меняем слово
					position += word_source_web.size(); // смещаем позицию на после замененного слова
					break;
				}

				position = 0;
				position = str.find(word_source_transport, position);
				if (position != std::string::npos)
				{
					temp += str.substr(position_start, position - position_start); // пишем до найденного
					temp += word_destinate_transport; // меняем слово
					position += word_source_transport.size(); // смещаем позицию на после замененного слова
					break;
				}

				position = 0;
				position = str.find(word_source_agent, position);
				if (position != std::string::npos)
				{
					temp += str.substr(position_start, position - position_start); // пишем до найденного
					temp += word_destinate_agent; // меняем слово
					position += word_source_agent.size(); // смещаем позицию на после замененного слова
					break;
				}

				temp += str.substr(position_start, position - position_start);
			}
			temp += "\n";
		}
		fin.close();

		
		// перезаписываем файл
		std::ofstream fout(config);
		fout << temp.substr(0, temp.size());
		fout.close();

		return 0;
	}

	return 1;
}

// Исправляем батник в первом утм, чтобы служба не стартовала сама
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

		// читаем построчно
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
					temp += str.substr(position_start, position - position_start); // пишем до найденного
					temp += word_destinate; // меняем слово
					position += word_source.size(); // смещаем позицию на после замененного слова
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


		// перезаписываем файл
		std::ofstream fout(config);
		fout << temp.substr(0, temp.size());
		fout.close();

		return 0;
	}

	return 1;
}

// Завершаем процесс javaw.exe
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

// Удаляем из автозагрузки
int deleteFileUTMlnk()
{
	// удаляем файл
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

// Откат изменений при ошибке или отмене
int backChange(std::vector<std::string> vecRutokens,
	std::vector<std::string> vecAttrRutokens,
	SCARDCONTEXT ctx,
	HWND hDlg)
{
	logger("Отмена изменений", "INFO");
	SetDlgItemText(hDlg, IDC_STATIC_PROGRESSBAR, "Отмена изменений");

	int err;
	std::string error;

	// Удаляем все новый ридеры
	for (std::string elem : vecRutokens)
	{
		err = deleteReader(elem, ctx);
		if (err == ERROR_FILE_NOT_FOUND) // если нет такого ридера, продолжаем
		{
			continue;
		}
		if (err != 0)
		{
			flagChangeStatic = false;
			error = "Отмена изменений. Удаление всех новых ридеров завершилось ошибкой! Код ошибки - " + std::to_string(err);
			logger(error, "ERROR");
			MessageBox(hDlg, error.c_str(), "Ошибка", MB_ICONERROR);
			EndDialog(hDlg, 0);
			return 1;
		}
	}
	logger("Отмена изменений. Удаление всех новых ридеров успешно завершено", "INFO");

	// Добавляем все старые ридеры
	for (unsigned int i = 0; i < vecRutokens.size(); ++i)
	{
		err = addNameReader(vecRutokens[i], ctx, vecAttrRutokens[i]);
		if (err != 0)
		{
			flagChangeStatic = false;
			error = "Отмена изменений. Добавление всех старых ридеров завершилось ошибкой! Код ошибки - " + std::to_string(err);
			logger(error, "ERROR");
			MessageBox(hDlg, error.c_str(), "Ошибка", MB_ICONERROR);
			EndDialog(hDlg, 0);
			return 1;
		}
	}
	logger("Отмена изменений. Добавление всех старых ридеров успешно завершено", "INFO");

	// Удаление папок и служб УТМ
	err = deleteUTMBackChange(hDlg);
	if (err != 0)
	{
		flagChangeStatic = false;
		error = "Отмена изменений. Не удалось удаление папок и служб УТМ, код ошибки " + std::to_string(GetLastError());
		logger(error, "ERROR");
		MessageBox(hDlg, error.c_str(), "Ошибка", MB_ICONERROR);
		EndDialog(hDlg, 0);
		return 1;
	}
	logger("Отмена изменений. Удаление папок и служб УТМ успешно завершено", "INFO");

	return 0;
}

// Остановка и удаление служб и папок
int deleteUTMBackChange(HWND hDlg)
{
	logger("Отмена изменений. Выполняеться удаление УТМ", "INFO");

	std::string error;
	int err;
	std::string countUTM;

	// получаем кол-во утм из конфига
	err = readConfigCountUTM(countUTM);
	if (err != 0)
	{
		flagChangeStatic = false;
		error = "Отмена изменений. Не удалось получить кол-во УТМ из конфига, код ошибки " + std::to_string(err);
		logger(error, "ERROR");
		MessageBox(hDlg, error.c_str(), "Ошибка", MB_ICONERROR);
		EndDialog(hDlg, 0);
		return 1;
	}

	// удаление первого утм чере uninstaller
	err = startProcessInstallUTM("C:\\UTM\\unins000.exe");
	if (err == 1)
	{
		flagChangeStatic = false;
		error = "Отмена изменений. Удаление УТМ завершилась ошибкой! Код ошибки - " + std::to_string(GetLastError());
		MessageBox(hDlg, error.c_str(), "Ошибка", MB_ICONERROR);
		logger(error, "ERROR");
		EndDialog(hDlg, 0);
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
		err = stopServiceUTM("Transport" + std::to_string(i));
		if (err == ERROR_FILE_NOT_FOUND) // если нет такого, продолжаем
		{
			continue;
		}
		if (err != 0)
		{
			flagChangeStatic = false;
			error = "Отмена изменений. Не удалось остановить службу Transport" + std::to_string(i) + ", код ошибки " + std::to_string(err);
			MessageBox(hDlg, error.c_str(), "Ошибка", MB_ICONERROR);
			logger(error, "ERROR");
			EndDialog(hDlg, 0);
			return 1;
		}
		Sleep(500);
		err = deleteServiceUTM(i);
		if (err == ERROR_FILE_NOT_FOUND) // если нет такого, продолжаем
		{
			continue;
		}
		if (err != 0)
		{
			flagChangeStatic = false;
			error = "Отмена изменений. Не удалось удалить службу Transport" + std::to_string(i) + ", код ошибки " + std::to_string(GetLastError());
			MessageBox(hDlg, error.c_str(), "Ошибка", MB_ICONERROR);
			logger(error, "ERROR");
			EndDialog(hDlg, 0);
			return 1;
		}
		Sleep(500);
		err = deleteFolderUTMBackChange("C:\\UTM_" + std::to_string(i));
		if (err == ERROR_FILE_NOT_FOUND) // если нет такого, продолжаем
		{
			continue;
		}
		if (err != 0)
		{
			flagChangeStatic = false;
			error = "Отмена изменений. Не удалось удалить C:\\UTM_" + std::to_string(i) + ", код ошибки " + std::to_string(GetLastError());
			MessageBox(hDlg, error.c_str(), "Ошибка", MB_ICONERROR);
			logger(error, "ERROR");
			EndDialog(hDlg, 0);
			return 1;
		}
	}

	flagChangeStatic = false;
	logger("Отмена изменений. Удаление УТМ успешно завершено", "INFO");
	MessageBox(hDlg, "Отмена изменений. Удаление УТМ успешно завершено", "Успешно", MB_ICONINFORMATION);
	EndDialog(hDlg, 0);

	return 0;
}

// удалить папку утм
int deleteFolderUTMBackChange(std::string src)
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
				deleteFolderUTMBackChange(src2); // рекурсия
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