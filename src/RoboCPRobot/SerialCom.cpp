#include "SerialCom.h"

SerialCom::SerialCom(LPCSTR PortName, int BaudRate)
{
  out = new char[READ_BUFF_SIZE];
  outSize = 0;
  RAW_LOG (INFO, "SerialCom(%s): connecting...",PortName);
  hComm = CreateFile(PortName, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
  if (hComm == INVALID_HANDLE_VALUE){
    int err = GetLastError();
    if (err == ERROR_FILE_NOT_FOUND){
      RAW_LOG (INFO, "SerialCom(%s): can't connect - this com port or device doesn't exist",PortName);
    }else{
      if (err = ERROR_ACCESS_DENIED){
        RAW_LOG (INFO, "SerialCom(%s): can't connect - this com port already in use",PortName);
      }else{
        RAW_LOG (INFO, "SerialCom(%s): can't connect - unknown error",PortName);
      }
    }
  }else{
    RAW_LOG (INFO, "SerialCom(%s): connected!",PortName);
  }
  if (!SetupComm(hComm,1024,1024)){
    RAW_LOG (INFO, "SerialCom(%s): can't setup i/o buffers",PortName);
  }
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
  conf.dcb.fTXContinueOnXoff = FALSE;     // XOFF continues Tx
  conf.dcb.fOutX = FALSE;                // No XON/XOFF out flow control
  conf.dcb.fInX = FALSE;                 // No XON/XOFF in flow control
  conf.dcb.fErrorChar = FALSE;           // Disable error replacement
  conf.dcb.fNull = FALSE;                // Disable null stripping
  conf.dcb.fRtsControl = RTS_CONTROL_ENABLE; // RTS flow control
  conf.dcb.fAbortOnError = FALSE;        // Do not abort reads/writes on error
  conf.dcb.ByteSize = 8;                 // Number of bits/byte, 4-8
  conf.dcb.Parity = NOPARITY;            // 0-4=no,odd,even,mark,space
  conf.dcb.StopBits = ONESTOPBIT;
  if (!SetCommState(hComm, &conf.dcb)){
    RAW_LOG (INFO, "SerialCom(%s): can't set comm state",PortName);
  }
  COMMTIMEOUTS commTimeouts;
  GetCommTimeouts(hComm, &commTimeouts);
  commTimeouts.ReadIntervalTimeout = 1;
  commTimeouts.ReadTotalTimeoutMultiplier = 0;
  commTimeouts.ReadTotalTimeoutConstant = 0;
  commTimeouts.WriteTotalTimeoutMultiplier = 0;
  commTimeouts.WriteTotalTimeoutConstant = 0;
  if (!SetCommTimeouts(hComm,&commTimeouts)){
    RAW_LOG (INFO, "SerialCom(%s): can't set timeouts",PortName);
  }
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
    outSize = bytesRead;
  }
  CloseHandle(osReader.hEvent);
  return out;
}

void SerialCom::Write(char *Data, int DataSize)
{
  DWORD dwBytesWritten;
  OVERLAPPED osWriter = {0};
  osWriter.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
  WriteFile(hComm, &Data[0], DataSize, &dwBytesWritten, &osWriter);
  WaitForSingleObject(osWriter.hEvent, SERIAL_WRITE_WAIT_MS);
  CloseHandle(osWriter.hEvent);
}

int SerialCom::GetOutSize(void)
{
  return outSize;
}

SerialCom::~SerialCom(void)
{
  CloseHandle(hComm);
  delete [] out;
}