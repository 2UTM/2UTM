#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "logging.h"

// Структура нужна для вывода полей сертификата, взял из примера
std::pair<X500Dn::RdnId::Type, std::string> rdnIds[] = {
    std::make_pair(X500Dn::RdnId::commonName, "commonName"),
    std::make_pair(X500Dn::RdnId::surname, "surname"),
    std::make_pair(X500Dn::RdnId::givenName, "givenName"),
    std::make_pair(X500Dn::RdnId::title, "title"),
    std::make_pair(X500Dn::RdnId::pseudonym, "pseudonym"),
    std::make_pair(X500Dn::RdnId::emailAddress, "emailAddress"),
    std::make_pair(X500Dn::RdnId::countryName, "countryName"),
    std::make_pair(X500Dn::RdnId::localityName, "localityName"),
    std::make_pair(X500Dn::RdnId::stateOrProvinceName, "stateOrProvinceName"),
    std::make_pair(X500Dn::RdnId::organization, "organization"),
    std::make_pair(X500Dn::RdnId::organizationalUnit, "organizationalUnit"),
    std::make_pair(X500Dn::RdnId::street, "street"),
    std::make_pair(X500Dn::RdnId::ogrn, "ogrn"),
    std::make_pair(X500Dn::RdnId::ogrnip, "ogrnip"),
    std::make_pair(X500Dn::RdnId::snils, "snils"),
    std::make_pair(X500Dn::RdnId::inn, "inn")
};


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    MainWindow::show_device();

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

        if (lst_start[2] == "false")
        {
            show();
        }
        if (lst_start[0] == "true")
        {
            MainWindow::start_th();
        }
    }

    // Инициализируем иконку трея, устанавливаем иконку из набора системных иконок,а также задаем всплывающую подсказку
    p_trayIcon = new QSystemTrayIcon();
    p_trayIcon->setIcon(QIcon(QApplication::applicationDirPath() + "/image/2UTM.ico"));
    p_trayIcon->setToolTip("2UTM");

    // После чего создаем контекстное меню из трех пунктов
    QMenu *menu = new QMenu(this);
    QAction *showWindow = new QAction("Показать 2UTM", this);
    QAction *hideWindow = new QAction("Скрыть 2UTM", this);
    QAction *quitAction = new QAction("Выйти из 2UTM", this);

    // подключаем сигналы нажатий на пункты меню к соответсвующим слотам.
    connect(showWindow, &QAction::triggered,this,&QMainWindow::show);
    connect(hideWindow, &QAction::triggered,this,&QMainWindow::hide);
    connect(quitAction, &QAction::triggered, this, &MainWindow::close);
    menu->addAction(showWindow);
    menu->addAction(hideWindow);
    menu->addAction(quitAction);

    // Устанавливаем контекстное меню на иконку и показываем иконку приложения в трее
    p_trayIcon->setContextMenu(menu);
    p_trayIcon->show();
    //Также подключаем сигнал нажатия на иконку к обработчику данного нажатия
    connect(p_trayIcon, &QSystemTrayIcon::activated, this, &MainWindow::iconActivated);

    qInfo(logInfo()) << "Приложение запущено";
}

MainWindow::~MainWindow()
{
    delete ui;
}

// Нажатие на трей
void MainWindow::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    this->setFocus(Qt::FocusReason(reason));
    switch (reason)
    {
        case QSystemTrayIcon::Trigger:
            if(!this->isVisible())
            {
                this->show();
            }
            else
            {
                this->hide();
            }
        break;
    default:
        break;
    }
}

// Метод, который обрабатывает событие закрытия окна приложения
void MainWindow::closeEvent(QCloseEvent * event)
{
    if(this->isVisible())
    {
        event->ignore();
        this->hide();
        QSystemTrayIcon::MessageIcon icon = QSystemTrayIcon::MessageIcon(QSystemTrayIcon::Information);
        p_trayIcon->showMessage("2UTM", ("Приложение свернуто в панель задач."), icon, 2000);
    }
    else
    {
        MainWindow::exit_prog();
    }
}

