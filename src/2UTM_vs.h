#pragma once

#include "Resource.h"
#include <string>
#include <vector>
#include <map>
#include <thread>
#include "dbt.h" // для подписки на события
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
#pragma comment(lib, "Winscard.lib") // для смарткарт
#pragma comment(lib, "Wtsapi32.lib") // для получения токена пользователя
#pragma comment(lib, "Userenv.lib") // для окружения пользователя
#pragma comment(lib, "Comctl32.lib") // для вкладок

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

// создание иконки в трее
int createTrayWindow(HWND hwnd);

// показать сообщение в трее
void showMessageInTray();

// Установить службу 2UTM
int InstallService2UTM();

// Удалить службу 2UTM
int RemoveService2UTM();

// Запуск процеса из службы с токеном службы в окружении пользователя
int startProcessTokenServiceEnvUser(DWORD sessionID);

// Запуск службы 2UTM
int startService2UTM();

// Остановка службы 2UTM
int stopService2UTM();

// Проверка на существование процесса explorer.exe
int checkExplorerExe();

// создание иконки в трее
int createTrayWindow(HWND hwnd);

// показать сообщение в трее
void showMessageInTray();

// Установить службу 2UTM
int InstallService2UTM();

// Удалить службу 2UTM
int RemoveService2UTM();

// Запуск процеса из службы с токеном службы в окружении пользователя
int startProcessTokenServiceEnvUser(DWORD sessionID);

// Запуск службы 2UTM
int startService2UTM();

// Остановка службы 2UTM
int stopService2UTM();

// Проверка на существование процесса explorer.exe
int checkExplorerExe();

//////////////////////////////////////////////////////////////////////////////////////////////////////////
// Создание вкладок
//////////////////////////////////////////////////////////////////////////////////////////////////////////
void StartCommonControls(DWORD flags); // создание окружения для вкладок

// Создание окна вкладок
HWND CreateTab(const HWND hParent, const HINSTANCE hInst, DWORD dwStyle, const RECT& rc, const int id);

// Добавление вкладок
int InsertItem(HWND hTc, std::string txt, int item_index, int image_index, UINT mask = TCIF_TEXT | TCIF_IMAGE);

int OnCreate(const HWND hwnd, CREATESTRUCT* cs); // создание окна с вкладками
void OnSize(const HWND hwnd, int cx, int cy, UINT flags); // регулировка размера и расположения вкладок

int showOrHideOption(int tab); // Показать/скрыть виджеты в определенной влкадке
//////////////////////////////////////////////////////////////////////////////////////////////////////////

int checkServiceUTM(); // проверка на наличие служб утм по кол ву из конфига, и запрос на главную страницу

int getServiceList(std::vector<std::string> vecUTMService, std::vector<std::string>& vecServiceStatus); // получение списка всех служб

int fillListBoxUTMService(std::vector<std::string> vecServiceStatus, std::vector<int> vecUTMError); // заполнение листбокса утм и службы

int CreateColumn(HWND hwndLV, int iCol, CHAR* Text, int iWidth); // создание колонки в листвью
int CreateItem(HWND hwndList, CHAR* text, int index, int status); // создание элемента в листвью

// для обработки событий от виджетов
enum MainWidgetId
{
	LISTVIEW_TOKENS,
	TRAY_SHOW2UTM,
	TRAY_INSTALL_SERVICE,
	TRAY_DELETE_SERVICE,
	TRAY_EXIT,
	TAB_CONTOL,
	LISTVIEW_UTM_SERVICE
};
