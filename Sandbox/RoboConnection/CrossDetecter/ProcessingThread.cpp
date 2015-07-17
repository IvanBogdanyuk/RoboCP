#include "ProcessingThread.h"

ProcessingThread::ProcessingThread(TSDataHandler<Mat> *dh_in, TSDataHandler<Mat> *dh_out)
{
  // инициализация
  this->mDataHandler_in = dh_in;

  // если выходной буфер не указан, пишем во входной
  if (dh_out == NULL)
    this->mDataHandler_out = this->mDataHandler_in;
  else
    this->mDataHandler_out = dh_out;
}

void ProcessingThread::run()
{
  Mat orig, previmg;
  vector<Point2f> prev_pts, orig_pts;

  TimerCreate();

  while (isRunning())
  {
    // засечение времени
    TimerUpdate();
    while (mDataHandler_in->Read(orig))
    {

      // вызов обработчика
      mOpticalFlowHandle(previmg, orig, prev_pts, orig_pts);

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

bool CheckCrossParams(vector<Point> cross)
{
  //фильтр контуров по параметрам углов
  if (cross.size() != 8) return false;
  for (int j = 0; j < cross.size() - 3; j++)
  {
    double ang1 = angle(cross[j], cross[j + 1], cross[j + 2]);
    double ang2 = angle(cross[j + 1], cross[j + 2], cross[j + 3]);
    if (ang1 > 0.7)
    {
      if (!(ang1 > 0.7 && ang2 < 0.3))
      {
        return false;
      }
    }
  }

  int x[8], y[8];
  for (int i = 0; i < 8; i++)
  {
    x[i] = cross[i].x;
    y[i] = cross[i].y;
  }

  // проверка соотношений ширины/длины сторон креста
  double length_top = (((abs(x[0] - x[1]) + abs(x[0] - x[7])) / 2) + ((abs(y[0] - y[1]) + abs(y[0] - y[7])) / 2)) / 2;
  double length_bot = (((abs(x[3] - x[4]) + abs(x[4] - x[5])) / 2) + ((abs(y[3] - y[4]) + abs(y[4] - y[5])) / 2)) / 2;
  double ratio1 = ((((length_top + length_bot) / length_top - 0.5) + ((length_top + length_bot) / length_bot - 0.5))) / 2 - 0.5;

  double length_left = (((abs(x[2] - x[1]) + abs(x[2] - x[3])) / 2) + ((abs(y[2] - y[1]) + abs(y[2] - y[3])) / 2)) / 2;
  double length_right = (((abs(x[6] - x[7]) + abs(x[6] - x[5])) / 2) + ((abs(y[6] - y[7]) + abs(y[6] - y[5])) / 2)) / 2;
  double ratio2 = ((((length_left + length_right) / length_left - 0.5) + ((length_left + length_right) / length_right - 0.5))) / 2 - 0.5;

  if (abs((ratio1 + ratio2) / 2 - 1) > 0.2)
  {
    return false;
  }
  return true;
}

void ProcessingThread::mOpticalFlowHandle(Mat &previmg, Mat lastimg, vector<Point2f> &prev_pts, vector<Point2f> &orig_pts)
{
  // создание вывода для отладки
  DBG_InitOutputImage();
  DBG_CreateOutputFromImage(lastimg);
  Mat nextimg, mask, m_error;
  vector<Point2f> next_pts, tracked_pts, orig_pts_new;
  vector<uchar> m_status;
  cvtColor(lastimg, nextimg, CV_BGR2GRAY);
  vector<Point> prev_pts_vp(8);


  // алгоритм обнаружения на данном этапе всегда вернёт 8 точек
  if (orig_pts.size() != 8)
  {
    prev_pts.clear();

    // в случае обнаружения креста задаём начальные данные для OpticalFlow
    if (mCrossDetect(nextimg, prev_pts))
    {
      cvtColor(lastimg, previmg, CV_BGR2GRAY);
      orig_pts = prev_pts;
    }
  }
  else
  {
    for (int i = 0; i < prev_pts.size(); i++)
    {
      prev_pts_vp[i] = prev_pts[i];
    }
    // просчёт смещения точек
    if (prev_pts.size() > 0 && !previmg.empty() && CheckCrossParams(prev_pts_vp))
    {
      calcOpticalFlowPyrLK(previmg, nextimg, prev_pts, next_pts, m_status, m_error);
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

    // вывод новых данных в соответствующие переменные


    orig_pts = orig_pts_new;
    prev_pts = tracked_pts;
    nextimg.copyTo(previmg);
  }
  DBG_WriteFrame(mDataHandler_out);

}


bool ProcessingThread::mCrossDetect(Mat gray, vector<Point2f> &cross)
{
  double tresholdmin = 0.6;
  int tresholdmin_int = 6;
  int tresholdmax_int = 6;
  int tresholdCannyMin = 1400;
  int tresholdCannyMax = 1500;

  bool found = true;
  vector<Mat> contours;
  vector<Point> approx;

  // фильтр Кенни
  Mat bw;
  Canny(gray, bw, tresholdCannyMin, tresholdCannyMax, 5);

  // нахождение контуров
  findContours(bw, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

  for (int i = 0; i < contours.size(); i++)
  {
    // приближение контуров ломаными
    approxPolyDP(Mat(contours[i]), approx, arcLength(Mat(contours[i]), true)*0.02, true);

    if (CheckCrossParams(approx))
    {
      for (int i = 0; i < approx.size(); i++)
        cross.push_back((Point2i)approx[i]);
      return true;
    }

  }
  return found;
}
