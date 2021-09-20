#ifndef SETTING_H
#define SETTING_H

#include <QDialog>
#include <QApplication>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>

namespace Ui {
class Setting;
}

class Setting : public QDialog
{
    Q_OBJECT

public:
    explicit Setting(QWidget *parent = nullptr);
    ~Setting();

private slots:
    // Применить
    void on_pushButton_set_ok_clicked();

    // Отмена
    void on_pushButton_set_exit_clicked();

private:
    Ui::Setting *ui;
    QVector<QString> lst_start;
};

#endif // SETTING_H
