#include "CameraController.h"



CameraController::CameraController(CameraReceivedBuffer *buf)
{
  buffer = buf;
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
  CvCapture *Capture = cvCreateCameraCapture(CAMERA_NUM);
  IplImage *Frame;
  cvSetCaptureProperty(Capture,CV_CAP_PROP_FRAME_WIDTH,320);
  cvSetCaptureProperty(Capture,CV_CAP_PROP_FRAME_HEIGHT,240);
  cvSetCaptureProperty(Capture,CV_CAP_PROP_FPS,180);
  while (true){
    Frame = cvQueryFrame(Capture);
    boost::shared_ptr<CameraReceived> CameraImg(new CameraReceived(Frame));
    CameraImg->Time = time(NULL);
    buffer->Enqueue(CameraImg);
  }
}
