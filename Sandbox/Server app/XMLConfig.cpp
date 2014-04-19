#pragma once
#include <iostream>
#include <string>
#include <fstream>

#include "pcl/compression/octree_pointcloud_compression.h"

#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/nvp.hpp>


using namespace pcl;
using namespace pcl::octree;

using namespace std;



class XMLConfig {
private:
  friend class boost::serialization::access;
 
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version) {
	ar & BOOST_SERIALIZATION_NVP(IP);
	ar & BOOST_SERIALIZATION_NVP(KinectPort);
	ar & BOOST_SERIALIZATION_NVP(CommandPort);
	ar & BOOST_SERIALIZATION_NVP(compressionProfile);
	ar & BOOST_SERIALIZATION_NVP(showStatistics);
	ar & BOOST_SERIALIZATION_NVP(pointResolution);
	ar & BOOST_SERIALIZATION_NVP(octreeResolution);
	ar & BOOST_SERIALIZATION_NVP(doVoxelGridDownDownSampling);
	ar & BOOST_SERIALIZATION_NVP(iFrameRate);
	ar & BOOST_SERIALIZATION_NVP(doColorEncoding);
	ar & BOOST_SERIALIZATION_NVP(colorBitResolution);
  }
	
public:
  string IP;
  string KinectPort;
  string CommandPort;

  pcl::octree::compression_Profiles_e compressionProfile;

  bool showStatistics;
  double pointResolution;
  double octreeResolution;
  bool doVoxelGridDownDownSampling;
  unsigned int iFrameRate;
  bool doColorEncoding;
  unsigned int colorBitResolution;
	

  XMLConfig() {
    // default values
	IP = "127.0.0.1";
	KinectPort = "6666";
	CommandPort = "6667";
	compressionProfile = pcl::octree::LOW_RES_ONLINE_COMPRESSION_WITHOUT_COLOR;
    showStatistics = false;
    pointResolution = 0.001;
    octreeResolution = 0.01;
    doVoxelGridDownDownSampling = false;
    iFrameRate = 30;
    doColorEncoding = false;
    colorBitResolution = 6;
  }
};
