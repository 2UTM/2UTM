#ifndef START_H
#define START_H

#include <QObject>
#include <QProcess>
#include <QDir>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QDebug>
#include <QEventLoop>
#include <QThread>
#include <QDir>

class Start : public QObject
{
    Q_OBJECT
public:
    explicit Start(QObject *parent = nullptr);

    // Метод проверки доступа к УТМу
    QString check_utm(QString url);

    // Передача параметров
    void set_settings(QString f_app, QString dev);

signals:
    // Сигнал неудачного запуска УТМов
    void signalStartErrorEnd();

    // Сигнал неудачного автозапуска УТМов
    void signalAutoStartErrorEnd();

    // Сигнал удачного запуска УТМов
    void signalStartEnd();

public slots:
    // Метод запуска УТМов
    void start_utm();

private:
    QProcess *process;
    QString host_utm1 = "http://localhost:8080/app/";
    QString host_utm2 = "http://localhost:8081/app/";
    QString res1;
    QString res2;
    QString file_app;
    QString device;
};

#endif // START_H
