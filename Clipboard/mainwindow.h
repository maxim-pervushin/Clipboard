#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileSystemWatcher>
#include <QModelIndex>

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
    QFileSystemWatcher fileSystemWatcher;
    void setFolder(QString folder);
    void loadClipboard(QString filePath);

private slots:
    void selectFolder();
    void saveClipboard();
    void reloadDirectoryContents(QString directory);
    void clipboardDataChanged();
    void contentsListWidgetDoubleClicked(QModelIndex modelIndex);
};

#endif // MAINWINDOW_H
