#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "common.h"
#include <QProcess>
#include "windows.h"
#include "cfgmgr32.h"
#include "setupapi.h"
#include <QMessageBox>
#include <QDebug>
#include "installgui.h"
#include "start.h"
#include "loader.h"
#include <QThread>
#include "stop.h"
#include "uninstall.h"
#include "reconnect.h"
#include "change_device.h"
#include <QSystemTrayIcon>
#include <QCloseEvent>
#include <QTimer>
#include "setting.h"

typedef DWORD DEVNODE, DEVINST;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE
using namespace rutoken::pkicore;

class MainWindow : public QMainWindow
{
    Q_OBJECT

protected:
    // Виртуальная функция родительского класса в нашем классе переопределяется для изменения поведения приложения, чтобы оно сворачивалось в трей, когда мы этого хотим
    void closeEvent(QCloseEvent * event);

signals:
    void name_device(QVector<QString> device);

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    // Метод получения детей устройства
    QString device_child(WCHAR DeviceInstanceID[]);

    // Метод получения инфы об устройствах
    void show_device();

    // Метод создания потока запуска утм
    void start_th();

    // Метод создания потока остановки утм
    void stop_th();

    // Метод создания потока удаления утм
    void uninstall_th();

    // Метод создания потока переподключения устройств
    void reconnect_th();

    // Выход
    void exit_prog();

public slots:
    // Метод после удаления утм
    void after_uninstall();

    // Метод после неудачного удаления утм
    void after_error_uninstall();

    // Метод после авто старта
    void after_auto_start();

    // Метод после старта утм
    void after_start();

    // Метод после неудачного старта утм
    void after_error_start();

    // Метод после остановки утм
    void after_stop();

    // Метод после неудачной остановки утм
    void after_error_stop();

    // Сигнал после переподключения устройств
    void after_reconnect();

    // Сигнал после неудачного переподключения устройств
    void after_error_reconnect();

private slots:
    // Нажатие на трей
    void iconActivated(QSystemTrayIcon::ActivationReason reason);

    // О программе
    void on_actionAboutProg_triggered();

    // Прочитать устройства
    void on_action_read_device_triggered();

    // Включить устройство
    void on_action_enable_device_triggered();

    // Установить УТМ
    void on_action_install_utm_triggered();

    // Удалить утм
    void on_action_uninstall_utm_triggered();

    // Связь с разработчиком
    void on_action_callback_triggered();

    // Выход
    void on_action_exit_triggered();

    // Отключить устройство
    void on_action_disable_device_triggered();

    // Переподключить устройства
    void on_action_reconnect_device_triggered();

    // Запустить утм
    void on_action_start_utm_triggered();

    // Остановить утм
    void on_action_stop_utm_triggered();

    // Поменять порядок устройств
    void on_action_change_device_triggered();

    // Настройки
    void on_action_settings_triggered();

private:
    QSystemTrayIcon *p_trayIcon;
    Ui::MainWindow *ui;
    QProcess process;
    QString message_statusbar;
    QVector<QString> device;
    QVector<QString> info_device;
    QVector<QString> owner_device;
    Start *start;
    QThread *thread_start;
    Stop *stop;
    QThread *thread_stop;
    Uninstall *uninstall;
    QThread *thread_uninstall;
    Loader *loader = nullptr;
    Reconnect *reconnect;
    QThread *thread_reconnect;
    Change_device *change_device;
    Setting *setting;
    QVector<QString> lst_start;
};
#endif // MAINWINDOW_H
