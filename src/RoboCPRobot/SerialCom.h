#pragma once
#include <Windows.h>
#include <WinDef.h>

#define READ_BUFF_SIZE 256

class SerialCom
{
private:
  char *out;
  int outSize;
  HANDLE hComm;
public:
  char *Read(void);
  void Write(char *Data);
  int GetOutSize();
  SerialCom(LPCSTR PortName, int BaudRate);
  ~SerialCom(void);
};

