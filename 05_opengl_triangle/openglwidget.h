#pragma once

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLVertexArrayObject> // VAO
#include <QOpenGLBuffer> // VBO
#include <QOpenGLShaderProgram> // Program
#include <QMouseEvent>
#include <QWheelEvent>
#include "camera.h"

class OpenGLWidget : public QOpenGLWidget, private QOpenGLFunctions_3_3_Core
{
    Q_OBJECT

    QOpenGLVertexArrayObject vao;
    QOpenGLBuffer vbo;
    int vertexCount;
    QOpenGLShaderProgram program;
    Camera camera;
    QPoint lastPos;

public:
    explicit OpenGLWidget(QWidget *parent = nullptr);

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
};
