#ifndef APPLICATION_H
#define APPLICATION_H

#include <QApplication>
#include <QMenu>
#include <QSystemTrayIcon>
#include "clipboardmanager.h"
#include "mainwindow.h"

class Application : public QApplication
{
    Q_OBJECT

private:
    ClipboardManager mClipboardManager;
    MainWindow mMainWindow;
    QMenu mMenu;
    QSystemTrayIcon *mSystemTrayIcon;

    void reloadMenu();

public:
    explicit Application(int& argc, char** argv);
    ClipboardManager *getClipboardManager();

private slots:
    void systemTrayIconActivated(QSystemTrayIcon::ActivationReason activationReason);
    void restoreLastClipboard();
    void restoreClipboard();
    void clipboardsListChanged();
};

#endif // APPLICATION_H
