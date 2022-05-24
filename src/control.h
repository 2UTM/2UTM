#ifndef CONTROL_H
#define CONTROL_H

#include "common.h"
#include "winscard.h"

#include <QObject>
#include <QVector>
#include <QDir>
#include "windows.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>
#include <QProcess>

using namespace rutoken::pkicore;

class Control : public QObject
{
    Q_OBJECT
public:
    explicit Control(QObject *parent = nullptr);

    // Получение ридеров в контексте, NULL - вне контекста
    static int listDevice(QVector<QString> &vecReader, SCARDCONTEXT hContext = NULL);

    // Получение атрибутов ридеров
    static int getAttrReader(QVector<QString> &vecAttrReader, SCARDCONTEXT hContext, QVector<QString> vecReader);

    // Получаем контекст
    static int getContextCard(SCARDCONTEXT &hContext);

    // Удаление ридера
    static int removeNameReader(QString oldName, SCARDCONTEXT hContext = NULL);

    // Добавление ридера
    static int addNameReader(QString newName, SCARDCONTEXT hContext, QString pbAttr);

    // Передача параметров для чтения сертификатов
    void setParametrReadCert(uint numberDevice, uint lenVector);

    // Прочитать сертификаты с устройств
    void readCert();

    // Копирование папки утм
    static int copyPath(QString sourceDir, QString destinationDir);

    // Выполнение команды в консоли
    static int executeCommand(QProcess *process, const QString &command);

    // Переименование папки УТМ
    static int renamePathUTM(QDir &dir, const QString &newPathUTM);

    // Замена батников в УТМ
    static int replaceBatUTM(const QString &copyPathDelete,
                             const QString &copyPathInstall,
                             const QString &copyPathRun,
                             const QString &copyPathStop,
                             const QString &pastePathDelete,
                             const QString &pastePathInstall,
                             const QString &pastePathRun,
                             const QString &pastePathStop);

    // Редактирование порта в конфиге
    static int replacePortUTM(QByteArray &fileData, const QString &pathConfig, QString &port);

    // Метод проверки доступа к УТМу
    static QString check_utm(QString url);

    // Получение всех серийных номеров
    static int getSerialNumberDevice(QVector<QString> &vecSerialNumberDevice);
signals:
    // Сигнал об статусе считывания сертификатов, 0 - успех
    void signalErrorReadCert(QString error, int codeError);

private:
    uint controlNumberDevice;  // номер токена
    uint controlLenVector;     // длина вектора устройств
};

#endif // CONTROL_H
