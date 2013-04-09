#include "ReceivedBuffer.h"

template <class Type>
ReceivedBuffer<Type>::ReceivedBuffer(int size)
{
	Vacant = new boost::interprocess::interprocess_semaphore (size);
	Used = new boost::interprocess::interprocess_semaphore (0);
	Mtx = new boost::interprocess::interprocess_mutex;
}

template <class Type>
ReceivedBuffer<Type>::~ReceivedBuffer(void)
{
	delete Vacant;
	delete Used;
	delete Mtx;
}

template <class Type>
void ReceivedBuffer<Type>::Enqueue (Type a)
{
  Vacant->wait();
  Mtx->lock();
    q.push (a);
  Mtx->unlock();
  Used->post();
}

template <class Type>
Type ReceivedBuffer<Type>::Dequeue ()
{
  Used->wait();
  Mtx->lock();
    Type value = q.front();
    q.pop ();
  Mtx->unlock();
  Vacant->post();
  return value;
}

template class ReceivedBuffer<int>;
template class ReceivedBuffer< boost::shared_ptr<KinectData> >;
template class ReceivedBuffer< boost::shared_ptr<NanoReceived> >;
template class ReceivedBuffer< boost::shared_ptr<ArduCopterReceived> >;