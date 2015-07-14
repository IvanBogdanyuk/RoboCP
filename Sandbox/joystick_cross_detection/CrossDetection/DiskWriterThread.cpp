#include "DiskWriterThread.h"


DiskWriterThread::DiskWriterThread(TSDataHandler *dh)
{
	this->dh = dh;
	running = true;
	vw.open("asd.avi", -1, 25, cv::Size(640, 480));
}

void DiskWriterThread::run()
{
	std::string name = "output";
	while (running)
	{
		QMutex m;
		m.lock();
		cv::Mat frame;
		while (running && dh->ReadFrame(frame))
		{
			vw.write(frame);
			msleep(100); // take your time
		}
		m.unlock();
	}
	vw.release();
}

void DiskWriterThread::stop()
{
	running = false;
}

DiskWriterThread::~DiskWriterThread()
{
	vw.release();
}
