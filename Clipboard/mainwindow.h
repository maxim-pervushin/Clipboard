#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QModelIndex>
#include "clipboardmanager.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    ClipboardManager *getClipboardManager();

private slots:
    void folderChanged();
    void aliasChanged();

    void selectFolder();
    void saveClipboard();
    void clipboardDataChanged();
    void contentsListWidgetDoubleClicked(QModelIndex modelIndex);
};

#endif // MAINWINDOW_H
