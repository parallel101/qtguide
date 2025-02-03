#include <QApplication>
#include <QMainWindow>
#include <QDebug>
#include <QPushButton>
#include <QLineEdit>
#include <QIntValidator>
#include <QGridLayout>
#include <QtMath>

class PushButton : public QPushButton
{
public:
    PushButton(QWidget *parent = nullptr)
        : QPushButton(parent)
    {
        setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    }
};

class MainWindow : public QMainWindow
{
    QLineEdit *m_line;

    enum Operator {
        None,
        Plus,
        Minus,
        Multiply,
        Divide,
        Modular,
        Sqrt,
    };
    double m_lastValue = 0;
    Operator m_lastOperator = None;
    double m_memoryValue = 0;
    double m_grandTotal = 0;
    bool m_needClearInput = false;

public:
    explicit MainWindow(QWidget *parent = nullptr)
        : QMainWindow(parent)
    {
        setWindowTitle("宝宝计算器");

        setCentralWidget(new QWidget);
        QGridLayout *layout = new QGridLayout;
        centralWidget()->setLayout(layout);

        m_line = new QLineEdit;
        m_line->setText("0");
        m_line->setAlignment(Qt::AlignRight);
        m_line->setValidator(new QDoubleValidator);
        layout->addWidget(m_line, 0, 0, 1, 6);

        {
            PushButton *button = new PushButton;
            button->setText("AC");
            layout->addWidget(button, 2, 0);

            connect(button, &PushButton::clicked, this, &MainWindow::pressAllClear);
        }

        {
            PushButton *button = new PushButton;
            button->setText("+/-");
            layout->addWidget(button, 3, 0);

            connect(button, &PushButton::clicked, this, &MainWindow::pressSign);
        }

        {
            PushButton *button = new PushButton;
            button->setText(">");
            layout->addWidget(button, 4, 0);

            connect(button, &PushButton::clicked, this, &MainWindow::pressBackspace);
        }

        {
            PushButton *button = new PushButton;
            button->setText("C");
            layout->addWidget(button, 5, 0);

            connect(button, &PushButton::clicked, this, &MainWindow::pressClear);
        }

        {
            PushButton *button = new PushButton;
            button->setText("MU");
            layout->addWidget(button, 1, 0);

            connect(button, &PushButton::clicked, this, &MainWindow::pressMemoryUpdate);
        }

        {
            PushButton *button = new PushButton;
            button->setText("M+");
            layout->addWidget(button, 1, 1);

            connect(button, &PushButton::clicked, this, &MainWindow::pressMemoryPlus);
        }

        {
            PushButton *button = new PushButton;
            button->setText("M-");
            layout->addWidget(button, 1, 2);

            connect(button, &PushButton::clicked, this, &MainWindow::pressMemoryMinus);
        }

        {
            PushButton *button = new PushButton;
            button->setText("MR");
            layout->addWidget(button, 1, 3);

            connect(button, &PushButton::clicked, this, &MainWindow::pressMemoryRead);
        }

        {
            PushButton *button = new PushButton;
            button->setText("MC");
            layout->addWidget(button, 1, 4);

            connect(button, &PushButton::clicked, this, &MainWindow::pressMemoryClear);
        }

        {
            PushButton *button = new PushButton;
            button->setText("GT");
            layout->addWidget(button, 1, 5);

            connect(button, &PushButton::clicked, this, &MainWindow::pressGrandTotal);
        }

        for (int i = 1; i <= 9; ++i) {
            PushButton *button = new PushButton;
            button->setText(QString::number(i));
            layout->addWidget(button, 2 + (i - 1) / 3, 1 + (i - 1) % 3);

            connect(button, &PushButton::clicked, this, [this, i] {
                pressNumber(i);
            });
        }

        {
            PushButton *button = new PushButton;
            button->setText("0");
            layout->addWidget(button, 5, 1);

            connect(button, &PushButton::clicked, this, [this] {
                pressNumber(0);
            });
        }

        {
            PushButton *button = new PushButton;
            button->setText("00");
            layout->addWidget(button, 5, 2);

            connect(button, &PushButton::clicked, this, [this] {
                pressNumber(0);
                pressNumber(0);
            });
        }

        {
            PushButton *button = new PushButton;
            button->setText(".");
            layout->addWidget(button, 5, 3);

            connect(button, &PushButton::clicked, this, &MainWindow::pressDot);
        }

        QList<QPair<Operator, QString>> operators = {
            {Modular, "%"},
            {Sqrt, "sqrt"},
            {Multiply, "*"},
            {Divide, "/"},
            {Plus, "+"},
            {Minus, "-"},
        };

        for (int i = 0; i < operators.size(); ++i) {
            Operator op = operators[i].first;
            QString name = operators[i].second;

            PushButton *button = new PushButton;
            button->setText(name);
            layout->addWidget(button, 2 + (i / 2), 4 + (i % 2));

            connect(button, &PushButton::clicked, this, [this, op] {
                pressOperator(op);
            });
        }

        {
            PushButton *button = new PushButton;
            button->setText("=");
            layout->addWidget(button, 5, 4, 1, 2);

            connect(button, &PushButton::clicked, this, &MainWindow::pressEqual);
        }
    }

