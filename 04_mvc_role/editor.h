#pragma once

#include <QWidget>
#include <QLabel>
#include <QSlider>
#include <QLineEdit>
#include <QCheckBox>
#include <QBoxLayout>

class Value : public QObject {
private:
    virtual void dummy() = 0;

public:
    virtual int getInt() const {
        throw;
    }

    virtual bool getBool() const {
        throw;
    }

    virtual QString getString() const {
        throw;
    }
};

class IntValue : public Value {
    int m_value;

    virtual void dummy() override {}

public:
    IntValue(int value) : m_value(value) {}

    virtual int getInt() const override {
        return m_value;
    }

    virtual bool getBool() const override {
        return m_value != 0;
    }

    virtual QString getString() const override {
        return QString::number(m_value);
    }
};

class BoolValue : public Value {
    bool m_value;

    virtual void dummy() override {}

public:
    BoolValue(bool value) : m_value(value) {}

    virtual bool getBool() const override {
        return m_value;
    }

    virtual int getInt() const override {
        return m_value ? 1 : 0;
    }

    virtual QString getString() const override {
        return m_value ? "true" : "false";
    }
};

struct StringValue : public Value {
    QString m_value;

    virtual void dummy() override {}

public:
    StringValue(QString value) : m_value(value) {}

    virtual QString getString() const override {
        return m_value;
    }

    virtual int getInt() const override {
        return m_value.toInt();
    }

    virtual bool getBool() const override {
        return !m_value.isEmpty();
    }
};

class Editor : public QWidget
{
    Q_OBJECT

public:
    enum Role {
        TitleRole,
        MinValueRole,
        MaxValueRole,
        CheckableRole,
    };

    explicit Editor(QWidget *parent = nullptr);

    virtual Value *value() const = 0;
    virtual void setProperty(Role role, Value *value) = 0;

Q_SIGNALS:
    void valueChanged();
};

class IntEditor : public Editor
{
    QLabel *m_label;
    QSlider *m_slider;

public:
    IntEditor() {
        auto layout = new QVBoxLayout;
        setLayout(layout);

        m_slider = new QSlider;
        m_slider->setOrientation(Qt::Horizontal);
        layout->addWidget(m_slider);
        connect(m_slider, &QSlider::valueChanged, this, &Editor::valueChanged);

        m_label = new QLabel;
        layout->addWidget(m_label);
    }

    virtual void setProperty(Role role, Value *value) override {
        switch (role) {
        case TitleRole:
            m_label->setText(value->getString());
            break;
        case MaxValueRole:
            m_slider->setMaximum(value->getInt());
            break;
        case MinValueRole:
            m_slider->setMinimum(value->getInt());
            break;
        default:
            throw;
        }
    }

    virtual Value *value() const override {
        return new IntValue(m_slider->value());
    }
};

class BoolEditor : public Editor
{
    QCheckBox *m_check;

public:
    BoolEditor() {
        auto layout = new QVBoxLayout;
        setLayout(layout);

        m_check = new QCheckBox;
        layout->addWidget(m_check);
        connect(m_check, &QCheckBox::stateChanged, this, &Editor::valueChanged);
    }

    virtual void setProperty(Role role, Value *value) override {
        switch (role) {
        case TitleRole:
            m_check->setText(value->getString());
            break;
        case CheckableRole:
            m_check->setCheckable(value->getBool());
            break;
        default:
            throw;
        }
    }

    virtual Value *value() const override {
        return new BoolValue(m_check->isChecked());
    }
};

class StringEditor : public Editor
{
    QLabel *m_label;
    QLineEdit *m_edit;

public:
    StringEditor() {
        auto layout = new QVBoxLayout;
        setLayout(layout);

        m_edit = new QLineEdit;
        layout->addWidget(m_edit);
        connect(m_edit, &QLineEdit::textEdited, this, &Editor::valueChanged);

        m_label = new QLabel;
        layout->addWidget(m_label);
    }

    virtual void setProperty(Role role, Value *value) override {
        switch (role) {
        case TitleRole:
            m_edit->setText(value->getString());
            break;
        default:
            throw;
        }
    }

    virtual Value *value() const override {
        return new StringValue(m_edit->text());
    }
};
