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
    this->device = new QSerialPort(this);
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
        ui->comboBoxDevices->addItem(devices.at(i).portName() + ":\t" + devices.at(i).description());

        if (devices.at(i).portName() == ui->comboBoxDevices->currentText().split(":\t").first()) {
            if (devices.at(i).description() == ui->comboBoxDevices->currentText().split(":\t").last()) {
                qDebug() << "Known device";
            } else {
                //comment
            }
        }
        qDebug() << "number of devices" + (QString) ui->comboBoxDevices->count();
    }
    qDebug() << "Search completed. " << devices.count() << " devices found";
}

void MainWindow::addToLogs(QString message)
{
    QString currentDateTime = QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss");
    ui->textEditLog->append(currentDateTime + "\t" + message);
}

void MainWindow::on_pushButtonConnect_clicked()
{
    if(ui->comboBoxDevices->count() == 0) {
        this->addToLogs("No devices detected!");
        return;
    }

    QString portName = ui->comboBoxDevices->currentText().split(":\t").first();
    this->device->setPortName(portName);

    // OPEN AND CONFIGURE SERIAL PORT:
    if(!device->isOpen()) {
        if(device->open(QSerialPort::ReadWrite)) {
            this->device->setBaudRate(QSerialPort::Baud9600);
            this->device->setDataBits(QSerialPort::Data8);
            this->device->setParity(QSerialPort::NoParity);
            this->device->setStopBits(QSerialPort::OneStop);
            this->device->setFlowControl(QSerialPort::NoFlowControl);

            this->addToLogs("Serial Port open.");
        } else {
            this->addToLogs("Failed to open Serial Port!");
        }/**/
    } else {
        this->addToLogs("Serial Port already open!");
    }
}

void MainWindow::on_pushButtonDisconnect_clicked()
{
    if (this->device->isOpen()) {
        this->device->close();
        this->addToLogs("Connection closed.");
        qDebug() << "Is serial port still open? " << this->device->isOpen();
    } else {
        this->addToLogs("No open connection available!");
        return;
    }
}
