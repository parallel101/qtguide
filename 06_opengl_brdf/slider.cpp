#include "slider.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QSlider>
#include <QtMath>

Slider::Slider(QWidget *parent)
    : QWidget(parent)
{
    auto layout = new QHBoxLayout;
    setLayout(layout);

    m_slider = new QSlider;
    m_slider->setOrientation(Qt::Horizontal);
    m_slider->setRange(0, 100);

    m_labelTitle = new QLabel;
    m_labelTitle->setFixedWidth(80);

    m_labelValue = new QLabel;
    m_labelValue->setText(QString::number(m_slider->value() * 0.01, 'n', 2));
    m_labelValue->setFixedWidth(30);

    connect(m_slider, &QSlider::valueChanged, this, [this] (int value) {
        m_labelValue->setText(QString::number(value * 0.01, 'n', 2));
        emit valueChanged(value * 0.01);
    });

    layout->addWidget(m_labelTitle, 0);
    layout->addWidget(m_slider, 1);
    layout->addWidget(m_labelValue, 0);
}

void Slider::setTitle(QString title)
{
    m_labelTitle->setText(title);
}

void Slider::setValue(double value)
{
    m_slider->setValue(int(qRound(value * 100)));
}

double Slider::value() const
{
    return m_slider->value() * 0.01;
}
