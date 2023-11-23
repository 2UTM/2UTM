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
	std::vector<std::string> vecAttr, HWND btnCancel, HWND hDlg); // общая функция, установка УТМ
void changeStaticText(HWND dlg); // перебор многоточия в статике, подаем вид, что то делаем
int startProcessInstallUTM(std::string fileUTM); // запуск и ожидание завершения процесса, установка УТМ

// копируем папку УТМ
int CopyFolder(std::string src, std::string dst);
int copyFolderUTM(int numberUTM);

// копирование батников в УТМ
int copyBatFiles(int numberUTM);

// исправление конфигов УТМ (замена портов)
int changePortConfigUTM(int numberUTM, std::string port);

// Исправляем батник в первом утм, чтобы служба не стартовала сама
int changeInstallBatUTM();

// Завершаем процесс javaw.exe
int killProcessJava();

// Удаляем из автозагрузки
int deleteFileUTMlnk();

// Откат изменений при ошибке или отмене
int backChange(std::vector<std::string> vecRutokens,
	std::vector<std::string> vecAttrRutokens,
	SCARDCONTEXT ctx,
	HWND hDlg);

// Остановка и удаление служб и папок
int deleteUTMBackChange(HWND hDlg);

// Удаление папок УТМ
int deleteFolderUTMBackChange(std::string src);