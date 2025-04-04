#include "coloreditor_hs.h"
#include <QPainter>
#include <QConicalGradient>
#include <QRadialGradient>
#include <QDebug>
#include <QMouseEvent>
#include <QVector2D>
#include <QtMath>


ColorEditor_HS::ColorEditor_HS(QWidget *parent)
    : QWidget(parent), pointPos(110, 110), value(1), isPressed(false)
{
    setFixedSize(220, 220);
}

void ColorEditor_HS::setValue(double v)
{
    value = v;
    repaint();
    emit colorChanged(color());
}

QColor ColorEditor_HS::color() const
{
    return pointColor(pointPos);
}

void ColorEditor_HS::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    {
        QConicalGradient brush;
        for (int i = 0; i <= 6; ++i) {
            double hue = i / 6.0;
            QColor color = QColor::fromHsvF(hue, 1, value);
            brush.setColorAt(1.0 - i / 6.0, color);
        }
        brush.setAngle(-90.0);
        brush.setCenter(110, 110);
        painter.setBrush(brush);
        painter.setPen(Qt::NoPen);
        painter.drawEllipse(10, 10, 200, 200);
    }

    {
        QRadialGradient brush;
        auto white = QColor::fromHsvF(0, 0, value);
        brush.setColorAt(0, white);
        white.setAlphaF(0);
        brush.setColorAt(1, white);

        brush.setCenter(110, 110);
        brush.setFocalPoint(110, 110);
        brush.setRadius(100);

        painter.setBrush(brush);
        painter.setPen(Qt::NoPen);
        painter.drawEllipse(10, 10, 200, 200);
    }

    {
        painter.setBrush(pointColor(pointPos));
        painter.setPen(0x444444);
        int radius = 5;
        if (isPressed) {
            radius = 8;
        }
        painter.drawEllipse(pointPos, radius, radius);
    }
}

void ColorEditor_HS::mousePressEvent(QMouseEvent *event)
{
    setCursor(Qt::BlankCursor);
    onMousePress(event->pos());
}

void ColorEditor_HS::mouseMoveEvent(QMouseEvent *event)
{
    onMousePress(event->pos());
}

void ColorEditor_HS::mouseReleaseEvent(QMouseEvent *event)
{
    setCursor(Qt::ArrowCursor);
    isPressed = false;
    repaint();
}

void ColorEditor_HS::onMousePress(QPoint pos)
{
    auto center = QVector2D(110, 110);
    auto delta = QVector2D(pos.x(), pos.y()) - center;
    if (delta.length() > 100) {
        delta *= 100 / delta.length();
        delta += center;
        pos.setX(delta.x());
        pos.setY(delta.y());
    }
    pointPos = pos;
    isPressed = true;
    emit colorChanged(pointColor(pointPos));
    repaint();
}

QColor ColorEditor_HS::pointColor(QPoint pos) const
{
    auto center = QVector2D(110, 110);
    auto delta = QVector2D(pos.x(), pos.y()) - center;
    delta /= 100;
    auto hue = qRadiansToDegrees(qAtan2(delta.y(), delta.x()));
    hue -= 90;
    if (hue < 0) {
        hue += 360;
    }
    hue /= 360;
    auto saturation = qMin(double(delta.length()), 1.0);

    return QColor::fromHsvF(hue, saturation, value);
}
