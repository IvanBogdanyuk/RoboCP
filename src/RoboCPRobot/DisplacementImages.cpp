#include "DisplacementImages.h"


DisplacementImages::DisplacementImages(void)
{
	Vectors = new Vector[MAX_COUNT+1];
}

double DisplacementImages::Dist(CvPoint2D32f Point1, CvPoint2D32f Point2)
{
	return sqrt((Point1.x - Point2.x)*(Point1.x - Point2.x) + (Point1.y - Point2.y)*(Point1.y - Point2.y));
}

void DisplacementImages::CountMotion()
{
	Vector Min;
	Min.Length = -1;
	for(int i=0; i< NumVectors; i++){
		if(Min.Length > Vectors[i].Length || Min.Length == -1)
		{
			Min = Vectors[i];
		}
	}
	Motion = Min;
}


DisplacementImages::~DisplacementImages(void)
{
	//delete Vectors;
}
