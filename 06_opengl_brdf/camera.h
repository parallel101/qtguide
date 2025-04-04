#pragma once

#include <QMatrix4x4>
#include <QtMath>

struct Camera
{
    QMatrix4x4 model;
    QMatrix4x4 view;
    QMatrix4x4 projection;

    // 投影矩阵影响
    float aspect = 1;

    // 视角矩阵影响
    QVector3D target{0, 0, 0};
    float distance = 8;
    float phi = qDegreesToRadians(120.0f);  // 方位 (0~2PI)
    float theta = qDegreesToRadians(-25.0f); // 高度 (-PI/2~PI/2)
    float fov = 30.0f; // 视角宽度 (0~180)

    Camera()
    {
        updateProjection();
        updateView();
    }

    void setAspect(int w, int h)
    {
        aspect = (float)w / h;
        updateProjection();
    }

    void mouseMove(float dx, float dy)
    {
        dx *= qTan(fov / 2) * 4 * aspect;
        dy *= qTan(fov / 2) * 4;

        phi += dx;
        if (phi > M_PI * 2) {
            phi -= M_PI * 2;
        }
        if (phi < M_PI * 2) {
            phi += M_PI * 2;
        }
        theta += dy;
        theta = qMin<float>(theta, M_PI / 2 - 1e-6f);
        theta = qMax<float>(theta, -M_PI / 2 + 1e-6f);
        updateView();
    }

    void mouseWheel(float delta)
    {
        distance *= qExp(-0.12f * delta);
        updateView();
    }

private:
    void updateProjection()
    {
        projection.setToIdentity();
        projection.perspective(fov, aspect, 0.01f, 1000.0f);
    }

    void updateView()
    {
        QVector3D direction(qCos(theta) * qCos(phi), // x
                            qCos(theta) * qSin(phi), // y
                            qSin(theta));            // z
        view.setToIdentity();
        view.lookAt(target - direction * distance,
                    target, QVector3D(0, 0, 1));
    }
};
