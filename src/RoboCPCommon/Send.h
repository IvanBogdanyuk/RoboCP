#pragma once
#define _XOPEN_SOURCE
#include <time.h>
#include "Point3d.h"
#include "DisplacementImages.h"

#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/shared_ptr.hpp>

#include "QtXml\qdom.h"
#include <time.h>


class Send
{

private:
  
  friend class boost::serialization::access;
	
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version) {
	
	ar & BOOST_SERIALIZATION_NVP(TopSonicSensor);
	ar & BOOST_SERIALIZATION_NVP(FrontSonicSensor);
	ar & BOOST_SERIALIZATION_NVP(LeftSonicSensor);
	ar & BOOST_SERIALIZATION_NVP(RightSonicSensor);
	ar & BOOST_SERIALIZATION_NVP(BackSonicSensor);

	ar & BOOST_SERIALIZATION_NVP(Roll);
	ar & BOOST_SERIALIZATION_NVP(Pitch);
	ar & BOOST_SERIALIZATION_NVP(Yaw);
	ar & BOOST_SERIALIZATION_NVP(AltitudeSonic);
	ar & BOOST_SERIALIZATION_NVP(AltitudeBarometer);

	ar & BOOST_SERIALIZATION_NVP(Acceleration);
	ar & BOOST_SERIALIZATION_NVP(PacketType);
  ar & BOOST_SERIALIZATION_NVP(Time);
	ar & BOOST_SERIALIZATION_NVP(Motion);
  }
  

  QDomElement serialize(QDomDocument& doc){
  
	  QDomElement elem = doc.createElement("Send");
	  elem.setAttribute("TopSonicSensor",TopSonicSensor);
	  elem.setAttribute("FrontSonicSensor",FrontSonicSensor);
	  elem.setAttribute("LeftSonicSensor",LeftSonicSensor);
	  elem.setAttribute("RightSonicSensor",RightSonicSensor);
	  elem.setAttribute("BackSonicSensor",BackSonicSensor);

	  elem.setAttribute("Roll",Roll);
	  elem.setAttribute("Pitch",Pitch);
	  elem.setAttribute("Yaw",Yaw);
	  elem.setAttribute("AltitudeSonic",AltitudeSonic);
	  elem.setAttribute("AltitudeBarometer",AltitudeBarometer);
	  
	  elem.setAttribute("AccelerationX",Acceleration.x);
	  elem.setAttribute("AccelerationY",Acceleration.y);
	  elem.setAttribute("AccelerationZ",Acceleration.z);
	  elem.setAttribute("PacketType",PacketType);
	  elem.setAttribute("Time",Time);
	  elem.setAttribute("MotionBegX",Motion.BeginningX);
	  elem.setAttribute("MotionEndX",Motion.EndX);
	  elem.setAttribute("MotionBegY",Motion.BeginningY);
	  elem.setAttribute("MotionEndY",Motion.EndY);
	  elem.setAttribute("MotionLength",Motion.Length);

	  return elem;
  
  }

  void deserialize(const QDomElement& node){
  
	  TopSonicSensor=node.attribute("TopSonicSensor").toShort();
	  FrontSonicSensor=node.attribute("FrontSonicSensor").toShort();
	  LeftSonicSensor=node.attribute("LeftSonicSensor").toShort();
	  RightSonicSensor=node.attribute("RightSonicSensor").toShort();
	  BackSonicSensor=node.attribute("BackSonicSensor").toShort();
	  
	  Roll=node.attribute("Roll").toFloat();
	  Pitch=node.attribute("Pitch").toFloat();
	  Yaw=node.attribute("Yaw").toFloat();
	  AltitudeSonic=node.attribute("AltitudeSonic").toFloat();
	  AltitudeBarometer=node.attribute("AltitudeBarometer").toFloat();
	  
	  
	  Acceleration.x=node.attribute("AccelerationX").toShort();
	  Acceleration.y=node.attribute("AccelerationY").toShort();
	  Acceleration.z=node.attribute("AccelerationZ").toShort();
	  PacketType=node.attribute("PacketType").toInt();
	  
	  struct std::tm tm;
	  std::istringstream ss(node.attribute("Time").toStdString());
      ss >> std::get_time(&tm, "%H:%M:%S"); 
	  Time=mktime(&tm);

	  Motion.BeginningX=node.attribute("MotionBegX").toFloat();
	  Motion.EndX=node.attribute("MotionEndX").toFloat();
	  Motion.BeginningY=node.attribute("MotionBegY").toFloat();
	  Motion.EndY=node.attribute("MotionEndY").toFloat();
	  Motion.Length=node.attribute("Length").toDouble();
  }


public:
  unsigned short TopSonicSensor;
  unsigned short FrontSonicSensor;
  unsigned short LeftSonicSensor;
  unsigned short RightSonicSensor;
  unsigned short BackSonicSensor;

  float Roll;
  float Pitch;
  float Yaw;
  float AltitudeSonic;
  float AltitudeBarometer;
    
  Point3d Acceleration;
  int PacketType;
  time_t Time;
  //IplImage *Frame;
  Vector Motion;

  Send(void);
  ~Send(void); 

};

//BOOST_SERIALIZATION_SHARED_PTR(Send)