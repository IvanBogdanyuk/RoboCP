#pragma once
#include "Controller.h"
#include "NanoReceivedBuffer.h"
#include "XMLConfig.h"
#include "SerialCom.h"
#include <string.h>
#include <time.h>

#define NANO_BAUD_RATE 115200
#define NANO_SECONDS_TO_RECONNECT 5

class NanoController :
  public Controller
{
private:
  NanoReceivedBuffer *buffer;
  SerialCom *nanoCom;
  string nanoPort;
  time_t lastReadTime;
public:
  void Start(void);
  NanoReceivedBuffer *GetBuffer(void);
  NanoController(XMLConfig *x, NanoReceivedBuffer *buf);
  ~NanoController(void);
};

