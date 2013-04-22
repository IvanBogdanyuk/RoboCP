#pragma once
#include <cv.h>
#include <highgui.h>

#define MAX_COUNT 400 

typedef struct Vector
{
	CvPoint2D32f Beginning, End;
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

