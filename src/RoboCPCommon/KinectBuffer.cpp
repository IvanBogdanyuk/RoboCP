#include "KinectBuffer.h"


KinectBuffer::KinectBuffer(int count)
{
  vacant = new boost::interprocess::interprocess_semaphore (count);
  used = new boost::interprocess::interprocess_semaphore (0);
}

void KinectBuffer::Add (boost::shared_ptr<KinectData> d)
{
  vacant->wait();
  q.push (d);
  used->post();
}

boost::shared_ptr<KinectData> KinectBuffer::Rec (void)
{
  used->wait();
  boost::shared_ptr<KinectData> dat = q.front();
  q.pop ();
  vacant->post();
  return dat;
}

KinectBuffer::~KinectBuffer(void)
{
}
