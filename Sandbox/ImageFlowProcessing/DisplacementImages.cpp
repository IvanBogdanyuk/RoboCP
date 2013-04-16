#include "DisplacementImages.h"


DisplacementImages::DisplacementImages(void)
{
	Vectors = new Vector[MAX_COUNT+1];
}


DisplacementImages::~DisplacementImages(void)
{
	//delete Vectors;
}
