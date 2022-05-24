#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "logging.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->textEditInfoCert->setReadOnly(true);
    setFixedSize(382, 518);
    setWindowFlags(Qt::Dialog);
    setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);

    readDevice(lenVector);

    // Соединяем сигнал itemClicked со слотом showCertInfo, при выборе токена вывод инфы о нем
    connect(ui->listWidgetToken, &QListWidget::itemClicked, this, &MainWindow::showCertInfo);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// Прочитать устройства
void MainWindow::readDevice(uint &lenVector)
{
    ui->listWidgetToken->clear();
    mapAttrReaderNameReader.clear();
    QVector<QString> vecAttrReader;
    QVector<QString> vecReader;

    Control::getSerialNumberDevice(mainSerialNumberDevice);

    // Получаем контекст
    int result = Control::getContextCard(hContext);
    if (result)
    {
        qWarning(logWarning()) << "Ошибка getContextCard, код ошибки - " + QString::number(result);
        ui->statusbar->showMessage("Ошибка getContextCard, код ошибки - " + QString::number(result));
        return;
    }

    // Получаем ридеры
    result = Control::listDevice(vecReader, hContext);
    if (result == 1)
    {
        qInfo(logInfo()) << "Устройства не найдены";
        ui->statusbar->showMessage("Устройства не найдены");
        ui->textEditInfoCert->clear();
        return;
    }
    if (result)
    {
        qWarning(logWarning()) << "Ошибка listDevice, код ошибки - " + QString::number(result);
        ui->statusbar->showMessage("Ошибка listDevice, код ошибки - " + QString::number(result));
        return;
    }

    // Получаем атрибуты ридеров
    result = Control::getAttrReader(vecAttrReader, hContext, vecReader);
    if (result)
    {
        qWarning(logWarning()) << "Ошибка getAttrReader, код ошибки - " + QString::number(result);
        ui->statusbar->showMessage("Ошибка getAttrReader, код ошибки - " + QString::number(result));
        return;
    }

    // Заполняем list widget
    lenVector = vecReader.size();
    for (int i = 0; i < vecReader.size(); ++i)
    {
        QListWidgetItem *item = new QListWidgetItem();
        item->setIcon(QIcon("./image/token.png"));
        item->setText("          " + vecReader[i]);
        ui->listWidgetToken->insertItem(i, item);
    }

    // Заполянем мап атрибутов и ридеров
    for (int i = 0; i < vecReader.size(); ++i)
    {
        mapAttrReaderNameReader[vecAttrReader[i]] = vecReader[i];
    }
    mainVecReader = vecReader;
    mainVecAttrReader = vecAttrReader;

    ui->statusbar->showMessage("Выполнено");
}

// Метод запуска потока считывания сертификатов
void MainWindow::startThreadReadCert(uint numberDevice, uint lenVector)
{
    //Создание объектов рабочего класа и потока
    threadReadCert = new QThread();
    control = new Control();
    control->setParametrReadCert(numberDevice, lenVector);
    control->moveToThread(threadReadCert);

    //Соединение слотов и сигналов с потоками
    connect(threadReadCert, &QThread::started, control, &Control::readCert);

    connect(control, &Control::signalErrorReadCert, threadReadCert, &QThread::quit);
    connect(control, &Control::signalErrorReadCert, control, &Control::deleteLater);
    connect(control, &Control::signalErrorReadCert, this, &MainWindow::slotErrorReadCert);

    connect(threadReadCert, &QThread::finished, threadReadCert, &QThread::deleteLater);

    threadReadCert->start();
}

// Меню выход
void MainWindow::on_actionExit_triggered()
{
    QApplication::exit(0);
}

// Перечитать устройства
void MainWindow::on_actionReadDevice_triggered()
{
    Control::getSerialNumberDevice(mainSerialNumberDevice);
    readDevice(lenVector);
}

// Показать инфу о сертификатах на выбранных токене
void MainWindow::showCertInfo(QListWidgetItem *item)
{   
    loader = new Loader(this);
    loader->setAttribute(Qt::WA_ShowModal, true);
    loader->show();

    nubmerDevice = ui->listWidgetToken->currentRow();
    nameDevice = item->text();
    startThreadReadCert(nubmerDevice, lenVector);
}

