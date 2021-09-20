#ifndef STOP_H
#define STOP_H

#include <QObject>
#include <QProcess>
#include <QThread>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>
#include <QDir>

class Stop : public QObject
{
    Q_OBJECT
public:
    explicit Stop(QObject *parent = nullptr);

    // Метод проверки доступа к УТМу
    QString check_utm(QString url);

signals:
    // Сигнал неудачной остановки УТМов
    void signalStopErrorEnd();

    // Сигнал удачной остановки УТМов
    void signalStopEnd();

public slots:
    // Метод остановки УТМов
    void stop_utm();

private:
    QProcess *process;
    QString res1;
    QString res2;
    QString host_utm1 = "http://localhost:8080/app/";
    QString host_utm2 = "http://localhost:8081/app/";
};

#endif // STOP_H
