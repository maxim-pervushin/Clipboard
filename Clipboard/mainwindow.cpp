#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "application.h"
#include "clipboardmanager.h"
#include <QApplication>
#include <QClipboard>
#include <QMimeData>
#include <QDebug>
#include <QDir>
#include <QFileDialog>
#include <QDate>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    ui->folderEdit->setText(getClipboardManager()->folder());
    ui->aliasEdit->setText(getClipboardManager()->alias());

    connect(ui->folderEdit, SIGNAL(textEdited(QString)), this, SLOT(folderChanged()));
    connect(ui->aliasEdit, SIGNAL(textEdited(QString)), this, SLOT(aliasChanged()));
    connect(ui->selectFolderButton, SIGNAL(clicked()), this, SLOT(selectFolder()));
    connect(ui->saveClipboardButton, SIGNAL(clicked()), this, SLOT(saveClipboard()));
    connect(ui->contentsListWidget, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(contentsListWidgetDoubleClicked(QModelIndex)));
}

MainWindow::~MainWindow() {
    delete ui;
}

ClipboardManager *MainWindow::getClipboardManager() {
    return ((Application *)QApplication::instance())->getClipboardManager();
}

void MainWindow::folderChanged() {
    getClipboardManager()->setFolder(ui->folderEdit->text());
}

void MainWindow::aliasChanged() {
    getClipboardManager()->setAlias(ui->aliasEdit->text());
}

void MainWindow::selectFolder() {
    QFileDialog dialog;
    dialog.setFileMode(QFileDialog::Directory);
    dialog.setOption(QFileDialog::ShowDirsOnly);
    if (dialog.exec()) {
        ui->folderEdit->setText(dialog.selectedFiles().at(0));
    }
}

void MainWindow::saveClipboard() {
    getClipboardManager()->storeClipboard();
}

void MainWindow::clipboardDataChanged() {
    if (!ui->showClipboardContentCheckBox->isChecked()) {
        return;
    }

    const QMimeData *mimeData = QApplication::clipboard()->mimeData();
    ui->clipboardContentTableWidget->clear();
    ui->clipboardContentTableWidget->setRowCount(mimeData->formats().count());
    ui->clipboardContentTableWidget->setColumnCount(3);
    QStringList headers;
    headers << "Mimetype" << "Size" << "Content";
    ui->clipboardContentTableWidget->setHorizontalHeaderLabels(headers);
    int index = 0;
    foreach (QString format, mimeData->formats()) {
        QTableWidgetItem *item1 = new QTableWidgetItem(format);
        QByteArray data = mimeData->data(format);
        QTableWidgetItem *item2 = new QTableWidgetItem(QString("%1").arg(data.size()));
        QTableWidgetItem *item3 = new QTableWidgetItem(QString(data));
        ui->clipboardContentTableWidget->setItem(index, 0, item1);
        ui->clipboardContentTableWidget->setItem(index, 1, item2);
        ui->clipboardContentTableWidget->setItem(index, 2, item3);
        index++;
    }
}

void MainWindow::contentsListWidgetDoubleClicked(QModelIndex modelIndex) {
    Q_UNUSED(modelIndex)
    QString filePath = QString("%1/%2").arg(ui->folderEdit->text(), ui->contentsListWidget->currentItem()->text());
}
