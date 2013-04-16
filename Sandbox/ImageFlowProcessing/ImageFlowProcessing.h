#pragma once
#include <cv.h>
#include <highgui.h>
#include "DisplacementImages.h"

#define PI 3.1415  


class ImageFlowProcessing
{
public:
	void CountDisplacement(IplImage *Img1, IplImage *Img2, DisplacementImages *Displacement);
	void ShowOpticalFlow(CvCapture * capture);
	ImageFlowProcessing(void);
	~ImageFlowProcessing(void);
};

