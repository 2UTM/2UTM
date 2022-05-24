#ifndef STARTUTM_H
#define STARTUTM_H

#include "control.h"

#include <QObject>
#include <QSettings>

class StartUTM : public QObject
{
    Q_OBJECT
public:
    explicit StartUTM(QObject *parent = nullptr);

    // Запуск УТМ
    void startUtm();

    // Передача параметров для запуска
    void setParametrsStart(SCARDCONTEXT &hContext,
                           QVector<QString> &vecSerialNumberDevice,
                           QVector<QString> &vecAttrReader,
                           QVector<QString> &vecReader);

signals:
    void signalStartUTM(QString result, int codeError);

private:
    SCARDCONTEXT startHContext;         // контекст смарткарт
    QVector<QString> startVecSerialNumberDevice;
    QVector<QString> startVecAttrReader;
    QVector<QString> startVecReader;
};

#endif // STARTUTM_H
