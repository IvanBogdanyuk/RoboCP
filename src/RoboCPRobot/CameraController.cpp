#include "CameraController.h"



CameraController::CameraController(XMLConfig *x, CameraReceivedBuffer *buf)
{
  buffer = buf;
  cameraNum = x->CameraNumber;
  fps = x->CameraFramesPerSecond;
  width = x->CameraFrameWidth;
  height = x->CameraFrameHeight;
}

CameraReceivedBuffer *CameraController::GetBuffer(void)
{
  return buffer;
}

CameraController::~CameraController(void)
{
}

void CameraController::Start(void)
{
  CameraReceived *DataToStorage;
  CvCapture *Capture = cvCreateCameraCapture(cameraNum);
  IplImage *Frame;
  cvSetCaptureProperty(Capture,CV_CAP_PROP_FRAME_WIDTH,width);
  cvSetCaptureProperty(Capture,CV_CAP_PROP_FRAME_HEIGHT,height);
  cvSetCaptureProperty(Capture,CV_CAP_PROP_FPS,fps);
  while (true){
    Frame = cvQueryFrame(Capture);
    boost::shared_ptr<CameraReceived> CameraImg(new CameraReceived(Frame));
    CameraImg->Time = time(NULL);
    buffer->Enqueue(CameraImg);
  }
}
