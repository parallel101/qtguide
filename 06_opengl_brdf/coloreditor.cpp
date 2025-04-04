#include "coloreditor.h"
#include "coloreditor_hs.h"
#include "coloreditor_v.h"
#include <QHBoxLayout>


ColorEditor::ColorEditor(QWidget *parent)
    : QWidget(parent)
{
    auto layout = new QHBoxLayout;
    setLayout(layout);

    hsEdit = new ColorEditor_HS;
    vEdit = new ColorEditor_V;

    connect(vEdit, &ColorEditor_V::valueChanged,
            hsEdit, &ColorEditor_HS::setValue);
    connect(hsEdit, &ColorEditor_HS::colorChanged,
            this, &ColorEditor::colorChanged);

    layout->addWidget(hsEdit);
    layout->addWidget(vEdit);

    vEdit->setValue(0.8);
}

QColor ColorEditor::color() const
{
    return hsEdit->color();
}
