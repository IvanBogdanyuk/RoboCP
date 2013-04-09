#pragma once
#include "ReceivedBuffer.h"
#include "ArduCopterReceived.h"

class ArduCopterBuffer : public ReceivedBuffer < boost::shared_ptr<ArduCopterReceived> >
{
public:
	ArduCopterBuffer (int size) : ReceivedBuffer < boost::shared_ptr<ArduCopterReceived> > (size) {}
};

