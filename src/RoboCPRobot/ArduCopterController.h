#pragma once
#include "controller.h"
#include "ArduCopterBuffer.h"
#include "XMLConfig.h"
#include "SerialCom.h"
#include <string.h>
#include <time.h>

//#define COPTER_MSG_TYPES_TEST
//#define COPTER_TELEMETRY_TEST

#define COPTER_BAUD_RATE 115200
#define COPTER_SECONDS_TO_RECONNECT 5

class ArduCopterController :
  public Controller
{
private:
  ArduCopterBuffer *buffer;
  SerialCom *copterCom;
  string copterPort;
  time_t lastReadTime;
  char stage;
  void sendInitionalData(void);
public:
  void Start(void);
  ArduCopterController(XMLConfig *x, ArduCopterBuffer *buf);
  ~ArduCopterController(void);
};

