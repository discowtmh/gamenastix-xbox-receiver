#ifndef SERIALTESTERMAINWINDOW_H
#define SERIALTESTERMAINWINDOW_H

#include <QComboBox>
#include <QMainWindow>
#include <QSerialPort>
#include <memory>

namespace Ui {
class SerialTesterMainWindow;
}

class SerialTesterMainWindow : public QMainWindow {
  Q_OBJECT

public:
  explicit SerialTesterMainWindow(QWidget *parent = nullptr);
  ~SerialTesterMainWindow();

private slots:
  void on_refreshAvailablePortsButton_clicked();

  void on_connectButton_clicked();

  void on_resetReceivingButton_clicked();

  void on_sendButton_clicked();

  void on_sendRequestModelMessageButton_clicked();

  void on_sendCalibrateNPoseMessageButton_clicked();

  void on_sendCalibrateSPoseMessageButton_clicked();

private:
  void connectSerialPort(std::unique_ptr<QSerialPort> &serial);
  void connectSerialPort(std::unique_ptr<QSerialPort> &serialPortHandle,
                         QString port,
                         QSerialPort::BaudRate baudRate);
  void disconnectSerialPort(std::unique_ptr<QSerialPort> &serialPortHandle);
  void updateAvailablePortsComboBox(QComboBox &comPortComboBox);
  QSerialPort::BaudRate getBaudRate(int index);
  void errorOccurred(QSerialPort::SerialPortError error);
  void readData();
  void resetComPort(std::unique_ptr<QSerialPort> &serialPortHandle);
  void sendBufferContentWithReset();

private:
  Ui::SerialTesterMainWindow *ui;
  std::unique_ptr<QSerialPort> serialPortHandle;
};

#endif // SERIALTESTERMAINWINDOW_H
