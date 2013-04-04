#pragma once
#include "controller.h"
#include "KinectData.h"
#include "KinectBuffer.h"
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/io/openni_grabber.h>
#include <pcl/io/pcd_io.h>

using namespace pcl;
using namespace std;

class KinectController :
  public Controller
{
public:
  void Start ();
  void FakeStart ();
  KinectController(KinectBuffer * buf);
  ~KinectController(void);
private:
  KinectBuffer * buffer;
  void grabberCallBack (const pcl::PointCloud<pcl::PointXYZRGBA>::ConstPtr &cloud);
};

