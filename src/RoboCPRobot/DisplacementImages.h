#pragma once
#include <cv.h>
#include <highgui.h>
/*
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/serialization/nvp.hpp>
*/

#include "QtCore\qdatastream.h"

#include "QtXml\qdom.h"

#define MAX_COUNT 400 

typedef struct Vector
{
	/*
	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar & BOOST_SERIALIZATION_NVP(BeginningX);
		ar & BOOST_SERIALIZATION_NVP(BeginningY);
		ar & BOOST_SERIALIZATION_NVP(EndX);
		ar & BOOST_SERIALIZATION_NVP(EndY);
		ar & BOOST_SERIALIZATION_NVP(Length);
	}*/

	
	QDomElement serialize(QDomDocument& doc){
	
		QDomElement elem = doc.createElement("Vector");
		elem.setAttribute("BeginingX",BeginningX);
		elem.setAttribute("EndX",EndX);
		elem.setAttribute("BeginningY",BeginningY);
		elem.setAttribute("EndY",EndY);
		elem.setAttribute("Length",Length);

		return elem;
	}

	void deserialize(const QDomElement& node){
	
		BeginningX=node.attribute("BeginningX").toFloat();
		EndX=node.attribute("EndX").toFloat();
		BeginningY=node.attribute("BeginningY").toFloat();
		EndY=node.attribute("EndY").toFloat();
		Length=node.attribute("Length").toDouble();
	}


	float BeginningX, EndX, BeginningY, EndY;
	double Length;
}
Vector;

class DisplacementImages
{
public:
	Vector *Vectors;
	Vector Motion;
	int NumVectors;
	void CountMotion(void);
	double Dist(CvPoint2D32f Point1, CvPoint2D32f Point2);
	DisplacementImages(void);
	~DisplacementImages(void);
};

