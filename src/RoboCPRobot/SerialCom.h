#pragma once
#include <Windows.h>
#include <WinDef.h>
#include <tchar.h>
#include "XMLConfig.h"

#ifdef ENABLE_LOGGING
#define GLOG_NO_ABBREVIATED_SEVERITIES
#include <glog/logging.h>
#include <glog/raw_logging.h>
#endif

#define READ_BUFF_SIZE 256
#define SERIAL_WRITE_WAIT_MS 5000

class SerialCom
{
private:
  char *out;
  int outSize;
  HANDLE hComm;
public:
  char *Read(void);
  void Write(char *Data, int DataSize);
  int GetOutSize();
  SerialCom(char *PortName, int BaudRate);
  ~SerialCom(void);
};

