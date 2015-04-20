#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QClipboard>
#include <QMimeData>
#include <QDebug>
#include <QDir>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(QApplication::clipboard(), SIGNAL(dataChanged()), this, SLOT(clipboardDataChanged()));
    connect(ui->saveClipboardButton, SIGNAL(clicked()), this, SLOT(saveClipboard()));
    connect(ui->loadClipboardButton, SIGNAL(clicked()), this, SLOT(loadClipboard()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::saveClipboard()
{
    QDir dir(QDir::currentPath() + "/clpbrd");
    if (dir.exists())
    {
        dir.removeRecursively();
    }

    const QClipboard *clipboard = QApplication::clipboard();
    const QMimeData *mimeData = clipboard->mimeData();

    int index = 0;
    foreach (QString format, mimeData->formats())
    {
        QString path = QString("%1/%2/%3").arg(QDir::currentPath(), "clpbrd", QString::number(index++));
        QDir dir;
        if (!dir.mkpath(path))
        {
            qDebug() << "Unable to create: " << path;
        }

        QFile descriptionFile(QString("%1/dscr").arg(path));
        descriptionFile.open(QIODevice::WriteOnly);
        descriptionFile.write(format.toUtf8());
        descriptionFile.close();

        QFile dataFile(QString("%1/dt").arg(path));
        dataFile.open(QIODevice::WriteOnly);
        dataFile.write(mimeData->data(format));
        dataFile.close();
    }
}

void MainWindow::loadClipboard()
{
    QDir dir(QDir::currentPath() + "/clpbrd");
    if (!dir.exists())
    {
        qDebug() << "Nothing to restore";
        return;
    }

    QMimeData *mimeData = new QMimeData;
    foreach (QString directory, dir.entryList(QDir::NoDotAndDotDot | QDir::NoSymLinks | QDir::Dirs))
    {
        QString path = QString("%1/%2").arg(dir.absolutePath(), directory);

        QFile descriptionFile(QString("%1/dscr").arg(path));
        descriptionFile.open(QIODevice::ReadOnly);
        QString format = QString(descriptionFile.readAll());
        descriptionFile.close();

        QFile dataFile(QString("%1/dt").arg(path));
        dataFile.open(QIODevice::ReadOnly);
        QByteArray data = dataFile.readAll();
        dataFile.close();

        mimeData->setData(format, data);
    }

    clipboard->clear();
    clipboard->setMimeData(mimeData);
}

void MainWindow::clipboardDataChanged()
{
    qDebug() << "Clipboard data changed";

    const QClipboard *clipboard = QApplication::clipboard();
    const QMimeData *mimeData = clipboard->mimeData();

    qDebug() << mimeData->formats();

    ui->listWidget->clear();
    ui->listWidget->addItems(mimeData->formats());
}
