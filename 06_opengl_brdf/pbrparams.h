#pragma once

#include <QWidget>
#include <iterator>


class PBRParams : public QWidget
{
    Q_OBJECT

public:
    enum PBRParamRole
    {
        roughness,
        metallic,
        specular,
        emission,
    };

    static constexpr const char *kParamNames[] = {
        "roughness",
        "metallic",
        "specular",
        "emission",
    };

public:
    class Slider *m_sliders[std::size(kParamNames)];
    double m_sliderValues[std::size(kParamNames)];
    class ColorEditor *m_colorEditor;

public:
    explicit PBRParams(QWidget *parent = nullptr);

    double pbrParam(int role);
    QColor baseColor();

signals:
    void pbrParamChanged(int role, double value);
    void baseColorChanged(QColor color);
};
