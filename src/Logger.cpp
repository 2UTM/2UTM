#include "Logger.h"
#include "GetPathExe.h"

#define _CRT_SECURE_NO_WARNINGS

extern std::string pathExe;

FILE* file;

int openLogFile()
{
#pragma warning(suppress : 4996) // fopen deprecated
    if ((file = fopen((pathExe + "2UTM-vsLog.log").c_str(), "a")) == NULL)
    {
        return 1;
    }

    long long sizeLog = checkSizeLogFile();
    if (sizeLog > 100000000)
    {
        if (fclose(file) != 0)
        {
            return 1;
        }

        if (archiveLogFile() != 0)
        {
            return 1;
        }
#pragma warning(suppress : 4996) // fopen deprecated
        if ((file = fopen((pathExe + "2UTM-vsLog.log").c_str(), "a")) == NULL)
        {
            return 1;
        }
    }

    setvbuf(file, 0, _IONBF, 0); // ��������� ������ � ����� ������ � ����
    return 0;
}

int closeLogFile()
{
    if (fclose(file) != 0)
    {
        return 1;
    }
    return 0;
}

void logger(std::string strLog, std::string level)
{
    fprintf(file, "%s [%s]: %s\n", currentDateTimeMSSQL().c_str(), level.c_str(), strLog.c_str());
}

// �������� ������� ��� �����
long long checkSizeLogFile()
{
    _fseeki64(file, 0, SEEK_END); // ���������� ��������� �� ����� ��������� �����
    long long sizeFile = _ftelli64(file); // �������� ������ ��������� �����
    _fseeki64(file, 0, SEEK_SET); // ���������� ��������� �� �������� seek ��������� �����

    return sizeFile;
}

// ��������� ��� �����
int archiveLogFile()
{
    WIN32_FIND_DATA FindFileData; // ��������� ��� ������ ������
    HANDLE hFind;

    std::string fileLog = "2UTM-vsLog.log";

    std::string filter = pathExe + "*.log"; // ������ ������
    std::string src2;

    hFind = FindFirstFile(filter.c_str(), &FindFileData); // ����� ������
    if (hFind == NULL)
    {
        return 1;
    }

    std::vector<std::string> vecTemp;
    do
    {
        vecTemp.push_back(std::string(FindFileData.cFileName));
    } while (FindNextFile(hFind, &FindFileData)); // ���� ���� ��� ������

    int count = 1;
    while (true)
    {
        std::vector<std::string>::iterator it;
        it = std::find(vecTemp.begin(), vecTemp.end(), "2UTM-vsLog" + std::to_string(count) + ".log");
        if (it != vecTemp.end())
        {
            ++count;
        }
        else
        {
            if (MoveFile((pathExe + fileLog).c_str(), (pathExe + "2UTM-vsLog" + std::to_string(count) + ".log").c_str()) == 0)
            {
                return 1;
            }
            break;
        }
    }

    return 0;
}