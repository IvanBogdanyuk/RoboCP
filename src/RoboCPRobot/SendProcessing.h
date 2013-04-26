#pragma once
#include "ArduCopterReceived.h"
#include "ArduCopterBuffer.h"
#include "NanoReceivedBuffer.h"
#include "CameraReceivedBuffer.h"
#include "SendBuffer.h"
#include "NanoReceived.h"

class SendProcessing
{
private:
	NanoReceivedBuffer* nanoBuffer;
	ArduCopterBuffer* arduBuffer;
	CameraReceivedBuffer* cameraBuffer;
	SendBuffer* sendBuffer;
public:
	SendProcessing(NanoReceivedBuffer *nano, ArduCopterBuffer *ardu, CameraReceivedBuffer *camera, SendBuffer *send);
    ~SendProcessing(void);
	void Start();
};


