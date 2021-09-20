#include "installgui.h"
#include "ui_installgui.h"

InstallUTM::InstallUTM(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::InstallUTM)
{
    ui->setupUi(this);
    setWindowTitle("Установка");
    ui->label_install->hide();
    ui->progressBar->hide();
}

InstallUTM::~InstallUTM()
{
    delete ui;
}

// Метод передачи вектора из главного окна
void InstallUTM::installutm_name_device(QVector<QString> owner_dev, QVector<QString> dev)
{
    installutm_owner_device = owner_dev;
    installutm_device = dev;
    for (int i = 0; i < installutm_owner_device.size(); ++i)
    {
        ui->comboBox_UTM1->addItem(installutm_owner_device[i]);
        ui->comboBox_UTM2->addItem(installutm_owner_device[i]);
    }
}

// Обзор для файла утм
void InstallUTM::on_toolButton_clicked()
{
    file_utm = QFileDialog::getOpenFileName(this, tr("Открыть файлы"), QString(), tr("(*.exe)")).toUtf8();
    ui->lineEdit_utm->clear();
    ui->lineEdit_utm->setText(file_utm);
    ui->lineEdit_utm->setCursorPosition(0);
}

// Отмена
void InstallUTM::on_pushButton_cancel_clicked()
{
    install->set_status_thread(false);
    ui->pushButton_cancel->setEnabled(false);
    cancel_loader = new Loader(this);
    cancel_loader->setAttribute(Qt::WA_ShowModal, true);
    cancel_loader->show();
}

// Метод после завршения потока установки
void InstallUTM::after_install()
{
    loader->stop();
    QMessageBox::information(this, "Операция завершена", "УТМы успешно установлены!\nЕсли ошбика RSA ключа не пропадет, перезапустите УТМы, выбрав соответствующие пункты в меню");
    close();
}

// Метод после завршения потока установки c ошибкой
void InstallUTM::after_error_install()
{
    loader->stop();
    QMessageBox::warning(this, "Операция не завершена", "Операция завершена с ошибкой! Подробности смотрите в логе программы");
    close();
}

// Метод после отмены установки
void InstallUTM::after_cancel_install()
{
    loader->stop();
    cancel_loader->close();
    cancel_loader = nullptr;
    QMessageBox::information(this, "Операция не завершена", "Операция была отменена пользователем");
    ui->statusbar->showMessage("Установка отменена");
    close();
}

// Слот информации об установке
void InstallUTM::install_info(QString info, int progress_value)
{
    ui->label_install_info->setText(info);
    ui->progressBar->setValue(progress_value);
}

// Установить
void InstallUTM::on_pushButton_install_clicked()
{
    file_app = QApplication::applicationDirPath();
    int first_device_t = ui->comboBox_UTM1->currentIndex();
    int second_device_t = ui->comboBox_UTM2->currentIndex();
    if (first_device_t == second_device_t)
    {
        ui->statusbar->showMessage("Выбраны одинаковые устройства");
    }
    else
    {
        if (ui->lineEdit_utm->text() == "")
        {
            ui->statusbar->showMessage("Укажите путь до установочного файла УТМ");
        }
        else
        {
            first_device = installutm_device[first_device_t];
            second_device = installutm_device[second_device_t];

            // Записываем в конфиг порядок рутокенов
            QFile file_write(file_app + "/config.ini");
            if (!file_write.open(QIODevice::WriteOnly | QIODevice::Text))
            {
                QMessageBox::warning(this, "Операция не завершена", "Не удалось получить доступ к файлу настроек!");
                return;
            }
            QTextStream writeStream(&file_write);
            writeStream << first_device;
            writeStream << "\n";
            writeStream << second_device;
            file_write.close();

            ui->label->hide();
            ui->comboBox_UTM1->hide();
            ui->label_2->hide();
            ui->comboBox_UTM2->hide();
            ui->label_3->hide();
            ui->lineEdit_utm->hide();
            ui->toolButton->hide();
            ui->pushButton_install->hide();
            ui->pushButton_cancel_gui->hide();
            ui->statusbar->showMessage("Выполняется установка...");
            loader = new QMovie(QApplication::applicationDirPath() + "/gif/loader.gif");
            ui->label_install->show();
            ui->progressBar->show();
            ui->label_install_gif->setMovie(loader);
            loader->start();

            //Создание обектов рабочего класа и потока
            thread = new QThread(this);
            install = new Install();
            install->install_set_device(first_device, second_device, file_app, file_utm, installutm_device[0]);
            install->set_status_thread(true);
            install->moveToThread(thread);

            //Соединение слотов и сигналов с потоками
            connect(thread, &QThread::started, install, &Install::install_utm);
            connect(install, &Install::install_info_signal, this, &InstallUTM::install_info);

            connect(install, &Install::signalEnd, thread, &QThread::quit);
            connect(install, &Install::signalEnd, install, &Install::deleteLater);
            connect(install, &Install::signalEnd, this, &InstallUTM::after_install);

            connect(install, &Install::signalErrorEnd, thread, &QThread::quit);
            connect(install, &Install::signalErrorEnd, install, &Install::deleteLater);
            connect(install, &Install::signalErrorEnd, this, &InstallUTM::after_error_install);

            connect(install, &Install::signalCancelEnd, thread, &QThread::quit);
            connect(install, &Install::signalCancelEnd, install, &Install::deleteLater);
            connect(install, &Install::signalCancelEnd, this, &InstallUTM::after_cancel_install);

            connect(thread, &QThread::finished, thread, &QThread::deleteLater);

            //Старт потока
            thread->start();
        }
    }
}


// Выход из окна установки
void InstallUTM::on_pushButton_cancel_gui_clicked()
{
    close();
}

