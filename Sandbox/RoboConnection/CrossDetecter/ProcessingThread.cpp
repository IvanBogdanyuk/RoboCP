#include "ProcessingThread.h"

ProcessingThread::ProcessingThread(TSDataHandler<Mat> *dh_in, TSDataHandler<Point2f> *dh_out)
{
  // инициализация
  this->mDataHandler_in = dh_in;

  // если выходной буфер не указан, пишем во входной
  if (dh_out == NULL)
    this->mDataHandler_out = NULL;
  else
    this->mDataHandler_out = dh_out;

  object = { Point2f(50, 0), Point2f(0, 50), Point2f(100, 50), Point2f(50, 100),
    Point2f(45, 45), Point2f(55, 55), Point2f(45, 55), Point2f(55, 45) };
  frame = { Point3f(0, 0, 0), Point3f(50, 0, 0), Point3f(0, 50, 0), Point3f(0, 0, 50) };

  FileStorage fs;
  fs.open("cam.xml", FileStorage::READ);
  fs["Camera_Matrix"] >> mIntrinsics;
  fs["Distortion_Coefficients"] >> mDistortion;
}

void cameraPoseFromHomography(const Mat& H, Mat& rvec, Mat& tvec)
{
  rvec = Mat::eye(3, 3, CV_32FC1);      // 3x4 matrix, the camera pose
  tvec = Mat::zeros(3, 1, CV_32FC1);
  float norm1 = (float)norm(H.col(0));
  float norm2 = (float)norm(H.col(1));
  float tnorm = (norm1 + norm2) / 2.0f; // Normalization value

  Mat p1 = H.col(0);       // Pointer to first column of H
  Mat p2 = rvec.col(0);    // Pointer to first column of pose (empty)

  cv::normalize(p1, p2);   // Normalize the rotation, and copies the column to pose

  p1 = H.col(1);           // Pointer to second column of H
  p2 = rvec.col(1);        // Pointer to second column of pose (empty)

  cv::normalize(p1, p2);   // Normalize the rotation and copies the column to pose

  p1 = rvec.col(0);
  p2 = rvec.col(1);

  Mat p3 = p1.cross(p2);   // Computes the cross-product of p1 and p2
  Mat c2 = rvec.col(2);    // Pointer to third column of pose
  p3.copyTo(c2);       // Third column is the crossproduct of columns one and two

  tvec.col(0) = H.col(2) / tnorm;  //vector t [R|t] is the last column of pose
}

void ProcessingThread::run()
{
  Mat orig, previmg;
  vector<Point2f> prev_pts, orig_pts;
  Point2f offset(0, 0);

  TimerCreate();
  while (isRunning())
  {
    // засечение времени
    TimerUpdate();
    while (mDataHandler_in->Read(orig))
    {

      // вызов обработчика
      mOpticalFlowHandle(previmg, orig, prev_pts, orig_pts, offset);

    }
    // вывод времени
    TimerElapsed();

    yieldCurrentThread();
  }
}

ProcessingThread::~ProcessingThread()
{
}

static inline double angle(Point pt1, Point pt2, Point pt0)
{
  double dx1 = pt1.x - pt0.x;
  double dy1 = pt1.y - pt0.y;
  double dx2 = pt2.x - pt0.x;
  double dy2 = pt2.y - pt0.y;
  return (dx1*dx2 + dy1*dy2) / sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2) + 1e-10);
}

