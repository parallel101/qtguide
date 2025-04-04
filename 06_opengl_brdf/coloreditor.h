#pragma once

#include <QWidget>

class ColorEditor : public QWidget
{
    Q_OBJECT

    class ColorEditor_HS *hsEdit;
    class ColorEditor_V *vEdit;

public:
    explicit ColorEditor(QWidget *parent = nullptr);

    void setColor(QColor color);
    QColor color() const;

signals:
    void colorChanged(QColor color);
};
