#include "stoputm.h"

StopUTM::StopUTM(QObject *parent) : QObject(parent)
{

}

// Передача параметров для остановки
void StopUTM::setParametrsStopUTM(QVector<QString> &vecAttr)
{
    stopVectorAttr = vecAttr;
}

// Остановка УТМ
void StopUTM::stopUtm()
{
    if (stopVectorAttr.empty())
    {
        emit signalStopUTM("Пустой вектор с параметрами, остановка отменена", 3);
        return;
    }

    // Остановка УТМов
    // Остановка запущенных УТМ
    int result; // результат
    QProcess *process = new QProcess();
    for (int i = 0; i < stopVectorAttr.size(); ++i)
    {
        if (i == 0)
        {
            result = Control::executeCommand(process, "net stop Transport");
            if(result)
            {
                delete process;
                emit signalStopUTM("Не удалось остановить службу Transport, функция не ответила и завершилась по таймауту", 3);
                return;
            }
        }
        else
        {
            result = Control::executeCommand(process, "net stop Transport_" + QString::number(i + 1));
            if(result)
            {
                delete process;
                emit signalStopUTM("Не удалось остановить службу Transport_" + QString::number(i + 1) + ", функция не ответила и завершилась по таймауту", 3);
                return;
            }
        }
    }

    delete process;
    emit signalStopUTM("УТМы успешно остановлены", 0);
}
