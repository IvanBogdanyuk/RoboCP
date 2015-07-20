#pragma once

#include <QtCore/qthread.h>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/video/video.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/calib3d/calib3d.hpp>
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
  ProcessingThread(TSDataHandler<Mat> *dh_in, TSDataHandler<Mat> *dh_out = NULL);
  ~ProcessingThread();
private:
  bool mCrossDetect(Mat img, vector<Point2f> &cross);
  void mOpticalFlowHandle(Mat &previmg, Mat lastimg, vector<Point2f> &prev_pts, vector<Point2f> &orig_pts);
  void run();
  TSDataHandler<Mat> *mDataHandler_in, *mDataHandler_out;
  vector<Point2f> object;
  vector<Point3f> frame;
  Mat mIntrinsics, mDistortion;
};