// Метод создания потока запуска утм
void MainWindow::start_th()
{
    //Создание объектов рабочего класа и потока
    thread_start = new QThread(this);
    start = new Start();
    start->set_settings(QApplication::applicationDirPath(), device[0]);
    start->moveToThread(thread_start);

    //Соединение слотов и сигналов с потоками
    connect(thread_start, &QThread::started, start, &Start::start_utm);

    connect(start, &Start::signalStartEnd, thread_start, &QThread::quit);
    connect(start, &Start::signalStartEnd, start, &Start::deleteLater);
    connect(start, &Start::signalStartEnd, this, &MainWindow::after_start);

    connect(start, &Start::signalStartErrorEnd, thread_start, &QThread::quit);
    connect(start, &Start::signalStartErrorEnd, start, &Start::deleteLater);
    connect(start, &Start::signalStartErrorEnd, this, &MainWindow::after_error_start);

    connect(start, &Start::signalAutoStartErrorEnd, thread_start, &QThread::quit);
    connect(start, &Start::signalAutoStartErrorEnd, start, &Start::deleteLater);
    connect(start, &Start::signalAutoStartErrorEnd, this, &MainWindow::after_auto_start);

    connect(thread_start, &QThread::finished, thread_start, &QThread::deleteLater);

    if (QFile("./config.ini").exists() && device.size() > 1)
    {
        thread_start->start();
        ui->statusbar->showMessage("Запускаем УТМы...");
    }
}

// Метод создания потока остановки утм
void MainWindow::stop_th()
{
    //Создание объектов рабочего класа и потока
    thread_stop = new QThread(this);
    stop = new Stop();
    stop->moveToThread(thread_stop);

    //Соединение слотов и сигналов с потоками
    connect(thread_stop, &QThread::started, stop, &Stop::stop_utm);

    connect(stop, &Stop::signalStopEnd, thread_stop, &QThread::quit);
    connect(stop, &Stop::signalStopEnd, stop, &Stop::deleteLater);
    connect(stop, &Stop::signalStopEnd, this, &MainWindow::after_stop);

    connect(stop, &Stop::signalStopErrorEnd, thread_stop, &QThread::quit);
    connect(stop, &Stop::signalStopErrorEnd, stop, &Stop::deleteLater);
    connect(stop, &Stop::signalStopErrorEnd, this, &MainWindow::after_error_stop);

    connect(thread_stop, &QThread::finished, thread_stop, &QThread::deleteLater);

    thread_stop->start();
}

// Метод создания потока удаления утм
void MainWindow::uninstall_th()
{
    //Создание объектов рабочего класа и потока
    thread_uninstall = new QThread(this);
    uninstall = new Uninstall();
    uninstall->moveToThread(thread_uninstall);

    //Соединение слотов и сигналов с потоками
    connect(thread_uninstall, &QThread::started, uninstall, &Uninstall::uninstall_utm);

    connect(uninstall, &Uninstall::signalUninstallEnd, thread_uninstall, &QThread::quit);
    connect(uninstall, &Uninstall::signalUninstallEnd, uninstall, &Uninstall::deleteLater);
    connect(uninstall, &Uninstall::signalUninstallEnd, this, &MainWindow::after_uninstall);

    connect(uninstall, &Uninstall::signalUninstallErrorEnd, thread_uninstall, &QThread::quit);
    connect(uninstall, &Uninstall::signalUninstallErrorEnd, uninstall, &Uninstall::deleteLater);
    connect(uninstall, &Uninstall::signalUninstallErrorEnd, this, &MainWindow::after_error_uninstall);

    connect(thread_uninstall, &QThread::finished, thread_uninstall, &QThread::deleteLater);

    thread_uninstall->start();
}

