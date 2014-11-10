#include "ArduCopterController.h"


ArduCopterController::ArduCopterController(Config *x, ArduCopterBuffer *buf)
{
  ArduCopterConfig *config = (ArduCopterConfig*)x;
  if(config->IsAvailable)
  {
	buffer = buf;
    copterPort = config->getPort();
    char *cstr = new char[copterPort.length() + 1];
    strcpy(cstr, copterPort.c_str());
    copterCom = new SerialCom(cstr,COPTER_BAUD_RATE);
    lastReadTime = time(NULL);
  }
  else
  {
    if(config->DoFakeStart)
	{
	  #ifdef ENABLE_LOGGING
      RAW_LOG (INFO, "ArdoCopter is not available. Doing fake start");
      #endif
	  FakeStart();
	}
	else
	{
	  #ifdef ENABLE_LOGGING
      RAW_LOG (INFO, "ArdoCopter is not available. Close programme");
      #endif
	  exit(1);
	}
  }
}


ArduCopterController::~ArduCopterController(void)
{
}

void ArduCopterController::sendInitionalData(void)
{
  #ifdef ENABLE_LOGGING
  RAW_LOG(INFO, "ArduCopterController: sending initional data...");
  #endif
  unsigned char DataToSend[32];
  DataToSend[0] = 0xFE;
  DataToSend[1] = 0x02;
  DataToSend[2] = 0x00;
  DataToSend[3] = 0xFF;
  DataToSend[4] = 0xBE;
  DataToSend[5] = 0x15;
  DataToSend[6] = 0x01;
  DataToSend[7] = 0x01;
  DataToSend[8] = 0x79;
  DataToSend[9] = 0x37;
  copterCom->Write((char *)&DataToSend[0],10);
  Sleep(10000);
  
  DataToSend[0] = 0xFE;
  DataToSend[1] = 0x09;
  DataToSend[2] = 0x01;
  DataToSend[3] = 0xFF;
  DataToSend[4] = 0xBE;
  DataToSend[5] = 0x00;
  DataToSend[6] = 0x00;
  DataToSend[7] = 0x00;
  DataToSend[8] = 0x00;
  DataToSend[9] = 0x00;
  DataToSend[10] = 0x06;
  DataToSend[11] = 0x08;
  DataToSend[12] = 0x00;
  DataToSend[12] = 0x00;
  DataToSend[13] = 0x03;
  DataToSend[14] = 0xC2;
  DataToSend[15] = 0x3C;
  copterCom->Write((char *)&DataToSend[0],16);
  Sleep(50);

  DataToSend[0] = 0xFE; 
  DataToSend[1] = 0x06;
  DataToSend[2] = 0x02;
  DataToSend[3] = 0xFF;
  DataToSend[4] = 0xBE;
  DataToSend[5] = 0x42;
  DataToSend[6] = 0x02;
  DataToSend[7] = 0x00;
  DataToSend[8] = 0x01;
  DataToSend[9] = 0x01;
  DataToSend[10] = 0x02;
  DataToSend[11] = 0x01;
  DataToSend[12] = 0xC0;
  DataToSend[13] = 0x34;
  copterCom->Write((char *)&DataToSend[0],14);
  Sleep(100);
  
  DataToSend[0] = 0xFE; 
  DataToSend[1] = 0x06;
  DataToSend[2] = 0x03;
  DataToSend[3] = 0xFF;
  DataToSend[4] = 0xBE;
  DataToSend[5] = 0x42;
  DataToSend[6] = 0x02;
  DataToSend[7] = 0x00;
  DataToSend[8] = 0x01;
  DataToSend[9] = 0x01;
  DataToSend[10] = 0x02;
  DataToSend[11] = 0x01;
  DataToSend[12] = 0x51;
  DataToSend[13] = 0x61;
  copterCom->Write((char *)&DataToSend[0],14);
  Sleep(50);

  DataToSend[0] = 0xFE; 
  DataToSend[1] = 0x06;
  DataToSend[2] = 0x04;
  DataToSend[3] = 0xFF;
  DataToSend[4] = 0xBE;
  DataToSend[5] = 0x42;
  DataToSend[6] = 0x03;
  DataToSend[7] = 0x00;
  DataToSend[8] = 0x01;
  DataToSend[9] = 0x01;
  DataToSend[10] = 0x06;
  DataToSend[11] = 0x01;
  DataToSend[12] = 0x03;
  DataToSend[13] = 0x3D;
  copterCom->Write((char *)&DataToSend[0],14);
  Sleep(50);

  DataToSend[0] = 0xFE; 
  DataToSend[1] = 0x06;
  DataToSend[2] = 0x05;
  DataToSend[3] = 0xFF;
  DataToSend[4] = 0xBE;
  DataToSend[5] = 0x42;
  DataToSend[6] = 0x03;
  DataToSend[7] = 0x00;
  DataToSend[8] = 0x01;
  DataToSend[9] = 0x01;
  DataToSend[10] = 0x06;
  DataToSend[11] = 0x01;
  DataToSend[12] = 0x92;
  DataToSend[13] = 0x68;
  copterCom->Write((char *)&DataToSend[0],14);
  Sleep(50);

  DataToSend[0] = 0xFE; 
  DataToSend[1] = 0x06;
  DataToSend[2] = 0x06;
  DataToSend[3] = 0xFF;
  DataToSend[4] = 0xBE;
  DataToSend[5] = 0x42;
  DataToSend[6] = 0x0A;
  DataToSend[7] = 0x00;
  DataToSend[8] = 0x01;
  DataToSend[9] = 0x01;
  DataToSend[10] = 0x0A;
  DataToSend[11] = 0x01;
  DataToSend[12] = 0xBB;
  DataToSend[13] = 0x72;
  copterCom->Write((char *)&DataToSend[0],14);
  Sleep(50);

  DataToSend[0] = 0xFE; 
  DataToSend[1] = 0x06;
  DataToSend[2] = 0x07;
  DataToSend[3] = 0xFF;
  DataToSend[4] = 0xBE;
  DataToSend[5] = 0x42;
  DataToSend[6] = 0x0A;
  DataToSend[7] = 0x00;
  DataToSend[8] = 0x01;
  DataToSend[9] = 0x01;
  DataToSend[10] = 0x0A;
  DataToSend[11] = 0x01;
  DataToSend[12] = 0x2A;
  DataToSend[13] = 0x27;
  copterCom->Write((char *)&DataToSend[0],14);
  Sleep(50);

  DataToSend[0] = 0xFE; 
  DataToSend[1] = 0x06;
  DataToSend[2] = 0x08;
  DataToSend[3] = 0xFF;
  DataToSend[4] = 0xBE;
  DataToSend[5] = 0x42;
  DataToSend[6] = 0x0A;
  DataToSend[7] = 0x00;
  DataToSend[8] = 0x01;
  DataToSend[9] = 0x01;
  DataToSend[10] = 0x0B;
  DataToSend[11] = 0x01;
  DataToSend[12] = 0xBA;
  DataToSend[13] = 0x61;
  copterCom->Write((char *)&DataToSend[0],14);
  Sleep(50);

  DataToSend[0] = 0xFE; 
  DataToSend[1] = 0x06;
  DataToSend[2] = 0x09;
  DataToSend[3] = 0xFF;
  DataToSend[4] = 0xBE;
  DataToSend[5] = 0x42;
  DataToSend[6] = 0x0A;
  DataToSend[7] = 0x00;
  DataToSend[8] = 0x01;
  DataToSend[9] = 0x01;
  DataToSend[10] = 0x0B;
  DataToSend[11] = 0x01;
  DataToSend[12] = 0x2B;
  DataToSend[13] = 0x34;
  copterCom->Write((char *)&DataToSend[0],14);
  Sleep(100);

  DataToSend[0] = 0xFE; 
  DataToSend[1] = 0x06;
  DataToSend[2] = 0x0A;
  DataToSend[3] = 0xFF;
  DataToSend[4] = 0xBE;
  DataToSend[5] = 0x42;
  DataToSend[6] = 0x02;
  DataToSend[7] = 0x00;
  DataToSend[8] = 0x01;
  DataToSend[9] = 0x01;
  DataToSend[10] = 0x0C;
  DataToSend[11] = 0x01;
  DataToSend[12] = 0x71;
  DataToSend[13] = 0x98;
  copterCom->Write((char *)&DataToSend[0],14);
  Sleep(50);

  DataToSend[0] = 0xFE; 
  DataToSend[1] = 0x06;
  DataToSend[2] = 0x0B;
  DataToSend[3] = 0xFF;
  DataToSend[4] = 0xBE;
  DataToSend[5] = 0x42;
  DataToSend[6] = 0x02;
  DataToSend[7] = 0x00;
  DataToSend[8] = 0x01;
  DataToSend[9] = 0x01;
  DataToSend[10] = 0x0C;
  DataToSend[11] = 0x01;
  DataToSend[12] = 0xE0;
  DataToSend[13] = 0xCD;
  copterCom->Write((char *)&DataToSend[0],14);
  Sleep(50);

  DataToSend[0] = 0xFE; 
  DataToSend[1] = 0x06;
  DataToSend[2] = 0x0C;
  DataToSend[3] = 0xFF;
  DataToSend[4] = 0xBE;
  DataToSend[5] = 0x42;
  DataToSend[6] = 0x02;
  DataToSend[7] = 0x00;
  DataToSend[8] = 0x01;
  DataToSend[9] = 0x01;
  DataToSend[10] = 0x01;
  DataToSend[11] = 0x01;
  DataToSend[12] = 0x79;
  DataToSend[13] = 0x92;
  copterCom->Write((char *)&DataToSend[0],14);
  Sleep(50);

  DataToSend[0] = 0xFE; 
  DataToSend[1] = 0x06;
  DataToSend[2] = 0x0D;
  DataToSend[3] = 0xFF;
  DataToSend[4] = 0xBE;
  DataToSend[5] = 0x42;
  DataToSend[6] = 0x02;
  DataToSend[7] = 0x00;
  DataToSend[8] = 0x01;
  DataToSend[9] = 0x01;
  DataToSend[10] = 0x01;
  DataToSend[11] = 0x01;
  DataToSend[12] = 0xE8;
  DataToSend[13] = 0xC7;
  copterCom->Write((char *)&DataToSend[0],14);
  Sleep(50);

  DataToSend[0] = 0xFE; 
  DataToSend[1] = 0x06;
  DataToSend[2] = 0x0E;
  DataToSend[3] = 0xFF;
  DataToSend[4] = 0xBE;
  DataToSend[5] = 0x42;
  DataToSend[6] = 0x02;
  DataToSend[7] = 0x00;
  DataToSend[8] = 0x01;
  DataToSend[9] = 0x01;
  DataToSend[10] = 0x03;
  DataToSend[11] = 0x01;
  DataToSend[12] = 0xE3;
  DataToSend[13] = 0x8C;
  copterCom->Write((char *)&DataToSend[0],14);
  Sleep(50);

  DataToSend[0] = 0xFE; 
  DataToSend[1] = 0x06;
  DataToSend[2] = 0x0F;
  DataToSend[3] = 0xFF;
  DataToSend[4] = 0xBE;
  DataToSend[5] = 0x42;
  DataToSend[6] = 0x02;
  DataToSend[7] = 0x00;
  DataToSend[8] = 0x01;
  DataToSend[9] = 0x01;
  DataToSend[10] = 0x03;
  DataToSend[11] = 0x01;
  DataToSend[12] = 0x72;
  DataToSend[13] = 0xD9;
  copterCom->Write((char *)&DataToSend[0],14);
  Sleep(50);
  stage = -6;
  #ifdef ENABLE_LOGGING
  RAW_LOG(INFO, "ArduCopterController: Initional data sent!");
  #endif
}

