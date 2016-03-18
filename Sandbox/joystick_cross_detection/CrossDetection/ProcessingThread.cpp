#include "ProcessingThread.h"


ProcessingThread::ProcessingThread(TSDataHandler *dh_in, TSDataHandler *dh_out)
{
	this->dh_in = dh_in;
	if (dh_out == nullptr)
		this->dh_out = this->dh_in;
	else
		this->dh_out = dh_out;
}

void ProcessingThread::run()
{
	Mat orig, previmg, output, mask;
	vector<Point2f> prev_pts, orig_pts;
	vector<uchar> m_status;
	Mat m_error;

	TimerCreate();

	while (isRunning())
	{
		while (dh_in->ReadFrame(orig))
		{
			TimerUpdate();
			OpticalFlowHandle(previmg, orig, prev_pts, orig_pts);
			TimerElapsed();
		}
		yieldCurrentThread();
	}
}

ProcessingThread::~ProcessingThread()
{
}

void ProcessingThread::OpticalFlowHandle(Mat &previmg, Mat lastimg, vector<Point2f> &prev_pts, vector<Point2f> &orig_pts)
{
	DBG_InitOutputImage();
	DBG_CreateOutputFromImage(lastimg);
	Mat nextimg, mask, m_error;
	vector<Point2f> next_pts, tracked_pts, orig_pts_new;
	vector<uchar> m_status;

	cvtColor(lastimg, nextimg, CV_BGR2GRAY);
	if (orig_pts.size() != 8)
	{
		prev_pts.clear();
		
		//goodFeaturesToTrack(nextimg, prev_pts, 10, 0.05, 0.2, mask);
		if (CrossDetect(nextimg, prev_pts))
		{
			cvtColor(lastimg, previmg, CV_BGR2GRAY);
			orig_pts = prev_pts;
		}
	}
	else
	{
		//cvtColor(lastimg, nextimg, CV_BGR2GRAY);
		if (prev_pts.size() > 0 && !previmg.empty())
		{
			calcOpticalFlowPyrLK(previmg, nextimg, prev_pts, next_pts, m_status, m_error);
		}
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

		orig_pts = orig_pts_new;
		prev_pts = tracked_pts;
		nextimg.copyTo(previmg);
		DBG_WriteFrame(dh_out);
	}

}

static inline double angle(Point pt1, Point pt2, Point pt0)
{
	double dx1 = pt1.x - pt0.x;
	double dy1 = pt1.y - pt0.y;
	double dx2 = pt2.x - pt0.x;
	double dy2 = pt2.y - pt0.y;
	return (dx1*dx2 + dy1*dy2) / sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2) + 1e-10);
}

bool ProcessingThread::CrossDetect(Mat gray, vector<Point2f> &cross)
{
	double tresholdmin = 0.6;
	int tresholdmin_int = 6;
	int tresholdmax_int = 6;
	int tresholdCannyMin = 1400;
	int tresholdCannyMax = 1500;

	bool found = true;
	vector<Mat> contours;
	vector<Point> approx;

	//Mat gray;
	//cvtColor(img, gray, CV_BGR2GRAY);

	Mat bw;
	Canny(gray, bw, tresholdCannyMin, tresholdCannyMax, 5);

	findContours(bw.clone(), contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

	for (int i = 0; i < contours.size(); i++)
	{
		approxPolyDP(Mat(contours[i]), approx, arcLength(Mat(contours[i]), true)*0.02, true);

		if (fabs(contourArea(contours[i])) < 100 || isContourConvex(approx) || (approx.size() != 8))
			continue;

		double x0 = approx[0].x;
		double x1 = approx[1].x;
		double x2 = approx[2].x;
		double x3 = approx[3].x;
		double x4 = approx[4].x;
		double x5 = approx[5].x;
		double x6 = approx[6].x;
		double x7 = approx[7].x;

		double y0 = approx[0].y;
		double y1 = approx[1].y;
		double y2 = approx[2].y;
		double y3 = approx[3].y;
		double y4 = approx[4].y;
		double y5 = approx[5].y;
		double y6 = approx[6].y;
		double y7 = approx[7].y;

		double length_top = (((abs(x0 - x1) + abs(x0 - x7)) / 2) + ((abs(y0 - y1) + abs(y0 - y7)) / 2)) / 2;
		double length_bot = (((abs(x3 - x4) + abs(x4 - x5)) / 2) + ((abs(y3 - y4) + abs(y4 - y5)) / 2)) / 2;
		double ratio1 = ((((length_top + length_bot) / length_top - 0.5) + ((length_top + length_bot) / length_bot - 0.5))) / 2 - 0.5;

		double length_left = (((abs(x2 - x1) + abs(x2 - x3)) / 2) + ((abs(y2 - y1) + abs(y2 - y3)) / 2)) / 2;
		double length_right = (((abs(x6 - x7) + abs(x6 - x5)) / 2) + ((abs(y6 - y7) + abs(y6 - y5)) / 2)) / 2;
		double ratio2 = ((((length_left + length_right) / length_left - 0.5) + ((length_left + length_right) / length_right - 0.5))) / 2 - 0.5;

		if (abs((ratio1 + ratio2) / 2 - 1) > 0.2)
		{
			found = false;
			continue;
		}

		for (int j = 0; j < approx.size() - 3; j++){
			double ang1 = angle(approx[j], approx[j + 1], approx[j + 2]);
			double ang2 = angle(approx[j + 1], approx[j + 2], approx[j + 3]);
			//printf("ang1: %f\t, ang2: %f \n", ang1, ang2);
			if (ang1 > 0.7){
				if (!(ang1 > 0.7 && ang2 < 0.3))
				{
					found = false;
					continue;
				}
			}
		}

		if (found)
		{
			for each(Point pt in approx)
				cross.push_back((Point2f)pt);
			return true;
		}

	}
	return found;
}
