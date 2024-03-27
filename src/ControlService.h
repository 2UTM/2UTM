#pragma once
#include "windows.h"
#include <string>
#include <vector>
#include "GetPathExe.h"

#define DEFAULT_TIMEOUT 86

int stopServiceUTM(std::string nameService); // остановка
int startServiceUTM(std::string nameService); // старт
int installServiceUTM(int numberUTM); // установка службы
int deleteServiceUTM(int numberUTM); // удаление службы
int checkHomePageUTM(std::string port, bool flagStartUTM, int timeout); // проверка работы УТМ (запрос на главную страницу УТМ)
int checkUTMError(int& error, std::string port); // запрос у утм инфы через апи, смотрим поле rsaError
int exec(std::string cmd, std::string& result); // выполнение команды в консоли и чтение ее вывода
