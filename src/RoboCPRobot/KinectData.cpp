#include "KinectData.h"


KinectData::KinectData(void)
{
  PointCloud<PointXYZRGBA>::Ptr cl (new PointCloud<PointXYZRGBA>);
  Cloud = cl;
}


KinectData::~KinectData(void)
{
}
