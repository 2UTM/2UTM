#pragma once
#include "stdio.h"
#include "GetCurrentDateTime.h"
#include <string>
#include <vector>

int openLogFile(); // �������� ����� ����
int closeLogFile(); // �������� ����� ����
void logger(std::string strLog, std::string level); // ������ � ���
long long checkSizeLogFile(); // �������� ������� ��� �����
int archiveLogFile(); // ��������� ��� �����