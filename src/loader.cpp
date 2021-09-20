#include "loader.h"
#include "ui_loader.h"

Loader::Loader(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Loader)
{
    ui->setupUi(this);
    setWindowFlag(Qt::FramelessWindowHint);
    ui->progressBar->setMaximum(0);
    ui->progressBar->setMinimum(0);
}

Loader::~Loader()
{
    delete ui;
}
