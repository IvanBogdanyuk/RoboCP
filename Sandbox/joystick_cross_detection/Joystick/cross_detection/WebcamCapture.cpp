#include "WebcamCapture.h"

void WebcamCapture::run()
{

	Mat orig;
	TimerCreate();
	while (isRunning())
	{
		TimerUpdate();
		vector<unsigned char> m_status;
		vector<float>         m_error;
		vector<Point2f> trackedPts;
		if (vc->isOpened())
		{
			if (vc->read(orig))
				dh_out->WriteFrame(orig);
		}
		else
		{
			this->exit(-1);
			
		}
		
		TimerElapsed();
	}
	vc->release();
	this->exit(0);
}

WebcamCapture::WebcamCapture(TSDataHandler *dh_out)
{
	// init our capture
	this->vc = new VideoCapture;
	
	vc->open(0); // open default device
	vc->set(CV_CAP_PROP_FRAME_HEIGHT, 240);
	vc->set(CV_CAP_PROP_FRAME_WIDTH, 320);
	this->dh_out = dh_out;
}

WebcamCapture::~WebcamCapture()
{
	vc->release();
}
