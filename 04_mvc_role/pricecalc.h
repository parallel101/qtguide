#pragma once

#include "editor.h"
#include <QWidget>
#include <QLabel>

enum EditorType {
    GovPatch,
    SpicyMoney,
    SweetMoney,
    Discount,
    Taoness,
    Killness,
    CustomerName,
    MaxEditorType,
};

class PriceCalc : public QWidget
{
    Q_OBJECT

    Editor *m_editors[MaxEditorType];
    QLabel *m_priceLabel;

public:
    explicit PriceCalc(QWidget *parent = nullptr);

    void computePrice();
};
