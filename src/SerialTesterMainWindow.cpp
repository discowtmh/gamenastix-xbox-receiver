#include <QSerialPortInfo>

#include "SerialTesterMainWindow.h"
#include "ui_SerialTesterMainWindow.h"
#include <SystemClock.h>
#include <protocol.h>
#include <iostream>

#include <deepModel/Treadmill.h>
#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>


SerialTesterMainWindow::SerialTesterMainWindow(QWidget *parent)
        : QMainWindow(parent)
        , ui(new Ui::SerialTesterMainWindow)
{
    ui->setupUi(this);
    updateAvailablePortsComboBox(*ui->availablePortsComboBox_FromSupervisor);
    updateAvailablePortsComboBox(*ui->availablePortsComboBox_ToXBoxPad);

    compass = std::make_unique<Compass>(*this->ui->compassLabel, *this->ui->compassStatus);
    compass->update(0, false);

    joystickPreview = std::make_unique<JoystickPreview>(*ui->joystickPreviewLeftCanvas,
                                                        *ui->joystickPreviewRightCanvas);
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
            return QSerialPort::Baud115200;
        default:
            return QSerialPort::Baud115200;
    }
}

void SerialTesterMainWindow::errorOccurred(QSerialPort::SerialPortError error)
{
    std::cerr << error << std::endl;
}

void SerialTesterMainWindow::disconnectSerialPort(std::unique_ptr<QSerialPort> &serial)
{
    serial.reset();
}

