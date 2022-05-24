#include "installutm.h"
#include "control.h"

InstallUTM::InstallUTM(QObject *parent) : QObject(parent)
{

}

// Получение параметров для установки
void InstallUTM::setParametrsInstall(QVector<QString> &vectorDeviceChoose,
                                     QVector<QString> &vectorPortChoose,
                                     QMap<QString, QString> &mapParametrsInstall,
                                     QString &fileUTM,
                                     SCARDCONTEXT &hContext,
                                     QString &fileAppPath,
                                     QVector<QString> &serialNumberDevice)
{
    installDeviceChoose = vectorDeviceChoose;
    installPortChoose = vectorPortChoose;
    mapinstallParametrsInstall = mapParametrsInstall;
    installFileUTM = fileUTM;
    installHContext = hContext;
    installFileAppPath = fileAppPath;
    installSerialNumberDevice = serialNumberDevice;
}

// Прерывание установки
void InstallUTM::set_status_thread(bool status_thread)
{
    if (m_status_thread == status_thread)
        return;

    m_status_thread = status_thread;
}

// Установка
void InstallUTM::installUTM()
{
    /* Удаляются все ридеры, и добавляются выбранные ридеры по очереди, начиная с конца
     * при этом мы выстраиваем правильную очередь ридеров
     *
     * номер последнего выбранного ридера считаеться так
     * (<длина вектора всех ридеров> - (<длина вектора всех ридеров> - <длина вектора выбранных ридеров>)) - 1
     * номер каждого последующего ридера уменьшается на 1
     *
     * таким образом будет выстроена правильная очередь с конца к началу
     * не выбранные ридеры будут в конце
     */

    QProcess *process = new QProcess();
    int progress = 5;
    QByteArray fileData; // для чтения конфига утм
    QString srcPath = "C:/UTM";
    QString urlUTM = "http://localhost:";
    int numService = installDeviceChoose.size(); // номер службы УТМ
    QString nameReader = "Aktiv Rutoken ECP "; // шаблон имени ридера
    int numberReader = (mapinstallParametrsInstall.size() - (mapinstallParametrsInstall.size() - installDeviceChoose.size())) - 1; // номер ридера
    int numberReaderOff = mapinstallParametrsInstall.size() - 1; // номер для не выбранных ридеров
    int startPortCount = installPortChoose.size() - 1; // индекс порта в векторе
    int result; // результаты функций

    emit signalProgressBar("Удаляем все ридеры...", progress);
    // Удаляем все ридеры
    for (QMap<QString, QString>::iterator i = mapinstallParametrsInstall.begin(); i != mapinstallParametrsInstall.end(); ++i)
    {
        result = Control::removeNameReader(i.value(), installHContext);
        if (result)
        {
            delete process;
            emit signalInstallUTMErrorCode("Ошибка removeNameReader, код ошибки - " + QString::number(result), result);
            return;
        }
    }
    progress += 5;

    // Запускаем установку утм
    emit signalProgressBar("Устанавливаем УТМ...", progress);
    int count = 3; // кол во попыток для установки
    do
    {
        if (!m_status_thread)
        {
            delete process;
            emit signalInstallUTMErrorCode("Операция установки прервана пользователем", 2);
            return;
        }
        result = Control::executeCommand(process, "\"" + installFileUTM + "\"");
        if(result)
        {
            if (!m_status_thread)
            {
                delete process;
                emit signalInstallUTMErrorCode("Операция установки прервана пользователем", 2);
                return;
            }
            result = Control::executeCommand(process, "taskkill /IM silent-setup-4.2.0-b2466.tmp /F");
            if(result)
            {
                if (!m_status_thread)
                {
                    delete process;
                    emit signalInstallUTMErrorCode("Операция установки прервана пользователем", 2);
                    return;
                }
                emit signalInstallUTMErrorCode("Не удалось установить УТМ, функция не ответила и завершилась по таймауту", 3);
                return;
            }
            --count;
            continue;
        }
        else
        {
            break;
        }
    }
    while (count > 0);
    progress += 5;

    // Подменяем батники в утм
    emit signalProgressBar("Подменяем батники в УТМ...", progress);
    Control::replaceBatUTM(installFileAppPath + "/transport/delete.bat",
                           installFileAppPath + "/transport/install.bat",
                           installFileAppPath + "/transport/run.bat",
                           installFileAppPath + "/transport/stop.bat",
                           srcPath +"/transporter/bin/delete.bat",
                           srcPath +"/transporter/bin/install.bat",
                           srcPath +"/transporter/bin/run.bat",
                           srcPath +"/transporter/bin/stop.bat");
    if (!m_status_thread)
    {
        delete process;
        emit signalInstallUTMErrorCode("Операция установки прервана пользователем", 2);
        return;
    }
    progress += 5;

    // Делаем копии утм
    for (int i = 2; i <= installDeviceChoose.size(); ++i)
    {
        QString num = QString::number(i);
        QString dstPath = srcPath + "_" + num; // путь до следующего утм

        emit signalProgressBar("Копируем УТМ " + num + "...", progress);
        result = Control::copyPath(srcPath, dstPath);
        if (result)
        {
            delete process;
            emit signalInstallUTMErrorCode("Не удалось сделать копию УТМ, функция не ответила и завершилась по таймауту", 3);
            return;
        }
        if (!m_status_thread)
        {
            delete process;
            emit signalInstallUTMErrorCode("Операция установки прервана пользователем", 2);
            return;
        }

        // Редактируем порт в конфиге утм
        emit signalProgressBar("Редактируем порт в конфиге УТМ " + num + "...", progress);
        result = Control::replacePortUTM(fileData, dstPath +"/transporter/conf/transport.properties", installPortChoose[i - 1]);
        if (result)
        {
            delete process;
            emit signalInstallUTMErrorCode("Не удалось открыть файл transport.properties, функция не ответила и завершилась по таймауту", 3);
            return;
        }
        if (!m_status_thread)
        {
            delete process;
            emit signalInstallUTMErrorCode("Операция установки прервана пользователем", 2);
            return;
        }

        // Устанавливаем службу
        emit signalProgressBar("Устанавливаем службу УТМ " + num + "...", progress);
        result = Control::executeCommand(process, dstPath + "/transporter/bin/install.bat Transport_" + num);
        if(result)
        {
            delete process;
            emit signalInstallUTMErrorCode("Не удалось установить службу Transport_" + num +", функция не ответила и завершилась по таймауту", 3);
            return;
        }
        if (!m_status_thread)
        {
            delete process;
            emit signalInstallUTMErrorCode("Операция установки прервана пользователем", 2);
            return;
        }
        progress += 5;
    }

    // Редактируем порт в конфиге первого утм
    emit signalProgressBar("Редактируем порт в конфиге УТМ 1...", progress);
    result = Control::replacePortUTM(fileData, srcPath +"/transporter/conf/transport.properties", installPortChoose[0]);
    if (result)
    {
        delete process;
        emit signalInstallUTMErrorCode("Не удалось открыть файл transport.properties, функция не ответила и завершилась по таймауту", 3);
        return;
    }
    progress += 5;

    //Запуск УТМ
    QSettings setting("config.ini", QSettings::IniFormat);
    setting.clear();
    for(; numberReader + 1 > 0; --numberReader)
    {
        if (installDeviceChoose.empty())
        {
            break;
        }
        // Добавляем последний выбранный ридер с правильным порядковым номером, удаляем его из вектора и из мапа
        for (QMap<QString, QString>::iterator i = mapinstallParametrsInstall.begin(); i != mapinstallParametrsInstall.end();)
        {
            if (i.value() == installDeviceChoose.back())
            {
                result = Control::addNameReader(nameReader + QString::number(numberReader), installHContext, i.key());
                if (result)
                {
                    delete process;
                    emit signalInstallUTMErrorCode("Ошибка addNameReader, код ошибки - " + QString::number(result), result);
                    return;
                }
                if (numService == 1)
                {
                    setting.setValue("UTM/nameReader", nameReader + QString::number(numberReader));
                    setting.setValue("UTM/attrReader", i.key());
                    setting.setValue("UTM/serialNumber", installSerialNumberDevice[i.value().right(1).toInt()]);
                }
                else
                {
                    setting.setValue("UTM_" + QString::number(numService) + "/nameReader", nameReader + QString::number(numberReader));
                    setting.setValue("UTM_" + QString::number(numService) + "/attrReader", i.key());
                    setting.setValue("UTM_" + QString::number(numService) + "/serialNumber", installSerialNumberDevice[i.value().right(1).toInt()]);
                }
                installDeviceChoose.pop_back();
                i = mapinstallParametrsInstall.erase(i);
                count = 0;

                break;
            }
            else
            {
                ++i;
                ++count;
            }           
        }

        // Запускаем службу утма
        emit signalProgressBar("Запускаем службу УТМ " + QString::number(numService) + "...", progress);
        if (numService == 1)
        {
            result = Control::executeCommand(process, "net start Transport");
            if(result)
            {
                delete process;
                emit signalInstallUTMErrorCode("Не удалось запустить службу Transport, функция не ответила и завершилась по таймауту", 3);
                return;
            }
        }
        else
        {
            result = Control::executeCommand(process, "net start Transport_" + QString::number(numService));
            if(result)
            {
                delete process;
                emit signalInstallUTMErrorCode("Не удалось запустить службу Transport_" + QString::number(numService) +", функция не ответила и завершилась по таймауту", 3);
                return;
            }
        }
        // Проверяем доступность странички УТМ
        int count = 60;
        while(m_status_thread)
        {
            QString result = Control::check_utm(urlUTM + installPortChoose[startPortCount]);
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
            emit signalInstallUTMErrorCode("Не удалось запустить службу Transport, функция не ответила и завершилась по таймауту", 3);
            return;
        }
        if (!m_status_thread)
        {
            delete process;
            emit signalInstallUTMErrorCode("Операция установки прервана пользователем", 2);
            return;
        }

        emit signalProgressBar("Ждем пока УТМ отдуплится, бывает, залипает...", progress);
        Sleep(5000); // ждем пока УТМ отдуплится))

        if (numService == 1)
        {
            setting.setValue("UTM/port", installPortChoose[startPortCount]);
        }
        else
        {
            setting.setValue("UTM_" + QString::number(numService) + "/port", installPortChoose[startPortCount]);
        }

        --numService;
        --startPortCount;
        progress += 5;
    }

    emit signalProgressBar("Добавляем не выбранные ридеры...", progress);
    // Добавляем не выбранные ридеры
    count = 1;
    for (QMap<QString, QString>::iterator i = mapinstallParametrsInstall.begin(); i != mapinstallParametrsInstall.end();)
    {
        result = Control::addNameReader(nameReader + QString::number(numberReaderOff), installHContext, i.key());
        if (result)
        {
            delete process;
            emit signalInstallUTMErrorCode("Ошибка addNameReader, код ошибки - " + QString::number(result), result);
            return;
        }
        ++count;
        --numberReaderOff;
        ++i;
    }

    delete process;
    progress = 100;
    emit signalProgressBar("УТМы успешно установлены...", progress);
    emit signalInstallUTMErrorCode("УТМы успешно установлены", 0);
}
