#include "KinectData.h"


KinectData::KinectData(void)
{
  PointCloud<PointXYZ>::Ptr cl (new PointCloud<PointXYZ>);
  Cloud = cl;
}

KinectData::KinectData(PointCloud<PointXYZ>::Ptr cloud, time_t time)
{
  Time = time;
  Cloud = cloud;
}

KinectData::~KinectData(void)
{
}
