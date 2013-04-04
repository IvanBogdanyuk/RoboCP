#pragma once
#include "ReceivedBuffer.h"
#include "KinectData.h"
#include <boost/asio.hpp>


class KinectBuffer :
  public ReceivedBuffer
{
public:
  boost::shared_ptr<KinectData> data;
  void Add (boost::shared_ptr<KinectData> d);
  boost::shared_ptr<KinectData> Rec ();
  KinectBuffer(void);
  ~KinectBuffer(void);
};

