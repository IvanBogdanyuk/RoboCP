#pragma once
#include "ReceivedBuffer.h"
#include "NanoReceived.h"

class NanoReceivedBuffer :
  public ReceivedBuffer< boost::shared_ptr<NanoReceived> >
{
public:
	NanoReceivedBuffer (int size) : ReceivedBuffer< boost::shared_ptr<NanoReceived> > (size) {}
};

