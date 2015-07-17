#pragma once
#include <QtCore/qmutex.h>
#include <deque>

// Thread Safe Data Handler
template <class T> class TSDataHandler
{
public:
  TSDataHandler(int frameLimit = 10);
  ~TSDataHandler();
  void Write(T input);
  bool Read(T &output);
  bool Peek(T &output);
  int Size();
private:
  int mCapacity;
  // Очередь данных
  std::deque<T> mQueue;
  QMutex mMutex;
};

template <class T>
TSDataHandler<T>::TSDataHandler(int frameLimit) : mMutex()
{
  // лимит изображений в очереди
  mCapacity = frameLimit;
}

// функция записи в очередь
template <class T>
void TSDataHandler<T>::Write(T input)
{
  mMutex.lock();

  if (mQueue.size() > mCapacity)
  {
    mMutex.unlock();
    return;
  }

  mQueue.push_front(input);

  mMutex.unlock();
}

// функция считывания из очереди
template <class T>
bool TSDataHandler<T>::Read(T &output)
{
  mMutex.lock();

  if (mQueue.empty())
  {
    mMutex.unlock();
    return false;
  }

  output = mQueue.back();
  mQueue.pop_back();

  mMutex.unlock();
  return true;
}

template <class T>
int TSDataHandler<T>::Size()
{
  return mQueue.size();
}

template <class T>
bool TSDataHandler<T>::Peek(T &output)
{
  mMutex.lock();

  if (mQueue.empty())
  {
    mMutex.unlock();
    return false;
  }

  output = mQueue.back();

  mMutex.unlock();
  return true;
}

template <class T>
TSDataHandler<T>::~TSDataHandler()
{

}