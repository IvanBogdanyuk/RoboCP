#include "TSDataHandler.h"


TSDataHandler::TSDataHandler(int frameLimit)
{
	// лимит изображений в очереди
	mFrameLimit = frameLimit;
}

// функция считывания изображения из очереди
bool TSDataHandler::ReadFrame(cv::Mat &output)
{
	QMutex m;
	m.lock();

	if (mFrameQueue.empty())	
    {
		m.unlock();
		return false;
	}

	mFrame = mFrameQueue.back();
	mFrame.copyTo(output);
	mFrameQueue.pop_back();
	mFrame.release();

	m.unlock();
	return true;
}

// функция записи изображения из очереди
void TSDataHandler::WriteFrame(cv::Mat input)
{
	QMutex m;
	m.lock();

	if (input.empty() || mFrameQueue.size() > mFrameLimit)
	{
		m.unlock();
		return;
	}

	mFrameQueue.push_front(input);

	m.unlock();
}

TSDataHandler::~TSDataHandler()
{
}
