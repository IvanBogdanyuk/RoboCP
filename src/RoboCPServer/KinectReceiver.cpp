#pragma once
#include "KinectReceiver.h"


KinectReceiver::KinectReceiver (XMLConfig * x, KinectBuffer* b)
{
  ip = x->IP; // Reading IP from config
  port = x->KinectPort; // Reading port from config
  kinectBuffer = b;

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
      cout << "KinectReceiver: Connected!" << endl; // TODO: write in log
	  RAW_LOG (INFO,  "KinectReceiver: Connected!");
	  Sleep (5000);

	  while (true ) {
		boost::shared_ptr<KinectData> kData (new KinectData); // Creating new KinectData
		socketStream >> kData->Time; // Receivig time
		octreeCoder->decodePointCloud (socketStream, kData->Cloud); // Then receiving point cloud
		kinectBuffer->Enqueue (kData); // adding KinectData in KinectBuffer
	  }
	
	}
  }
  catch (exception& e) {
    cout << "KinectReceiver: Exception: " << e.what () << endl; // TODO: write in log
	RAW_LOG (INFO,  "KinectReceiver: Exception: %s", e.what());
  }
}