#include "mainwindow.h"
#include "openglwidget.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setFixedSize(1024, 768);
    setWindowTitle("Qt OpenGL"); // Qt = glad + glfw
    setCentralWidget(new OpenGLWidget);
}
