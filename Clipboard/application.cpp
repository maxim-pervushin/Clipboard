#include "application.h"
#include <QDebug>

Application::Application(int& argc, char** argv) : QApplication(argc, argv) {
    setQuitOnLastWindowClosed(false);

    reloadMenu();

    mSystemTrayIcon = new QSystemTrayIcon(QIcon(":/img/clipboard-icon-128.png"), this);
    mSystemTrayIcon->setContextMenu(&mMenu);
    mSystemTrayIcon->show();
    connect(mSystemTrayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(systemTrayIconActivated(QSystemTrayIcon::ActivationReason)));
    connect(&mClipboardManager, SIGNAL(clipboardsListChanged()), this, SLOT(clipboardsListChanged()));
}

ClipboardManager *Application::getClipboardManager() {
    return &mClipboardManager;
}

void Application::reloadMenu() {
    mMenu.clear();
    foreach (QString clipboardName, mClipboardManager.clipboards()) {
        mMenu.addAction(clipboardName, this, SLOT(restoreClipboard()));
    }

    mMenu.addSeparator();
    mMenu.addAction("Store Clipboard", &mClipboardManager, SLOT(storeClipboard()));
    mMenu.addAction("Show Window", &mMainWindow, SLOT(show()));
    mMenu.addSeparator();
    mMenu.addAction("Quit", this, SLOT(quit()));
}

void Application::systemTrayIconActivated(QSystemTrayIcon::ActivationReason activationReason) {
    switch (activationReason) {
        default:
        case QSystemTrayIcon::Unknown:
        case QSystemTrayIcon::Context:
        case QSystemTrayIcon::DoubleClick:
            break;
        case QSystemTrayIcon::Trigger:
                restoreLastClipboard();
            break;
        case QSystemTrayIcon::MiddleClick:
                mClipboardManager.storeClipboard();
            break;
    }
}

void Application::restoreLastClipboard() {
    QStringList clipboards = mClipboardManager.clipboards();
    if (clipboards.count() > 0) {
        mClipboardManager.restoreClipboard(clipboards.at(0));
    }
}

void Application::restoreClipboard() {
    mClipboardManager.restoreClipboard(((QAction *)sender())->text());
}

void Application::clipboardsListChanged() {
    mSystemTrayIcon->showMessage(QCoreApplication::applicationName(), "New Clipboard Available!", QSystemTrayIcon::Information, 500);
    reloadMenu();
}
