#pragma once
#include <iostream>
#include <stdio.h>
#include <sstream>
#include <stdlib.h>
#include <string>
#include "XMLConfig.h"
#include "KinectData.h"
#include "KinectBuffer.h"
#include "pcl/compression/octree_pointcloud_compression.h"
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/visualization/cloud_viewer.h>

#include <boost/asio.hpp>

using boost::asio::ip::tcp;

using namespace pcl;
using namespace pcl::octree;

using namespace std;

class KinectViewer
{
public:
  void Start ();
  KinectViewer(XMLConfig * x);
  ~KinectViewer(void);
private:
  PointCloudCompression<PointXYZ>* octreeCoder;
  string ip;
  string port;
};