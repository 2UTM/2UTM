#ifndef INSTALLGUI_H
#define INSTALLGUI_H

#include "installutm.h"
#include "loader.h"

#include <QDialog>
#include <QComboBox>
#include <QLabel>
#include <QSpinBox>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QSet>
#include <QThread>
#include <QMovie>

namespace Ui {
class InstallGUI;
}

class InstallGUI : public QDialog
{
    Q_OBJECT

public:
    explicit InstallGUI(QWidget *parent = nullptr);
    ~InstallGUI();

    // Метод передачи параметров
    void setParametrs(QMap<QString, QString> &mapAttrReaderNameReader,
                      SCARDCONTEXT &hContext,
                      QVector<QString> &serialNumberDevice);

    // Метод добавления комбобокса
    QComboBox* addComboBox();

    // Метод добавления лабел для комбобокса
    QLabel* addLabel();

    // Метод добавления спинбокса для порта УТМ
    QSpinBox *addSpinBox();
private slots:
    // Добавить УТМ
    void on_pushButtonInstallAddUTM_clicked();

    // Отменить в установке
    void on_pushButtonInstallCancel_clicked();

    // Отменить в смене ключа
    void on_pushButtonChangeCancel_clicked();

    // Установить в закладке установке
    void on_pushButtonInstallInstall_clicked();

    // Обзор для файла утм
    void on_toolButton_clicked();

    // Удалить УТМ
    void on_pushButtonInstallDeleteUTM_clicked();

    // Слот получения статуса установки УТМ
    void slotErrorInstallUTM(QString error, int errorCode);

    // Слот для прогрессбара
    void slotProgressBar(QString label, int progress);

    // Прервать установку
    void on_pushButtonBreakInstall_clicked();

signals:
    // Сигнал об успешной установке в главное окно
    void signalSuccesInstall();

private:
    Ui::InstallGUI *ui;
    Loader *loader; // окно загрузки
    QMap<QString, QString> mapParametrsInstall; // мап с сопоставленными именами и атрибутами ридеров
    QVector<QComboBox*> vectorComboBox; // вектор комбобокс для выбора ключа
    QVector<QLabel*> vectorLabel;  // вектор лабел для комбобокса
    QVector<QSpinBox*> vectorSpinBox; // вектор спинбокс для выбора порта
    QGridLayout *layout;  // лайоут для скрол арены
    QVector<QString> installGuiSerialNumberDevice; // вектор серийных номеров устройств для записи в конфиг
    InstallUTM *installUtm;
    QThread *threadInstallUtm;
    QMovie *loaderInstall; // гифка
    SCARDCONTEXT installGUIhContext; // контекст смарткарт
    int w = 300; // ширина виджетов выбора ключа
    int h = 20;  // высота виджетов выбора ключа
    int count = 0; // номер УТМа для лейбла
    int portCount = 8080; // начальный порт для утм
};

#endif // INSTALLGUI_H
