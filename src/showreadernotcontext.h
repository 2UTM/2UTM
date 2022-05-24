#ifndef SHOWREADERNOTCONTEXT_H
#define SHOWREADERNOTCONTEXT_H

#include "control.h"
#include "logging.h"

#include <QDialog>
#include <QMessageBox>

namespace Ui {
class ShowReaderNotContext;
}

class ShowReaderNotContext : public QDialog
{
    Q_OBJECT

public:
    explicit ShowReaderNotContext(QWidget *parent = nullptr);
    ~ShowReaderNotContext();

    // Показать все ридеры вне контекста
    void showAllReaderNotContext();

private slots:
    // Отмена
    void on_pushButtonCancel_clicked();

    // Удалить ридер
    void on_pushButtonDeleteReader_clicked();

private:
    Ui::ShowReaderNotContext *ui;
};

#endif // SHOWREADERNOTCONTEXT_H
