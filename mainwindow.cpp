#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "listusbdrive.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    getUsbDrives();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::getUsbDrives()
{
    QStringList usbDrives = ListUsbDrive();

    ui->comboBox->clear();
    foreach(const QString &usbDrive, usbDrives) {
        ui->comboBox->addItem(usbDrive);
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    getUsbDrives();
}
