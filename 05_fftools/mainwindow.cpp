#include "mainwindow.h"
#include "fftools.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("视频工具");
    setCentralWidget(new FFTools);
}
