#pragma once
#include "Received.h"
#include <time.h>
class NanoReceived : public Received
{
public:
  int TopSonicSensor;
  int FrontSonicSensor;
  int LeftSonicSensor;
  int RightSonicSensor;
  int BackSonicSensor;
  time_t Time;
  NanoReceived(void);
  ~NanoReceived(void);
};

