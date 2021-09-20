#include "stop.h"
#include "logging.h"

Stop::Stop(QObject *parent) : QObject(parent)
{
}

void Stop::stop_utm()
{
    // Проверка на существования УТМ
    QDir dir1("C:/UTM");
    QDir dir2("C:/UTM_2");
    if (!dir1.exists() || !dir2.exists())
    {
        emit signalStopEnd();
        qWarning(logWarning()) << "УТМы не установлены, остановка не произведена";
        return;
    }

    // Проверка, запущен ли утмы
    res1 = Stop::check_utm(host_utm1);
    res2 = Stop::check_utm(host_utm2);
    if (res1 == "" && res2 == "")
    {
        emit signalStopEnd();
        qInfo(logInfo()) << "Ответ на запрос пустой, УТМы не запущены";
        return;
    }

    qInfo(logInfo()) << "Начало остановки...";

    process = new QProcess();
    // Останавливаем службу утма
    process->start("net stop Transport");
    if(!process->waitForFinished(60000)) {
        emit signalStopErrorEnd();
        qWarning(logWarning()) << "Не удалось остановить службу Transport, функция не ответила и завершилась по таймауту";
        return;
    }
    qInfo(logInfo()) << "Остановлена служба Transport";
    QThread::msleep(5000);

    process->start("net stop Transport_UTM_2");
    if(!process->waitForFinished(60000)) {
        emit signalStopErrorEnd();
        qWarning(logWarning()) << "Не удалось остановить службу Transport_UTM_2, функция не ответила и завершилась по таймауту";
        return;
    }
    qInfo(logInfo()) << "УТМы успешно остановлены";
    emit signalStopEnd();
}

// Метод проверки доступа к УТМу
QString Stop::check_utm(QString url)
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
