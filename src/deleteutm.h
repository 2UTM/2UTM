#ifndef DELETEUTM_H
#define DELETEUTM_H

#include "control.h"

#include <QObject>

class DeleteUTM : public QObject
{
    Q_OBJECT
public:
    explicit DeleteUTM(QObject *parent = nullptr);

    // Передача параметров для удаления
    void setParametrsDeleteUTM(QVector<QString> &vecAttr);

    // Удаление УТМ
    void deleteUtm();

signals:
    void signalDeleteUTM(QString error, int errorCode);

private:
    QVector<QString> deleteVectorAttr;
};

#endif // DELETEUTM_H
