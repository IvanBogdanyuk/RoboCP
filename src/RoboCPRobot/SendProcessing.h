#pragma once
#include "ArduCopterReceived.h"
#include "ArduCopterBuffer.h"
#include "NanoReceivedBuffer.h"
#include "SendBuffer.h"
#include "NanoReceived.h"

class SendProcessing
{
private:
	NanoReceivedBuffer* nanoBuffer;
	ArduCopterBuffer* arduBuffer;
	SendBuffer* sendBuffer;
public:
	SendProcessing(NanoReceivedBuffer *nano, ArduCopterBuffer *ardu, SendBuffer *send);
    ~SendProcessing(void);
	void Start();
};