// Метод создания потока переподключения устройств
void MainWindow::reconnect_th()
{
    //Создание объектов рабочего класа и потока
    thread_reconnect = new QThread(this);
    reconnect = new Reconnect();
    reconnect->set_settings(QApplication::applicationDirPath());
    reconnect->moveToThread(thread_reconnect);

    //Соединение слотов и сигналов с потоками
    connect(thread_reconnect, &QThread::started, reconnect, &Reconnect::reconnect_device);

    connect(reconnect, &Reconnect::signalReconnectEnd, thread_reconnect, &QThread::quit);
    connect(reconnect, &Reconnect::signalReconnectEnd, reconnect, &Reconnect::deleteLater);
    connect(reconnect, &Reconnect::signalReconnectEnd, this, &MainWindow::after_reconnect);

    connect(reconnect, &Reconnect::signalReconnectErrorEnd, thread_reconnect, &QThread::quit);
    connect(reconnect, &Reconnect::signalReconnectErrorEnd, reconnect, &Reconnect::deleteLater);
    connect(reconnect, &Reconnect::signalReconnectErrorEnd, this, &MainWindow::after_error_reconnect);

    connect(thread_reconnect, &QThread::finished, thread_reconnect, &QThread::deleteLater);

    thread_reconnect->start();
}

// Метод получения детей устройства
QString MainWindow::device_child(WCHAR DeviceInstanceID[])
{
    QString result;

    CONFIGRET CMResult = CR_SUCCESS;

    DEVNODE ParentDeviceNode = 0;
    CMResult = CM_Locate_DevNodeW((PDEVINST) &ParentDeviceNode, DeviceInstanceID, CM_LOCATE_DEVNODE_NORMAL); // дескриптор родительского экземпляра

    if (CMResult == CR_SUCCESS)
    {
        DEVINST FirstChildDeviceNode = 0;
        CMResult = CM_Get_Child((PDEVINST) &FirstChildDeviceNode, ParentDeviceNode, 0x0); // Получаем первый дочерний элемент

        if (CMResult == CR_SUCCESS)
        {
            DEVINST SecondChildDeviceNode = 0;
            CMResult = CM_Get_Sibling((PDEVINST) &SecondChildDeviceNode, FirstChildDeviceNode, 0x0); // Получаем второй дочерний элементы

            if (CMResult == CR_SUCCESS)
            {
                ULONG ChildDBuffLength = 255;
                WCHAR * ChildInstanceIDBuff = (WCHAR *) malloc(ChildDBuffLength); // выделяем память для пути до дочернего элемента
                CMResult = CM_Get_Device_IDW(SecondChildDeviceNode, ChildInstanceIDBuff, ChildDBuffLength, 0x0); // получаем путь до дочернего элемента

                if (CMResult == CR_SUCCESS)
                {
                    result = QString::fromWCharArray(ChildInstanceIDBuff);
                    message_statusbar = "Выполнено";
                }

                free(ChildInstanceIDBuff);
            }
        }
    }
    else
    {
        message_statusbar = "Не удалось получить дочерние элементы устройств";
        qWarning(logWarning()) << "Не удалось получить дочерние элементы устройств";
    }
    return result;
}

