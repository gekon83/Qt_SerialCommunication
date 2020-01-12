#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QList>
#include <QSerialPortInfo>
#include <QDebug>
#include <QDateTime>
#include <QString>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButtonSearch_clicked()
{
    qDebug() << "Searching devices...";
    QList<QSerialPortInfo> devices;
    devices = QSerialPortInfo::availablePorts();
    for(int i = 0; i < devices.count(); i++) {
        qDebug() << devices.at(i).portName() << devices.at(i).description();
        addToLogs("Found device: " + devices.at(i).portName() + ": " + devices.at(i).description());
        ui->comboBoxDevices->addItem(devices.at(i).portName() + ": " + devices.at(i).description());
        if (ui->comboBoxDevices->itemData(ui->comboBoxDevices->currentIndex()) == (devices.at(i).portName() + ": " + devices.at(i).description()))
            qDebug() << "Known device";
        qDebug() << "number of devices" + (QString) ui->comboBoxDevices->count();
    }
    qDebug() << "Search completed. " << devices.count() << " devices found";
}

void MainWindow::addToLogs(QString message)
{
    QString currentDateTime = QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss");
    ui->textEditLog->append(currentDateTime + "\t" + message);
}
