#pragma once
#include "KinectReceiver.h"


KinectReceiver::KinectReceiver (XMLConfig * x, KinectViewer* v)
{
  ip = x->IP; // Reading IP from config
  port = x->KinectPort; // Reading port from config
  kinectViewer = v;

  // We will receive encoded point clouds, so we need to decode them
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
    tcp::iostream socketStream (ip.c_str(), port.c_str() ); // Trying to connect

    if (!socketStream.fail() ) {
      cout << "KinectViewer: Connected!" << endl; // TODO: write in log
	  RAW_LOG (INFO,  "KinectViewer: Connected!");

	  boost::shared_ptr<KinectData> kData (new KinectData); // Creating new KinectData
	  Sleep (5000);

	  while (true ) {
		socketStream >> kData->Time; // Receivig time
		octreeCoder->decodePointCloud (socketStream, kData->Cloud); // Then receiving point cloud
		kinectViewer->viewer->updatePointCloud (kData->Cloud, "cloud"); // updating cloud in KinectViewer
		
		// updating CloudTime text line in KinectViewer
		char buf[50]; 
	    sprintf (buf, "Cloud time: %s", ctime(&kData->Time) );
		kinectViewer->viewer->updateText (buf, 10, 20, 10, 1, 1, 1, "CloudTime");
	  }
	
	}
  }
  catch (exception& e) {
    cout << "KinectViewer: Exception: " << e.what () << endl; // TODO: write in log
	RAW_LOG (INFO,  "KinectViewer: Exception: %s", e.what());
  }
}