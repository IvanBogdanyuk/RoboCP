#pragma once
#include <time.h>
#include "Point3d.h"
class Send
{
public:
  int TopSonicSensor;
  int FrontSonicSensor;
  int LeftSonicSensor;
  int RightSonicSensor;
  int BackSonicSensor;

  float Roll;
  float Pitch;
  float Yaw;
  float AltitudeSonic;
  float AltitudeBarometer;
  struct Point3d Acceleration;

  time_t Time;
  Send(void);
  ~Send(void);
};

