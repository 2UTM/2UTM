#include "GetPathExe.h"

// �������� ���� �� exe
std::string getPathExe()
{
	// �������� ���� �� ������������ �����
	CHAR exePathTchar[MAX_PATH];
	GetModuleFileNameA(0, (LPSTR)exePathTchar, MAX_PATH);
	char* ptr = strrchr(exePathTchar, '\\'); //�������� ��������� ����������� ��� ����� � ���������� ����� ��� \0, ����� ������
	if (ptr)
	{
		ptr[1] = '\0';
	}
	std::string exePath;
	exePath.append(exePathTchar);

	return exePath;
}

std::string pathExe = getPathExe();