// Метод получения инфы об устройствах
void MainWindow::show_device()
{
    // Конструктор и деструктор от рутокенов
    rutoken::pkicore::initialize(".");
    SCOPE_EXIT() {
        rutoken::pkicore::deinitialize();
    };

    ui->listWidget->clear();
    owner_device.clear();

    // Результаты
    QVector<QString> vec_device;
    QVector<QString> vec_info_device;
    QVector<QString> vec_device_off;
    QString res_str;

    // Счетчик
    int count = 0;

    // Получаем пути до экземпляров устройств
    process.start("devcon status *VID_0A89*");
    if(!process.waitForFinished(60000) )
    {
        ui->statusbar->showMessage("Не удалось получить список устройств");
        qWarning(logWarning()) << "Не удалось получить список устройств";
    }
    else
    {
        QString r;
        r = process.readAllStandardOutput();
        QStringList list_device_parent = r.split("\n");
        if (list_device_parent.size() != 0)
        {
            for (int i = 0; i < list_device_parent.size(); ++i)
            {
                if (list_device_parent[i].contains("disabled"))
                    vec_device_off.append(list_device_parent[i - 1]);
            }
        }

        // Получаем список подключенных устройств
        std::vector<Pkcs11Device> devices = Pkcs11Device::enumerate();

        // Выводим устройства
        if (devices.size() != 0)
        {
            // Делаем вектор с сопоставленными адресами
            QString dev_parent;
            QString dev_child;
            dev_parent = list_device_parent[count].replace("\r", "");
            for (int i = 0; i < list_device_parent.size() - 2; i += 2)
            {
                dev_parent = list_device_parent[i].replace("\r", "");
                dev_child = MainWindow::device_child((WCHAR *)dev_parent.utf16());
                if (dev_child.contains("AKTIV_RUTOKEN_ECP_0"))
                {
                    vec_device.prepend(dev_parent);
                }
                if (dev_child.contains("AKTIV_RUTOKEN_ECP_1"))
                {
                    vec_device.append(dev_parent);
                }
            }
            for (__gnu_cxx::__normal_iterator<Pkcs11Device *, std::vector<Pkcs11Device, std::allocator<Pkcs11Device>>> i = devices.begin(); i != devices.end(); ++i)
            {
                Pkcs11Device& d = *i;

                // Выводим серийный номер устройства
                res_str = "Устройство с серийным номером: " + QString::fromStdString(d.getSerialNumber()) + "\n";

                // Выводим путь до экземпляра устройства
                if (count < vec_device.size())
                {
                    res_str += "Путь до экземпляра устройства: " + vec_device[count] + "\n";
                    ++count;
                }
                else
                {
                    res_str += "Путь до экземпляра устройства: Недоступно\n";
                }

                // Кому принадлежит сертификат на устройстве
                std::vector<Pkcs11Cert> certs = d.enumerateCerts();
                Pkcs11Cert& cert = certs[0];
                X500Dn subject = cert.getSubject();
                for(int i = 0; i < subject.getRdn(rdnIds[0].first).length(); ++i)
                {
                    if (int(subject.getRdn(rdnIds[0].first)[i]) < 48 || int(subject.getRdn(rdnIds[0].first)[i]) > 58)//48-58 это ASCII-коды цифр
                    {
                        res_str +="Владелец устройства: " + QString::fromStdString(subject.getRdn(rdnIds[0].first)) + "\n";
                        owner_device.append(QString::fromStdString(subject.getRdn(rdnIds[0].first)));
                        break;
                    }
                    else
                    {
                        Pkcs11Cert& cert = certs[1];
                        X500Dn subject = cert.getSubject();
                        res_str +="Владелец устройства: " + QString::fromStdString(subject.getRdn(rdnIds[0].first)) + "\n";
                        owner_device.append(QString::fromStdString(subject.getRdn(rdnIds[0].first)));
                        break;
                    }
                }
                // Статус устройства
                res_str += "Статус устройства: Включено\n";
                vec_info_device.append(res_str);
            }
        }
        if (vec_device_off.size() != 0)
        {
            count = 0;
            for (int i = 0; i < vec_device_off.size(); ++i)
            {
                // Добавляем путь в вектор для отключенного утсройства
                vec_device.append(vec_device_off[count].replace("\r", ""));

                // Выводим серийный номер устройства
                res_str = "Устройство с серийным номером: Недоступно\n";

                // Выводим путь до экземпляра устройства
                res_str += "Путь до экземпляра устройства: " + vec_device_off[count] + "\n";
                ++count;

                // Кому принадлежит сертификат на устройстве
                res_str +="Владелец устройства: Недоступно\n";

                // Статус устройства
                res_str += "Статус устройства: Отключено\n";
                vec_info_device.append(res_str);
            }
        }
        qInfo(logInfo()) << "Успешно получили список устройств";
    }
    if (device.size() == 0)
    {
        device = vec_device;
    }
    if (info_device.size() == 0)
    {
        info_device = vec_info_device;
    }
}

// Сигнал после переподключения устройств
void MainWindow::after_reconnect()
{
    ui->statusbar->showMessage("Устройства переподключены");
    if (loader)
    {
        loader->close();
        loader = nullptr;
    }
}

// Сигнал после неудачного переподключения устройств
void MainWindow::after_error_reconnect()
{
    ui->statusbar->showMessage("Устройства не переподключены, произошла ошибка! Подробности смотрите в логе программы");
    if (loader)
    {
        loader->close();
        loader = nullptr;
    }
}

