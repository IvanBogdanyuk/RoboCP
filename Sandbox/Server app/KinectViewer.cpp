#pragma once
#include "XMLConfig.cpp"
#include <iostream>
#include <stdio.h>
#include <sstream>
#include <stdlib.h>
#include <string>

#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/io/openni_grabber.h>
#include <pcl/visualization/cloud_viewer.h>
#include <pcl/console/parse.h>
#include <pcl/common/time.h>
#include <pcl/filters/passthrough.h>
#include <pcl/io/pcd_io.h>
#include "pcl/compression/octree_pointcloud_compression.h"

#include <boost/asio.hpp>


using boost::asio::ip::tcp;

using namespace pcl;
using namespace pcl::octree;

using namespace std;



class KinectViewer {
private:
  PointCloudCompression<PointXYZRGBA>* OctreeCoder;
  string IP;
  string port;

public:
  void run () {
    try {
      tcp::iostream socketStream (IP.c_str(), port.c_str() );

      if (!socketStream.fail() )
        cout << "KinectViewer Connected!" << endl;

      pcl::visualization::CloudViewer viewer ("Decoded Point Cloud - PCL Compression Viewer");

	  PointCloud<PointXYZRGBA>::Ptr cloudOut (new PointCloud<PointXYZRGBA> ());

	  while (!viewer.wasStopped() ) {
	    Sleep (3000);
        OctreeCoder->decodePointCloud (socketStream, cloudOut);
        viewer.showCloud (cloudOut);
	  }

    }
    catch (exception& e) {
      cout << "KinectViewer Exception: " << e.what () << endl;
    }

  }
  KinectViewer (XMLConfig c) {
	IP = c.IP;
	port = c.KinectPort;
	OctreeCoder = new PointCloudCompression<PointXYZRGBA> (c.compressionProfile, c.showStatistics, c.pointResolution,
                                                         c.octreeResolution, c.doVoxelGridDownDownSampling, c.iFrameRate,
                                                         c.doColorEncoding, c.colorBitResolution);
  }
  ~KinectViewer () {
    delete(OctreeCoder);
  }
};