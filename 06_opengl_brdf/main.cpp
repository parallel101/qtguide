#include <QApplication>
#include <QDir>
#include "mainwindow.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    QDir::setCurrent("..");

    MainWindow win;
    win.show();
    return app.exec();
}
