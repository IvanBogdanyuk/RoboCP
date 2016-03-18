#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include "pcl/compression/octree_pointcloud_compression.h"
#include "QtCore\qdatastream.h"
/*
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/nvp.hpp>
*/

#include "QtXml\qdom.h"


//#define ENABLE_LOGGING

using namespace pcl;
using namespace pcl::octree;
using namespace std;


// Config class
class XMLConfig {
private:
  
	/*
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
  }*/
  

  QDomElement serialize(QDomDocument& doc){
  
   QDomElement elem = doc.createElement("XMLConfig");
   elem.setAttribute("IP",IP);
   elem.setAttribute("KinectPort",KinectPort);
   elem.setAttribute("CommandPort",CommandPort);
   elem.setAttribute("SendPort",SendPort);
   elem.setAttribute("CarduinoPort",CarduinoPort);
   elem.setAttribute("ArducopterPort",ArducopterPort);

   QString s;
   s="LOW_RES_ONLINE_COMPRESSION_WITHOUT_COLOR";

   if (CompressionProfile==LOW_RES_ONLINE_COMPRESSION_WITHOUT_COLOR)
	   s="LOW_RES_ONLINE_COMPRESSION_WITHOUT_COLOR";
   if (CompressionProfile==LOW_RES_ONLINE_COMPRESSION_WITH_COLOR)
	   s="LOW_RES_ONLINE_COMPRESSION_WITH_COLOR";
   if (CompressionProfile==MED_RES_ONLINE_COMPRESSION_WITHOUT_COLOR)
	   s="MED_RES_ONLINE_COMPRESSION_WITHOUT_COLOR";
   if (CompressionProfile==MED_RES_ONLINE_COMPRESSION_WITH_COLOR)
	   s="MED_RES_ONLINE_COMPRESSION_WITH_COLOR";
   if (CompressionProfile==HIGH_RES_ONLINE_COMPRESSION_WITHOUT_COLOR)
	   s="HIGH_RES_ONLINE_COMPRESSION_WITHOUT_COLOR";
   if (CompressionProfile==HIGH_RES_ONLINE_COMPRESSION_WITH_COLOR)
	   s="HIGH_RES_ONLINE_COMPRESSION_WITH_COLOR";
   if (CompressionProfile==LOW_RES_OFFLINE_COMPRESSION_WITHOUT_COLOR)
	   s="LOW_RES_OFFLINE_COMPRESSION_WITHOUT_COLOR";
   if (CompressionProfile==LOW_RES_OFFLINE_COMPRESSION_WITH_COLOR)
	   s="LOW_RES_OFFLINE_COMPRESSION_WITH_COLOR";
   if (CompressionProfile==MED_RES_OFFLINE_COMPRESSION_WITHOUT_COLOR)
	   s="MED_RES_OFFLINE_COMPRESSION_WITHOUT_COLOR";
   if (CompressionProfile==MED_RES_OFFLINE_COMPRESSION_WITH_COLOR)
	   s="MED_RES_OFFLINE_COMPRESSION_WITH_COLOR";
   if (CompressionProfile==HIGH_RES_OFFLINE_COMPRESSION_WITHOUT_COLOR)
	   s="HIGH_RES_OFFLINE_COMPRESSION_WITHOUT_COLOR";
   if (CompressionProfile==HIGH_RES_OFFLINE_COMPRESSION_WITH_COLOR)
	   s="HIGH_RES_OFFLINE_COMPRESSION_WITH_COLOR";
   
   elem.setAttribute("CompressionProfile",s);

   if (ShowStatistics)
   elem.setAttribute("ShowStatistics","1");
   else elem.setAttribute("ShowStatistics","0");
   elem.setAttribute("PointResolution",PointResolution);
   elem.setAttribute("OctreeResolution",OctreeResolution);
   if (DoVoxelGridDownDownSampling)
	   elem.setAttribute("DoVoxelGridDownDownSampling","1");
   else elem.setAttribute("DoVoxelGridDownDownSampling","0");
   elem.setAttribute("IFrameRate",IFrameRate);
   if (DoColorEncoding)
   elem.setAttribute("DoColorEncoding","1");
   else elem.setAttribute("DoColorencoding","0");
   elem.setAttribute("ColorBitResolution",ColorBitResolution);
   
   elem.setAttribute("CameraNumber",CameraNumber);
   elem.setAttribute("CameraFramesPerSecond",CameraFramesPerSecond);
   elem.setAttribute("CameraFrameWidth",CameraFrameWidth);
   elem.setAttribute("CameraFrameHeight",CameraFrameHeight);
   
   return elem;
  
  }

