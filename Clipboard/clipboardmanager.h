#ifndef CLIPBOARDMANAGER_H
#define CLIPBOARDMANAGER_H

#include <QObject>
#include <QFileSystemWatcher>

class ClipboardManager : public QObject
{
    Q_OBJECT

private:
    QFileSystemWatcher fileSystemWatcher;

public:
    explicit ClipboardManager(QObject *parent = 0);

    ~ClipboardManager();


    QStringList clipboards();

    void setFolder(QString newFolder);

    QString folder();

    void setAlias(QString alias);

    QString alias();

signals:
    void clipboardsListChanged();

private slots:
    void directoryChanged(QString directory);

public slots:
    void storeClipboard();
    void restoreClipboard(QString clipboard);
};

#endif // CLIPBOARDMANAGER_H
