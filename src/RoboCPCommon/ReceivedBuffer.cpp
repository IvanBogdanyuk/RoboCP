#include "ReceivedBuffer.h"

template <class Type>
ReceivedBuffer<Type>::ReceivedBuffer(int size)
{
	Vacant = new boost::interprocess::interprocess_semaphore (size);
	Used = new boost::interprocess::interprocess_semaphore (0);
	mtx = new boost::interprocess::interprocess_mutex;
}

template <class Type>
ReceivedBuffer<Type>::~ReceivedBuffer(void)
{
	delete Vacant;
	delete Used;
	delete mtx;
}

template <class Type>
void ReceivedBuffer<Type>::Enqueue (Type a)
{
  Vacant->wait();
  mtx->lock();
    queue.push (a);
  mtx->unlock();
  Used->post();
}

template <class Type>
Type ReceivedBuffer<Type>::Dequeue ()
{
  Used->wait();
  mtx->lock();
    Type value = queue.front();
    queue.pop ();
  mtx->unlock();
  Vacant->post();
  return value;
}

template class ReceivedBuffer<int>;
template class ReceivedBuffer< boost::shared_ptr<KinectData> >;
template class ReceivedBuffer< boost::shared_ptr<NanoReceived> >;
template class ReceivedBuffer< boost::shared_ptr<ArduCopterReceived> >;
template class ReceivedBuffer< boost::shared_ptr<CameraReceived> >;
template class ReceivedBuffer< boost::shared_ptr<Send> >;