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

//
//SerialCom::SerialCom(char *PortName, int BaudRate)
//{
//  TCHAR *pcCommPort = TEXT(PortName);
//  out = new char[READ_BUFF_SIZE];
//  outSize = 0;
//  #ifdef ENABLE_LOGGING
//  RAW_LOG (INFO, "SerialCom(%s): connecting...",PortName);
//  #endif
//  hComm = CreateFile(pcCommPort, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
//  if (hComm == INVALID_HANDLE_VALUE){
//    int err = GetLastError();
//    if (err == ERROR_FILE_NOT_FOUND){
//      #ifdef ENABLE_LOGGING
//      RAW_LOG (INFO, "SerialCom(%s): can't connect - this com port or device doesn't exist",PortName);
//      #endif
//    }else{
//      if (err = ERROR_ACCESS_DENIED){
//        #ifdef ENABLE_LOGGING
//        RAW_LOG (INFO, "SerialCom(%s): can't connect - this com port already in use",PortName);
//        #endif
//      }else{
//        #ifdef ENABLE_LOGGING
//        RAW_LOG (INFO, "SerialCom(%s): can't connect - unknown error",PortName);
//        #endif
//      }
//    }
//  }else{
//    #ifdef ENABLE_LOGGING
//    RAW_LOG (INFO, "SerialCom(%s): connected!",PortName);
//    #endif
//  }
//  if (!SetupComm(hComm,1024,1024)){
//    #ifdef ENABLE_LOGGING
//    RAW_LOG (INFO, "SerialCom(%s): can't setup i/o buffers",PortName);
//    #endif
//  }
//  COMMCONFIG conf;
//  conf.dcb.DCBlength = sizeof(DCB);
//  GetCommState(hComm, &conf.dcb);
//  conf.dcb.BaudRate = BaudRate;              // Current baud
//  conf.dcb.fBinary = TRUE;               // Binary mode; no EOF check
//  conf.dcb.fParity = FALSE;               // Enable parity checking
//  conf.dcb.fOutxCtsFlow = FALSE;         // No CTS output flow control
//  conf.dcb.fOutxDsrFlow = FALSE;         // No DSR output flow control
//  conf.dcb.fDtrControl = DTR_CONTROL_ENABLE; // DTR flow control type
//  conf.dcb.fDsrSensitivity = FALSE;      // DSR sensitivity
//  conf.dcb.fTXContinueOnXoff = FALSE;     // XOFF continues Tx
//  conf.dcb.fOutX = FALSE;                // No XON/XOFF out flow control
//  conf.dcb.fInX = FALSE;                 // No XON/XOFF in flow control
//  conf.dcb.fErrorChar = FALSE;           // Disable error replacement
//  conf.dcb.fNull = FALSE;                // Disable null stripping
//  conf.dcb.fRtsControl = RTS_CONTROL_ENABLE; // RTS flow control
//  conf.dcb.fAbortOnError = FALSE;        // Do not abort reads/writes on error
//  conf.dcb.ByteSize = 8;                 // Number of bits/byte, 4-8
//  conf.dcb.Parity = NOPARITY;            // 0-4=no,odd,even,mark,space
//  conf.dcb.StopBits = ONESTOPBIT;
//  if (!SetCommState(hComm, &conf.dcb)){
//    #ifdef ENABLE_LOGGING
//    RAW_LOG (INFO, "SerialCom(%s): can't set comm state",PortName);
//    #endif
//  }
//  COMMTIMEOUTS commTimeouts;
//  GetCommTimeouts(hComm, &commTimeouts);
//  commTimeouts.ReadIntervalTimeout = 1;
//  commTimeouts.ReadTotalTimeoutMultiplier = 0;
//  commTimeouts.ReadTotalTimeoutConstant = 0;
//  commTimeouts.WriteTotalTimeoutMultiplier = 0;
//  commTimeouts.WriteTotalTimeoutConstant = 0;
//  if (!SetCommTimeouts(hComm,&commTimeouts)){
//    #ifdef ENABLE_LOGGING
//    RAW_LOG (INFO, "SerialCom(%s): can't set timeouts",PortName);
//    #endif
//  }
//  delete [] PortName;
//  Sleep(3000);
//}
//
//char *SerialCom::Read(void)
//{
//  COMSTAT status;
//  DWORD errors;
//  DWORD bytesRead;
//  DWORD toRead;
//  OVERLAPPED osReader = {0};
//  osReader.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
//  ClearCommError(hComm, &errors, &status);
//  outSize = 0;
//  if (status.cbInQue>=1){
//    if (status.cbInQue<READ_BUFF_SIZE-1){
//      toRead = status.cbInQue;
//    }else{
//      toRead = READ_BUFF_SIZE-1;
//    }
//    ReadFile(hComm,out,toRead, &bytesRead, &osReader);
//    outSize = bytesRead;
//  }
//  CloseHandle(osReader.hEvent);
//  return out;
//}
//
//void SerialCom::Write(char *Data, int DataSize)
//{
//  DWORD dwBytesWritten;
//  OVERLAPPED osWriter = {0};
//  osWriter.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
//  WriteFile(hComm, &Data[0], DataSize, &dwBytesWritten, &osWriter);
//  WaitForSingleObject(osWriter.hEvent, SERIAL_WRITE_WAIT_MS);
//  CloseHandle(osWriter.hEvent);
//}
//
//int SerialCom::GetOutSize(void)
//{
//  return outSize;
//}
//
//SerialCom::~SerialCom(void)
//{
//  CloseHandle(hComm);
//  delete [] out;
//}