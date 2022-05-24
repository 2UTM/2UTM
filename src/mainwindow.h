#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "control.h"
#include "loader.h"
#include "installgui.h"
#include "showreadernotcontext.h"
#include "startutm.h"
#include "stoputm.h"
#include "deleteutm.h"

#include <QMainWindow>
#include <QListWidgetItem>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QListWidget>
#include <QMessageBox>
#include <QThread>
#include <QSettings>

#include <QDebug>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    // Прочитать устройства
    void readDevice(uint &lenVector);

    // Метод запуска потока считывания сертификатов
    void startThreadReadCert(uint numberDevice, uint lenVector);

private slots:
    // Меню выход
    void on_actionExit_triggered();

    // Перечитать устройства
    void on_actionReadDevice_triggered();

    // Показать инфу о сертификатах на токене
    void showCertInfo(QListWidgetItem *item);

    // Слот получения статуса считывания сертификата
    void slotErrorReadCert(QString resultCert, int codeError);

    // Слот получения сигнала об успешной установке
    void slotSuccesInstall();

    // Слот получения статуса запуска УТМ
    void slotStartUTM(QString result, int codeError);

    // Слот получения статуса остановки УТМ
    void slotStopUTM(QString result, int codeError);

    // Слот получения статуса удаления УТМ
    void slotDeleteUTM(QString result, int codeError);

    // Установить УТМы
    void on_actionInstallUTM_triggered();

    // Меню показать все ридеры вне контекста
    void on_actionShowReaderNotContext_triggered();

    // Запустить УТМ
    void on_actionStartUTM_triggered();

    // Остановить УТМы
    void on_actionStopUTM_triggered();

    // Удалить УТМ
    void on_actionDeleteUTM_triggered();

    // О программе
    void on_actionAboutProgramm_triggered();

    // Связь с разработчиком
    void on_actionCallBack_triggered();

private:
    Ui::MainWindow *ui;
    Loader *loader;
    Control *control;
    QThread *threadReadCert;
    QThread *threadStartUTM;
    QThread *threadStopUTM;
    QThread *threadDeleteUTM;
    DeleteUTM *deleteUTM;
    StopUTM *stopUTM;
    InstallGUI *installGui;
    StartUTM *startUTM;
    ShowReaderNotContext *showReaderNotContext;
    QVector<QString> mainSerialNumberDevice;
    QVector<QString> mainVecAttrReader;
    QVector<QString> mainVecReader;
    uint lenVector; // для проверки, изменилось ли кол во подключенных токенов
    int nubmerDevice; // выбранный токен в списке
    QString nameDevice; // имя выбранного токена
    QMap<QString, QString> mapAttrReaderNameReader; // мап атрибутов и ридеров
    SCARDCONTEXT hContext; // контекст смарткарт
    int countResult; // счетчик совпавших ридеров (при запуске УТМов)
};
#endif // MAINWINDOW_H
