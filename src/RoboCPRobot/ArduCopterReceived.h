#pragma once
#include "Received.h"
#include <time.h>
class ArduCopterReceived :
  public Received
{
public:
  float Roll;
  float Pitch;
  float Yaw;
  float Altitude;
  time_t Time;
  ArduCopterReceived(void);
  ~ArduCopterReceived(void);
};

