#pragma once
#include "windows.h"
#include <string>
#include <vector>
#include "GetPathExe.h"

int stopServiceUTM(std::string nameService); // остановка
int startServiceUTM(std::string nameService); // старт
int installServiceUTM(int numberUTM); // установка службы
int deleteServiceUTM(int numberUTM); // удаление службы
int checkHomePageUTM(std::string port, bool flagStartUTM); // проверка работы УТМ (запрос на главную страницу УТМ)
int exec(std::string cmd, std::string& result); // выполнение команды в консоли и чтение ее вывода
