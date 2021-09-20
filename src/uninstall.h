#ifndef UNINSTALL_H
#define UNINSTALL_H

#include <QObject>
#include <QProcess>
#include <QThread>
#include <QDir>

class Uninstall : public QObject
{
    Q_OBJECT
public:
    explicit Uninstall(QObject *parent = nullptr);

signals:
    // Сигнал неудачного удаления утм
    void signalUninstallErrorEnd();

    // Сигнал удачного удаления утм
    void signalUninstallEnd();

public slots:
    // Метод удаления УТМов
    void uninstall_utm();

private:
    QProcess *process;
};

#endif // UNINSTALL_H
