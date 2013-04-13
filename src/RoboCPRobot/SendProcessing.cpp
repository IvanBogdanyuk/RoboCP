#include "SendProcessing.h"


SendProcessing::SendProcessing(NanoReceivedBuffer *nano, ArduCopterBuffer *ardu, SendBuffer *send)
{
  nanoBuffer = nano;
  arduBuffer = ardu;
  sendBuffer = send;
}

void SendProcessing::Start()
{
	
  while (true) {
	boost::shared_ptr <ArduCopterReceived> arduData;
	boost::shared_ptr <NanoReceived> nanoData;

	arduData = arduBuffer->Dequeue();
	nanoData = nanoBuffer->Dequeue();

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
	}

	sendBuffer->Enqueue (sendData);
  }

}

SendProcessing::~SendProcessing(void)
{
}
