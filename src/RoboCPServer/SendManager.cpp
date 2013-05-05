#pragma once
#include "SendManager.h"


SendManager::SendManager (SendBuffer* b, KinectViewer* v)
{
  sendBuffer = b;
  kinectViewer = v;
}

SendManager::~SendManager ()
{
}

void SendManager::Start ()
{
	  boost::shared_ptr<Send> sendData;

	  while ( true ) {
		sendData = sendBuffer->Dequeue(); // Receiving
		kinectViewer->Mtx->lock();
		  updateText(sendData); // Updating KinectViewer info
		kinectViewer->Mtx->unlock();
	  }
}

void SendManager::updateText (boost::shared_ptr<Send> s)
{
  char buf[400];

  // Filling buf with sprintf, then updating text in KinectViewer 

  sprintf (buf, "Acceleration x: %d", s->Acceleration.x);
  kinectViewer->viewer->updateText (buf, 5, 290, 10, 1, 1, 1, "AccelerationX");
  sprintf (buf, "Acceleration y: %d", s->Acceleration.y);
  kinectViewer->viewer->updateText (buf, 5, 278, 10, 1, 1, 1, "AccelerationY");
  sprintf (buf, "Acceleration z: %d", s->Acceleration.z);
  kinectViewer->viewer->updateText (buf, 5, 268, 10, 1, 1, 1, "AccelerationZ");

  sprintf (buf, "Top sonic: %d", s->TopSonicSensor);
  kinectViewer->viewer->updateText (buf, 5, 248, 10, 1, 1, 1, "TopSonic");
  sprintf (buf, "Left sonic: %d", s->LeftSonicSensor);
  kinectViewer->viewer->updateText (buf, 5, 238, 10, 1, 1, 1, "LeftSonic");
  sprintf (buf, "Right sonic: %d", s->RightSonicSensor);
  kinectViewer->viewer->updateText (buf, 5, 224, 10, 1, 1, 1, "RightSonic");
  sprintf (buf, "Front sonic: %d", s->FrontSonicSensor);
  kinectViewer->viewer->updateText (buf, 5, 214, 10, 1, 1, 1, "FrontSonic");
  sprintf (buf, "Back sonic: %d", s->BackSonicSensor);
  kinectViewer->viewer->updateText (buf, 5, 202, 10, 1, 1, 1, "BackSonic");

  sprintf (buf, "Alt barometer: %f", s->AltitudeBarometer);
  kinectViewer->viewer->updateText (buf, 5, 186, 10, 1, 1, 1, "AltBarometer");
  sprintf (buf, "Alt sonic: %f", s->AltitudeSonic);
  kinectViewer->viewer->updateText (buf, 5, 174, 10, 1, 1, 1, "AltSonic");
  sprintf (buf, "Pitch: %f", s->Pitch);
  kinectViewer->viewer->updateText (buf, 5, 162, 10, 1, 1, 1, "Pitch");
  sprintf (buf, "Roll: %f", s->Roll);
  kinectViewer->viewer->updateText (buf, 5, 150, 10, 1, 1, 1, "Roll");
  sprintf (buf, "Yaw: %f", s->Yaw);
  kinectViewer->viewer->updateText (buf, 5, 138, 10, 1, 1, 1, "Yaw");

  sprintf (buf, "Time: %s", ctime(&(s->Time)) );
  kinectViewer->viewer->updateText (buf, 5, 122, 10, 1, 1, 1, "SendTime");

  sprintf (buf, "Motion x1: %f", s->Motion.BeginningX);
  kinectViewer->viewer->updateText (buf, 5, 112, 10, 1, 1, 1, "BeginningX");
  sprintf (buf, "Motion y1: %f", s->Motion.BeginningY);
  kinectViewer->viewer->updateText (buf, 5, 102, 10, 1, 1, 1, "BeginningY");
  sprintf (buf, "Motion x2: %f", s->Motion.EndX);
  kinectViewer->viewer->updateText (buf, 5, 92, 10, 1, 1, 1, "EndX");
  sprintf (buf, "Motion y2: %f", s->Motion.EndY);
  kinectViewer->viewer->updateText (buf, 5, 82, 10, 1, 1, 1, "EndY");
    
}
