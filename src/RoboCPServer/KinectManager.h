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

#define GLOG_NO_ABBREVIATED_SEVERITIES
#include <glog/logging.h>
#include <glog/raw_logging.h>

using boost::asio::ip::tcp;

using namespace pcl;
using namespace pcl::octree;

using namespace std;

// Class purpose: taking KinectData from buffer and giving them to KinectViewer
class KinectManager
{
public:
  void Start ();
  KinectManager(KinectBuffer* b, KinectViewer* v);
  ~KinectManager(void);
private:
  KinectBuffer* kinectBuffer;
  KinectViewer* kinectViewer;
};