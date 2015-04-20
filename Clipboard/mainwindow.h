#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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
    QClipboard *clipboard;

private slots:
    void saveClipboard();
    void loadClipboard();
    void clipboardDataChanged();
};

#endif // MAINWINDOW_H
