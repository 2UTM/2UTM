#include "reconnect.h"
#include "logging.h"

Reconnect::Reconnect(QObject *parent) : QObject(parent)
{
}

// Передача параметров
void Reconnect::set_settings(QString f_app)
{
    file_app = f_app;
}

// Переподключение устройств
void Reconnect::reconnect_device()
{
    process = new QProcess();

    qInfo(logInfo()) << "Начало переподключения...";

    // Читаем из конфига порядок рутокенов
    QVector<QString> lst_config;
    QFile file_read(file_app + "/config.ini");
    if (!file_read.open(QIODevice::ReadOnly)) {
        emit signalReconnectErrorEnd();
        qWarning(logWarning()) << "Не удалось открыть файл config.ini, функция не ответила и завершилась по таймауту";
        return;
    }
    while(!file_read.atEnd())
    {
        QString str = file_read.readLine();
        str.replace("\r", "");
        str.replace("\n", "");
        lst_config.append(str);
    }
    file_read.close();
    qInfo(logInfo()) << "Получили данные из config.ini";
    QThread::msleep(5000);

    // Отключаем первый рутокен
    process->start("devcon disable \"@" + lst_config[0]);
    if(!process->waitForFinished(60000)) {
        emit signalReconnectErrorEnd();
        qWarning(logWarning()) << "Не удалось отключить устройство " + lst_config[0] + ", функция не ответила и завершилась по таймауту";
        return;
    }
    qInfo(logInfo()) << "Отключено устройство " + lst_config[0];
    QThread::msleep(5000);

    // Отключаем второй рутокен
    process->start("devcon disable \"@" + lst_config[1]);
    if(!process->waitForFinished(60000)) {
        emit signalReconnectErrorEnd();
        qWarning(logWarning()) << "Не удалось отключить устройство " + lst_config[1] + ", функция не ответила и завершилась по таймауту";
        return;
    }
    qInfo(logInfo()) << "Отключено устройство " + lst_config[1];
    QThread::msleep(5000);

    // Включаем первый рутокен
    process->start("devcon enable \"@" + lst_config[0]);
    if(!process->waitForFinished(60000)) {
        emit signalReconnectErrorEnd();
        qWarning(logWarning()) << "Не удалось включить устройство " + lst_config[0] + ", функция не ответила и завершилась по таймауту";
        return;
    }
    qInfo(logInfo()) << "Включено устройство " + lst_config[0];
    QThread::msleep(5000);

    // Включаем второй рутокен
    process->start("devcon enable \"@" + lst_config[1]);
    if(!process->waitForFinished(60000)) {
        emit signalReconnectErrorEnd();
        qWarning(logWarning()) << "Не удалось включить устройство " + lst_config[1] + ", функция не ответила и завершилась по таймауту";
        return;
    }
    qInfo(logInfo()) << "Включено устройство " + lst_config[1];
    QThread::msleep(5000);

    qInfo(logInfo()) << "Устройства успешно переподключены";
    emit signalReconnectEnd();
}
