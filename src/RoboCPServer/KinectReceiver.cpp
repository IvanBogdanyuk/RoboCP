#pragma once
#include "KinectReceiver.h"


KinectReceiver::KinectReceiver (XMLConfig * x, KinectViewer* v)
{
  ip = x->IP;
  port = x->KinectPort;
  kinectViewer = v;
  octreeCoder = new PointCloudCompression<PointXYZ> (x->CompressionProfile, x->ShowStatistics, x->PointResolution,
                                                         x->OctreeResolution, x->DoVoxelGridDownDownSampling, x->IFrameRate,
                                                         x->DoColorEncoding, x->ColorBitResolution);
}

KinectReceiver::~KinectReceiver ()
{
  delete (octreeCoder);
}

void KinectReceiver::Start ()
{
  try {
    tcp::iostream socketStream (ip.c_str(), port.c_str() );

    if (!socketStream.fail() ) {
      cout << "KinectViewer: Connected!" << endl;

	  boost::shared_ptr<KinectData> kData (new KinectData);
	  Sleep (5000);

	  while (true ) {
		Sleep (100);
		socketStream >> kData->Time;
		octreeCoder->decodePointCloud (socketStream, kData->Cloud);
		kinectViewer->viewer->updatePointCloud (kData->Cloud, "cloud");
		char buf[50]; 
	    sprintf (buf, "Cloud time: %s", ctime(&kData->Time) );
		kinectViewer->viewer->updateText (buf, 10, 20, 10, 1, 1, 1, "CloudTime");
	  }
	
	}
  }
  catch (exception& e) {
    cout << "KinectViewer: Exception: " << e.what () << endl;
  }
}