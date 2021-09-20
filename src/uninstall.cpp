#include "uninstall.h"
#include "logging.h"

Uninstall::Uninstall(QObject *parent) : QObject(parent)
{
}

// Метод удаления УТМов
void Uninstall::uninstall_utm()
{
    QDir dir1("C:/UTM");
    QDir dir2("C:/UTM_2");

    // Проверка на существования УТМ
    if (!dir1.exists() || !dir2.exists())
    {
        emit signalUninstallEnd();
        qWarning(logWarning()) << "УТМы не установлены, удаление не произведено";
        return;
    }

    qInfo(logInfo()) << "Начало удаления...";

    process = new QProcess();
    // Останавливаем службы утма
    process->start("net stop Transport");
    if(!process->waitForFinished(60000)) {
        emit signalUninstallErrorEnd();
        qWarning(logWarning()) << "Не удалось остановить службу Transport, функция не ответила и завершилась по таймауту";
        return;
    }
    qInfo(logInfo()) << "Остановлена служба Transport";
    QThread::msleep(5000);

    process->start("net stop Transport_UTM_2");
    if(!process->waitForFinished(60000)) {
        emit signalUninstallErrorEnd();
        qWarning(logWarning()) << "Не удалось остановить службу Transport_UTM_2, функция не ответила и завершилась по таймауту";
        return;
    }
    qInfo(logInfo()) << "УТМы остановлены";
    QThread::msleep(5000);

    // Запускаем штатное удаление первого утм
    process->start("C:/UTM/unins000.exe");
    if(!process->waitForFinished(60000)) {
        emit signalUninstallErrorEnd();
        qWarning(logWarning()) << "Не удалось запустить штатное удаление УТМ, функция не ответила и завершилась по таймауту";
        return;
    }
    qInfo(logInfo()) << "УТМ удален штатными средствами";
    QThread::msleep(5000);

    // Удаляем службы УТМ
    process->start("sc delete Transport");
    if(!process->waitForFinished(60000)) {
        emit signalUninstallErrorEnd();
        qWarning(logWarning()) << "Не удалось удалить службу Transport, функция не ответила и завершилась по таймауту";
        return;
    }
    qInfo(logInfo()) << "Удалена служба Transport";
    QThread::msleep(5000);

    process->start("sc delete Transport_UTM_2");
    if(!process->waitForFinished(60000)) {
        emit signalUninstallErrorEnd();
        qWarning(logWarning()) << "Не удалось удалить службу Transport_UTM_2, функция не ответила и завершилась по таймауту";
        return;
    }
    qInfo(logInfo()) << "Удалена служба Transport_UTM_2";
    QThread::msleep(5000);

    // Удаляем папки с УТМ
    if (!dir1.removeRecursively())
    {
        emit signalUninstallErrorEnd();
        qWarning(logWarning()) << "Не удалось удалить папку UTM, функция не ответила и завершилась по таймауту";
        return;
    }
    if (!dir2.removeRecursively())
    {
        emit signalUninstallErrorEnd();
        qWarning(logWarning()) << "Не удалось удалить папку UTM_2, функция не ответила и завершилась по таймауту";
        return;
    }
    QThread::msleep(1000);

    qInfo(logInfo()) << "УТМы успешно удалены";
    emit signalUninstallEnd();
}
