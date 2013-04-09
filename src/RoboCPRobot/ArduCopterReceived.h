#pragma once
#include "Received.h"
#include <time.h>
#include "Point3d.h"
class ArduCopterReceived :
  public Received
{
public:
  float Roll;
  float Pitch;
  float Yaw;
  float AltitudeSonic;
  float AltitudeBarometer;
  struct Point3d Acceleration;
  time_t Time;
  ArduCopterReceived(void);
  ~ArduCopterReceived(void);
};