template<typename T>
bool CheckCrossParams(vector<T> cross)
{
  for (int j = 0; j < cross.size() - 3; j++)
  {
    double ang1 = angle(cross[j], cross[j + 1], cross[j + 2]);
    double ang2 = angle(cross[j + 1], cross[j + 2], cross[j + 3]);
    if (ang1 > 0.7)
    {
      if (!(ang1 > 0.7 && ang2 > 0.3))
      {
        return false;
      }
    }
  }

  // проверка соотношений ширины/длины сторон креста
  double length_top = (abs(cross[0].x - cross[2].x) + abs(cross[0].y - cross[2].y)) / 2;
  double length_bot = (abs(cross[2].x - cross[3].x) + abs(cross[2].y - cross[3].y)) / 2;
  double length_left = (abs(cross[1].x - cross[2].x) + abs(cross[1].y - cross[2].y)) / 2;
  double length_right = (abs(cross[2].x - cross[4].x) + abs(cross[2].y - cross[4].y)) / 2;

  double eps = 0.3;
  if ((length_top / length_bot) - 1 > eps) return false;
  if ((length_left / length_right) - 1 > eps) return false;

  double ratio1 = ((abs(length_top - length_bot) / length_top + abs(length_top - length_bot) / length_bot)) / 2;
  double ratio2 = ((abs(length_left - length_right) / length_left + abs(length_left - length_right) / length_right)) / 2;

  double avgRatio = (ratio1 + ratio2) / 2;
  //printf(" %f %f %f %f \n", length_top, length_bot, length_left, length_right);

  if (abs(ratio1 + ratio2) / 2 > 0.2)
  {
    return false;
  }
  return true;
}

void ProcessingThread::mOpticalFlowHandle(Mat &previmg, Mat lastimg, vector<Point2f> &prev_pts, vector<Point2f> &orig_pts, Point2f &offset)
{
  // создание вывода для отладки
  DBG_InitOutputImage();
  DBG_CreateOutputFromImage(lastimg);
  Mat nextimg, mask, m_error;
  vector<Point2f> next_pts, tracked_pts, orig_pts_new;
  vector<uchar> m_status;
  cvtColor(lastimg, nextimg, CV_BGR2GRAY);
  Point2f center = Point2f(previmg.size().width / 2, previmg.size().height / 2);
  vector<Point> prevptsasd(prev_pts.begin(), prev_pts.end());
  // алгоритм обнаружения на данном этапе всегда вернёт 8 точек
  if (orig_pts.size() != 5)
  {
    prev_pts.clear();

    // в случае обнаружения креста задаём начальные данные для OpticalFlow
    //goodFeaturesToTrack(nextimg, prev_pts, 8, 0.01, 2);
    if (mCrossDetect(nextimg, prev_pts))
    {
      cvtColor(lastimg, previmg, CV_BGR2GRAY);
      orig_pts = prev_pts;
    }
  }
  else
  {
    // просчёт смещения точек
    if (prev_pts.size() > 0 && !previmg.empty() && CheckCrossParams(prevptsasd))
    {
      calcOpticalFlowPyrLK(previmg, nextimg, prev_pts, next_pts, m_status, m_error);
      offset = next_pts[2] - center;
    }

    // проверка наличия и запись нового положения точек
    for (int i = 0; i < m_status.size(); i++)
    {
      int j = 1;
      if (m_status[i])
      {
        tracked_pts.push_back(next_pts[i]);
        DBG_DrawOutputCircle(next_pts[i]);
        orig_pts_new.push_back(orig_pts[i]);
      }
    }
    vector<Point2f> imgpts;
    // вывод новых данных в соответствующие переменные
    if (tracked_pts.size() == object.size())
    {
      //Mat homo = cv::findHomography(orig_pts, tracked_pts);
      Mat rvec, tvec;
      //cameraPoseFromHomography(homo, rvec, tvec);

      //warpPerspective(OUTPUT_IMG_VAR, OUTPUT_IMG_VAR, homo.inv(), OUTPUT_IMG_VAR.size());
      //solvePnP(object, tracked_pts, intrinsics,distortion);
      //projectPoints(frame, rvec, tvec, mIntrinsics, mDistortion, imgpts);
    }
    for each(Point2f pt in imgpts)
      DBG_DrawOutputCircle(pt);
    orig_pts = orig_pts_new;
    prev_pts = tracked_pts;
    nextimg.copyTo(previmg);
  }
  //DBG_WriteFrame(mDataHandler_out);
  mDataHandler_out->Write(offset);

}

