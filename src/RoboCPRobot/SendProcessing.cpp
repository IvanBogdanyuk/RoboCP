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
	bool HasCopterData = false;
	bool HasNanoData = false;
	boost::shared_ptr <ArduCopterReceived> arduData;
	boost::shared_ptr <NanoReceived> nanoData;
	boost::shared_ptr <CameraReceived> cameraData;
  boost::shared_ptr <Send> sendData(new Send);
  while (true){
	  if (arduBuffer->Used > 0 && HasCopterData == false){
      arduData = arduBuffer->Dequeue();
      sendData->Acceleration = arduData->Acceleration;
		  sendData->Roll = arduData->Roll;
		  sendData->Pitch = arduData->Pitch;
		  sendData->Yaw = arduData->Yaw;
		  sendData->AltitudeSonic = arduData->AltitudeSonic;
		  sendData->AltitudeBarometer = arduData->AltitudeBarometer;
		  HasCopterData = true;
    }
	  if (nanoBuffer->Used > 0 && HasNanoData == false){
	    nanoData = nanoBuffer->Dequeue();
      sendData->TopSonicSensor = nanoData->TopSonicSensor;
		  sendData->FrontSonicSensor = nanoData->FrontSonicSensor;
		  sendData->LeftSonicSensor = nanoData->LeftSonicSensor;
		  sendData->RightSonicSensor = nanoData->RightSonicSensor;
		  sendData->BackSonicSensor = nanoData->BackSonicSensor;
      HasNanoData = true;
    }
	  if(cameraBuffer->Used > 0) cameraData = cameraBuffer->Dequeue();

    if (HasNanoData == true && HasCopterData == true){
      sendData->Time = time(NULL);
      sendBuffer->Enqueue (sendData);
      HasNanoData = false;
      HasCopterData = false;
      sendData = (boost::shared_ptr <Send>)(new Send);
      Sleep (20);
    }
	  Sleep(1);
  }
}

SendProcessing::~SendProcessing(void)
{
}
