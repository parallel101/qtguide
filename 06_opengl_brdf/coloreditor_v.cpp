#include "coloreditor_v.h"
#include <QPainter>
#include <QLinearGradient>
#include <QDebug>
#include <QMouseEvent>
#include <QVector2D>
#include <QtMath>


ColorEditor_V::ColorEditor_V(QWidget *parent)
    : QWidget(parent), height(10), isPressed(false)
{
    setFixedSize(40, 220);
}

void ColorEditor_V::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    {
        QLinearGradient brush;
        brush.setColorAt(0, Qt::black);
        brush.setColorAt(1, Qt::white);
        brush.setStart(0, 210);
        brush.setFinalStop(0, 10);
        painter.setBrush(brush);
        painter.setPen(Qt::NoPen);
        painter.drawRect(10, 10, 20, 200);
    }

    {
        painter.setBrush(heightColor(height));
        painter.setPen(0x444444);
        int radius = 1;
        if (isPressed) {
            radius = 3;
        }
        painter.drawRect(10 - radius, height - 3 - radius, 20 + 2 * radius, 6 + 2 * radius);
    }
}

void ColorEditor_V::mousePressEvent(QMouseEvent *event)
{
    // setCursor(Qt::BlankCursor);
    onMousePress(event->pos());
}

void ColorEditor_V::mouseMoveEvent(QMouseEvent *event)
{
    onMousePress(event->pos());
}

void ColorEditor_V::mouseReleaseEvent(QMouseEvent *event)
{
    setCursor(Qt::ArrowCursor);
    isPressed = false;
    repaint();
}

void ColorEditor_V::onMousePress(QPoint pos)
{
    height = qMax(10, qMin(pos.y(), 210));
    isPressed = true;
    repaint();

    auto value = (210 - height) / 200.0;
    emit valueChanged(value);
}

QColor ColorEditor_V::heightColor(int h)
{
    auto value = (210 - h) / 200.0;
    return QColor::fromHsvF(0, 0, value);
}

void ColorEditor_V::setValue(double value)
{
    height = 210 - int(std::round(value * 200.0));
    repaint();
    emit valueChanged(value);
}
