#pragma once

#include <QWidget>
#include <QSlider>
#include <QLabel>


class Slider : public QWidget
{
    Q_OBJECT

    QSlider *m_slider;
    QLabel *m_labelTitle;
    QLabel *m_labelValue;

public:
    explicit Slider(QWidget *parent = nullptr);

    void setTitle(QString title);
    void setValue(double value);
    double value() const;

signals:
    void valueChanged(double value);
};
