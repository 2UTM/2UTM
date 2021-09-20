#ifndef INSTALLGUI_H
#define INSTALLGUI_H

#include <QMainWindow>
#include <QFileDialog>
#include <install.h>
#include <QThread>
#include <QMovie>
#include <QMessageBox>
#include "loader.h"

namespace Ui {
class InstallUTM;
}

class InstallUTM : public QMainWindow
{
    Q_OBJECT

signals:
    // Сигнал прекращения установки
    void signal_cancel_install();

public:
    explicit InstallUTM(QWidget *parent = nullptr);
    ~InstallUTM();

   // Метод передачи вуктора из главного окна
   void installutm_name_device(QVector<QString> owner_dev, QVector<QString> dev);

public slots:
   // Слот информации об установке
   void install_info(QString info, int progress_value);

   // Метод после завршения потока установки
   void after_install();

   // Метод после завршения потока установки c ошибкой
   void after_error_install();

   // Метод после отмены установки
   void after_cancel_install();

private slots:
   // Обзор для файла утм
   void on_toolButton_clicked();

   // Отмена
   void on_pushButton_cancel_clicked();

   // Установить
   void on_pushButton_install_clicked();

   // Выход из окна установки
   void on_pushButton_cancel_gui_clicked();

private:
    Ui::InstallUTM *ui;
    QVector<QString> installutm_device;
    QVector<QString> installutm_owner_device;
    QString file_utm;
    QString file_app;
    QString first_device;
    QString second_device;
    Install *install;
    QThread *thread;
    QMovie *loader;
    Loader *cancel_loader;
};

#endif // INSTALLGUI_H
