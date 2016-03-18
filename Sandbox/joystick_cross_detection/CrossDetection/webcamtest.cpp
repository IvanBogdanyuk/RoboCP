#include <opencv2\highgui\highgui.hpp>

#include "WebcamCapture.h"
#include "ProcessingThread.h"
#include "DiskWriterThread.h"
#include "TSDataHandler.h"

int main(int argc, char* argv[])
{
	// init
	TSDataHandler* dh_cap2proc = new TSDataHandler();
	TSDataHandler* dh_proc2out = new TSDataHandler();
	cv::Mat img;

	// spawn threads
	WebcamCapture capThread(dh_cap2proc);
	ProcessingThread procThread(dh_cap2proc, dh_proc2out);
	capThread.start();
	procThread.start();

	// work
	forever
	{
		if (dh_proc2out->ReadFrame(img))
		{
			cv::imshow("Output", img);
			cv::waitKey(1);
		}
	}

	// exit
	capThread.exit();
	procThread.exit();

	return 0;
}