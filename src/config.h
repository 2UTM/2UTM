#pragma once
#include "GetPathExe.h"
#include <vector>

// запись в конфиг
int writeConfig(std::string numberUTM, std::string nameReader, std::string attrReader,
	std::string serialNumber, std::string port);
int writeConfigCountUTM(std::string countUTM);

// чтение из конфига
int readConfigNameAndReader(int countUTM, std::vector<std::string>& nameReader,
	std::vector<std::string>& attrReader,
	std::vector<std::string>& ports,
	std::vector<std::string>& serialNumber);
int readConfigCountUTM(std::string& count);

// проверка на существование файла конфига
bool configExists();