// Метод после старта утм
void MainWindow::after_start()
{
    ui->statusbar->showMessage("УТМы запущены");
    if (loader)
    {
        loader->close();
        loader = nullptr;
    }
    QSystemTrayIcon::MessageIcon icon = QSystemTrayIcon::MessageIcon(QSystemTrayIcon::Information);
    if (lst_start[1] == "true")
    {
        p_trayIcon->showMessage("2UTM", ("УТМы запущены! Программа будет закрыта через минуту..."), icon, 2000);
        QTimer::singleShot(60000, this, &MainWindow::exit_prog);
    }
}

// Метод после авто старта
void MainWindow::after_auto_start()
{
    ui->statusbar->showMessage("УТМы не установлены, запуск не произведен");
    if (loader)
    {
        loader->close();
        loader = nullptr;
    }
}

// Метод после неудачного старта утм
void MainWindow::after_error_start()
{
    ui->statusbar->showMessage("УТМы не запущены, произошла ошибка! Подробности смотрите в логе программы");
    if (loader)
    {
        loader->close();
        loader = nullptr;
    }
}

// Метод после остановки утм
void MainWindow::after_stop()
{
    ui->statusbar->showMessage("УТМы остановлены");
    loader->close();
    loader = nullptr;
}

// Метод после неудачной остановки утм
void MainWindow::after_error_stop()
{
    ui->statusbar->showMessage("УТМы не остановлены, произошла ошибка! Подробности смотрите в логе программы");
    loader->close();
    loader = nullptr;
}

// Метод после удаления утм
void MainWindow::after_uninstall()
{
    ui->statusbar->showMessage("УТМы удалены");
    loader->close();
    loader = nullptr;
}

// Метод после неудачного удаления утм
void MainWindow::after_error_uninstall()
{
    ui->statusbar->showMessage("УТМы не удалены, произошла ошибка! Подробности смотрите в логе программы");
    loader->close();
    loader = nullptr;
}

// О программе
void MainWindow::on_actionAboutProg_triggered()
{
    QMessageBox::information(this, "О программе", "Позволяет установить 2 УТМа на один компьютер, хоть и ФСРАР говорит, что нельзя...");
}

// Прочитать устройства
void MainWindow::on_action_read_device_triggered()
{
    device.clear();
    info_device.clear();
    MainWindow::show_device();
    for (int i = 0; i < info_device.size(); ++i)
    {
        ui->listWidget->addItem(info_device[i]);
    }
    ui->statusbar->showMessage(message_statusbar);
}

// Включить устройство
void MainWindow::on_action_enable_device_triggered()
{
    int dev_on = ui->listWidget->currentRow();
    if (dev_on == -1)
    {
        message_statusbar = "Выберите одно устройство";
        ui->statusbar->showMessage(message_statusbar);
    }
    else
    {
        process.start("devcon enable \"@" + device[dev_on]);
        if(!process.waitForFinished(60000))
        {
            ui->statusbar->showMessage("Не удалось включить устройство");
            qWarning(logWarning()) << "Не удалось включить устройство, функция не ответила и завершилась по таймауту";
            return;
        }
        info_device[dev_on].replace("Отключено", "Включено");
        ui->listWidget->clear();
        for (int i = 0; i < info_device.size(); ++i)
        {
            ui->listWidget->addItem(info_device[i]);
        }
        Sleep(1000);
        ui->statusbar->showMessage("Устройство включено");
        qInfo(logInfo()) << "Включено устройство " + info_device[dev_on];
    }
}

// Установить УТМ
void MainWindow::on_action_install_utm_triggered()
{
    if (QDir("C:/UTM").exists())
    {
        QMessageBox::warning(this, "Ошибка", "Удалите УТМ, который у вас уже установлен!");
    }
    else
    {
        InstallUTM *install = new InstallUTM(this);
        install->setAttribute(Qt::WA_ShowModal, true);
        install->installutm_name_device(owner_device, device);
        install->show();
    }
}

