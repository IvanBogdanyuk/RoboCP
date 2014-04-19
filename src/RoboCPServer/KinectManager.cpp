#pragma once
#include "KinectManager.h"


KinectManager::KinectManager (KinectBuffer* b, KinectViewer* v)
{
  kinectBuffer = b;
  kinectViewer = v;
}

KinectManager::~KinectManager ()
{
}

void KinectManager::Start ()
{
  boost::shared_ptr<KinectData> kData;

  while (true ) {
	kData = kinectBuffer->Dequeue ();
	kinectViewer->Mtx->lock();
	  kinectViewer->viewer->updatePointCloud (kData->Cloud, "cloud"); // updating cloud in KinectViewer
		
	  // updating CloudTime text line in KinectViewer
	  char buf[50]; 
	  sprintf (buf, "Cloud time: %s", ctime(&kData->Time) );
	  kinectViewer->viewer->updateText (buf, 10, 20, 10, 1, 1, 1, "CloudTime");
	kinectViewer->Mtx->unlock();
  }
}