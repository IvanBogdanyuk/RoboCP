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
	boost::shared_ptr <ArduCopterReceived> arduData (new ArduCopterReceived);
	boost::shared_ptr <NanoReceived> nanoData (new NanoReceived);
	boost::shared_ptr <CameraReceived> cameraData;
  
    while (true){
	  boost::shared_ptr <Send> sendData(new Send);
	  sendData->Time = time(NULL);
	  
	  if (arduBuffer->Used->try_wait()){
		arduBuffer->Used->post();
        arduData = arduBuffer->Dequeue();
	  }
        sendData->Acceleration = arduData->Acceleration;
		sendData->Roll = arduData->Roll;
		sendData->Pitch = arduData->Pitch;
		sendData->Yaw = arduData->Yaw;
		sendData->AltitudeSonic = arduData->AltitudeSonic;
		sendData->AltitudeBarometer = arduData->AltitudeBarometer;
	  
	  if (nanoBuffer->Used->try_wait()){
		nanoBuffer->Used->post();
	    nanoData = nanoBuffer->Dequeue();
	  }
        sendData->TopSonicSensor = nanoData->TopSonicSensor;
		sendData->FrontSonicSensor = nanoData->FrontSonicSensor;
		sendData->LeftSonicSensor = nanoData->LeftSonicSensor;
		sendData->RightSonicSensor = nanoData->RightSonicSensor;
		sendData->BackSonicSensor = nanoData->BackSonicSensor;
	  
	  //if (cameraBuffer->Used > 0) cameraData = cameraBuffer->Dequeue();

      sendBuffer->Enqueue (sendData);
      Sleep (50); 
  }
}

SendProcessing::~SendProcessing(void)
{
}
