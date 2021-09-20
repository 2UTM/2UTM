#ifndef CHANGE_DEVICE_H
#define CHANGE_DEVICE_H

#include <QDialog>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>

namespace Ui {
class Change_device;
}

class Change_device : public QDialog
{
    Q_OBJECT

public:
    explicit Change_device(QWidget *parent = nullptr);
    ~Change_device();

    // Метод передачи вуктора из главного окна
    void change_device_name_device(QVector<QString> owner_dev, QVector<QString> dev);

private slots:
    // Применить
    void on_pushButton_change_clicked();

    // Отменить
    void on_pushButton_cancel_gui_clicked();

private:
    Ui::Change_device *ui;
    QVector<QString> installutm_device;
    QVector<QString> installutm_owner_device;
    QString first_device;
    QString second_device;
};

#endif // CHANGE_DEVICE_H
