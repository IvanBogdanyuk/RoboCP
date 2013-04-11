#pragma once
#include "controller.h"
#include "ArduCopterBuffer.h"
#include "SerialCom.h"
#include <time.h>

#define COPTER_BAUD_RATE 115200
#define COPTER_COM_PORT "COM4"
#define COPTER_SECONDS_TO_RECONNECT 5

class ArduCopterController :
  public Controller
{
private:
  ArduCopterBuffer *buffer;
  SerialCom *copterCom;
  time_t lastReadTime;
  char stage;
  void sendInitionalData(void);
public:
  void Start(void);
  ArduCopterController(ArduCopterBuffer *buf);
  ~ArduCopterController(void);
};