// Слот получения статуса считывания сертификата
void MainWindow::slotErrorReadCert(QString resultCert, int codeError)
{
    // Серийный номер устройства
    QString serNumber = resultCert.mid(31, 10);

    for (QMap<QString, QString>::iterator i = mapAttrReaderNameReader.begin(); i != mapAttrReaderNameReader.end();)
    {
        if (i.value() == nameDevice.trimmed())
        {
            resultCert = "Атрибут считывателя: " + i.key() + "\n\n" + resultCert;
            resultCert = "Имя считывателя: " + i.value() + "\n" + resultCert;
            break;
        }
        ++i;
    }

    if (codeError == 1)
    {
        ui->listWidgetToken->clear();
        ui->textEditInfoCert->clear();
        qWarning(logWarning()) << "Ошибка readCert, код ошибки - " + QString::number(codeError) + ", устройства не найдены";
        ui->statusbar->showMessage("Устройства не найдены");

        loader->close();
        delete loader;
    }
    else if (codeError == 2)
    {
        readDevice(lenVector);
        ui->textEditInfoCert->clear();
        ui->statusbar->showMessage("Устройства перечитаны");

        loader->close();
        delete loader;
    }
    else if (codeError == 0)
    {
        ui->textEditInfoCert->clear();
        ui->textEditInfoCert->insertPlainText(resultCert);
        ui->statusbar->showMessage("Выполнено");

        loader->close();
        delete loader;
    }
}

// Слот получения сигнала об успешной установке
void MainWindow::slotSuccesInstall()
{
    activateWindow();
    readDevice(lenVector);
}

// Слот получения статуса запуска УТМ
void MainWindow::slotStartUTM(QString result, int codeError)
{
    if (codeError == 0)
    {
        ui->statusbar->showMessage(result);
        readDevice(lenVector);
        loader->close();
        delete loader;
    }
    else if (codeError == 3)
    {
        qWarning(logWarning()) << result;
        QMessageBox::warning(this, "Ошибка", result);
        readDevice(lenVector);
        ui->statusbar->showMessage("Ошибка");
        loader->close();
        delete loader;
    }
    else
    {
        qWarning(logWarning()) << result;
        QMessageBox::warning(this, "Ошибка", result);
        readDevice(lenVector);
        ui->statusbar->showMessage(result);
        loader->close();
        delete loader;
    }
}

// Слот получения статуса остановки УТМ
void MainWindow::slotStopUTM(QString result, int codeError)
{
    if (codeError == 0)
    {
        ui->statusbar->showMessage(result);
        readDevice(lenVector);
        loader->close();
        delete loader;
    }
    else if (codeError == 3)
    {
        qWarning(logWarning()) << result;
        QMessageBox::warning(this, "Ошибка", result);
        readDevice(lenVector);
        ui->statusbar->showMessage("Ошибка");
        loader->close();
        delete loader;
    }
    else
    {
        qWarning(logWarning()) << result;
        QMessageBox::warning(this, "Ошибка", result);
        readDevice(lenVector);
        ui->statusbar->showMessage(result);
        loader->close();
        delete loader;
    }
}

// Слот получения статуса удаления УТМ
void MainWindow::slotDeleteUTM(QString result, int codeError)
{
    if (codeError == 0)
    {
        ui->statusbar->showMessage(result);
        readDevice(lenVector);
        loader->close();
        delete loader;
    }
    else if (codeError == 3)
    {
        qWarning(logWarning()) << result;
        QMessageBox::warning(this, "Ошибка", result);
        readDevice(lenVector);
        ui->statusbar->showMessage("Ошибка");
        loader->close();
        delete loader;
    }
    else
    {
        qWarning(logWarning()) << result;
        QMessageBox::warning(this, "Ошибка", result);
        readDevice(lenVector);
        ui->statusbar->showMessage(result);
        loader->close();
        delete loader;
    }
}

// Установить УТМы
void MainWindow::on_actionInstallUTM_triggered()
{
    // Если есть установленные УТМы, прерываем
    QDir dir;
    dir.cd("C:/");
    dir.setFilter(QDir::Dirs);
    QFileInfoList list = dir.entryInfoList();
    for (int i = 0; i < list.size(); ++i)
    {
        QFileInfo fileInfo = list[i];
        if (fileInfo.filePath() == "C:/2UTM")
        {
            continue;
        }
        if (fileInfo.filePath().contains("UTM"))
        {
            qWarning(logWarning()) << "Ошибка начала установки, удалите уже существующие УТМы";
            QMessageBox::warning(this, "Ошибка", "Ошибка начала установки, удалите уже существующие УТМы!");
            return;
        }
    }

    installGui = new InstallGUI();
    connect(installGui, &InstallGUI::signalSuccesInstall, this, &MainWindow::slotSuccesInstall);
    installGui->setParametrs(mapAttrReaderNameReader, hContext, mainSerialNumberDevice);
    installGui->exec();
    delete installGui;
}

// Меню показать все ридеры вне контекста
void MainWindow::on_actionShowReaderNotContext_triggered()
{
    showReaderNotContext = new ShowReaderNotContext(this);
    showReaderNotContext->exec();
    delete showReaderNotContext;
}

