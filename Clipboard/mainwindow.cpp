#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QClipboard>
#include <QMimeData>
#include <QDebug>
#include <QDir>
#include <QFileDialog>
#include <QDate>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QString userName = qgetenv("USER");
    if (userName.isEmpty())
    {
        userName = qgetenv("USERNAME");
    }

    ui->aliasEdit->setText(userName);
    setFolder(QDir::currentPath());

    connect(QApplication::clipboard(), SIGNAL(dataChanged()), this, SLOT(clipboardDataChanged()));
    connect(ui->selectFolderButton, SIGNAL(clicked()), this, SLOT(selectFolder()));
    connect(ui->saveClipboardButton, SIGNAL(clicked()), this, SLOT(saveClipboard()));
    connect(ui->contentsListWidget, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(contentsListWidgetDoubleClicked(QModelIndex)));

    connect(&fileSystemWatcher, SIGNAL(directoryChanged(QString)), this, SLOT(reloadDirectoryContents(QString)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setFolder(QString folder)
{
    if (!fileSystemWatcher.removePath(ui->folderEdit->text()))
    {
        qDebug() << "Unable to remove path:" << ui->folderEdit->text();
    }

    ui->folderEdit->setText(folder);

    if (!fileSystemWatcher.addPath(folder))
    {
        qDebug() << "Unable to add path:" << folder;
    }

    reloadDirectoryContents(folder);
}

void MainWindow::selectFolder()
{
    QFileDialog dialog;
    dialog.setFileMode(QFileDialog::Directory);
    dialog.setOption(QFileDialog::ShowDirsOnly);
    if (dialog.exec())
    {
        setFolder(dialog.selectedFiles().at(0));
    }
}

void MainWindow::saveClipboard()
{
    QString fileName = QString("%1 %2 %3.%4").arg(ui->aliasEdit->text(),
                                               QDate::currentDate().toString("dd-MM-yyyy"),
                                               QTime::currentTime().toString("hh_mm_ss"),
                                               "clipboard");
    QString filePath = QString("%1/%2").arg(ui->folderEdit->text(), fileName);

    const QClipboard *clipboard = QApplication::clipboard();
    const QMimeData *mimeData = clipboard->mimeData();

    QVariantMap map;
    foreach (QString format, mimeData->formats())
    {
        map.insert(format, QVariant(mimeData->data(format)));
    }

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly))
    {
        qDebug() << "Unable to open file for writing:" << filePath;
        return;
    }

    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_5_0);
    out << map;
    file.flush();
    file.close();
}

void MainWindow::loadClipboard(QString filePath)
{
    QFile file(filePath);
    if (!file.exists())
    {
        qDebug() << "File doesn't exists:" << filePath;
        return;
    }

    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "Unable to open file for reading:" << filePath;
        return;
    }

    QDataStream in(&file);
    in.setVersion(QDataStream::Qt_5_0);
    QVariantMap map;
    in >> map;
    file.close();

    QMimeData *mimeData = new QMimeData;
    foreach (QString key, map.keys())
    {
        mimeData->setData(key, map.value(key).toByteArray());
    }
    QApplication::clipboard()->clear();
    QApplication::clipboard()->setMimeData(mimeData);
}

void MainWindow::reloadDirectoryContents(QString directory)
{
    ui->contentsListWidget->clear();
    QDir dir(directory);
    QStringList files = dir.entryList(QStringList("*.clipboard"), QDir::Files, QDir::Time);
    ui->contentsListWidget->addItems(files);

    qDebug() << directory;
}

void MainWindow::clipboardDataChanged()
{
    const QClipboard *clipboard = QApplication::clipboard();
    const QMimeData *mimeData = clipboard->mimeData();

    ui->listWidget->clear();
    ui->listWidget->addItems(mimeData->formats());
}

void MainWindow::contentsListWidgetDoubleClicked(QModelIndex modelIndex)
{
    QString filePath = QString("%1/%2").arg(ui->folderEdit->text(), ui->contentsListWidget->currentItem()->text());
    loadClipboard(filePath);
    //qDebug() << ui->folderEdit->text() << "/" << ui->contentsListWidget->currentItem()->text();
}
