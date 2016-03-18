#pragma once
/*
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/serialization/nvp.hpp>
*/
#include "QtXml\qdom.h"

struct Point3d
{
	/*
  template <typename Archive>
  void serialize(Archive& ar, const unsigned int version)
  {
    ar & BOOST_SERIALIZATION_NVP(x);
    ar & BOOST_SERIALIZATION_NVP(y);
    ar & BOOST_SERIALIZATION_NVP(z);
  }*/
  
 QDomElement serialize(QDomDocument& doc)
	{
    QDomElement elem = doc.createElement("Point3d");
	elem.setAttribute("x",x);
	elem.setAttribute("y",y);
	elem.setAttribute("z",z);
	
	return elem;
 }

 void deserialize(const QDomElement& node){
 
	 x=node.attribute("x").toShort();
	 y=node.attribute("y").toShort();
	 z=node.attribute("z").toShort();
  }

  short x;
  short y; 
  short z;
};