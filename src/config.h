#pragma once
#include "GetPathExe.h"
#include <vector>

// ������ � ������
int writeConfig(std::string numberUTM, std::string nameReader, std::string attrReader,
	std::string serialNumber, std::string port);
int writeConfigCountUTM(std::string countUTM);
int writeConfigAutoStart(std::string autostart);

// ������ �� �������
int readConfigNameAndReader(int countUTM, std::vector<std::string>& nameReader,
	std::vector<std::string>& attrReader,
	std::vector<std::string>& ports,
	std::vector<std::string>& serialNumber);
int readConfigCountUTM(std::string& count);
int readConfigAutoStart(std::string& autostart);
int readConfigPorts(int countUTM, std::vector<std::string>& ports);

// �������� �� ������������� ����� �������
bool configExists();