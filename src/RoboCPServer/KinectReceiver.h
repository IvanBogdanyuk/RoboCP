#pragma once
#include <iostream>
#include <stdio.h>
#include <sstream>
#include <stdlib.h>
#include <string>
#include "XMLConfig.h"
#include "KinectData.h"
#include "KinectBuffer.h"
#include "KinectViewer.h"
#include "pcl/compression/octree_pointcloud_compression.h"
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>

#include <boost/asio.hpp>

using boost::asio::ip::tcp;

using namespace pcl;
using namespace pcl::octree;

using namespace std;

class KinectReceiver
{
public:
  void Start ();
  KinectReceiver(XMLConfig * x, KinectViewer* v);
  ~KinectReceiver(void);
private:
  KinectViewer* kinectViewer;
  PointCloudCompression<PointXYZ>* octreeCoder;
  string ip;
  string port;
};