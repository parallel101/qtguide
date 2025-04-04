#include "pbrparams.h"
#include <QHBoxLayout>
#include "slider.h"
#include "coloreditor.h"

PBRParams::PBRParams(QWidget *parent)
    : QWidget(parent)
{
    auto layout = new QHBoxLayout;
    setLayout(layout);
    auto vLayout = new QVBoxLayout;

    m_colorEditor = new ColorEditor;
    connect(m_colorEditor, &ColorEditor::colorChanged,
            this, &PBRParams::baseColorChanged);

    layout->addWidget(m_colorEditor);
    layout->addLayout(vLayout);

    static const double kParamDefaults[std::size(kParamNames)] = {
        0.05,
        0.0,
        0.5,
        0.0,
    };

    for (int role = 0; role < std::size(kParamNames); ++role) {
        m_sliders[role] = new Slider;
        m_sliders[role]->setTitle(kParamNames[role]);
        m_sliders[role]->setValue(kParamDefaults[role]);
        vLayout->addWidget(m_sliders[role]);

        connect(m_sliders[role], &Slider::valueChanged,
                this, [this, role] (double value) {
                    emit pbrParamChanged(role, value);
                });
    }
}

double PBRParams::pbrParam(int role)
{
    return m_sliders[role]->value();
}

QColor PBRParams::baseColor()
{
    return m_colorEditor->color();
}
