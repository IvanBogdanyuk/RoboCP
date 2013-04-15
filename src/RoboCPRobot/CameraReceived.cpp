#include "CameraReceived.h"


CameraReceived::CameraReceived(IplImage *ImageToSave)
{
  Frame = 0;
  Frame = cvCloneImage(ImageToSave);
}


CameraReceived::~CameraReceived(void)
{
  cvReleaseImage(&Frame);
}