  void deserialize(const QDomElement& node){
  
	  IP=node.attribute("IP");
	  KinectPort=node.attribute("KinectPort");
	  CommandPort=node.attribute("CommandPort");
	  SendPort=node.attribute("SendPort");
	  CarduinoPort=node.attribute("CarduinoPort");
	  ArducopterPort=node.attribute("ArducopterPort");

	  QString s=node.attribute("CompressionProfile");
	  CompressionProfile=LOW_RES_ONLINE_COMPRESSION_WITHOUT_COLOR;

	   if(s=="LOW_RES_ONLINE_COMPRESSION_WITHOUT_COLOR")
	   CompressionProfile=LOW_RES_ONLINE_COMPRESSION_WITHOUT_COLOR;
	   if(s=="LOW_RES_ONLINE_COMPRESSION_WITH_COLOR")
	   CompressionProfile=LOW_RES_ONLINE_COMPRESSION_WITH_COLOR;
	   if(s=="MED_RES_ONLINE_COMPRESSION_WITHOUT_COLOR")
	   CompressionProfile=MED_RES_ONLINE_COMPRESSION_WITHOUT_COLOR;
	   if (s=="MED_RES_ONLINE_COMPRESSION_WITH_COLOR")
	   CompressionProfile=MED_RES_ONLINE_COMPRESSION_WITH_COLOR;
	   if (s=="HIGH_RES_ONLINE_COMPRESSION_WITHOUT_COLOR")
	   CompressionProfile=HIGH_RES_ONLINE_COMPRESSION_WITHOUT_COLOR;
	   if (s=="HIGH_RES_ONLINE_COMPRESSION_WITH_COLOR")
	   CompressionProfile=HIGH_RES_ONLINE_COMPRESSION_WITH_COLOR;
	   if (s=="LOW_RES_OFFLINE_COMPRESSION_WITHOUT_COLOR")
	   CompressionProfile=LOW_RES_OFFLINE_COMPRESSION_WITHOUT_COLOR;
	   if (s=="LOW_RES_OFFLINE_COMPRESSION_WITH_COLOR")
	   CompressionProfile=LOW_RES_OFFLINE_COMPRESSION_WITH_COLOR;
	   if (s=="MED_RES_OFFLINE_COMPRESSION_WITHOUT_COLOR")
	   CompressionProfile=MED_RES_OFFLINE_COMPRESSION_WITHOUT_COLOR;
	   if (s=="MED_RES_OFFLINE_COMPRESSION_WITH_COLOR")
		   CompressionProfile=MED_RES_OFFLINE_COMPRESSION_WITH_COLOR;
	   if (s=="HIGH_RES_OFFLINE_COMPRESSION_WITHOUT_COLOR")
		   CompressionProfile=HIGH_RES_OFFLINE_COMPRESSION_WITHOUT_COLOR;
	   if (s=="HIGH_RES_OFFLINE_COMPRESSION_WITH_COLOR")
		   CompressionProfile=HIGH_RES_OFFLINE_COMPRESSION_WITH_COLOR;

	  if (node.attribute("ShowStatistics")=="1")
		  ShowStatistics=true;
	  else ShowStatistics=false;
	  PointResolution=node.attribute("PointResolution").toDouble();
	  OctreeResolution=node.attribute("OctreeResolution").toDouble();
	  if (node.attribute("DoVoxelGridDownDownSampling")=="1")
		  DoVoxelGridDownDownSampling=true;
	  else DoVoxelGridDownDownSampling=false;
	  IFrameRate=node.attribute("IFrameRate").toInt();
	  if (node.attribute("DoColorEncoding")=="1")
		  DoColorEncoding=true;
	  else DoColorEncoding=false;
	  ColorBitResolution=node.attribute("ColorBitREsolution").toInt();
	  CameraNumber=node.attribute("CameraNumber").toInt();
	  CameraFramesPerSecond=node.attribute("CameraFramesPerSecond").toInt();
	  CameraFrameWidth=node.attribute("CameraFrameWidth").toInt();
	  CameraFrameHeight=node.attribute("CameraFrameHeight").toInt();
  }

public:
  QString IP;
  QString KinectPort;
  QString CommandPort;
  QString SendPort;
  QString CarduinoPort;
  QString ArducopterPort;

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