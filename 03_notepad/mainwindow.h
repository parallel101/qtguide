#pragma once

#include <QMainWindow>
#include <QTextEdit>

class MainWindow : public QMainWindow
{
    Q_OBJECT

    QTextEdit *m_edit;
    QString m_currentPath;
    bool m_hasUnsavedModify = false;

public:
    explicit MainWindow(QWidget *parent = nullptr);

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    bool checkUnsavedModify();
    void openFile(QString path);
    void saveFile(QString path);

private Q_SLOTS:
    void on_edit_textChanged();
    void on_open_triggered();
    void on_save_triggered();
    void on_saveAs_triggered();
    void on_autoNewLine_triggered(bool checked);
};
