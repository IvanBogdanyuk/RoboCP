#pragma once

#include <QtCore\qthread.h>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>

#include "TSDataHandler.h"
#include "TimingsDebug.h"

using std::vector;
using namespace cv;

class WebcamCapture : public QThread
{
public:
	WebcamCapture(TSDataHandler *dh_out);
	WebcamCapture();
	~WebcamCapture();
private:
	cv::VideoCapture *vc;
	void run();
	TSDataHandler *dh_out;
};