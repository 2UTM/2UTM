#include "start.h"
#include "logging.h"

Start::Start(QObject *parent) : QObject(parent)
{
}

// Передача параметров
void Start::set_settings(QString f_app, QString dev)
{
    file_app = f_app;
    device = dev;
    qInfo(logInfo()) << "Успешно получены данные для начало запуска УТМ";
}

// Метод запуска УТМов
void Start::start_utm()
{
    // Проверка на существования УТМ
    QDir dir1("C:/UTM");
    QDir dir2("C:/UTM_2");
    if (!dir1.exists() || !dir2.exists())
    {
        emit signalAutoStartErrorEnd();
        qWarning(logWarning()) << "УТМы не установлены, запуск не произведен";
        return;
    }

    // Проверка, запущен ли утмы
    res1 = Start::check_utm(host_utm1);
    res2 = Start::check_utm(host_utm2);
    if (res1 != "" && res2 != "")
    {
        emit signalStartEnd();
        qInfo(logInfo()) << "Ответ на запрос не пустой, УТМы запущены";
        return;
    }

    qInfo(logInfo()) << "Начало запуска...";

    // Читаем из конфига порядок рутокенов
    QVector<QString> lst_config;
    QFile file_read(file_app + "/config.ini");
    if (!file_read.open(QIODevice::ReadOnly)) {
        emit signalStartErrorEnd();
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

    // Если первый рутокен из конфига сопадает с утройствами, то отключаем первый рутокен, иначе наоборот
    if (device == lst_config[0])
    {
        // Останавливаем службу утма
        process = new QProcess();
        process->start("net stop Transport");
        if(!process->waitForFinished(60000)) {
            emit signalStartErrorEnd();
            qWarning(logWarning()) << "Не удалось остановить службу Transport, функция не ответила и завершилась по таймауту";
            return;
        }
        qInfo(logInfo()) << "Остановлена служба Transport";
        QThread::msleep(5000);

        // Отключаем первый рутокен
        process->start("devcon disable \"@" + lst_config[0]);
        if(!process->waitForFinished(60000)) {
            emit signalStartErrorEnd();
            qWarning(logWarning()) << "Не удалось отключить устройство " + lst_config[0] + ", функция не ответила и завершилась по таймауту";
            return;
        }
        qInfo(logInfo()) << "Отключено устройство " + lst_config[0];
        QThread::msleep(5000);

        // Запускаем службу утма
        process->start("net start Transport_UTM_2");
        if(!process->waitForFinished(60000)) {
            emit signalStartErrorEnd();
            qWarning(logWarning()) << "Не удалось запустить службу Transport_UTM_2, функция не ответила и завершилась по таймауту";
            return;
        }
        while(true) {
            res1 = Start::check_utm(host_utm2);
            if (res1 == "") {
                continue;
            }
            else {
                break;
            }
        }
        qInfo(logInfo()) << "Запущена служба Transport_UTM_2";

        // Включаем первый рутокен
        process->start("devcon enable \"@" + lst_config[0]);
        if(!process->waitForFinished(60000)) {
            emit signalStartErrorEnd();
            qWarning(logWarning()) << "Не удалось включить устройство " + lst_config[0] + ", функция не ответила и завершилась по таймауту";
            return;
        }
        qInfo(logInfo()) << "Включено устройство " + lst_config[0];
        QThread::msleep(5000);

        // Запускаем службу утма
        process->start("net start Transport");
        if(!process->waitForFinished(60000)) {
            emit signalStartErrorEnd();
            qWarning(logWarning()) << "Не удалось запустить службу Transport, функция не ответила и завершилась по таймауту";
            return;
        }
        while(true) {
            res1 = Start::check_utm(host_utm1);
            if (res1 == "") {
                continue;
            }
            else {
                break;
            }
        }
    }
    else
    {
        // Останавливаем службу утма
        process = new QProcess();
        process->start("net stop Transport_UTM_2");
        if(!process->waitForFinished(60000)) {
            emit signalStartErrorEnd();
            qWarning(logWarning()) << "Не удалось остановить службу Transport_UTM_2, функция не ответила и завершилась по таймауту";
            return;
        }
        qInfo(logInfo()) << "Остановлена служба Transport_UTM_2";
        QThread::msleep(5000);

        // Отключаем второй рутокен
        process->start("devcon disable \"@" + lst_config[1]);
        if(!process->waitForFinished(60000)) {
            emit signalStartErrorEnd();
            qWarning(logWarning()) << "Не удалось отключить устройство " + lst_config[1] + ", функция не ответила и завершилась по таймауту";
            return;
        }
        qInfo(logInfo()) << "Отключено устройство " + lst_config[1];
        QThread::msleep(5000);

        // Запускаем службу утма
        process->start("net start Transport");
        if(!process->waitForFinished(60000)) {
            emit signalStartErrorEnd();
            qWarning(logWarning()) << "Не удалось запустить службу Transport, функция не ответила и завершилась по таймауту";
            return;
        }
        while(true) {
            res1 = Start::check_utm(host_utm1);
            if (res1 == "") {
                continue;
            }
            else {
                break;
            }
        }
        qInfo(logInfo()) << "Запущена служба Transport";

        // Включаем второй рутокен
        process->start("devcon enable \"@" + lst_config[1]);
        if(!process->waitForFinished(60000)) {
            emit signalStartErrorEnd();
            qWarning(logWarning()) << "Не удалось включить устройство " + lst_config[1] + ", функция не ответила и завершилась по таймауту";
            return;
        }
        qInfo(logInfo()) << "Включено устройство " + lst_config[1];
        QThread::msleep(5000);

        // Запускаем службу утма
        process->start("net start Transport_UTM_2");
        if(!process->waitForFinished(60000)) {
            emit signalStartErrorEnd();
            qWarning(logWarning()) << "Не удалось запустить службу Transport_UTM_2, функция не ответила и завершилась по таймауту";
            return;
        }
        while(true) {
            res1 = Start::check_utm(host_utm2);
            if (res1 == "") {
                continue;
            }
            else {
                break;
            }
        }
    }

    qInfo(logInfo()) << "УТМ успешно запущены";
    emit signalStartEnd();
}

// Метод проверки доступа к УТМу
QString Start::check_utm(QString url)
{
    QString res_utm;
    //Выполняем запрос к сайту
    QNetworkAccessManager manager;
    QNetworkRequest request(QUrl(url.toUtf8()));
    request.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);
    QNetworkReply *response = manager.get(request);
    QEventLoop event;
    connect(response, &QNetworkReply::finished, &event, &QEventLoop::quit);
    event.exec();
    //Получаем ответ
    res_utm = response->readAll();
    return res_utm;
}
