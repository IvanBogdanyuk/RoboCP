#pragma once
#include <Windows.h>
#include <WinDef.h>

#define READ_BUFF_SIZE 256

class SerialCom
{
private:
  char *out;
  HANDLE hComm;
public:
  char *Read(void);
  void Write(char *Data);
  SerialCom(LPCSTR PortName);
  ~SerialCom(void);
};

