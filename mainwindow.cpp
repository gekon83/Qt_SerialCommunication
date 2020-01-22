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
    qDebug() << "Search completed. " << devices.count() << " devices detected";

    for(int i = 0; i < devices.count(); i++) {
        // comparing new device to known devices in the combo box. Updating combo box only for new devices.
        if (ui->comboBoxDevices->findText(devices.at(i).portName() + ": " + devices.at(i).description()) == -1) {
            qDebug() << "new device detected: " << devices.at(i).portName() << devices.at(i).description();
            addToLogs("Found device: " + devices.at(i).portName() + ": " + devices.at(i).description());
            ui->comboBoxDevices->addItem(devices.at(i).portName() + ": " + devices.at(i).description());
        } else {
            qDebug() << "hey... I know this device and will not add it to my combolist";
            addToLogs("Found already known device: " + devices.at(i).portName() + ": " + devices.at(i).description());
        }
        qDebug() << "number of devices on the list" << ui->comboBoxDevices->count();
    }
    /*
    for (int i=0; i<ui->comboBoxDevices->count(); ++i) {
        qDebug() << "combo: " << ui->comboBoxDevices->itemText(i);
    }
    */
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
            // CONNECT:
            qDebug() << connect(this->device, SIGNAL(readyRead()), this, SLOT(readFromPort()));
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

void MainWindow::readFromPort() {
  while(this->device->canReadLine()) {
    QString line = this->device->readLine();
    qDebug() << "line: " + line;

    QString terminator = "\r";
    int pos = line.lastIndexOf(terminator);
    qDebug() << "line.left(pos): " + line.left(pos);

    this->addToLogs(line.left(pos));
  }

}

void MainWindow::sendMessageToDevice(QString message)
{
    if(this->device->isOpen() && this->device->isWritable()) {
        this->addToLogs("Sending information to device " + message);
        this->device->write(message.toStdString().c_str());
     } else {
        this->addToLogs("Cannot send information. Serial Port is closed!");
     }
}

void MainWindow::on_pushButton_LedOn_clicked()
{
    this->sendMessageToDevice("1");
}

void MainWindow::on_pushButton_LedOff_clicked()
{
    this->sendMessageToDevice("0");
}
