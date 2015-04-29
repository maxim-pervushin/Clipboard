#include "clipboardmanager.h"
#include <QApplication>
#include <QClipboard>
#include <QDate>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QMimeData>
#include <QSettings>

ClipboardManager::ClipboardManager(QObject *parent) : QObject(parent) {
    connect(&fileSystemWatcher, SIGNAL(directoryChanged(QString)), this, SLOT(directoryChanged(QString)));
    setFolder(folder());
    setAlias(alias());
}

ClipboardManager::~ClipboardManager() {}

void ClipboardManager::storeClipboard() {
    QString fileName = QString("%1 %2 %3.%4").arg(alias(),
                                               QDate::currentDate().toString("dd-MM-yyyy"),
                                               QTime::currentTime().toString("hh_mm_ss"),
                                               "clipboard");
    QString filePath = QString("%1/%2").arg(folder(), fileName);

    const QClipboard *clipboard = QApplication::clipboard();
    const QMimeData *mimeData = clipboard->mimeData();

    QVariantMap map;
    foreach (QString format, mimeData->formats()) {
        map.insert(format, QVariant(mimeData->data(format)));
    }

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << "Unable to open file for writing:" << filePath;
        return;
    }

    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_5_0);
    out << map;
    file.flush();
    file.close();
}

QStringList ClipboardManager::clipboards() {
    QDir dir(folder());
    return dir.entryList(QStringList("*.clipboard"), QDir::Files, QDir::Time);
}

void ClipboardManager::restoreClipboard(QString clipboard) {
    QString filePath = QString("%1/%2").arg(folder(), clipboard);
    QFile file(filePath);
    if (!file.exists()) {
        qDebug() << "File doesn't exists:" << filePath;
        return;
    }

    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Unable to open file for reading:" << filePath;
        return;
    }

    QDataStream in(&file);
    in.setVersion(QDataStream::Qt_5_0);
    QVariantMap map;
    in >> map;
    file.close();

    QMimeData *mimeData = new QMimeData;
    foreach (QString key, map.keys()) {
        QString originalKey = key;
        // Hack for Windows
        if (key.startsWith("application/x-qt-windows-mime;value=")) {
            key.replace("application/x-qt-windows-mime;value=\"", "");
            key.replace("\"", "");
        }
        mimeData->setData(key, map.value(originalKey).toByteArray());
    }
    QApplication::clipboard()->clear();
    QApplication::clipboard()->setMimeData(mimeData);
}

void ClipboardManager::setFolder(QString newFolder) {
    if (!fileSystemWatcher.removePath(folder())) {
        qDebug() << "Unable to remove path:" << folder();
    }

    QSettings settings;
    settings.setValue("folder", newFolder);
    settings.sync();

    if (!fileSystemWatcher.addPath(newFolder)) {
        qDebug() << "Unable to add path:" << newFolder;
    }

    directoryChanged(newFolder);
}

QString ClipboardManager::folder() {
    QSettings settings;
    QString value = settings.value("folder").value<QString>();
    if (value.trimmed().isEmpty()) {
        value = QDir::currentPath();
    }
    return value;
}

void ClipboardManager::setAlias(QString alias) {
    QSettings settings;
    settings.setValue("alias", alias);
    settings.sync();
}

QString ClipboardManager::alias() {
    QString userName = qgetenv("USER");
    if (userName.isEmpty()) {
        userName = qgetenv("USERNAME");
    }
    QSettings settings;
    QString value = settings.value("alias").value<QString>();
    if (value.trimmed().isEmpty()) {
        value = userName;
    }
    return value;
}

void ClipboardManager::directoryChanged(QString directory) {
    Q_UNUSED(directory);
    clipboardsListChanged();
}
