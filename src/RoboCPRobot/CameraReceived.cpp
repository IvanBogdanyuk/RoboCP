#include "CameraReceived.h"


CameraReceived::CameraReceived(IplImage *ImageToSave)
{
 
	Frame = cvCloneImage(ImageToSave);
  
}

CameraReceived::CameraReceived()
{
  Motion.BeginningX = 0;
  Motion.BeginningY = 0;
  Motion.EndX = 0;
  Motion.EndY = 0;
  Frame = 0;
}

CameraReceived::~CameraReceived(void)
{
  cvReleaseImage(&Frame);
}