// Запустить УТМ
void MainWindow::on_actionStartUTM_triggered()
{
    // Если нет установленных УТМ, прерываем
    bool deleteStatus = true;
    QDir dir;
    dir.cd("C:/");
    dir.setFilter(QDir::Dirs);
    QFileInfoList list = dir.entryInfoList();
    for (int i = 0; i < list.size(); ++i)
    {
        QFileInfo fileInfo = list[i];
        if (fileInfo.filePath() == "C:/2UTM")
        {
            continue;
        }
        if (fileInfo.filePath().contains("UTM"))
        {
            deleteStatus = false;
        }
    }
    if (deleteStatus)
    {
        qWarning(logWarning()) << "Ошибка начала запуска, нет установленных УТМ";
        QMessageBox::warning(this, "Ошибка", "Ошибка начала запуска, нет установленных УТМ!");
        return;
    }

    readDevice(lenVector);
    countResult = 0;

    // Если конфиг не совпадает с существующим мапом ридеров, прерываем
    // Чтение из конфига
    QSettings setting("config.ini", QSettings::IniFormat);
    QStringList keysList = setting.allKeys(); // ключи из конфига

    if (keysList.empty())
    {
        qWarning(logWarning()) << "Не найден файл конфига, запуск прерван";
        QMessageBox::warning(this, "Ошибка", "Не найден файл конфига, запуск прерван!");
        return;
    }

    // Делаем вектора из конфига из каждой группы
    QVector<QString> vectorName;
    QVector<QString> vectorAttr;
    QVector<QString> vectorSerialNumberDevice;

    // Заполняем их
    for (int i = 0; i < keysList.size(); i += 4)
    {
        vectorAttr.push_back(setting.value(keysList[i]).toString());
        vectorName.push_back(setting.value(keysList[i + 1]).toString());
        vectorSerialNumberDevice.push_back(setting.value(keysList[i + 3]).toString());
    }

    // Делаем мап для сравнения
    QMap<QString, QString> configMap;
    for (int i = 0; i < vectorAttr.size(); ++i)
    {
        configMap[vectorAttr[i]] = vectorName[i];
    }

    // Если длина мапа из кофига больше, чем есть реально, прерываем
    if (configMap.size() > mapAttrReaderNameReader.size())
    {
        qWarning(logWarning()) << "Существующие ридеры меньше, чем в конфиге, запуск прерван";
        QMessageBox::warning(this, "Ошибка", "Существующие ридеры меньше, чем в конфиге, запуск прерван!");
        return;
    }
    // Сверяем серийные номера, если не совпадают, прерываем
    for (int i = 0; i < vectorSerialNumberDevice.size(); ++i)
    {
        for (int j = 0; j < mainSerialNumberDevice.size(); ++j)
        {
            if (vectorSerialNumberDevice[i] == mainSerialNumberDevice[j])
            {
                ++countResult;
            }
        }
    }
    if (vectorSerialNumberDevice.size() != countResult)
    {
        qWarning(logWarning()) << "Рутокены не совпадают с конфигом, запуск прерван";
        QMessageBox::warning(this, "Ошибка", "Рутокены не совпадают с конфигом, запуск прерван!");
        return;
    }

    loader = new Loader(this);
    loader->setAttribute(Qt::WA_ShowModal, true);
    loader->show();

    //Создание объектов рабочего класа и потока
    threadStartUTM = new QThread();
    startUTM = new StartUTM();
    startUTM->setParametrsStart(hContext, mainSerialNumberDevice, mainVecAttrReader, mainVecReader);
    startUTM->moveToThread(threadStartUTM);

    //Соединение слотов и сигналов с потоками
    connect(threadStartUTM, &QThread::started, startUTM, &StartUTM::startUtm);

    connect(startUTM, &StartUTM::signalStartUTM, threadStartUTM, &QThread::quit);
    connect(startUTM, &StartUTM::signalStartUTM, startUTM, &StartUTM::deleteLater);
    connect(startUTM, &StartUTM::signalStartUTM, this, &MainWindow::slotStartUTM);

    connect(threadStartUTM, &QThread::finished, threadStartUTM, &QThread::deleteLater);

    threadStartUTM->start();
}

