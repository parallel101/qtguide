#include "mainwindow.h"
#include "openglwidget.h"
#include "pbrparams.h"
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setGeometry(500, 500, 1024, 768);
    setWindowTitle("Qt OpenGL"); // Qt = glad + glfw

    auto widget = new QWidget;
    setCentralWidget(widget);

    auto layout = new QVBoxLayout;
    widget->setLayout(layout);

    auto pbrParams = new PBRParams;
    auto openglWidget = new OpenGLWidget;

    layout->addWidget(pbrParams);
    layout->addWidget(openglWidget, 1);

    connect(pbrParams, &PBRParams::pbrParamChanged,
            openglWidget, &OpenGLWidget::setPBRParam);
    connect(pbrParams, &PBRParams::baseColorChanged,
            openglWidget, &OpenGLWidget::setBaseColor);

    for (int role = 0; role < std::size(PBRParams::kParamNames); ++role) {
        openglWidget->setPBRParam(role, pbrParams->pbrParam(role));
    }
    openglWidget->setBaseColor(pbrParams->baseColor());
}
