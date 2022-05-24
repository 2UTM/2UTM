#include "deleteutm.h"

DeleteUTM::DeleteUTM(QObject *parent) : QObject(parent)
{

}

// Передача параметров для остановки
void DeleteUTM::setParametrsDeleteUTM(QVector<QString> &vecAttr)
{
    deleteVectorAttr = vecAttr;
}

// Удаление УТМ
void DeleteUTM::deleteUtm()
{
    if (deleteVectorAttr.empty())
    {
        emit signalDeleteUTM("Пустой вектор с параметрами, остановка отменена", 3);
        return;
    }

    // Удаление УТМов
    // Остановка запущенных УТМ и удаление служб
    int result; // результат
    QProcess *process = new QProcess();
    for (int i = 0; i < deleteVectorAttr.size(); ++i)
    {
        if (i == 0)
        {
            result = Control::executeCommand(process, "net stop Transport");
            if(result)
            {
                delete process;
                emit signalDeleteUTM("Не удалось остановить службу Transport, функция не ответила и завершилась по таймауту", 3);
                return;
            }
            result = Control::executeCommand(process, "sc delete Transport");
            if(result)
            {
                delete process;
                emit signalDeleteUTM("Не удалось удалить службу Transport, функция не ответила и завершилась по таймауту", 3);
                return;
            }
        }
        else
        {
            result = Control::executeCommand(process, "net stop Transport_" + QString::number(i + 1));
            if(result)
            {
                delete process;
                emit signalDeleteUTM("Не удалось остановить службу Transport_" + QString::number(i + 1) + ", функция не ответила и завершилась по таймауту", 3);
                return;
            }
            result = Control::executeCommand(process, "sc delete Transport_" + QString::number(i + 1));
            if(result)
            {
                delete process;
                emit signalDeleteUTM("Не удалось удалить службу Transport_" + QString::number(i + 1) + ", функция не ответила и завершилась по таймауту", 3);
                return;
            }
        }
    }

    // Удаление первого УТМа через деинсталятор
    result = Control::executeCommand(process, "C:/UTM/unins000.exe");
    if(result)
    {
        delete process;
        emit signalDeleteUTM("Не удалось удалить УТМ, функция не ответила и завершилась по таймауту", 3);
        return;
    }
    delete process;

    // Удаление остальных УТМов
    for (int i = 2; i != deleteVectorAttr.size() + 1; ++i)
    {
        QDir dir("C:/UTM_" + QString::number(i));
        if (!dir.removeRecursively())
        {
            emit signalDeleteUTM("Не удалось удалить УТМ" + QString::number(i) + ", функция не ответила и завершилась по таймауту", 3);
            return;
        }
    }

    emit signalDeleteUTM("УТМы успешно остановлены", 0);
}
