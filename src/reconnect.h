#ifndef RECONNECT_H
#define RECONNECT_H

#include <QObject>
#include <QFile>
#include <QProcess>
#include <QThread>

class Reconnect : public QObject
{
    Q_OBJECT
public:
    explicit Reconnect(QObject *parent = nullptr);

    // Передача параметров
    void set_settings(QString f_app);

signals:
    // Ошибка при переподключении
    void signalReconnectErrorEnd();

    // Конец переподключения
    void signalReconnectEnd();

public slots:
    // Переподключение устройств
    void reconnect_device();

private:
    QString file_app;
    QProcess *process;
};

#endif // RECONNECT_H
