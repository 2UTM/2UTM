#include "installgui.h"
#include "ui_installgui.h"
#include "logging.h"

InstallGUI::InstallGUI(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InstallGUI)
{
    installUtm = new InstallUTM();
    ui->setupUi(this);
    setFixedSize(529, 410);
    setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowTitle("Установка");

    // TODO
    ui->tabWidget->removeTab(1);

    ui->labelInstall->hide();
    ui->labelInstallGif->hide();
    ui->labelInstallInfo->hide();
    ui->progressBar->hide();
    ui->pushButtonBreakInstall->hide();
    ui->scrollArea->setWidgetResizable(true);
    layout = new QGridLayout();
    ui->scrollAreaWidgetContents->setLayout(layout);
}

InstallGUI::~InstallGUI()
{
    delete ui;
    delete layout;
}

// Метод передачи параметров
void InstallGUI::setParametrs(QMap<QString, QString> &mapAttrReaderNameReader,
                              SCARDCONTEXT &hContext,
                              QVector<QString> &serialNumberDevice)
{
    mapParametrsInstall = mapAttrReaderNameReader;
    installGUIhContext = hContext;
    installGuiSerialNumberDevice = serialNumberDevice;
}

// Добавить УТМ
void InstallGUI::on_pushButtonInstallAddUTM_clicked()
{
    if (vectorComboBox.size() == 10)
    {
        QMessageBox::warning(this, "Внимание", "Достигнуто максимальное количество УТМ! (10)");
        return;
    }
    if (vectorComboBox.size() == mapParametrsInstall.size())
    {
        QMessageBox::warning(this, "Внимание", "Добавление отменено, так как не достаточно активных токенов!");
        return;
    }
    vectorComboBox.push_back(addComboBox());

    // Размещение виджетов в скролл арене
    int offset = 0; // смещение виджетов
    for (int i = 0; i < vectorLabel.size(); ++i)
    {
        layout->addWidget(vectorLabel[i], i + offset, 0);
        layout->addWidget(vectorComboBox[i], (i + offset) + 1, 0);
        layout->addWidget(vectorSpinBox[i], (i + offset) + 1, 1);
        offset += 2;
    }
}

// Метод добавления спинбокса для порта УТМ
QSpinBox* InstallGUI::addSpinBox()
{
    QSpinBox *spinBox = new QSpinBox(ui->scrollArea);
    spinBox->setMinimum(1);
    spinBox->setMaximum(65535);
    spinBox->setValue(portCount);
    ++portCount;

    return spinBox;
}

// Метод добавления лабел для комбобокса
QLabel* InstallGUI::addLabel()
{
    ++count;
    QLabel *label = new QLabel(ui->scrollArea);
    label->setText("Выберите ключ и порт для УТМ " + QString::number(count));
    label->setFixedSize(w, h);

    return label;
}

// Метод добавления комбобокса
QComboBox* InstallGUI::addComboBox()
{
    vectorLabel.push_back(addLabel());
    vectorSpinBox.push_back(addSpinBox());

    QComboBox *comboBox = new QComboBox(ui->scrollArea);

    for (auto i = mapParametrsInstall.begin(); i != mapParametrsInstall.end(); ++i)
    {
        comboBox->addItem(i.value());
    }
    comboBox->setFixedSize(w, h);

    return comboBox;
}

// Отменить в закладке установки
void InstallGUI::on_pushButtonInstallCancel_clicked()
{
    installUtm->set_status_thread(false);
    for (int i = 0; i < vectorLabel.size(); ++i)
    {
        delete vectorLabel[i];
        delete vectorComboBox[i];
        delete vectorSpinBox[i];
    }
    close();
}

// Отменить в закладке смена ключа
void InstallGUI::on_pushButtonChangeCancel_clicked()
{
    for (int i = 0; i < vectorLabel.size(); ++i)
    {
        delete vectorLabel[i];
        delete vectorComboBox[i];
    }
    close();
}

