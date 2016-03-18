#include "TSDataHandler.h"

TSDataHandler::TSDataHandler(int frameLimit)
{
	this->frameLimit = frameLimit;
}

bool TSDataHandler::ReadFrame(cv::Mat &output)
{
	QMutex m;
	m.lock();

	if (frame_queue.empty())
		return false;
	frame = frame_queue.back();
	frame.copyTo(output);
	//printf("wrote a frame, %i left\n", frame_queue.size());
	frame_queue.pop_back();
	frame.release();

	m.unlock();
	return true;
}

void TSDataHandler::WriteFrame(cv::Mat input)
{
	QMutex m;
	m.lock();

	if (input.dims == 0 || frame_queue.size() > this->frameLimit)
		return;

	frame_queue.push_front(input);

	m.unlock();
}

TSDataHandler::~TSDataHandler()
{
}
