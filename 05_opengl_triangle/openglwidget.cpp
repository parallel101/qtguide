#include "openglwidget.h"
#include "assimp.h"

OpenGLWidget::OpenGLWidget(QWidget *parent)
    : QOpenGLWidget(parent)
{
}

void OpenGLWidget::initializeGL()
{
    initializeOpenGLFunctions();

    glEnable(GL_DEPTH_TEST);

    vao.create();
    vbo.create();

    auto vertices = Assimp::loadObj("monkey.obj");
    vertexCount = vertices.size();
    vbo.bind();
    vbo.allocate(vertices.data(), vertices.size() * sizeof(vertices[0]));
    vbo.release();

    program.addShaderFromSourceFile(QOpenGLShader::Vertex, "orange.vert");
    program.addShaderFromSourceFile(QOpenGLShader::Fragment, "orange.frag");
    program.link();
}

void OpenGLWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
    camera.setAspect(w, h);
}

void OpenGLWidget::paintGL()
{
    qDebug() << "paintGL";
    vao.bind();

    vbo.bind();
    program.bind();

    program.setUniformValue("model", camera.model);
    program.setUniformValue("view", camera.view);
    program.setUniformValue("projection", camera.projection);

    QVector3D lightDir(2, 3, 4);
    lightDir.normalize();
    program.setUniformValue("lightDir", lightDir);

    program.setAttributeBuffer("position", GL_FLOAT,
                               offsetof(Assimp::Vertex, position), 3,
                               sizeof(Assimp::Vertex));
    program.setAttributeBuffer("normal", GL_FLOAT,
                               offsetof(Assimp::Vertex, normal), 3,
                               sizeof(Assimp::Vertex));
    program.enableAttributeArray("position");
    program.enableAttributeArray("normal");

    glDrawArrays(GL_TRIANGLES, 0, vertexCount);

    program.release();
    vbo.release();

    vao.release();
}

void OpenGLWidget::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();
}

void OpenGLWidget::mouseMoveEvent(QMouseEvent *event)
{
    QPointF deltaPos = event->pos() - lastPos;
    lastPos = event->pos();

    camera.mouseMove(deltaPos.x() / width(), deltaPos.y() / height());
    repaint();
}

void OpenGLWidget::mouseReleaseEvent(QMouseEvent *event)
{
}

void OpenGLWidget::wheelEvent(QWheelEvent *event)
{
    camera.mouseWheel(event->angleDelta().y() / 120.0f);
    repaint();
}
