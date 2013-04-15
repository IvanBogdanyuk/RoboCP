#pragma once
#include "Received.h"
#include <time.h>
#include <cv.h>
class CameraReceived :
  public Received
{
public:
  IplImage *Frame;
  time_t Time;
  CameraReceived(IplImage *ImageToSave);
  ~CameraReceived(void);
};

