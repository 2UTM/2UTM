#include "GetPathExe.h"

// получаем путь до exe
std::string getPathExe()
{
	// Получаем путь до исполняемого файла
	CHAR exePathTchar[MAX_PATH];
	GetModuleFileNameA(0, (LPSTR)exePathTchar, MAX_PATH);
	char* ptr = strrchr(exePathTchar, '\\'); //получаем указатель напоследнии два слеша и записываем после них \0, конец масива
	if (ptr)
	{
		ptr[1] = '\0';
	}
	std::string exePath;
	exePath.append(exePathTchar);

	return exePath;
}

std::string pathExe = getPathExe();