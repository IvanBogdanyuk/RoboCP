#pragma once
#include "Config.h"
#include "configFactory.h"
class CameraConfig : public Config{
public:
  CameraConfig();
  friend class configFactory;
  ~CameraConfig();
  int getNumber();
  int getFramesPerSecond();
  int getFrameWidth();
  int getFrameHeight();

private:
  int Number;
  int FramesPerSecond;
  int FrameWidth;
  int FrameHeight;
};