Point intersection(Point p1, Point p2, Point p3, Point p4)
{

  double x1 = p1.x, x2 = p2.x, x3 = p3.x, x4 = p4.x;
  double y1 = p1.y, y2 = p2.y, y3 = p3.y, y4 = p4.y;

  double d = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
  // If d is zero, there is no intersection
  if (d == 0) return NULL;

  // Get the x and y
  double pre = (x1*y2 - y1*x2), post = (x3*y4 - y3*x4);
  double x = (pre * (x3 - x4) - (x1 - x2) * post) / d;
  double y = (pre * (y3 - y4) - (y1 - y2) * post) / d;

  // Check if the x and y coordinates are within both lines
  if (x < min(x1, x2) || x > max(x1, x2) ||
    x < min(x3, x4) || x > max(x3, x4)) return NULL;
  if (y < min(y1, y2) || y > max(y1, y2) ||
    y < min(y3, y4) || y > max(y3, y4)) return NULL;

  // Return the point of intersection
  Point ret;
  ret.x = x;
  ret.y = y;
  return ret;
}

bool ProcessingThread::mCrossDetect(Mat gray, vector<Point2f> &cross)
{
  RotatedRect rRect;
  int aPDcoeff_int = 20;
  double tresholdmin = 0.6;
  int tresholdmin_int = 6;
  int tresholdmax_int = 6;
  int tresholdCannyMin = 1100;
  int tresholdCannyMax = 1500;
  bool iscross = true;

  double aPDcoeff = (double)aPDcoeff_int / 1000;
  if (aPDcoeff <= 0) aPDcoeff = 0.01;

  Mat bw;
  Mat blurr;
  vector<Mat> contours;
  vector<Point> approx;
  vector<Point>hull;

  blur(gray, gray, Point(3, 3));

  //threshold(gray, bw, tresholdCannyMin, tresholdCannyMax, THRESH_OTSU);

  Canny(gray, bw, tresholdCannyMin, tresholdCannyMax, 5);

  findContours(bw.clone(), contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

  //vector<vector<Point> >hull(contours.size());
  //aPDcoeff = 0.01;
  for (int i = 0; i < contours.size(); i++)
  {
    //arcLength(Mat(contours[i]), true)*aPDcoeff
    approxPolyDP(Mat(contours[i]), approx, arcLength(Mat(contours[i]), true)*aPDcoeff, true);
    //!(approx.size() == 8)
    if (fabs(contourArea(contours[i])) < 200 || isContourConvex(approx))
      continue;

    convexHull(Mat(contours[i]), hull, false);

    //сравнение площади контура и convexHull
    double ColoredPercentage = double(contourArea(contours[i])) / double(contourArea(hull));
    //printf("ColoredPercentage: %f \n", ColoredPercentage);
    if (ColoredPercentage > 0.35 || ColoredPercentage < 0.2) continue;

    //нахождение углов креста
    int max_x = 0;
    int max_y = 0;
    int min_x = gray.size().width + 1;
    int min_y = gray.size().height + 1;
    int i1, i2, i3, i4;
    for (int i = 0; i < hull.size(); i++)
    {
      if (hull[i].x > max_x) { max_x = hull[i].x; i1 = i; }
      if (hull[i].y > max_y) { max_y = hull[i].y; i2 = i; }
      if (hull[i].x < min_x) { min_x = hull[i].x; i3 = i; }
      if (hull[i].y < min_y) { min_y = hull[i].y; i4 = i; }
    }
    Point inter = intersection(Point(min_x, hull[i3].y), Point(max_x, hull[i1].y), Point(hull[i4].x, min_y), Point(hull[i2].x, max_y));

    cross.clear();
    cross.push_back(Point(hull[i2].x, max_y));
    cross.push_back(Point(min_x, hull[i3].y));
    cross.push_back(inter);
    cross.push_back(Point(hull[i4].x, min_y));
    cross.push_back(Point(max_x, hull[i1].y));

    if (!CheckCrossParams(cross))
    {
      cross.clear();
      continue;
    }
    else
    {
      return true;
    }

    LineIterator it(gray, Point(min_x, hull[i3].y), Point(max_x, hull[i1].y), 8);
    vector<Vec3b> buf(it.count);
    vector<Point> points(it.count);

    //line(display, vertices[0], vertices[2], Scalar(255, 255, 0));
    int avg_sum = 0;
    for (int i = 0; i < it.count; i++, ++it)
    {
      points[i] = it.pos();
      Vec3b colour = gray.at<Vec3b>(points[i]);
      avg_sum += (colour.val[0] + colour.val[1] + colour.val[2]) / 3;
      if (it.count - i - 1 == 0)  avg_sum = avg_sum / i;
    }
  }
}
