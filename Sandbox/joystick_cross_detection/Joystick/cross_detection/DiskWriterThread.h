#pragma once

#include "TSDataHandler.h"
#include "TimingsDebug.h"
#include <QtCore\qthread.h>

class DiskWriterThread : public QThread
{
public:
	DiskWriterThread(TSDataHandler *dh);
	~DiskWriterThread();
	void stop();
private:
	void run();
	cv::VideoWriter vw;
	bool running;
	TSDataHandler *dh;
};

