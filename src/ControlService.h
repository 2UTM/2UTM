#pragma once
#include "windows.h"
#include <string>
#include <vector>
#include "GetPathExe.h"

#define DEFAULT_TIMEOUT 86

int stopServiceUTM(std::string nameService); // ���������
int startServiceUTM(std::string nameService); // �����
int installServiceUTM(int numberUTM); // ��������� ������
int deleteServiceUTM(int numberUTM); // �������� ������
int checkHomePageUTM(std::string port, bool flagStartUTM, int timeout); // �������� ������ ��� (������ �� ������� �������� ���)
int checkUTMError(int& error, std::string port); // ������ � ��� ���� ����� ���, ������� ���� rsaError
int exec(std::string cmd, std::string& result); // ���������� ������� � ������� � ������ �� ������
