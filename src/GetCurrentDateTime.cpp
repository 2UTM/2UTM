#include "GetCurrentDateTime.h"

// �������� ������� ����� � ����
std::string currentDateTimeMSSQL()
{
    std::time_t t = std::time(nullptr);

#pragma warning(suppress : 4996) // ��������� ��������������, ������� localtime �������� unsafe
    std::tm* now = std::localtime(&t);

    char buffer[20];
    strftime(buffer, sizeof(buffer), "%X %d.%m.%Y", now);

    std::string date(buffer, strlen(buffer));

    return date;
}