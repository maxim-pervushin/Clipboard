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
    void storeFormat(int index, QString format, QByteArray data);

private slots:
    void storeClipboard();
    void restoreClipboard();
    void clipboardDataChanged();
};

#endif // MAINWINDOW_H
