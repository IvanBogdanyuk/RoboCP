#pragma once
#include <time.h>
#include "Point3d.h"
class Send
{
public:
  unsigned short TopSonicSensor;
  unsigned short FrontSonicSensor;
  unsigned short LeftSonicSensor;
  unsigned short RightSonicSensor;
  unsigned short BackSonicSensor;

  float Roll;
  float Pitch;
  float Yaw;
  float AltitudeSonic;
  float AltitudeBarometer;
  struct Point3d Acceleration;
  
  bool TopSonicSensorDataExists;
  bool FrontSonicSensorDataExists;
  bool LeftSonicSensorDataExists;
  bool RightSonicSensorDataExists;
  bool BackSonicSensorDataExists;
  bool RollDataExists;
  bool PitchDataExists;
  bool YawDataExists;
  bool AltitudeSonicDataExists;
  bool AltitudeBarometerDataExists;
  bool AccelerationDataExists;
  time_t Time;
  Send(void);
  ~Send(void);
};