void SerialTesterMainWindow::connectSerialPort(std::unique_ptr<QSerialPort> &serialPortHandle,
                                               QComboBox &comboBoxPort,
                                               QComboBox &comboBoxBaud)
{
    QString port = comboBoxPort.currentText();
    QSerialPort::BaudRate baudRate = getBaudRate(comboBoxBaud.currentIndex());
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

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
////////////////
/// CUT HERE ///
////////////////

#define YAW_INDEX 0
#define PITCH_INDEX 1
#define ROLL_INDEX 2

namespace {

glm::vec3 getRotated(glm::vec3 normal, float pitch, float roll)
{
    return glm::rotate(glm::rotate(normal, roll, glm::vec3{0, 0, 1}), pitch, glm::vec3{1, 0, 0});
}

glm::vec3 getFootPosition(float yaw, glm::vec3 offset, glm::vec3 yawPitchRollUp, glm::vec3 yawPitchRollBottom)
{
    glm::vec3 pelvisToKneeNPose = {0, -45, 0};
    glm::vec3 kneeToFeetNPose = {0, -45, 0};

    return glm::rotate(
        offset
            + getRotated(pelvisToKneeNPose, yawPitchRollUp[PITCH_INDEX], yawPitchRollUp[ROLL_INDEX])
            + getRotated(kneeToFeetNPose, yawPitchRollBottom[PITCH_INDEX], yawPitchRollBottom[ROLL_INDEX]),
        yaw,
        glm::vec3{0, 1, 0});
}

glm::vec3 getFootPosition(float yaw, const std::vector<float> &offset, const std::vector<float> &yawPitchRollUp, const std::vector<float> &yawPitchRollBottom)
{
    return getFootPosition(yaw,
                           glm::vec3{offset[0], offset[1], offset[2]},
                           glm::vec3{yawPitchRollUp[0], yawPitchRollUp[1], yawPitchRollUp[2]},
                           glm::vec3{yawPitchRollBottom[0], yawPitchRollBottom[1], yawPitchRollBottom[2]});
}
}

void SerialTesterMainWindow::handleFrame(Message &message)
{
    static biomodel::deepModel::Model model = biomodel::deepModel::Treadmill().getModel();
    model.update(message);

    float modelYaw = model.get(Part::CRANE)[YAW_INDEX];

    glm::vec3 leftFootPosition = getFootPosition(modelYaw,
                                                 model.get(Part::LEFT_LEG_OFFSET),
                                                 model.get(Part::LEFT_LEG_UPPER),
                                                 model.get(Part::LEFT_LEG_LOWER));

    glm::vec3 rightFootPosition = getFootPosition(modelYaw,
                                                  model.get(Part::RIGHT_LEG_OFFSET),
                                                  model.get(Part::RIGHT_LEG_UPPER),
                                                  model.get(Part::RIGHT_LEG_LOWER));

    joystickPreview->update(leftFootPosition[0], leftFootPosition[1], rightFootPosition[0], rightFootPosition[1]);
    sendXBoxState(leftFootPosition, rightFootPosition);
}

void SerialTesterMainWindow::sendXBoxState(glm::vec3 leftFootPosition, glm::vec3 rightFootPosition)
{
    if (serialPortHandle_ToXBoxPad)
    {
        char buffer[256];
        int size = sprintf(buffer,
                           "%d %d %d %d %d %d\n",
                           static_cast<int>(leftFootPosition[0]),
                           static_cast<int>(leftFootPosition[1]),
                           static_cast<int>(leftFootPosition[2]),
                           static_cast<int>(rightFootPosition[0]),
                           static_cast<int>(rightFootPosition[1]),
                           static_cast<int>(rightFootPosition[2]));
        serialPortHandle_ToXBoxPad->write(QByteArray(static_cast<const char *>(buffer), size));
        std::cout << buffer << std::endl;
    }
}

////////////////
/// CUT HERE ///
////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void SerialTesterMainWindow::readData()
{
    auto incomingData = serialPortHandle_FromSupervisor->readAll();
    ui->receivingTextEdit->append(incomingData);
    Message message(incomingData.begin(), incomingData.end());
    switch (Matcher::match(message))
    {
        case MessageType::ResponseModel:
        {
            ui->responseModelMessage->setText(incomingData);
            ui->responseModelTimestamp->setText(QString::number(SystemClock::millis()));
            break;
        }
        case MessageType::Frame:
        {
            ui->frameMessage->setText(incomingData);
            ui->frameTimestamp->setText(QString::number(SystemClock::millis()));
            handleFrame(message);
            break;
        }
    }
}

void SerialTesterMainWindow::resetComPort(
    std::unique_ptr<QSerialPort> &serial,
    QComboBox &comboBoxPort,
    QComboBox &comboBoxBaud,
    QPushButton &button,
    QString connectText,
    QString disconnectText)
{
    if (serial)
    {
        disconnectSerialPort(serial);
        button.setText(connectText);
    }
    else
    {
        connectSerialPort(serial, comboBoxPort, comboBoxBaud);
        button.setText(disconnectText);
    }
}

void SerialTesterMainWindow::on_resetReceivingButton_clicked()
{
    ui->receivingTextEdit->clear();
}

void SerialTesterMainWindow::sendBufferContentWithReset()
{
    auto data = ui->sendingTextEdit->toPlainText();
    ui->sendingTextEdit->clear();
    serialPortHandle_FromSupervisor->write(reinterpret_cast<const char *>(data.data()), data.size());
}

void SerialTesterMainWindow::on_sendButton_clicked()
{
    sendBufferContentWithReset();
}

void SerialTesterMainWindow::on_sendRequestModelMessageButton_clicked()
{
    static const char *requestModelMessage = "R";
    serialPortHandle_FromSupervisor->write(requestModelMessage, 1);
}

void SerialTesterMainWindow::on_sendCalibrateNPoseMessageButton_clicked()
{
    static const char *calibrateNPoseMessage = "N";
    serialPortHandle_FromSupervisor->write(calibrateNPoseMessage, 1);
}

void SerialTesterMainWindow::on_sendCalibrateSPoseMessageButton_clicked()
{
    static const char *calibrateSPoseMessage = "S";
    serialPortHandle_FromSupervisor->write(calibrateSPoseMessage, 1);
}

void SerialTesterMainWindow::on_refreshAvailablePortsButton_FromSupervisor_clicked()
{
    updateAvailablePortsComboBox(*ui->availablePortsComboBox_FromSupervisor);
}

void SerialTesterMainWindow::on_refreshAvailablePortsButton_ToXBoxPad_clicked()
{
    updateAvailablePortsComboBox(*ui->availablePortsComboBox_ToXBoxPad);
}

void SerialTesterMainWindow::on_connectButton_FromSupervisor_clicked()
{
    resetComPort(serialPortHandle_FromSupervisor, *ui->availablePortsComboBox_FromSupervisor, *ui->baudRateComboBox_FromSupervisor, *ui->connectButton_FromSupervisor, "Connect Supervisor", "Disconnect Supervisor");
}

void SerialTesterMainWindow::on_connectButton_ToXBoxPad_clicked()
{
    resetComPort(serialPortHandle_ToXBoxPad, *ui->availablePortsComboBox_ToXBoxPad, *ui->baudRateComboBox_ToXBoxPad, *ui->connectButton_ToXBoxPad, "Connect XBoxPad", "Disconnect XBoxPad");
}
