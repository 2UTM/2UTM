#pragma once
#include "stdio.h"
#include "GetCurrentDateTime.h"
#include <string>
#include <vector>

int openLogFile(); // открытие файла лога
int closeLogFile(); // закрытие файла лога
void logger(std::string strLog, std::string level); // запись в лог
long long checkSizeLogFile(); // проверка размера лог файла
int archiveLogFile(); // архивация лог файла