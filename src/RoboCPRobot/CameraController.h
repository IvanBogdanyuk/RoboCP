#pragma once
#include "Controller.h"
#include <time.h>
#include <cv.h>
#include <highgui.h>
#include "CameraReceivedBuffer.h"

#define CAMERA_NUM 1

class CameraController :
  public Controller
{
private:
  CameraReceivedBuffer *buffer;
public:
  void Start(void);
  CameraReceivedBuffer *GetBuffer(void);
  CameraController(CameraReceivedBuffer *buf);
  ~CameraController(void);
};