    void setLineText(QString text) {
        int pos;
        if (m_line->validator()->validate(text, pos) != QValidator::Invalid) {
            while (text.size() >= 2 && text[0] == '0' && (text[1] >= '0' && text[1] <= '9')) {
                text.remove(0, 1);
            }
            if (text.isEmpty()) {
                text = "0";
            }
            m_line->setText(text);
        }
    }

    void pressAllClear() {
        pressMemoryClear();
        m_lastOperator = None;
        m_lastValue = 0;
        pressClear();
    }

    void pressClear() {
        setLineText("0");
        m_needClearInput = false;
    }

    void pressMemoryUpdate() {
        double value = m_line->text().toDouble();
        m_memoryValue = value;
    }

    void pressMemoryPlus() {
        double value = m_line->text().toDouble();
        m_memoryValue += value;
    }

    void pressMemoryMinus() {
        double value = m_line->text().toDouble();
        m_memoryValue -= value;
    }

    void pressMemoryClear() {
        m_memoryValue = 0;
    }

    void pressMemoryRead() {
        setLineText(QString::number(m_memoryValue, 'g', 10));
    }

    void pressGrandTotal() {
        setLineText(QString::number(m_grandTotal, 'g', 10));
        m_grandTotal = 0;
    }

    void pressSign() {
        if (m_needClearInput) {
            setLineText("0");
            m_needClearInput = false;
        }
        QString text = m_line->text();
        if (!text.isEmpty()) {
            if (text.startsWith("-")) {
                text.remove(0, 1);
            } else {
                text = "-" + text;
            }
        }
        setLineText(text);
    }

    void pressDot() {
        if (m_needClearInput) {
            setLineText("0");
            m_needClearInput = false;
        }
        QString text = m_line->text();
        text += ".";
        setLineText(text);
    }

    void pressNumber(int i) {
        if (m_needClearInput) {
            setLineText("0");
            m_needClearInput = false;
        }
        QString text = m_line->text();
        text += QString::number(i);
        setLineText(text);
    }

    void pressBackspace() {
        if (m_needClearInput) {
            setLineText("0");
            m_needClearInput = false;
        }
        QString text = m_line->text();
        if (!text.isEmpty()) {
            text.remove(text.size() - 1, 1);
        }
        setLineText(text);
    }

    void pressOperator(Operator op) {
        double value = m_line->text().toDouble();
        m_lastValue = value;
        m_lastOperator = op;
        m_needClearInput = true;

        switch (op) {
        case Sqrt:
            pressEqual();
            break;
        default:
            break;
        };
    }

    void pressEqual() {
        double value1 = m_lastValue;
        double value2 = m_line->text().toDouble();
        double value;

        switch (m_lastOperator) {
        case None: value = value2; break;
        case Plus: value = value1 + value2; break;
        case Minus: value = value1 - value2; break;
        case Multiply: value = value1 * value2; break;
        case Divide: value = value1 / value2; break;
        case Modular: value = std::fmod(value1, value2); break;
        case Sqrt: value = std::sqrt(value2); break;
        }

        m_grandTotal += value;
        m_lastOperator = None;
        setLineText(QString::number(value, 'g', 10));
        m_needClearInput = true;
    }
};

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    MainWindow win;
    win.show();
    return app.exec();
}
