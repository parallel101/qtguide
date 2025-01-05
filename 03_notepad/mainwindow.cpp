#include "mainwindow.h"
#include <QMenuBar>
#include <QTextEdit>
#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>
#include <QCloseEvent>
#include <QDebug>
#include <QFile>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    auto menuBar = new QMenuBar;
    setMenuBar(menuBar);

    QMenu *menu = menuBar->addMenu("文件");

    QAction *action = menu->addAction("打开");
    action->setObjectName("open");
    action = menu->addAction("保存");
    action->setObjectName("save");
    action = menu->addAction("另存为");
    action->setObjectName("saveAs");

    menu = menuBar->addMenu("编辑");
    action = menu->addAction("自动换行");
    action->setObjectName("autoNewLine");
    action->setCheckable(true);

    m_edit = new QTextEdit;
    m_edit->setObjectName("edit");
    setCentralWidget(m_edit);

    QMetaObject::connectSlotsByName(this);

    QSettings settings("Archibate", "03_notepad");
    bool enableAutoNewLine = settings.value("enableAutoNewLine", true).toBool();
    action->setChecked(enableAutoNewLine);
    on_autoNewLine_triggered(enableAutoNewLine);
}

void MainWindow::on_edit_textChanged() {
    m_hasUnsavedModify = true;
}

bool MainWindow::checkUnsavedModify() {
    if (m_hasUnsavedModify) {
        auto btn = QMessageBox::question(this, "notepad", "您有未保存的修改，是否保存？", QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        switch (btn) {
            case QMessageBox::Save:
                on_save_triggered();
                break;
            case QMessageBox::Discard:
                break;
            case QMessageBox::Cancel:
                return true;
            default:
                break;
        }
    }
    return false;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (checkUnsavedModify()) {
        event->ignore();
    } else {
        QMainWindow::closeEvent(event);
    }
}

void MainWindow::openFile(QString path) {
    QFile file(path);
    if (!file.open(QFile::ReadOnly)) {
        qWarning() << "file open failed";
        return;
    }

    QString text = QString::fromUtf8(file.readAll());
    m_edit->setPlainText(text);
    m_currentPath = path;
    m_hasUnsavedModify = false;
}

void MainWindow::saveFile(QString path) {
    QFile file(path);
    if (!file.open(QFile::WriteOnly)) {
        qWarning() << "file save failed";
        return;
    }

    QString text = m_edit->toPlainText();
    file.write(text.toUtf8());
    m_currentPath = path;
    m_hasUnsavedModify = false;
}

void MainWindow::on_open_triggered() {
    if (checkUnsavedModify()) {
        return;
    }

    QString path = QFileDialog::getOpenFileName(this, "选择文件打开", "", "文本文件 (*.txt *.log);;所有文件 (*)");
    if (path.isEmpty()) {
        return;
    }

    openFile(path);
}

void MainWindow::on_save_triggered() {
    if (m_currentPath.isEmpty()) {
        on_saveAs_triggered();

    } else {
        saveFile(m_currentPath);
    }
}

void MainWindow::on_saveAs_triggered() {
    QString path = QFileDialog::getSaveFileName(this, "选择保存的文件名", "", "文本文件 (*.txt *.log);;所有文件 (*)");
    if (path.isEmpty()) {
        return;
    }

    saveFile(path);
}

void MainWindow::on_autoNewLine_triggered(bool checked) {
    if (checked) {
        m_edit->setLineWrapMode(QTextEdit::WidgetWidth);
    } else {
        m_edit->setLineWrapMode(QTextEdit::NoWrap);
    }
    QSettings settings("Archibate", "03_notepad");
    settings.setValue("enableAutoNewLine", checked);
}
