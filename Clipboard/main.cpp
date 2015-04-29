#include "mainwindow.h"
#include "application.h"
#include <QSystemTrayIcon>

int main(int argc, char *argv[]) {
    QCoreApplication::setOrganizationName("Maxim Pervushin");
    QCoreApplication::setOrganizationDomain("clipboardsaver.com");
    QCoreApplication::setApplicationName("Clipboard Saver");

    Application application(argc, argv);
    return application.exec();
}
