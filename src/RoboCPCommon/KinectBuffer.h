#pragma once
#include "ReceivedBuffer.h"
#include "KinectData.h"
#include <queue>
#include <boost/interprocess/sync/interprocess_semaphore.hpp>
#include <boost/asio.hpp>

using namespace std;

class KinectBuffer :
  public ReceivedBuffer
{
public:
  //boost::shared_ptr<KinectData> data;
  boost::interprocess::interprocess_semaphore * vacant;
  boost::interprocess::interprocess_semaphore * used;
  queue < boost::shared_ptr<KinectData> > q;
  void Add (boost::shared_ptr<KinectData> d);
  boost::shared_ptr<KinectData> Rec ();
  KinectBuffer(int count);
  ~KinectBuffer(void);
};

