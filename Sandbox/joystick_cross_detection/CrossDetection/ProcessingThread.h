#pragma once

#include <QtCore\qthread.h>
#include <opencv2\core\core.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\video\video.hpp>
#include "TSDataHandler.h"
#include "TimingsDebug.h"
#include "DebugOutput.h"

using namespace std;
using namespace cv;
using std::vector;

class ProcessingThread :
	public QThread
{
public:
	ProcessingThread(TSDataHandler *dh_in, TSDataHandler *dh_out = nullptr);
	~ProcessingThread();
private:
	bool CrossDetect(Mat img, vector<Point2f> &cross);
	void OpticalFlowHandle(Mat &previmg, Mat lastimg, vector<Point2f> &prev_pts, vector<Point2f> &orig_pts);
	void run();
	TSDataHandler *dh_in, *dh_out;
};