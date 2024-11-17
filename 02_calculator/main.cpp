#include <QApplication>
#include <QMainWindow>
#include <QDebug>
#include <QGridLayout>
#include <QLabel>

// QObject -> QWidget -> QLabel, QMainWindow, QPushButton, QComboBox, ...

class Label : public QLabel
{
public:
    explicit Label(QWidget *parent = nullptr)
        : QLabel(parent)
    {
        setAlignment(Qt::AlignCenter);
        setStyleSheet(R"(
                      QLabel {
                      margin: 20px;
                      font-size: 40px;
                      }
                      )");
    }
};

class MainWindow : public QMainWindow
{
public:
    explicit MainWindow(QWidget *parent = nullptr)
        : QMainWindow(parent)
    {
        setWindowTitle("宝宝乘法表");

        setCentralWidget(new QWidget);
        QGridLayout *layout = new QGridLayout;
        centralWidget()->setLayout(layout);

        for (int i = 1; i <= 9; ++i) {
            for (int j = 1; j <= 9; ++j) {
                Label *label = new Label;
                label->setText(QString("%1*%2=%3").arg(i).arg(j).arg(i * j));
                layout->addWidget(label, i - 1, j - 1);
            }
        }
    }
};

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    MainWindow win;
    win.show();
    return app.exec();
}
