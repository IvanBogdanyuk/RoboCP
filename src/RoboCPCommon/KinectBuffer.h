#pragma once
#include "ReceivedBuffer.h"
#include "KinectData.h"

using namespace std;

class KinectBuffer : public ReceivedBuffer < boost::shared_ptr<KinectData> >
{
public:
  KinectBuffer (int size) : ReceivedBuffer< boost::shared_ptr<KinectData> > (size) {}
};
