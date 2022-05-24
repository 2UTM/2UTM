#ifndef INSTALLUTM_H
#define INSTALLUTM_H

#include "control.h"

#include <QObject>
#include <QVector>
#include <QSettings>

#include <QDebug>

class InstallUTM : public QObject
{
    Q_OBJECT
public:
    explicit InstallUTM(QObject *parent = nullptr);

    // Получение параметров для установки
    void setParametrsInstall(QVector<QString> &vectorDeviceChoose,
                             QVector<QString> &vectorPortChoose,
                             QMap<QString, QString> &mapParametrsInstall,
                             QString &fileUTM,
                             SCARDCONTEXT &hContext,
                             QString &fileAppPath,
                             QVector<QString> &serialNumberDevice);

    // Установка
    void installUTM();

    // Прерывание установки
    void set_status_thread(bool status_thread);
signals:
    // Сигнал об статусе установки УТМ, 0 - успех
    void signalInstallUTMErrorCode(QString error, int errorCode);

    // Сигнал для прогрессбара
    void signalProgressBar(QString label, int progress);

private:
    QVector<QString> installDeviceChoose; // выбранные устройства
    QVector<QString> installPortChoose; // выбранные порты
    QMap<QString, QString> mapinstallParametrsInstall;
    QString installFileUTM;               // файл УТМ
    SCARDCONTEXT installHContext;         // контекст смарткарт
    bool m_status_thread; // отмена установки
    QVector<QString> installSerialNumberDevice; // вектор серийных номеров устройств для записи в конфиг
    QString installFileAppPath;
};

#endif // INSTALLUTM_H
