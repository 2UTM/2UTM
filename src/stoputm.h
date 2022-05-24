#ifndef STOPUTM_H
#define STOPUTM_H

#include "control.h"

#include <QObject>

class StopUTM : public QObject
{
    Q_OBJECT
public:
    explicit StopUTM(QObject *parent = nullptr);

    // Передача параметров для остановки
    void setParametrsStopUTM(QVector<QString> &vecAttr);

    // Остановка УТМ
    void stopUtm();

signals:
    void signalStopUTM(QString error, int errorCode);

private:
    QVector<QString> stopVectorAttr;
};

#endif // STOPUTM_H