void ArduCopterController::Start(void)
{
  #ifdef ENABLE_LOGGING
  RAW_LOG(INFO, "ArduCopterController: started");
  #endif
  #ifdef COPTER_MSG_TYPES_TEST
  bool used[256];
  for (int i = 0; i<256; i++) used[i] = true;
  #endif

  char *ReadenData;
  float *PFloatData;
  short *PShortData;

  unsigned char MavPacket[300];
  int NextByte;
  unsigned char DataToRead;
  unsigned char MsgType;
  sendInitionalData();
  while (true){
    ReadenData = copterCom->Read();
    if (copterCom->GetOutSize()>0){
      for (int i = 0; i<copterCom->GetOutSize(); i++){
        if (stage == -6){//packet start sign
          if ((unsigned char)ReadenData[i] == 0xFE){
            stage++;
          }
          continue;
        }
        if (stage == -5){//payload length
          DataToRead = 2+((unsigned char)ReadenData[i]);
          stage++;
          continue;
        }
        if (stage < -1){
          stage++;
          continue;
        }
        if (stage == -1){//message type
          MsgType = (unsigned char)ReadenData[i];
          NextByte = 0;
          stage++;
          continue;
        }
        if (stage == 0){
          if (DataToRead>0){
            DataToRead--;
            MavPacket[NextByte] = (unsigned char)ReadenData[i];
            NextByte++;
          }else{
            stage = -6;
            #ifdef COPTER_MSG_TYPES_TEST
            if (used[MsgType] == true){
              used[MsgType] = false;
              printf("%d\n",MsgType);
            }
            #endif
            #ifdef ENABLE_LOGGING
            RAW_LOG(INFO, "ArduCopterController: got message, mavlink id=%d",MsgType);
            #endif
            if (MsgType == 30){
              PFloatData = ((float *)(&MavPacket[4]));//[0] - roll, [1] - pitch, [2] - yaw, all in radians
              #ifdef COPTER_TELEMETRY_TEST
              printf("%f  %f  %f\n",PFloatData[0]*180/3.1416,PFloatData[1]*180/3.1416,PFloatData[2]*180/3.1416);
              #endif
              boost::shared_ptr<ArduCopterReceived> CopterReceived (new ArduCopterReceived());
              CopterReceived->Roll = PFloatData[0];
              CopterReceived->Pitch = PFloatData[1];
              CopterReceived->Yaw = PFloatData[2];
              CopterReceived->PacketType = AnglesPacket;
              CopterReceived->Time = time(NULL);
              #ifdef ENABLE_LOGGING
              RAW_LOG(INFO, "ArduCopterController: got data - roll = %f, pitch = %f, yaw = %f",PFloatData[0],PFloatData[1],PFloatData[2]);
              #endif
              buffer->Enqueue(CopterReceived);
            }
            if (MsgType == 74){
              PFloatData = ((float *)(&MavPacket[0]));//[3] - altitude
              #ifdef COPTER_TELEMETRY_TEST
              printf("%f\n",PFloatData[3]);
              #endif
              boost::shared_ptr<ArduCopterReceived> CopterReceived (new ArduCopterReceived());
              CopterReceived->AltitudeSonic = PFloatData[3];
              CopterReceived->PacketType = AltitudePacket;
              CopterReceived->Time = time(NULL);
              #ifdef ENABLE_LOGGING
              RAW_LOG(INFO, "ArduCopterController: got data - altitude sonic = %f",PFloatData[3]);
              #endif
              buffer->Enqueue(CopterReceived);
            }
            if (MsgType == 27){
              PShortData = ((short *)(&MavPacket[0]));//[4] - x accel, [5] - y accel, [6] - z accel
              #ifdef COPTER_TELEMETRY_TEST
              printf("%d %d %d\n",PShortData[4],PShortData[5],PShortData[6]);
              #endif
              boost::shared_ptr<ArduCopterReceived> CopterReceived (new ArduCopterReceived());
              CopterReceived->Acceleration.x = PShortData[4];
              CopterReceived->Acceleration.y = PShortData[5];
              CopterReceived->Acceleration.z = PShortData[6];
              CopterReceived->PacketType = AccelerationPacket;
              CopterReceived->Time = time(NULL);
              #ifdef ENABLE_LOGGING
              RAW_LOG(INFO, "ArduCopterController: got data - accel x = %d, accel y = %d, accel z = %d",PShortData[4],PShortData[5],PShortData[6]);
              #endif
              buffer->Enqueue(CopterReceived);
            }
          }
        }
      }
      lastReadTime = time(NULL);
    }else{
      if (difftime(time(NULL),lastReadTime)>COPTER_SECONDS_TO_RECONNECT){
        copterCom->~SerialCom();
        #ifdef ENABLE_LOGGING
        RAW_LOG(INFO, "ArduCopterController: reconnecting...");
        #endif
        char *cstr = new char[copterPort.length() + 1];
        strcpy(cstr, copterPort.c_str());
        copterCom = new SerialCom(cstr, COPTER_BAUD_RATE);
        sendInitionalData();
        lastReadTime = time(NULL);
      }
    }
  }
}

void ArduCopterController::FakeStart(void)
{
  int i = 0;
  while (true){
    boost::shared_ptr<ArduCopterReceived> CopterReceived (new ArduCopterReceived());
    if (i % 3 == 0){
      CopterReceived->Roll = (float)i/180*3.1416;
      CopterReceived->Pitch = (float)(i+1)/180*3.1416;
      CopterReceived->Yaw = (float)(i+2)/180*3.1416;
      CopterReceived->Time = time(NULL);
      buffer->Enqueue(CopterReceived);
      Sleep(317);
      i = (i+1)%360;
      continue;
    }
    if (i % 3 == 1){
      CopterReceived->Acceleration.x = 100-i;
      CopterReceived->Acceleration.y = 101-i;
      CopterReceived->Acceleration.z = 102-i;
      CopterReceived->Time = time(NULL);
      buffer->Enqueue(CopterReceived);
      Sleep(117);
      i = (i+1)%360;
      continue;
    }
    CopterReceived->AltitudeBarometer = i*2;
    CopterReceived->AltitudeSonic = i*2 - 1;
    CopterReceived->Time = time(NULL);
    buffer->Enqueue(CopterReceived);
    Sleep(233);
    i = (i+1)%360;
  }
}