#pragma once

#include "Resource.h"
#include <string>
#include <vector>
#include <thread>
#include "dbt.h" // для подписки на события
#include "Logger.h"
#include "CommCtrl.h"
#include "commdlg.h"
#include "config.h"
#include "installUTM.h"
#include "GetPathExe.h"
#pragma comment(lib, "Winscard.lib") // для смарткарт

// для прогрессбара
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

void getVersionOS(DWORD& dwMinorVersion); // получаем версию ос
void setStatusBar(std::string status); // установка текста в статус бар
void setInfoRutokens(std::string info); // установка текста в поле описание рутокена

void wraperReadDevice(int index); // обертка над чтением с устройства (чтобы отделить логику ридеров от окна)
int fillListBoxRutokens(std::vector<std::string> vecReader); // заполнение листбокса
int workCollectDevice(); // функция, объединяющая получение устройств

int fillListBoxRutokensNoContext(std::vector<std::string> vecReaderNoContext); // заполнение листбокса устройств вне контекста
int workCollectDeviceNoContext(); // функция, объединяющая получение устройств вне контекста

int FileOpenDlg(CHAR* fileName); // обзор, выбор файла

int checkUTM(); // проверка на уже установленные утм

int deleteUTM(); // удалить УМТы
int deleteFolderUTM(std::string src); // удалить папку утм

// запустить УТМы
int startUTM();

// откат изменений при запуске утм
int backChangeStartUTM();

// остановить УТМы
int stopUTM();

// для обработки событий от виджетов
enum MainWidgetId
{
	LISTBOX_TOKENS
};
