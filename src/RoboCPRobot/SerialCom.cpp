#include "SerialCom.h"

SerialCom::SerialCom(LPCSTR PortName, int BaudRate)
{
  out = new char[READ_BUFF_SIZE];
  hComm = CreateFile(PortName, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, 0);
  COMMCONFIG conf;
  conf.dcb.DCBlength = sizeof(DCB);
  GetCommState(hComm, &conf.dcb);
  conf.dcb.BaudRate = BaudRate;              // Current baud
  conf.dcb.fBinary = TRUE;               // Binary mode; no EOF check
  conf.dcb.fParity = FALSE;               // Enable parity checking
  conf.dcb.fOutxCtsFlow = FALSE;         // No CTS output flow control
  conf.dcb.fOutxDsrFlow = FALSE;         // No DSR output flow control
  conf.dcb.fDtrControl = DTR_CONTROL_ENABLE; // DTR flow control type
  conf.dcb.fDsrSensitivity = FALSE;      // DSR sensitivity
  conf.dcb.fTXContinueOnXoff = TRUE;     // XOFF continues Tx
  conf.dcb.fOutX = FALSE;                // No XON/XOFF out flow control
  conf.dcb.fInX = FALSE;                 // No XON/XOFF in flow control
  conf.dcb.fErrorChar = FALSE;           // Disable error replacement
  conf.dcb.fNull = FALSE;                // Disable null stripping
  conf.dcb.fRtsControl = RTS_CONTROL_ENABLE; // RTS flow control
  conf.dcb.fAbortOnError = FALSE;        // Do not abort reads/writes on error
  conf.dcb.ByteSize = 8;                 // Number of bits/byte, 4-8
  conf.dcb.Parity = NOPARITY;            // 0-4=no,odd,even,mark,space
  conf.dcb.StopBits = ONESTOPBIT;
  SetCommState(hComm, &conf.dcb);
  COMMTIMEOUTS commTimeouts;
  GetCommTimeouts(hComm, &commTimeouts);
  commTimeouts.ReadIntervalTimeout = MAXDWORD;
  commTimeouts.ReadTotalTimeoutMultiplier = 0;
  commTimeouts.ReadTotalTimeoutConstant = 0;
  commTimeouts.WriteTotalTimeoutMultiplier = 0;
  commTimeouts.WriteTotalTimeoutConstant = 0;
  SetCommTimeouts(hComm,&commTimeouts);
  Sleep(3000);
}

char *SerialCom::Read(void)
{
  COMSTAT status;
  DWORD errors;
  DWORD bytesRead;
  DWORD toRead;
  OVERLAPPED osReader = {0};
  osReader.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
  ClearCommError(hComm, &errors, &status);
  outSize = 0;
  if (status.cbInQue>=1){
    if (status.cbInQue<READ_BUFF_SIZE-1){
      toRead = status.cbInQue;
    }else{
      toRead = READ_BUFF_SIZE-1;
    }
    ReadFile(hComm,out,toRead, &bytesRead, &osReader);
    outSize = toRead;
  }
  return out;
}

void SerialCom::Write(char *Data)
{
  DWORD dwBytesWritten;
  WriteFile(hComm, &Data,(DWORD) sizeof(Data), &dwBytesWritten, NULL);
}

int SerialCom::GetOutSize(void)
{
  return outSize;
}

SerialCom::~SerialCom(void)
{
  delete [] out;
}
