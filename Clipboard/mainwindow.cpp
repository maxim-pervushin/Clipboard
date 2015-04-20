#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QClipboard>
#include <QMimeData>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(QApplication::clipboard(), SIGNAL(dataChanged()), this, SLOT(clipboardDataChanged()));
}

MainWindow::~MainWindow()
{
    disconnect(QApplication::clipboard(), SIGNAL(dataChanged()));
    delete ui;
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
