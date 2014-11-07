#include "ArduCopterReceived.h"


ArduCopterReceived::ArduCopterReceived(void)
{
  Roll = 0;
  Pitch = 0;
  Yaw = 0;
  AltitudeSonic = 0;
  AltitudeBarometer = 0;
  Acceleration.x = 0;
  Acceleration.y = 0;
  Acceleration.z = 0;
}


ArduCopterReceived::~ArduCopterReceived(void)
{
}
