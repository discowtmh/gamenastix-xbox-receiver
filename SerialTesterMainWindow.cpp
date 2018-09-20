#include <QSerialPortInfo>

#include "SerialTesterMainWindow.h"
#include "ui_SerialTesterMainWindow.h"
#include <iostream>


SerialTesterMainWindow::SerialTesterMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SerialTesterMainWindow)
{
    ui->setupUi(this);
}

SerialTesterMainWindow::~SerialTesterMainWindow()
{
    delete ui;
}



void SerialTesterMainWindow::updateAvailablePortsComboBox(QComboBox &availablePortsComboBox)
{
    availablePortsComboBox.clear();

    for (QSerialPortInfo port : QSerialPortInfo::availablePorts())
    {
        availablePortsComboBox.addItem(port.portName());
    }
}

QSerialPort::BaudRate SerialTesterMainWindow::getBaudRate(int index)
{
    switch (index)
    {
        case 0:
            return QSerialPort::Baud9600;
        case 1:
            return QSerialPort::Baud115200;
        default:
            return QSerialPort::Baud9600;
    }
}

void SerialTesterMainWindow::errorOccurred(QSerialPort::SerialPortError error)
{
    std::cerr << error << std::endl;
    on_connectButton_clicked();
}

void SerialTesterMainWindow::disconnectSerialPort(std::unique_ptr<QSerialPort> &serial)
{
    serial.reset();
}

void SerialTesterMainWindow::connectSerialPort(std::unique_ptr<QSerialPort> &serialPortHandle)
{
    QString port = ui->availablePortsComboBox->currentText();
    QSerialPort::BaudRate baudRate = getBaudRate(ui->baudRateComboBox->currentIndex());
    connectSerialPort(serialPortHandle, port, baudRate);
}

void SerialTesterMainWindow::connectSerialPort(
    std::unique_ptr<QSerialPort> &serial,
    QString port,
    QSerialPort::BaudRate baudRate)
{
    serial = std::make_unique<QSerialPort>(this);

    serial->setPortName(port);
    serial->setBaudRate(baudRate);
    serial->open(QIODevice::ReadWrite);

    connect(serial.get(), &QSerialPort::readyRead, this, &SerialTesterMainWindow::readData);
}

void SerialTesterMainWindow::readData()
{
    auto incomingData = serialPortHandle->readAll();
    ui->receivingTextEdit->append(incomingData);
}

void SerialTesterMainWindow::resetComPort(
    std::unique_ptr<QSerialPort> &serial)
{
    if (serial)
    {
        disconnectSerialPort(serial);
    }
    else
    {
        connectSerialPort(serial);
    }
}


void SerialTesterMainWindow::on_refreshAvailablePortsButton_clicked()
{
    updateAvailablePortsComboBox(*ui->availablePortsComboBox);
}

void SerialTesterMainWindow::on_connectButton_clicked()
{
    connectSerialPort(serialPortHandle);
}

void SerialTesterMainWindow::on_resetReceivingButton_clicked()
{
    ui->receivingTextEdit->clear();
}

void SerialTesterMainWindow::sendBufferContentWithReset()
{
    auto data = ui->sendingTextEdit->toPlainText();
    ui->sendingTextEdit->clear();
    serialPortHandle->write(reinterpret_cast<const char*>(data.data()), data.size());
}

void SerialTesterMainWindow::on_sendButton_clicked()
{
    sendBufferContentWithReset();
}
