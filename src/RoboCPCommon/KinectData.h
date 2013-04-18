#pragma once
#include <time.h>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>

using namespace pcl;

class KinectData
{
public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW  // Dont know if actually need it
  PointCloud<PointXYZ>::Ptr Cloud; // Boost shared_ptr to point cloud
  time_t Time; // Time
  KinectData(void);
  KinectData(PointCloud<PointXYZ>::Ptr cloud, time_t time);
  ~KinectData();
};