// Установить в закладке установки
void InstallGUI::on_pushButtonInstallInstall_clicked()
{
    QSet<QString> setDevice;
    QSet<QString> setPort;
    QVector<QString> vectorDeviceChoose;
    QVector<QString> vectorPortChoose;
    for (int i = 0; i < vectorComboBox.size(); ++i)
    {
        vectorDeviceChoose.push_back(vectorComboBox[i]->currentText());
        setDevice.insert(vectorComboBox[i]->currentText());

        vectorPortChoose.push_back(vectorSpinBox[i]->text());
        setPort.insert(vectorSpinBox[i]->text());
    }

    // Проверка на добавление утм
    if (vectorDeviceChoose.empty())
    {
        QMessageBox::warning(this, "Внимание", "Нет добаленных УТМ!");
        return;
    }

    // Шутеечка)
    if (vectorDeviceChoose.size() == 1)
    {
        QMessageBox::warning(this, "Внимание", "Один УТМ можно и руками установить))");
        return;
    }

    // Если сет не равен вектору, значит выбраны одинаковые устройства или одинаковые порты
    if (setDevice.size() != vectorDeviceChoose.size())
    {
        QMessageBox::warning(this, "Внимание", "Выбраны одинаковые токены для разных УТМ!");
        return;
    }

    // Если сет не равен вектору, значит выбраны одинаковые порты
    if (setPort.size() != vectorPortChoose.size())
    {
        QMessageBox::warning(this, "Внимание", "Выбраны одинаковые порты для разных УТМ!");
        return;
    }

    // Проверка что выбран путь до утм
    QString fileUTM = ui->lineEditFileUTM->text();
    if (fileUTM.isEmpty())
    {
        QMessageBox::warning(this, "Внимание", "Не выбран путь до установочного файла УТМ!");
        return;
    }

    // Скрываем все виджеты и показываем прогрессбар
    ui->tabWidget->hide();
    ui->labelInstall->show();
    ui->labelInstallGif->show();
    loaderInstall = new QMovie(QApplication::applicationDirPath() + "/gif/loader.gif");
    ui->labelInstallGif->setMovie(loaderInstall);
    loaderInstall->start();
    ui->labelInstallInfo->show();
    ui->progressBar->show();
    ui->pushButtonBreakInstall->show();

    // Передаем параметры для установки
    QString filaAppPath = QApplication::applicationDirPath();
    installUtm->setParametrsInstall(vectorDeviceChoose,
                                    vectorPortChoose,
                                    mapParametrsInstall,
                                    fileUTM,
                                    installGUIhContext,
                                    filaAppPath,
                                    installGuiSerialNumberDevice);
    installUtm->set_status_thread(true);

    // Запуск установки в отдельном потоке
    threadInstallUtm = new QThread();
    installUtm->moveToThread(threadInstallUtm);

    //Соединение слотов и сигналов с потоками
    connect(threadInstallUtm, &QThread::started, installUtm, &InstallUTM::installUTM);

    connect(installUtm, &InstallUTM::signalInstallUTMErrorCode, threadInstallUtm, &QThread::quit);
    connect(installUtm, &InstallUTM::signalInstallUTMErrorCode, installUtm, &InstallUTM::deleteLater);
    connect(installUtm, &InstallUTM::signalInstallUTMErrorCode, this, &InstallGUI::slotErrorInstallUTM);
    connect(installUtm, &InstallUTM::signalProgressBar, this, &InstallGUI::slotProgressBar);

    connect(threadInstallUtm, &QThread::finished, threadInstallUtm, &QThread::deleteLater);

    threadInstallUtm->start();
}

// Обзор для файла утм
void InstallGUI::on_toolButton_clicked()
{
    ui->lineEditFileUTM->clear();
    ui->lineEditFileUTM->setText(QFileDialog::getOpenFileName(this, tr("Открыть файлы"), QString(), tr("(*.exe)")).toUtf8());
    ui->lineEditFileUTM->setCursorPosition(0);
}

// Удалить УТМ
void InstallGUI::on_pushButtonInstallDeleteUTM_clicked()
{
    if (!vectorLabel.empty() && !vectorComboBox.empty())
    {
        delete vectorLabel.back();
        delete vectorComboBox.back();
        delete vectorSpinBox.back();
        vectorLabel.pop_back();
        vectorComboBox.pop_back();
        vectorSpinBox.pop_back();
        --count;
        --portCount;
    }
}

// Слот получения статуса установки УТМ
void InstallGUI::slotErrorInstallUTM(QString error, int errorCode)
{
    if (errorCode == 0)
    {
        loaderInstall->stop();
        delete loaderInstall;
        QMessageBox::information(this, "Успешно", error);
        qInfo(logInfo()) << error;
        emit signalSuccesInstall();
        close();
    }
    else if (errorCode == 2)
    {
        loaderInstall->stop();
        delete loaderInstall;
        loader->close();
        delete loader;
        QMessageBox::information(this, "Прервано", error);
        qInfo(logInfo()) << error;
        emit signalSuccesInstall();
        close();
    }
    else if (errorCode == 3)
    {
        loaderInstall->stop();
        delete loaderInstall;
        QMessageBox::warning(this, "Ошибка", error);
        qInfo(logWarning()) << error;
        emit signalSuccesInstall();
        close();
    }
    else
    {
        loaderInstall->stop();
        delete loaderInstall;
        QMessageBox::warning(this, "Ошибка", error);
        qWarning(logWarning()) << error;
        emit signalSuccesInstall();
        close();
    }
}

// Слот для прогрессбара
void InstallGUI::slotProgressBar(QString label, int progress)
{
    ui->labelInstallInfo->setText(label);
    ui->progressBar->setValue(progress);
}

// Прервать установку
void InstallGUI::on_pushButtonBreakInstall_clicked()
{
    loader = new Loader(this);
    loader->setAttribute(Qt::WA_ShowModal, true);
    loader->show();

    installUtm->set_status_thread(false);
}