// Удалить утм
void MainWindow::on_action_uninstall_utm_triggered()
{
    if (!QDir("C:/UTM").exists() && !QDir("C:/UTM_2").exists())
    {
        QMessageBox::warning(this, "Ошибка", "Не найдены установленные УТМ!");
    }
    else
    {
        int answer = QMessageBox::question(this, "Удаление УТМ", "Вы уверены, что хотите удалить УТМы?");
        if (answer == QMessageBox::Yes)
        {
            // Окно загрузки
            ui->statusbar->showMessage("Удаляем УТМы...");
            loader = new Loader(this);
            loader->setAttribute(Qt::WA_ShowModal, true);
            loader->show();

            MainWindow::uninstall_th();
        }
    }
}

// Связь с разработчиком
void MainWindow::on_action_callback_triggered()
{
    QMessageBox::information(this, "Связь с разработчиком", "github - https://github.com/2UTM/2UTM\nпочта - 2UTM.dev@gmail.com");
}

// Выход
void MainWindow::on_action_exit_triggered()
{
    p_trayIcon->hide();
    QApplication::exit(0);
}

// Отключить устройство
void MainWindow::on_action_disable_device_triggered()
{
    int dev_of = -1;
    dev_of = ui->listWidget->currentRow();
    if (dev_of == -1)
    {
        message_statusbar = "Выберите одно устройство";
        ui->statusbar->showMessage(message_statusbar);
    }
    else
    {
        process.start("devcon disable \"@" + device[dev_of]);
        if(!process.waitForFinished(60000))
        {
            ui->statusbar->showMessage("Не удалось отключить устройство");
            qWarning(logWarning()) << "Не удалось отключить устройство, функция не ответила и завершилась по таймауту";
            return;
        }
        info_device[dev_of].replace("Включено", "Отключено");
        ui->listWidget->clear();
        for (int i = 0; i < info_device.size(); ++i)
        {
            ui->listWidget->addItem(info_device[i]);
        }
        Sleep(1000);
        ui->statusbar->showMessage("Устройство отключено");
        qInfo(logInfo()) << "Отключено устройство " + info_device[dev_of];
    }
}

// Переподключить устройства
void MainWindow::on_action_reconnect_device_triggered()
{
    if (QFile("./config.ini").exists())
    {
        // Окно загрузки
        ui->statusbar->showMessage("Переподключаем устройства...");
        loader = new Loader(this);
        loader->setAttribute(Qt::WA_ShowModal, true);
        loader->show();

        MainWindow::reconnect_th();
    }
    else
    {
        ui->statusbar->showMessage("Не найден файл настроек");
    }
}

// Запустить утм
void MainWindow::on_action_start_utm_triggered()
{
    if (QFile("./config.ini").exists() && device.size() > 1)
    {
        // Окно загрузки
        ui->statusbar->showMessage("Запускаем УТМы...");
        loader = new Loader(this);
        loader->setAttribute(Qt::WA_ShowModal, true);
        loader->show();

        MainWindow::start_th();
    }
    else
    {
        ui->statusbar->showMessage("Нет установленных УТМ или подключенных рутокенов меньше двух");
    }
}

// Остановить утм
void MainWindow::on_action_stop_utm_triggered()
{
    if (QFile("./config.ini").exists())
    {
        // Окно загрузки
        ui->statusbar->showMessage("Останавливаем УТМы...");
        loader = new Loader(this);
        loader->setAttribute(Qt::WA_ShowModal, true);
        loader->show();

        MainWindow::stop_th();
    }
    else
    {
        ui->statusbar->showMessage("Нет установленных УТМ");
    }
}

// Поменять порядок устройств
void MainWindow::on_action_change_device_triggered()
{
    change_device = new Change_device();
    change_device->setAttribute(Qt::WA_ShowModal, true);
    change_device->change_device_name_device(owner_device, device);
    change_device->exec();
    change_device->deleteLater();
}

// Выход
void MainWindow::exit_prog()
{
    p_trayIcon->hide();
    QApplication::exit(0);
}

// Настройки
void MainWindow::on_action_settings_triggered()
{
    setting = new Setting();
    setting->exec();
    setting->deleteLater();
}

