#include "KinectBuffer.h"


KinectBuffer::KinectBuffer(void)
{
  boost::shared_ptr<KinectData> cld (new KinectData);
  this->Add (cld);
}

void KinectBuffer::Add (boost::shared_ptr<KinectData> d)
{
  data = d;
}

boost::shared_ptr<KinectData> KinectBuffer::Rec (void)
{
  return data;
}

KinectBuffer::~KinectBuffer(void)
{
}
