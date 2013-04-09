#pragma once
#include <time.h>
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
  float Altitude;

  time_t Time;
  Send(void);
  ~Send(void);
};

