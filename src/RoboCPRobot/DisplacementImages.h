#pragma once
#include <cv.h>
#include <highgui.h>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/serialization/nvp.hpp>

#define MAX_COUNT 400 

typedef struct Vector
{
	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar & BOOST_SERIALIZATION_NVP(BeginningX);
		ar & BOOST_SERIALIZATION_NVP(BeginningY);
		ar & BOOST_SERIALIZATION_NVP(EndX);
		ar & BOOST_SERIALIZATION_NVP(EndY);
		ar & BOOST_SERIALIZATION_NVP(Length);
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

