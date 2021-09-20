#ifndef LOADER_H
#define LOADER_H

#include <QMainWindow>

namespace Ui {
class Loader;
}

class Loader : public QMainWindow
{
    Q_OBJECT

public:
    explicit Loader(QWidget *parent = nullptr);
    ~Loader();

private:
    Ui::Loader *ui;
};

#endif // LOADER_H
