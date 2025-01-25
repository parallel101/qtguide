#include "mainwindow.h"
#include "pricecalc.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("优惠计算器");
    setCentralWidget(new PriceCalc);
}
