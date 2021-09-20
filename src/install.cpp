#include "install.h"
#include "logging.h"

Install::Install(QObject *parent) : QObject(parent)
{
}

// Метод передачи вектора из главного окна
void Install::install_set_device(QString first_dev, QString second_dev, QString file_app, QString file_utm, QString installutm_dev)
{
    first_device = first_dev;
    second_device = second_dev;
    file_utm_install = file_utm;
    file_app_install = file_app;
    installutm_device = installutm_dev;
    qInfo(logInfo()) << "Успешно получены данные для начало установки УТМ";
}

// Прерывание установки
void Install::set_status_thread(bool status_thread)
{
    if (m_status_thread == status_thread)
        return;

    m_status_thread = status_thread;
//    emit status_thread_change(status_thread);
}

// Метод установки УТМов
void Install::install_utm()
{
    qInfo(logInfo()) << "Начало установки...";
    // Отключаем первый рутокен
    emit install_info_signal("Отключаем первый рутокен...", 5);
    process = new QProcess();
    process->start("devcon disable \"@" + first_device);
    if(!process->waitForFinished(60000)) {
        emit signalErrorEnd();
        qWarning(logWarning()) << "Не удалось отключить устройство " + first_device + ", функция не ответила и завершилась по таймауту";
        return;
    }
    qInfo(logInfo()) << "Отключено устройство " + first_device;
    QThread::msleep(5000);
    if (!m_status_thread)
    {
        emit signalCancelEnd();
        qWarning(logWarning()) << "Операция установки отменена пользователем";
        return;
    }

    // Запускаем установку утм
    emit install_info_signal("Устанавливаем первый УТМ...", 11);
    int count = 3;
    while (count > 0)
    {
        process->start("\"" + file_utm_install + "\"");
        if(!process->waitForFinished(60000)) {
            process->start("taskkill /IM silent-setup-4.2.0-b2400.tmp /F");
            if(!process->waitForFinished(60000)) {
                emit signalErrorEnd();
                qWarning(logWarning()) << "Не удалось установить УТМ, функция не ответила и завершилась по таймауту";
                return;
            }
            --count;
            continue;
        }
        else
        {
            break;
        }
    }
    qInfo(logInfo()) << "Установлен УТМ";
    QThread::msleep(5000);
    if (!m_status_thread)
    {
        emit signalCancelEnd();
        qWarning(logWarning()) << "Операция установки отменена пользователем";
        return;
    }

    // Убиваем процесс джавы для освобождения папки
    emit install_info_signal("Завершаем процесс джавы...", 16);
    process->start("taskkill /IM javaw.exe /F");
    if(!process->waitForFinished(60000)) {
        emit signalErrorEnd();
        qWarning(logWarning()) << "Не удалось остановить процесс javaw.exe, функция не ответила и завершилась по таймауту";
        return;
    }
    qInfo(logInfo()) << "Остановлен процесс javaw.exe";
    QThread::msleep(5000);
    if (!m_status_thread)
    {
        emit signalCancelEnd();
        qWarning(logWarning()) << "Операция установки отменена пользователем";
        return;
    }

    // Удаляем службу транспорта
    emit install_info_signal("Удаляем службу транспорта...", 23);
    process->start("c:/UTM/transporter/bin/delete.bat");
    if(!process->waitForFinished(60000)) {
        emit signalErrorEnd();
        qWarning(logWarning()) << "Не удалось удалить службу Transport, функция не ответила и завершилась по таймауту";
        return;
    }
    qInfo(logInfo()) << "Удалена служба Transport";
    QThread::msleep(5000);
    if (!m_status_thread)
    {
        emit signalCancelEnd();
        qWarning(logWarning()) << "Операция установки отменена пользователем";
        return;
    }

    // Переименовываем папку утм
    emit install_info_signal("Переименовываем папку УТМ...", 29);
    if (dir.rename("c:/UTM", "c:/UTM_2"))
    {
    }
    else
    {
        emit signalErrorEnd();
        qWarning(logWarning()) << "Не удалось переименовать папку UTM, функция не ответила и завершилась по таймауту";
        return;
    }
    qInfo(logInfo()) << "Переименована папка UTM";
    QThread::msleep(1000);
    if (!m_status_thread)
    {
        emit signalCancelEnd();
        qWarning(logWarning()) << "Операция установки отменена пользователем";
        return;
    }

    // Удаляем батники из утм
    emit install_info_signal("Исправляем батники в УТМ...", 34);
    QFile::remove("c:/UTM_2/transporter/bin/delete.bat");
    QFile::remove("c:/UTM_2/transporter/bin/install.bat");
    QFile::remove("c:/UTM_2/transporter/bin/run.bat");
    QFile::remove("c:/UTM_2/transporter/bin/stop.bat");
    QThread::msleep(1000);
    if (!m_status_thread)
    {
        emit signalCancelEnd();
        qWarning(logWarning()) << "Операция установки отменена пользователем";
        return;
    }

    // Копируем свои батники в утм
    QFile::copy(file_app_install + "/transport/delete.bat", "c:/UTM_2/transporter/bin/delete.bat");
    QFile::copy(file_app_install + "/transport/install.bat", "c:/UTM_2/transporter/bin/install.bat");
    QFile::copy(file_app_install + "/transport/run.bat", "c:/UTM_2/transporter/bin/run.bat");
    QFile::copy(file_app_install + "/transport/stop.bat", "c:/UTM_2/transporter/bin/stop.bat");
    qInfo(logInfo()) << "Исправлены батники в УТМ";
    QThread::msleep(1000);
    if (!m_status_thread)
    {
        emit signalCancelEnd();
        qWarning(logWarning()) << "Операция установки отменена пользователем";
        return;
    }

    // Редактируем порт в конфиге утм
    emit install_info_signal("Исправляем конфиг УТМ...", 38);
    QFile file_read("c:/UTM_2/transporter/conf/transport.properties");
    if (!file_read.open(QIODevice::ReadOnly)) {
        emit signalErrorEnd();
        qWarning(logWarning()) << "Не удалось открыть файл transport.properties, функция не ответила и завершилась по таймауту";
        return;
    }
    if (!m_status_thread)
    {
        emit signalCancelEnd();
        qWarning(logWarning()) << "Операция установки отменена пользователем";
        return;
    }
    fileData = file_read.readAll();
    fileData.replace("8080", "8081");
    file_read.close();
    QFile file_clear("c:/UTM_2/transporter/conf/transport.properties");
    if (file_clear.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        file_clear.close();
    }
    if (!m_status_thread)
    {
        emit signalCancelEnd();
        qWarning(logWarning()) << "Операция установки отменена пользователем";
        return;
    }
    QFile file_write("c:/UTM_2/transporter/conf/transport.properties");
    if (!file_write.open(QIODevice::WriteOnly)) {
        emit signalErrorEnd();
        qWarning(logWarning()) << "Не удалось открыть файл transport.properties, функция не ответила и завершилась по таймауту";
        return;
    }
    if (!m_status_thread)
    {
        emit signalCancelEnd();
        qWarning(logWarning()) << "Операция установки отменена пользователем";
        return;
    }
    file_write.write(fileData);
    file_write.close();
    qInfo(logInfo()) << "Исправлен конфиг УТМ";
    QThread::msleep(5000);
    if (!m_status_thread)
    {
        emit signalCancelEnd();
        qWarning(logWarning()) << "Операция установки отменена пользователем";
        return;
    }

    // Устанавливаем службу утм
    emit install_info_signal("Устанавливаем службу УТМ...", 45);
    process->start("c:/UTM_2/transporter/bin/install.bat Transport_UTM_2");
    if(!process->waitForFinished(60000)) {
        emit signalErrorEnd();
        qWarning(logWarning()) << "Не удалось установить службу Transport_UTM_2, функция не ответила и завершилась по таймауту";
        return;
    }
    qInfo(logInfo()) << "Остановлена служба Transport_UTM_2";
    QThread::msleep(5000);
    if (!m_status_thread)
    {
        emit signalCancelEnd();
        qWarning(logWarning()) << "Операция установки отменена пользователем";
        return;
    }

    // Запускаем службу утма
    emit install_info_signal("Запускаем службу УТМ...", 51);
    process->start("net start Transport_UTM_2");
    if(!process->waitForFinished(60000)) {
        emit signalErrorEnd();
        qWarning(logWarning()) << "Не удалось запустить службу Transport_UTM_2, функция не ответила и завершилась по таймауту";
        return;
    }
    while(m_status_thread) {
        res = Install::check_utm(host_utm2);
        if (res == "") {
            continue;
        }
        else {
            break;
        }
    }
    qInfo(logInfo()) << "Запущена служба УТМ";
    if (!m_status_thread)
    {
        emit signalCancelEnd();
        qWarning(logWarning()) << "Операция установки отменена пользователем";
        return;
    }

    // Останавливаем службу утма
    emit install_info_signal("Останавливаем службу УТМ...", 57);
    process->start("net stop Transport_UTM_2");
    if(!process->waitForFinished(60000)) {
        emit signalErrorEnd();
        qWarning(logWarning()) << "Не удалось остановить службу Transport_UTM_2, функция не ответила и завершилась по таймауту";
        return;
    }
    qInfo(logInfo()) << "Остановлена служба УТМ";
    QThread::msleep(5000);
    if (!m_status_thread)
    {
        emit signalCancelEnd();
        qWarning(logWarning()) << "Операция установки отменена пользователем";
        return;
    }

    // Включаем первый рутокен
    emit install_info_signal("Включаем первый рутокен...", 63);
    process->start("devcon enable \"@" + first_device);
    if(!process->waitForFinished(60000)) {
        emit signalErrorEnd();
        qWarning(logWarning()) << "Не удалось включить устройство " + first_device + ", функция не ответила и завершилась по таймауту";
        return;
    }
    qInfo(logInfo()) << "Включено устройство " + first_device;
    QThread::msleep(5000);
    if (!m_status_thread)
    {
        emit signalCancelEnd();
        qWarning(logWarning()) << "Операция установки отменена пользователем";
        return;
    }

    // Отключаем второй рутокен
    emit install_info_signal("Отключаем второй рутокен", 68);
    process->start("devcon disable \"@" + second_device);
    if(!process->waitForFinished(60000)) {
        emit signalErrorEnd();
        qWarning(logWarning()) << "Не удалось отключить устройство " + second_device + ", функция не ответила и завершилась по таймауту";
        return;
    }
    qInfo(logInfo()) << "Отключено устройство " + second_device;
    QThread::msleep(5000);
    if (!m_status_thread)
    {
        emit signalCancelEnd();
        qWarning(logWarning()) << "Операция установки отменена пользователем";
        return;
    }

    // Устанавливаем второй утм
    emit install_info_signal("Устанавливаем второй УТМ...", 73);
    count = 3;
    while (count > 0)
    {
        process->start("\"" + file_utm_install + "\"");
        if(!process->waitForFinished(60000)) {
            process->start("taskkill /IM silent-setup-4.2.0-b2400.tmp /F");
            if(!process->waitForFinished(60000)) {
                emit signalErrorEnd();
                qWarning(logWarning()) << "Не удалось установить УТМ, функция не ответила и завершилась по таймауту";
                return;
            }
            --count;
            continue;
        }
        else
        {
            break;
        }
    }
    qInfo(logInfo()) << "Установлен УТМ";
    QThread::msleep(5000);
    if (!m_status_thread)
    {
        emit signalCancelEnd();
        qWarning(logWarning()) << "Операция установки отменена пользователем";
        return;
    }

    // Запускаем службу утма
    emit install_info_signal("Запускаем службу УТМ...", 77);
    process->start("net start Transport");
    if(!process->waitForFinished(60000)) {
        emit signalErrorEnd();
        qWarning(logWarning()) << "Не удалось запустить службу Transport, функция не ответила и завершилась по таймауту";
        return;
    }
    while(m_status_thread) {
        res = Install::check_utm(host_utm1);
        if (res == "") {
            continue;
        }
        else {
            break;
        }
    }
    qInfo(logInfo()) << "Запущена служба Transport";
    if (!m_status_thread)
    {
        emit signalCancelEnd();
        qWarning(logWarning()) << "Операция установки отменена пользователем";
        return;
    }

    // Включаем второй рутокен
    emit install_info_signal("Включаем второй рутокен...", 82);
    process->start("devcon enable \"@" + second_device);
    if(!process->waitForFinished(60000)) {
        emit signalErrorEnd();
        qWarning(logWarning()) << "Не удалось включить устройство " + second_device + ", функция не ответила и завершилась по таймауту";
        return;
    }
    qInfo(logInfo()) << "Включено устройство " + second_device;
    QThread::msleep(5000);
    if (!m_status_thread)
    {
        emit signalCancelEnd();
        qWarning(logWarning()) << "Операция установки отменена пользователем";
        return;
    }

    // Отключаем первый рутокен
    emit install_info_signal("Отключаем первый рутокен...", 87);
    process->start("devcon disable \"@" + first_device);
    if(!process->waitForFinished(60000)) {
        emit signalErrorEnd();
        qWarning(logWarning()) << "Не удалось отключить устройство " + first_device + ", функция не ответила и завершилась по таймауту";
        return;
    }
    qInfo(logInfo()) << "Отключено устройство " + first_device;
    QThread::msleep(5000);
    if (!m_status_thread)
    {
        emit signalCancelEnd();
        qWarning(logWarning()) << "Операция установки отменена пользователем";
        return;
    }

    // Запускаем службу утма
    emit install_info_signal("Запускаем службу УТМ...", 94);
    process->start("net start Transport_UTM_2");
    if(!process->waitForFinished(60000)) {
        emit signalErrorEnd();
        qWarning(logWarning()) << "Не удалось запустить службу Transport_UTM_2, функция не ответила и завершилась по таймауту";
        return;
    }
    while(m_status_thread) {
        res = Install::check_utm(host_utm2);
        if (res == "") {
            continue;
        }
        else {
            break;
        }
    }
    qInfo(logInfo()) << "Запущена службу Transport_UTM_2";
    if (!m_status_thread)
    {
        emit signalCancelEnd();
        qWarning(logWarning()) << "Операция установки отменена пользователем";
        return;
    }

    // Включаем первый рутокен
    emit install_info_signal("Включаем первый рутокен...", 100);
    process->start("devcon enable \"@" + first_device);
    if(!process->waitForFinished(60000)) {
        emit signalErrorEnd();
        qWarning(logWarning()) << "Не удалось включить устройство " + first_device + ", функция не ответила и завершилась по таймауту";
        return;
    }
    QThread::msleep(5000);
    if (!m_status_thread)
    {
        emit signalCancelEnd();
        qWarning(logWarning()) << "Операция установки отменена пользователем";
        return;
    }

    qInfo(logInfo()) << "УТМы успешно установлены";
    emit signalEnd();
}

// Метод проверки доступа к УТМу
QString Install::check_utm(QString url)
{
    QString res_utm;
    //Выполняем запрос к сайту
    QNetworkAccessManager manager;
    QNetworkRequest request(QUrl(url.toUtf8()));
    request.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);
    QNetworkReply *response = manager.get(request);
    QEventLoop event;
    connect(response, &QNetworkReply::finished, &event, &QEventLoop::quit);
    event.exec();
    //Получаем ответ
    res_utm = response->readAll();
    return res_utm;
}
