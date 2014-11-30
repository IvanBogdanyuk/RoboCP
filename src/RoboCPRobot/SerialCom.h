#pragma once
#include <qserialport.h>

#ifdef ENABLE_LOGGING
#define GLOG_NO_ABBREVIATED_SEVERITIES
#include <glog/logging.h>
#include <glog/raw_logging.h>
#endif

#define READ_BUFF_SIZE 256
#define SERIAL_WRITE_WAIT_MS 5000

//Structure with port's settings.
struct Settings {
    QString name;
    qint32 baudRate;
    QSerialPort::DataBits dataBits;
    QSerialPort::Parity parity;
    QSerialPort::StopBits stopBits;
    QSerialPort::FlowControl flowControl;
};

class SerialCom 
{
private:

  Settings SettingsPort;//Structure with setting

  int OutSize;//Size of out data.

  QSerialPort ThisPort;//Main object in this class.

public:

  SerialCom(char *PortName, int BoudRate);

  ~SerialCom();

  //Connecting(check all settings)
  void ConnectPort();

  //Write settings to setting's structure.
  void Write_Settings_Port(QString name, int baudrate, int DataBits, int Parity, int StopBits, int FlowControl);

  //Read data from port.
  char *Read(void);

  //Write data to port.
  void Write(char *Data, int DataSize);

  int GetOutSize();
};

