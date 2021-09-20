#ifndef INSTALL_H
#define INSTALL_H

#include <QObject>
#include <QProcess>
#include <QDir>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QDebug>
#include <QThread>
#include <QEventLoop>

class Install : public QObject
{
    Q_OBJECT

public:
    explicit Install(QObject *parent = nullptr);

    // Метод передачи вектора из главного окна
    void install_set_device(QString first_dev, QString second_dev, QString file_app, QString file_utm, QString installutm_dev);

    // Метод проверки доступа к УТМу
    QString check_utm(QString url);

public slots:
    // Прерывание установки, меняем переменную которая контролирует поток
    void set_status_thread(bool status_thread);

    // Метод установки УТМов
    void install_utm();

signals:
    // Сигнал выхода из потока по отмене
    void signalCancelEnd();

    // Сигнал информации об установке
    void install_info_signal(QString info, int progress_value);

    // Сигнал завершения потока с ошибкой
    void signalErrorEnd();

    // Сигнал завершения потока
    void signalEnd();

private:
    QString first_device;
    QString second_device;
    QString file_utm_install;
    QString file_app_install;
    QString installutm_device;
    QProcess *process;
    QDir dir;
    QByteArray fileData;
    QString host_utm1 = "http://localhost:8080/app/";
    QString host_utm2 = "http://localhost:8081/app/";
    QString res;
    bool m_status_thread;
};

#endif // INSTALL_H
