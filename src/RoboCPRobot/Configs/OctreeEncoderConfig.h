/*








//WARNING!!!!!!!! This is a template file. Do not change it without necessity.










*/
#pragma once
#include "Config.h"
#include "configFactory.h"

#include "pcl/compression/octree_pointcloud_compression.h"

class OctreeEncoderConfig : public Config{
public:
  OctreeEncoderConfig();
  friend class configFactory;
  ~OctreeEncoderConfig();
  pcl::octree::compression_Profiles_e getCompressionProfile();
  bool getShowStatistics();
  double getPointResolution();
  double getOctreeResolution();
  bool getDoVoxelGridDownDownSampling();
  unsigned int getIFrameRate();
  bool getDoColorEncoding();
  unsigned int getColorBitResolution();

private:
  pcl::octree::compression_Profiles_e CompressionProfile;
  bool ShowStatistics;
  double PointResolution;
  double OctreeResolution;
  bool DoVoxelGridDownDownSampling;
  unsigned int IFrameRate;
  bool DoColorEncoding;
  unsigned int ColorBitResolution;
};