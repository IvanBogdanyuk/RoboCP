#pragma once
#include "KinectViewer.h"


KinectViewer::KinectViewer (XMLConfig * x)
{
  ip = x->IP;
  port = x->KinectPort;
  octreeCoder = new PointCloudCompression<PointXYZ> (x->CompressionProfile, x->ShowStatistics, x->PointResolution,
                                                         x->OctreeResolution, x->DoVoxelGridDownDownSampling, x->IFrameRate,
                                                         x->DoColorEncoding, x->ColorBitResolution);
}

KinectViewer::~KinectViewer ()
{
  delete (octreeCoder);
}

void KinectViewer::Start ()
{
  try {
    tcp::iostream socketStream (ip.c_str(), port.c_str() );

    if (!socketStream.fail() ) {
      cout << "KinectViewer: Connected!" << endl;

      pcl::visualization::CloudViewer viewer ("Downsampled point cloud");

	  boost::shared_ptr<KinectData> kData (new KinectData);

	  while (!viewer.wasStopped() ) {
		socketStream >> kData->Time;
		octreeCoder->decodePointCloud (socketStream, kData->Cloud);
		//cout << ctime (&kData->Time);
		viewer.showCloud (kData->Cloud);
	  }
	
	}
  }
  catch (exception& e) {
    cout << "KinectViewer: Exception: " << e.what () << endl;
  }
}