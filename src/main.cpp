#include "mainwindow.h"
#include <QApplication>

// Умный указатель на файл логирования
QScopedPointer<QFile>   m_logFile;

// Объявляение обработчика лога
void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Устанавливаем файл логирования,
    m_logFile.reset(new QFile("./log.txt"));
    // Открываем файл логирования
    m_logFile.data()->open(QFile::Append | QFile::Text);
    // Устанавливаем обработчик
    qInstallMessageHandler(messageHandler);

    MainWindow w;
    w.setWindowTitle("2UTM");
    w.show();
    return a.exec();
}

// Реализация обработчика лога
void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    // Открываем поток записи в файл
    QTextStream out(m_logFile.data());
    // Записываем дату записи
    out << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz ");
    // По типу определяем, к какому уровню относится сообщение
    switch (type)
    {
        case QtInfoMsg:     break;
        case QtDebugMsg:    break;
        case QtWarningMsg:  break;
        case QtCriticalMsg: break;
        case QtFatalMsg:    break;
    }
    // Записываем в вывод категорию сообщения и само сообщение
    out << context.category << ": " << msg << endl;
    out.flush();    // Очищаем буферизированные данные
}
