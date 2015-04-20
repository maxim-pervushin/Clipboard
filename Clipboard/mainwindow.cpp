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
    connect(ui->storeClipboardButton, SIGNAL(clicked()), this, SLOT(storeClipboard()));
    connect(ui->restoreClipboardButton, SIGNAL(clicked()), this, SLOT(restoreClipboard()));

    qDebug() << QDir::currentPath();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::storeFormat(int index, QString format, QByteArray data)
{
    QString path = QString("%1/%2/%3").arg(QDir::currentPath(), "clpbrd", QString::number(index));
    QDir dir;
    if (!dir.mkpath(path))
    {
        qDebug() << "Unable to create: " << path;
    }
    else
    {
        qDebug() << "Created: " << dir.absolutePath();
    }

    QFile descriptionFile(QString("%1/dscr").arg(path));
    descriptionFile.open(QIODevice::WriteOnly);
    descriptionFile.write(format.toUtf8());
    descriptionFile.close();

    QFile dataFile(QString("%1/dt").arg(path));
    dataFile.open(QIODevice::WriteOnly);
    dataFile.write(data);
    dataFile.close();
}

void MainWindow::storeClipboard()
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
        storeFormat(index++, format, mimeData->data(format));
    }
}

void MainWindow::restoreClipboard()
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

        qDebug() << path;

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

    qDebug() << "Restore";
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
