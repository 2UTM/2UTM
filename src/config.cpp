#include "config.h"

extern std::string pathExe;

// запись в конфиг
int writeConfig(std::string numberUTM, std::string nameReader, std::string attrReader,
	std::string serialNumber, std::string port)
{
	if (nameReader == "" || attrReader == "" || serialNumber == "" || port == "")
	{
		if (WritePrivateProfileString(numberUTM.c_str(), NULL, NULL, (pathExe + "config.ini").c_str()) == 0)
		{
			return 1;
		}
	}
	else
	{
		if (WritePrivateProfileString(numberUTM.c_str(), "nameReader", nameReader.c_str(), (pathExe + "config.ini").c_str()) == 0)
		{
			return 1;
		}
		if (WritePrivateProfileString(numberUTM.c_str(), "attrReader", attrReader.c_str(), (pathExe + "config.ini").c_str()) == 0)
		{
			return 1;
		}
		if (WritePrivateProfileString(numberUTM.c_str(), "serialNumber", serialNumber.c_str(), (pathExe + "config.ini").c_str()) == 0)
		{
			return 1;
		}
		if (WritePrivateProfileString(numberUTM.c_str(), "port", port.c_str(), (pathExe + "config.ini").c_str()) == 0)
		{
			return 1;
		}
	}

	return 0;
}

int writeConfigCountUTM(std::string countUTM)
{
	if (WritePrivateProfileString("options", "countUTM", countUTM.c_str(), (pathExe + "config.ini").c_str()) == 0)
	{
		return 1;
	}
	return 0;
}

int writeConfigAutoStart(std::string autostart)
{
	if (WritePrivateProfileString("options", "autostart", autostart.c_str(), (pathExe + "config.ini").c_str()) == 0)
	{
		return 1;
	}
	return 0;
}

// чтение из конфига
int readConfigNameAndReader(int countUTM, std::vector<std::string>& nameReader,
	std::vector<std::string>& attrReader,
	std::vector<std::string>& ports,
	std::vector<std::string>& serialNumber)
{
	CHAR set[MAX_PATH];
	int err = 0;
	std::string numberUTM = "UTM_";
	for (int i = countUTM; i > 0; --i)
	{
		err = GetPrivateProfileString((numberUTM + std::to_string(i)).c_str(), "nameReader", 0, set, MAX_PATH, (pathExe + "config.ini").c_str());
		if (err == 0 || err == -2)
		{
			return 1;
		}
		nameReader.push_back(std::string(set));

		err = GetPrivateProfileString((numberUTM + std::to_string(i)).c_str(), "attrReader", 0, set, MAX_PATH, (pathExe + "config.ini").c_str());
		if (err == 0 || err == -2)
		{
			return 1;
		}
		attrReader.push_back(std::string(set));

		err = GetPrivateProfileString((numberUTM + std::to_string(i)).c_str(), "port", 0, set, MAX_PATH, (pathExe + "config.ini").c_str());
		if (err == 0 || err == -2)
		{
			return 1;
		}
		ports.push_back(std::string(set));
		err = GetPrivateProfileString((numberUTM + std::to_string(i)).c_str(), "serialNumber", 0, set, MAX_PATH, (pathExe + "config.ini").c_str());
		if (err == 0 || err == -2)
		{
			return 1;
		}
		serialNumber.push_back(std::string(set));
	}

	return 0;
}

int readConfigCountUTM(std::string& count)
{
	CHAR set[MAX_PATH];
	int err = 0;
	err = GetPrivateProfileString("options", "countUTM", 0, set, MAX_PATH, (pathExe + "config.ini").c_str());
	if (err == 0 || err == -2)
	{
		return 1;
	}
	count = std::string(set);

	return 0;
}

int readConfigAutoStart(std::string& autostart)
{
	CHAR set[MAX_PATH];
	int err = 0;
	err = GetPrivateProfileString("options", "autostart", 0, set, MAX_PATH, (pathExe + "config.ini").c_str());
	if (err == 0 || err == -2)
	{
		return 1;
	}
	autostart = std::string(set);

	return 0;
}

// проверка на существование файла конфига
bool configExists()
{
	DWORD dwAttrib = GetFileAttributesA((pathExe + "config.ini").c_str());

	return (dwAttrib != INVALID_FILE_ATTRIBUTES && !(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}
