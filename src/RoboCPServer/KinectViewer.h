#pragma once
#include <iostream>
#include <stdio.h>
#include <sstream>
#include <stdlib.h>
#include <string>
#include "XMLConfig.h"
#include "KinectData.h"
#include "Send.h"
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/visualization/pcl_visualizer.h>

#include <boost/asio.hpp>

using namespace pcl;

using namespace std;

class KinectViewer
{
public:
  void Start ();
  KinectViewer(XMLConfig * x);
  ~KinectViewer(void);
  pcl::visualization::PCLVisualizer* viewer;
};