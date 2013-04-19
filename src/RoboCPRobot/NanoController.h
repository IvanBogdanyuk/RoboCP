#pragma once
#include "Controller.h"
#include "NanoReceivedBuffer.h"
#include "XMLConfig.h"
#include "SerialCom.h"
#include <string.h>
#include <time.h>

//#define NANO_FPS_TEST
//#define NANO_TELEMETRY_TEST
//#define NANO_INPUT_DATA_TEST

#define NANO_BAUD_RATE 115200
#define NANO_SECONDS_TO_RECONNECT 5
#define TO_SEND_BUFF_SIZE 128

class NanoController :
  public Controller
{
private:
  char *dataToSend;
  bool readyToNewMessage;
  NanoReceivedBuffer *buffer;
  SerialCom *nanoCom;
  string nanoPort;
  time_t lastReadTime;
public:
  void Start(void);
  bool ChangeGPSMessage(char *UTC, char *Latitude, char *Longtitude, char *GSpeed, char *SpeedAngle);
  bool SetDefaultGPSMessage();
  NanoReceivedBuffer *GetBuffer(void);
  NanoController(XMLConfig *x, NanoReceivedBuffer *buf);
  ~NanoController(void);
};

