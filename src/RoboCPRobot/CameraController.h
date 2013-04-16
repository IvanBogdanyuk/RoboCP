#pragma once
#include "Controller.h"
#include <time.h>
#include <cv.h>
#include <highgui.h>
#include "XMLConfig.h"
#include "CameraReceivedBuffer.h"

class CameraController :
  public Controller
{
private:
  int cameraNum;
  int fps;
  int width;
  int height;
  CameraReceivedBuffer *buffer;
public:
  void Start(void);
  CameraReceivedBuffer *GetBuffer(void);
  CameraController(XMLConfig *x, CameraReceivedBuffer *buf);
  ~CameraController(void);
};

