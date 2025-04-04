#pragma once

#include <QWidget>

class ColorEditor_HS : public QWidget
{
    Q_OBJECT

    QPoint pointPos;
    double value;
    bool isPressed;

public:
    explicit ColorEditor_HS(QWidget *parent = nullptr);

    void setValue(double v);
    QColor color() const;

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    void onMousePress(QPoint pos);
    QColor pointColor(QPoint pos) const;

signals:
    void colorChanged(QColor color);
};
