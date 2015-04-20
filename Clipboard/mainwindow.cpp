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

QString MainWindow::clipboardFilePath()
{
    return QString("%1/%2.%3").arg(QDir::currentPath(), "saved", "clipboard");
}

void MainWindow::saveClipboard()
{
    const QClipboard *clipboard = QApplication::clipboard();
    const QMimeData *mimeData = clipboard->mimeData();

    QVariantMap map;
    foreach (QString format, mimeData->formats())
    {
        map.insert(format, QVariant(mimeData->data(format)));
    }

    QFile file(clipboardFilePath());
    if (!file.open(QIODevice::WriteOnly))
    {
        qDebug() << "Unable to open file for writing:" << clipboardFilePath();
        return;
    }

    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_5_4);
    out << map;
    file.flush();
    file.close();
}

void MainWindow::loadClipboard()
{
    QFile file(clipboardFilePath());
    if (!file.exists())
    {
        qDebug() << "File doesn't exists:" << clipboardFilePath();
        return;
    }

    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "Unable to open file for reading:" << clipboardFilePath();
        return;
    }

    QDataStream in(&file);
    QVariantMap map;
    in >> map;
    file.close();

    QMimeData *mimeData = new QMimeData;
    foreach (QString key, map.keys())
    {
        mimeData->setData(key, map.value(key).toByteArray());
        //qDebug() << key << QString("%1").arg(map.value(key).toString());
    }
    clipboard->clear();
    clipboard->setMimeData(mimeData);
}

void MainWindow::clipboardDataChanged()
{
    const QClipboard *clipboard = QApplication::clipboard();
    const QMimeData *mimeData = clipboard->mimeData();

    ui->listWidget->clear();
    ui->listWidget->addItems(mimeData->formats());
}
