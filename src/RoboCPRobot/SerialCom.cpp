#include "SerialCom.h"


//temporarily constructor isn't empty.
SerialCom :: SerialCom(char *PortName, int BoudRate)
{
	Write_Settings_Port(QString(PortName),BoudRate, QSerialPort::Data8, QSerialPort::NoParity, QSerialPort::OneStop, QSerialPort::NoFlowControl);

	ConnectPort();
}

SerialCom::~SerialCom()
{
	ThisPort.close();
}

int SerialCom::GetOutSize()
{
	return OutSize;
}

//Connecting(check all settings)
void SerialCom :: ConnectPort(){
  ThisPort.setPortName(SettingsPort.name);
  if (ThisPort.open(QIODevice::ReadWrite)) 
  {
    if (ThisPort.setBaudRate(SettingsPort.baudRate)
    && ThisPort.setDataBits(SettingsPort.dataBits)
    && ThisPort.setParity(SettingsPort.parity)
    && ThisPort.setStopBits(SettingsPort.stopBits)
    && ThisPort.setFlowControl(SettingsPort.flowControl))
    if (!ThisPort.isOpen())
    {
	  ThisPort.close();
	  #ifdef ENABLE_LOGGING
      RAW_LOG (INFO, "SerialCom(%s):" + thisPort.errorString(),PortName);
      #endif
	  //exit(1);?
	}
  }
  else 
  {
	ThisPort.close();
    #ifdef ENABLE_LOGGING
    RAW_LOG (INFO, "SerialCom(%s):" + thisPort.errorString(),PortName);
    #endif
	//exit(1);?
  }
}

//Write data to port.
void SerialCom :: Write(char *Data, int DataSize)
{
  if (ThisPort.isOpen())
  {
	  ThisPort.write(Data, DataSize);
	  ThisPort.waitForBytesWritten(0);//number of msecs of sleeep.
  }
}

//Read data from port.
char* SerialCom :: Read(void)
{
  char *temp = "";
  if(ThisPort.waitForReadyRead(1))
  {
    QByteArray arr = ThisPort.readAll();
    int number = arr.count();
    const char *data = arr.constData();
	temp = new char[number];
	for(int i = 0; i < number; i++)
	{
	  temp[i] = data[i];
	}
	OutSize = number;
  }
  return temp;
}

//Write settings to setting's structure.
void SerialCom :: Write_Settings_Port(QString name, int baudrate,int DataBits,
                        int Parity,int StopBits, int FlowControl){
    SettingsPort.name = name;
    SettingsPort.baudRate = (QSerialPort::BaudRate) baudrate;
    SettingsPort.dataBits = (QSerialPort::DataBits) DataBits;
    SettingsPort.parity = (QSerialPort::Parity) Parity;
    SettingsPort.stopBits = (QSerialPort::StopBits) StopBits;
    SettingsPort.flowControl = (QSerialPort::FlowControl) FlowControl;
}