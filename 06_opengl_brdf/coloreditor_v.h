#pragma once

#include <QWidget>

class ColorEditor_V : public QWidget
{
    Q_OBJECT

    int height;
    bool isPressed;

public:
    explicit ColorEditor_V(QWidget *parent = nullptr);
    void setValue(double value);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    void onMousePress(QPoint pos);
    QColor heightColor(int h);

signals:
    void valueChanged(double value);
};
