#include "startutm.h"

StartUTM::StartUTM(QObject *parent) : QObject(parent)
{
}

// Передача параметров для запуска
void StartUTM::setParametrsStart(SCARDCONTEXT &hContext,
                                 QVector<QString> &vecSerialNumberDevice,
                                 QVector<QString> &vecAttrReader,
                                 QVector<QString> &vecReader)
{
    startHContext = hContext;
    startVecSerialNumberDevice = vecSerialNumberDevice;
    startVecAttrReader = vecAttrReader;
    startVecReader = vecReader;
}

// Запуск УТМ
void StartUTM::startUtm()
{
    // Чтение из конфига
    QSettings setting("config.ini", QSettings::IniFormat);
    QStringList keysList = setting.allKeys(); // ключи из конфига

    // Делаем вектора из конфига из каждой группы
    QVector<QString> vectorName;
    QVector<QString> vectorAttr;
    QVector<QString> vectorSerialNumber;
    QVector<QString> vectorPort;
    // Заполняем их
    for (int i = 0; i < keysList.size(); i += 4)
    {
        vectorAttr.push_back(setting.value(keysList[i]).toString());
        vectorName.push_back(setting.value(keysList[i + 1]).toString());
        vectorPort.push_back(setting.value(keysList[i + 2]).toString());
        vectorSerialNumber.push_back(setting.value(keysList[i + 3]).toString());
    }
    // Делаем временные векторы
    QVector<QString> tempStartVecReader = startVecReader;
    QVector<QString> tempStartVecAttrReader = startVecAttrReader;

    // Удаляем все ридеры
    int result; // результат
    for (int i = 0; i < startVecReader.size(); ++i)
    {
        result = Control::removeNameReader(startVecReader[i], startHContext);
        if (result)
        {
            emit signalStartUTM("Ошибка removeNameReader, код ошибки - " + QString::number(result), result);
            return;
        }
    }

    // Остановка запущенных УТМ
    QProcess *process = new QProcess();
    for (int i = 0; i < vectorAttr.size(); ++i)
    {
        if (i == 0)
        {
            result = Control::executeCommand(process, "net stop Transport");
            if(result)
            {
                delete process;
                emit signalStartUTM("Не удалось остановить службу Transport, функция не ответила и завершилась по таймауту", 3);
                return;
            }
        }
        else
        {
            result = Control::executeCommand(process, "net stop Transport_" + QString::number(i + 1));
            if(result)
            {
                delete process;
                emit signalStartUTM("Не удалось остановить службу Transport_" + QString::number(i + 1) + ", функция не ответила и завершилась по таймауту", 3);
                return;
            }
        }
    }

    // Запуск УТМ
    // Добавляем последний ридер из выбранных
    QString urlUTM = "http://localhost:";
    int countService = vectorSerialNumber.size() - 1;
    QString nameReader = "Aktiv Rutoken ECP "; // шаблон имени ридера
    int numberReader = (startVecSerialNumberDevice.size() - (startVecSerialNumberDevice.size() - vectorSerialNumber.size())) - 1; // номер ридера
    int numberReaderOff = startVecSerialNumberDevice.size() - 1; // номер для не выбранных ридеров
    while (true)
    {
        if (countService == -1)
        {
            break;
        }
        for (int i = 0; i < startVecSerialNumberDevice.size(); ++i)
        {
            if (vectorSerialNumber.back() == startVecSerialNumberDevice[i])
            {
                result = Control::addNameReader(nameReader + QString::number(numberReader), startHContext, startVecAttrReader[i]);
                if (result)
                {
                    delete process;
                    emit signalStartUTM("Ошибка addNameReader, код ошибки - " + QString::number(result), result);
                    return;
                }
            }
            else
            {
                continue;
            }

            // Запускаем службу УТМ
            if (countService == 0)
            {
                result = Control::executeCommand(process, "net start Transport");
                if(result)
                {
                    delete process;
                    emit signalStartUTM("Не удалось запустить службу Transport, функция не ответила и завершилась по таймауту", 3);
                    return;
                }
                // Проверяем доступность странички УТМ
                int count = 60;
                while(count)
                {
                    QString result = Control::check_utm(urlUTM + vectorPort[countService]);
                    if (result == "")
                    {
                        Sleep(1000);
                        --count;
                        continue;
                    }
                    else
                    {
                        break;
                    }
                }
                if (count == 0)
                {
                    delete process;
                    emit signalStartUTM("Не удалось запустить службу Transport, функция не ответила и завершилась по таймауту", 3);
                    return;
                }
                Sleep(6000); // бывает, УТМ залипает...
            }
            else
            {
                result = Control::executeCommand(process, "net start Transport_" + QString::number(countService + 1));
                if(result)
                {
                    delete process;
                    emit signalStartUTM("Не удалось запустить службу Transport_" + QString::number(countService + 1) + ", функция не ответила и завершилась по таймауту", 3);
                    return;
                }
                // Проверяем доступность странички УТМ
                int count = 60;
                while(count)
                {
                    QString result = Control::check_utm(urlUTM + vectorPort[countService]);
                    if (result == "")
                    {
                        Sleep(1000);
                        --count;
                        continue;
                    }
                    else
                    {
                        break;
                    }
                }
                if (count == 0)
                {
                    delete process;
                    emit signalStartUTM("Не удалось запустить службу Transport_" + QString::number(countService + 1) + ", функция не ответила и завершилась по таймауту", 3);
                    return;
                }
                Sleep(6000); // бывает, УТМ залипает...
            }
            tempStartVecReader.remove(i);
            tempStartVecAttrReader.remove(i);
        }
        vectorSerialNumber.pop_back();
        --countService;
        --numberReader;
    }
    delete process;

    // Добавляем не выбранные ридеры
    for (int i = 0; i < tempStartVecReader.size(); ++i)
    {
        result = Control::addNameReader(nameReader + QString::number(numberReaderOff), startHContext, tempStartVecAttrReader[i]);
        if (result)
        {
            emit signalStartUTM("Ошибка addNameReader, код ошибки - " + QString::number(result), result);
            return;
        }
        --numberReaderOff;
    }

    emit signalStartUTM("УТМы успешно запущены", 0);
}
