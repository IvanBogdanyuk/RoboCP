#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include "pcl/compression/octree_pointcloud_compression.h"
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/nvp.hpp>

//#define ENABLE_LOGGING

using namespace pcl;
using namespace pcl::octree;
using namespace std;


// Config class
class XMLConfig {
private:
  friend class boost::serialization::access;
  template<class Archive>
  void serialize (Archive & ar, const unsigned int version)
  {
	// BOOST_SERIALIZATION_NVP macro used because of XML serialization
    ar & BOOST_SERIALIZATION_NVP(IP);
    ar & BOOST_SERIALIZATION_NVP(KinectPort);
    ar & BOOST_SERIALIZATION_NVP(CommandPort);
	ar & BOOST_SERIALIZATION_NVP(SendPort);
	ar & BOOST_SERIALIZATION_NVP(CarduinoPort);
	ar & BOOST_SERIALIZATION_NVP(ArducopterPort);

    ar & BOOST_SERIALIZATION_NVP(CompressionProfile);
    ar & BOOST_SERIALIZATION_NVP(ShowStatistics);
    ar & BOOST_SERIALIZATION_NVP(PointResolution);
    ar & BOOST_SERIALIZATION_NVP(OctreeResolution);
    ar & BOOST_SERIALIZATION_NVP(DoVoxelGridDownDownSampling);
    ar & BOOST_SERIALIZATION_NVP(IFrameRate);
    ar & BOOST_SERIALIZATION_NVP(DoColorEncoding);
    ar & BOOST_SERIALIZATION_NVP(ColorBitResolution);

    ar & BOOST_SERIALIZATION_NVP(CameraNumber);
    ar & BOOST_SERIALIZATION_NVP(CameraFramesPerSecond);
    ar & BOOST_SERIALIZATION_NVP(CameraFrameWidth);
    ar & BOOST_SERIALIZATION_NVP(CameraFrameHeight);
  }

public:
  string IP;
  string KinectPort;
  string CommandPort;
  string SendPort;
  string CarduinoPort;
  string ArducopterPort;

  pcl::octree::compression_Profiles_e CompressionProfile;

  bool ShowStatistics;
  double PointResolution;
  double OctreeResolution;
  bool DoVoxelGridDownDownSampling;
  unsigned int IFrameRate;
  bool DoColorEncoding;
  unsigned int ColorBitResolution;

  int CameraNumber;
  int CameraFramesPerSecond;
  int CameraFrameWidth;
  int CameraFrameHeight;

  XMLConfig();
};