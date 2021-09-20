#include "change_device.h"
#include "ui_change_device.h"

Change_device::Change_device(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Change_device)
{
    ui->setupUi(this);
}

Change_device::~Change_device()
{
    delete ui;
}

// Метод передачи вуктора из главного окна
void Change_device::change_device_name_device(QVector<QString> owner_dev, QVector<QString> dev)
{
    installutm_owner_device = owner_dev;
    installutm_device = dev;
    for (int i = 0; i < installutm_owner_device.size(); ++i)
    {
        ui->comboBox_UTM1->addItem(installutm_owner_device[i]);
        ui->comboBox_UTM2->addItem(installutm_owner_device[i]);
    }
}

// Применить
void Change_device::on_pushButton_change_clicked()
{
    int first_device_t = ui->comboBox_UTM1->currentIndex();
    int second_device_t = ui->comboBox_UTM2->currentIndex();
    if (first_device_t == second_device_t)
    {
        QMessageBox::warning(this, "Операция не завершена", "Выбраны одинаковые устройства!");
    }
    else
    {
        first_device = installutm_device[first_device_t];
        second_device = installutm_device[second_device_t];

        // Записываем в конфиг порядок рутокенов
        QFile file_write(QApplication::applicationDirPath() + "/config.ini");
        if (!file_write.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QMessageBox::warning(this, "Операция не завершена", "Не удалось получить доступ к файлу настроек!");
            return;
        }
        QTextStream writeStream(&file_write);
        writeStream << first_device;
        writeStream << "\n";
        writeStream << second_device;
        file_write.close();
        QMessageBox::information(this, "Операция завершена", "Изменения успешно применены!");
        close();
    }
}

// Отменить
void Change_device::on_pushButton_cancel_gui_clicked()
{
    close();
}