// Остановить УТМы
void MainWindow::on_actionStopUTM_triggered()
{
    // Чтение из конфига
    QSettings setting("config.ini", QSettings::IniFormat);
    QStringList keysList = setting.allKeys(); // ключи из конфига

    if (keysList.empty())
    {
        qWarning(logWarning()) << "Не найден файл конфига, запуск прерван";
        QMessageBox::warning(this, "Ошибка", "Не найден файл конфига, запуск прерван!");
        return;
    }

    // Делаем вектора из конфига из каждой группы
    QVector<QString> vectorAttr;

    // Заполняем их
    for (int i = 0; i < keysList.size(); i += 4)
    {
        vectorAttr.push_back(setting.value(keysList[i]).toString());
    }

    loader = new Loader(this);
    loader->setAttribute(Qt::WA_ShowModal, true);
    loader->show();

    //Создание объектов рабочего класа и потока
    threadStopUTM = new QThread();
    stopUTM = new StopUTM();
    stopUTM->setParametrsStopUTM(vectorAttr);
    stopUTM->moveToThread(threadStopUTM);

    //Соединение слотов и сигналов с потоками
    connect(threadStopUTM, &QThread::started, stopUTM, &StopUTM::stopUtm);

    connect(stopUTM, &StopUTM::signalStopUTM, threadStopUTM, &QThread::quit);
    connect(stopUTM, &StopUTM::signalStopUTM, stopUTM, &StopUTM::deleteLater);
    connect(stopUTM, &StopUTM::signalStopUTM, this, &MainWindow::slotStopUTM);

    connect(threadStopUTM, &QThread::finished, threadStopUTM, &QThread::deleteLater);

    threadStopUTM->start();
}

// Удалить УТМ
void MainWindow::on_actionDeleteUTM_triggered()
{
    int answer = QMessageBox::question(this, "Внимание", "Вы уверены, что хотите удалить УТМы?");
    if (answer != QMessageBox::Yes)
    {
        return;
    }
    // Если нет установленных УТМ, прерываем
    bool deleteStatus = true;
    QDir dir;
    dir.cd("C:/");
    dir.setFilter(QDir::Dirs);
    QFileInfoList list = dir.entryInfoList();
    for (int i = 0; i < list.size(); ++i)
    {
        QFileInfo fileInfo = list[i];
        if (fileInfo.filePath() == "C:/2UTM")
        {
            continue;
        }
        if (fileInfo.filePath().contains("UTM"))
        {
            deleteStatus = false;
        }
    }
    if (deleteStatus)
    {
        qWarning(logWarning()) << "Ошибка начала удаления, нет установленных УТМ";
        QMessageBox::warning(this, "Ошибка", "Ошибка начала удаления, нет установленных УТМ!");
        return;
    }

    // Чтение из конфига
    QSettings setting("config.ini", QSettings::IniFormat);
    QStringList keysList = setting.allKeys(); // ключи из конфига

    if (keysList.empty())
    {
        qWarning(logWarning()) << "Не найден файл конфига, запуск прерван";
        QMessageBox::warning(this, "Ошибка", "Не найден файл конфига, запуск прерван!");
        return;
    }

    // Делаем вектора из конфига из каждой группы
    QVector<QString> vectorAttr;

    // Заполняем их
    for (int i = 0; i < keysList.size(); i += 4)
    {
        vectorAttr.push_back(setting.value(keysList[i]).toString());
    }

    loader = new Loader(this);
    loader->setAttribute(Qt::WA_ShowModal, true);
    loader->show();

    //Создание объектов рабочего класа и потока
    threadDeleteUTM = new QThread();
    deleteUTM = new DeleteUTM();
    deleteUTM->setParametrsDeleteUTM(vectorAttr);
    deleteUTM->moveToThread(threadDeleteUTM);

    //Соединение слотов и сигналов с потоками
    connect(threadDeleteUTM, &QThread::started, deleteUTM, &DeleteUTM::deleteUtm);

    connect(deleteUTM, &DeleteUTM::signalDeleteUTM, threadDeleteUTM, &QThread::quit);
    connect(deleteUTM, &DeleteUTM::signalDeleteUTM, deleteUTM, &DeleteUTM::deleteLater);
    connect(deleteUTM, &DeleteUTM::signalDeleteUTM, this, &MainWindow::slotDeleteUTM);

    connect(threadDeleteUTM, &QThread::finished, threadDeleteUTM, &QThread::deleteLater);

    threadDeleteUTM->start();
}

// О программе
void MainWindow::on_actionAboutProgramm_triggered()
{
    QMessageBox::information(this, "О программе", "Программа позволяет запускать до 10ти УТМ одновременно.");
}

// Связь с разработчиком
void MainWindow::on_actionCallBack_triggered()
{
    QMessageBox::information(this, "Связь с разработчиком", "Разработчик: Королев Сергей\n"
                                                            "GitHub: https://github.com/2UTM/2UTM\n"
                                                            "Почта: 2UTM.dev@gmail.com\n"
                                                            "Telegram: @regnorr");
}
