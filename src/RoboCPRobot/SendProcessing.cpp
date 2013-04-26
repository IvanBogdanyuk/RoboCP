#include "SendProcessing.h"


SendProcessing::SendProcessing(NanoReceivedBuffer *nano, ArduCopterBuffer *ardu, CameraReceivedBuffer *camera, SendBuffer *send)
{
  nanoBuffer = nano;
  arduBuffer = ardu;
  sendBuffer = send;
  cameraBuffer = camera;
}

void SendProcessing::Start()
{
	
  while (true) {
	boost::shared_ptr <ArduCopterReceived> arduData;
	boost::shared_ptr <NanoReceived> nanoData;
	boost::shared_ptr <CameraReceived> cameraData;

	if (arduBuffer->Used > 0)
	  arduData = arduBuffer->Dequeue();
	if (nanoBuffer->Used > 0)
	  nanoData = nanoBuffer->Dequeue();
	if(cameraBuffer->Used > 0)
		cameraData = cameraBuffer->Dequeue();

	boost::shared_ptr <Send> sendData (new Send);

	{
		sendData->Acceleration = arduData->Acceleration;
		sendData->Roll = arduData->Roll;
		sendData->Pitch = arduData->Pitch;
		sendData->Yaw = arduData->Yaw;
		sendData->AltitudeSonic = arduData->AltitudeSonic;
		sendData->AltitudeBarometer = arduData->AltitudeBarometer;

		sendData->TopSonicSensor = nanoData->TopSonicSensor;
		sendData->FrontSonicSensor = nanoData->FrontSonicSensor;
		sendData->LeftSonicSensor = nanoData->LeftSonicSensor;
		sendData->RightSonicSensor = nanoData->RightSonicSensor;
		sendData->BackSonicSensor = nanoData->BackSonicSensor;

		sendData->Frame = cameraData->Frame;
		sendData->Motion = cameraData->Motion;
	}

	sendBuffer->Enqueue (sendData);
	Sleep (20);
  }

}

SendProcessing::~SendProcessing(void)
{
}
