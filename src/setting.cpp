#include "setting.h"
#include "ui_setting.h"
#include "logging.h"

Setting::Setting(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Setting)
{
    ui->setupUi(this);

    // Читаем из конфига параметры старта
    QFile file_read(QApplication::applicationDirPath() + "/start.ini");
    if (!file_read.open(QIODevice::ReadOnly))
    {
        qWarning(logWarning()) << "Не удалось открыть файл start.ini, функция не ответила и завершилась по таймауту";
    }
    else
    {
        while(!file_read.atEnd())
        {
            QString str = file_read.readLine();
            str.replace("\r", "");
            str.replace("\n", "");
            lst_start.append(str);
        }
        file_read.close();
        qInfo(logInfo()) << "Получили данные из start.ini";
        if (lst_start[0] == "true")
        {
            ui->checkBox_autostart->setCheckState(Qt::Checked);
        }
        if (lst_start[1] == "true")
        {
            ui->checkBox_autoexit->setCheckState(Qt::Checked);
        }
        if (lst_start[2] == "true")
        {
            ui->checkBox_tray->setCheckState(Qt::Checked);
        }
    }
}

Setting::~Setting()
{
    delete ui;
}

// Применить
void Setting::on_pushButton_set_ok_clicked()
{
    // Записываем в конфиг параметры запуска
    QFile file_write(QApplication::applicationDirPath() + "/start.ini");
    if (!file_write.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qWarning(logWarning()) << "Не удалось открыть файл start.ini, функция не ответила и завершилась по таймауту";
        QMessageBox::warning(this, "Операция не завершена", "Не удалось получить доступ к файлу настроек!");
        return;
    }
    QTextStream writeStream(&file_write);
    if (ui->checkBox_autostart->checkState() == Qt::Checked)
    {
        writeStream << "true";
    }
    else
    {
        writeStream << "false";
    }
    writeStream << "\n";
    if (ui->checkBox_autoexit->checkState() == Qt::Checked)
    {
        writeStream << "true";
    }
    else
    {
        writeStream << "false";
    }
    writeStream << "\n";
    if (ui->checkBox_tray->checkState() == Qt::Checked)
    {
        writeStream << "true";
    }
    else
    {
        writeStream << "false";
    }
    file_write.close();
    qInfo(logInfo()) << "Настройки изменены";
    QMessageBox::information(this, "Операция завершена", "Настройки сохранены!");
    close();
}

// Отмена
void Setting::on_pushButton_set_exit_clicked()
{
    close();
}

