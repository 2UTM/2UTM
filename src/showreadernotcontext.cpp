#include "showreadernotcontext.h"
#include "ui_showreadernotcontext.h"

ShowReaderNotContext::ShowReaderNotContext(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ShowReaderNotContext)
{
    ui->setupUi(this);
    setFixedSize(331, 379);
    setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowTitle("Ридеры вне контекста");
    showAllReaderNotContext();
}

ShowReaderNotContext::~ShowReaderNotContext()
{
    delete ui;
}

// Показать все ридеры вне контекста
void ShowReaderNotContext::showAllReaderNotContext()
{
    ui->listWidget->clear();
    QVector<QString> vecReaderNotContext;

    // Получаем ридеры вне контекста
    int result = Control::listDevice(vecReaderNotContext);
    if (result == 1)
    {
        QMessageBox::information(this, "Успешно", "Устройства не найдены!");
        qInfo(logInfo()) << "Устройства не найдены";
        return;
    }
    if (result)
    {
        qWarning(logWarning()) << "Ошибка listDevice, код ошибки - " + QString::number(result);
        QMessageBox::warning(this, "Ошибка", "Ошибка listDevice, код ошибки - " + QString::number(result));
        return;
    }

    for (int i = 0; i < vecReaderNotContext.size(); ++i)
    {
        QListWidgetItem *item = new QListWidgetItem();
        item->setIcon(QIcon("./image/token.png"));
        item->setText("          " + vecReaderNotContext[i]);
        ui->listWidget->insertItem(i, item);
    }
}

// Отмена
void ShowReaderNotContext::on_pushButtonCancel_clicked()
{
    close();
}

// Удалить ридер
void ShowReaderNotContext::on_pushButtonDeleteReader_clicked()
{
    if (!ui->listWidget->currentItem())
    {
        QMessageBox::warning(this, "Ошибка", "Выберите ридер!");
        return;
    }
    QString nameReader = ui->listWidget->currentItem()->text().trimmed();

    int answer = QMessageBox::question(this, "Внимание", "Если этот ридер единственный у смарт карты, "
                                            "вы потеряете к ней доступ. Вы уверены, что хотите удалить ридер " + nameReader + "?");
    if (answer == QMessageBox::Yes)
    {
        int result = Control::removeNameReader(ui->listWidget->currentItem()->text().trimmed());
        if (result)
        {
            qWarning(logWarning()) << "Ошибка removeNameReader, код ошибки - " + QString::number(result);
            QMessageBox::warning(this, "Ошибка", "Ошибка removeNameReader, код ошибки - " + QString::number(result));
            return;
        }
    }
    showAllReaderNotContext();
}

