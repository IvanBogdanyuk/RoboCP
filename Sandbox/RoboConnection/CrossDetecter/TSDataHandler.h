#pragma once
#include <opencv2\highgui\highgui.hpp>
#include <QtCore\qmutex.h>
#include <deque>
#include <list>

// Thread Safe Data Handler
template <class T> class TSDataHandler
{
public:
	TSDataHandler(int frameLimit = 10);
	~TSDataHandler();
	void Write(T input);
	bool Read(T &output);
private:
	int mCapacity;
	T mFrame;
	// Очередь фрэймов
	std::deque<T> mQueue;

};

template <class T>
TSDataHandler<T>::TSDataHandler(int frameLimit)
{
	// лимит изображений в очереди
	mCapacity = frameLimit;
}

// функция записи изображения из очереди
template <class T>
void TSDataHandler<T>::Write(T input)
{
	QMutex m;
	m.lock();

	if (input.empty() || mQueue.size() > mCapacity)
	{
		m.unlock();
		return;
	}

	mQueue.push_front(input);

	m.unlock();
}

// функция считывания изображения из очереди
template <class T>
bool TSDataHandler<T>::Read(T &output)
{
	QMutex m;
	m.lock();

	if (mQueue.empty())
	{
		m.unlock();
		return false;
	}

	mFrame = mQueue.back();
	mFrame.copyTo(output);
	mQueue.pop_back();
	mFrame.release();

	m.unlock();
	return true;
}

