#pragma once
#include "Received.h"
#include <time.h>
class NanoReceived : public Received
{
public:
  unsigned short TopSonicSensor;
  unsigned short FrontSonicSensor;
  unsigned short LeftSonicSensor;
  unsigned short RightSonicSensor;
  unsigned short BackSonicSensor;
  time_t Time;
  NanoReceived(void);
  ~NanoReceived(void);
};

