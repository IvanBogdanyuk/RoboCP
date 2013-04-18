#pragma once
#include <cv.h>
#include <highgui.h>

#define MAX_COUNT 400 

typedef struct Vector
{
	CvPoint2D32f Beginning, End;
}
Vector;

class DisplacementImages
{
public:
	Vector *Vectors;
	int NumVectors;
	DisplacementImages(void);
	~DisplacementImages(void);
};

