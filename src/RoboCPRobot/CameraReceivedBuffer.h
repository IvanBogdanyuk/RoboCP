#pragma once
#include "ReceivedBuffer.h"
#include "CameraReceived.h"

class CameraReceivedBuffer :  public ReceivedBuffer <boost::shared_ptr<CameraReceived>>
{
public:
  CameraReceivedBuffer (int size) : ReceivedBuffer <boost::shared_ptr<CameraReceived>> (size) {}
};

