#include "pricecalc.h"
#include <QCheckBox>
#include <QVBoxLayout>
#include <array>

class EditorFactory {
public:
    virtual Editor *createEditor() = 0;
    virtual void setProperty(Editor::Role role, Value *value) = 0;
};

template <class EditorType>
class EditorFactoryTemplate : public EditorFactory {
    std::map<Editor::Role, Value *> m_properties;

public:
    virtual Editor *createEditor() override {
        Editor *editor = new EditorType;
        for (auto const &[role, value]: m_properties) {
            editor->setProperty(role, value);
        }
        return editor;
    }

    virtual void setProperty(Editor::Role role, Value *value) override {
        m_properties[role] = value;
    }
};

class EditorFactoryBuilder {
    EditorFactory *m_editorFactory = nullptr;

public:
    template <class EditorType>
    EditorFactoryBuilder &withType() {
        m_editorFactory = new EditorFactoryTemplate<EditorType>;
        return *this;
    }

    EditorFactoryBuilder &withProperty(Editor::Role role, Value *value) {
        m_editorFactory->setProperty(role, value);
        return *this;
    }

    EditorFactory *build() {
        return m_editorFactory;
    }
};

const std::array<EditorFactory *, MaxEditorType> kEditorFactories = {
    EditorFactoryBuilder()
        .withType<BoolEditor>()
        .withProperty(Editor::TitleRole, new StringValue("国家补贴"))
        .build(),
    EditorFactoryBuilder()
        .withType<BoolEditor>()
        .withProperty(Editor::TitleRole, new StringValue("辣椒抵用券"))
        .build(),
    EditorFactoryBuilder()
        .withType<BoolEditor>()
        .withProperty(Editor::TitleRole, new StringValue("甜蜜抵用券"))
        .build(),
    EditorFactoryBuilder()
        .withType<BoolEditor>()
        .withProperty(Editor::TitleRole, new StringValue("商家折扣"))
        .build(),
    EditorFactoryBuilder()
        .withType<IntEditor>()
        .withProperty(Editor::TitleRole, new StringValue("淘气值"))
        .withProperty(Editor::MinValueRole, new IntValue(0))
        .withProperty(Editor::MaxValueRole, new IntValue(5))
        .build(),
    EditorFactoryBuilder()
        .withType<IntEditor>()
        .withProperty(Editor::TitleRole, new StringValue("杀气值"))
        .withProperty(Editor::MinValueRole, new IntValue(0))
        .withProperty(Editor::MaxValueRole, new IntValue(10))
        .build(),
    EditorFactoryBuilder()
        .withType<StringEditor>()
        .withProperty(Editor::TitleRole, new StringValue("客户姓名"))
        .build(),
};

PriceCalc::PriceCalc(QWidget *parent)
    : QWidget(parent)
{
    auto layout = new QVBoxLayout;
    setLayout(layout);

    for (int i = 0; i < MaxEditorType; ++i) {
        EditorFactory *factory = kEditorFactories[i];
        Editor *editor = factory->createEditor();
        layout->addWidget(editor);
        connect(editor, &Editor::valueChanged, this, &PriceCalc::computePrice);
        m_editors[i] = editor;
    }

    m_priceLabel = new QLabel;
    layout->addWidget(m_priceLabel);

    computePrice();
}

void PriceCalc::computePrice()
{
    double price = 50;
    if (m_editors[GovPatch]->value()->getBool()) {
        price -= 10;
    }
    if (m_editors[SpicyMoney]->value()->getBool()) {
        price -= 5;
    }
    if (m_editors[Discount]->value()->getBool()) {
        price -= 8;
    }
    price -= m_editors[Taoness]->value()->getInt();
    price -= m_editors[Killness]->value()->getInt();
    price -= m_editors[CustomerName]->value()->getString().size();

    m_priceLabel->setText(QString("价格：%1").arg(price));
}
