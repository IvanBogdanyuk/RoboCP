#pragma once
#include "controller.h"
#include "ArduCopterBuffer.h"
#include "Config.h"
#include "SerialCom.h"

#include <string.h>
#include <time.h>

#ifdef ENABLE_LOGGING
#define GLOG_NO_ABBREVIATED_SEVERITIES
#include <glog/logging.h>
#include <glog/raw_logging.h>
#endif

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
  void FakeStart(void);
  ArduCopterController(ArduCopterBuffer *buf);
  void Configure(Config *arducopterConfig);
  ~ArduCopterController(void);
};

