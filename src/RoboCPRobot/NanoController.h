#pragma once
#include "Controller.h"
#include "NanoReceivedBuffer.h"
#include "SerialCom.h"
#include <time.h>

#define NANO_BAUD_RATE 9600
#define NANO_COM_PORT "COM3"
#define SECONDS_TO_RECONNECT 5

class NanoController :
  public Controller
{
private:
  NanoReceivedBuffer *buffer;
  SerialCom *nanoCom;
  time_t lastReadTime;
public:
  void Start(void);
  NanoReceivedBuffer *GetBuffer(void);
  NanoController(NanoReceivedBuffer *buf);
  ~NanoController(void);
};

