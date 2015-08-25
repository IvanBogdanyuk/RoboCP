#include "NanoController.h"

#include "CarduinoConfig.h"

NanoReceivedBuffer *NanoController::GetBuffer(void)
{
  return buffer;
}

NanoController::NanoController(NanoReceivedBuffer *buf)
{
  buffer = buf;
  lastReadTime = time(NULL);
  
  dataToSend = new char(TO_SEND_BUFF_SIZE);
  readyToNewMessage = true;
}

void NanoController::Configure(Config* carduinoConfig)
{
  CarduinoConfig* x = (CarduinoConfig*) carduinoConfig;
  nanoPort = x->getPort();
  char *cstr = new char[nanoPort.length() + 1];
  strcpy(cstr, nanoPort.c_str());
  nanoCom = new SerialCom(cstr, NANO_BAUD_RATE);
}

bool NanoController::SetDefaultGPSMessage()
{
  if (readyToNewMessage == false) return false;
  sprintf(dataToSend,"$,0000.0005,00000.0005,,,*");
  readyToNewMessage = false;
  return true;
}

bool NanoController::ChangeGPSMessage(char *UTC, char *Latitude, char *Longtitude, char *GSpeed, char *SpeedAngle)
{
  if (readyToNewMessage == false) return false;
  sprintf(dataToSend,"$%s,%s,%s,%s,%s,*",UTC,Latitude,Longtitude,GSpeed,SpeedAngle);
  readyToNewMessage = false;
  return true;
}

NanoController::~NanoController(void)
{
  nanoCom->~SerialCom();
}

void NanoController::Start(void)
{
  #ifdef ENABLE_LOGGING
  RAW_LOG(INFO, "NanoController: started");
  #endif
  #ifdef NANO_FPS_TEST
  int count4 = 0;
  time_t FirstData;
  #endif

  unsigned char *ReadenData;
  unsigned short *PShortData;
  unsigned char DataPacket[10];
  int NextByte;
  int stage = -4;
  int DataLength;
  unsigned int Counter = 0;
  while (true){
    Sleep(1);
    Counter++;
    if ((readyToNewMessage == false)&&(Counter > 100)){
      DataLength = 0;
      while (dataToSend[DataLength] != '\0') DataLength++;
      #ifdef ENABLE_LOGGING
      RAW_LOG(INFO, "NanoController: updating GPS message...");
      #endif
      nanoCom->Write(dataToSend, DataLength);
      Counter = 0;
      readyToNewMessage = true;
    }
    ReadenData = (unsigned char *)nanoCom->Read();
    if (nanoCom->GetOutSize() > 0){
      for (int i = 0; i<nanoCom->GetOutSize(); i++){
        #ifdef NANO_INPUT_DATA_TEST
        printf("%c",ReadenData[i]);
        #endif
        printf("!");
        if (stage == -4){
          if (ReadenData[i] == 131){
            stage++;
            continue;
          }else{
            continue;
          }
        }
        if (stage == -3){
          if (ReadenData[i] == 130){
            stage++;
            continue;
          }else{
            stage = -4;
            continue;
          }
        }
        if (stage == -2){
          if (ReadenData[i] == 129){
            stage++;
            continue;
          }else{
            stage = -4;
            continue;
          }
        }
        if (stage == -1){
          if (ReadenData[i] == 128){
            stage++;
            NextByte = 0;
            continue;
          }else{
            stage = -4;
            continue;
          }
        }
        if (stage == 0){
          if (NextByte<10){
            DataPacket[NextByte] = ReadenData[i];
            NextByte++;
          }else{
            stage = -4;
            PShortData = (unsigned short *)&DataPacket[0];
            boost::shared_ptr<NanoReceived> NanoData (new NanoReceived());
            NanoData->FrontSonicSensor = PShortData[0];
            NanoData->RightSonicSensor = PShortData[1];
            NanoData->BackSonicSensor = PShortData[2];
            NanoData->LeftSonicSensor = PShortData[3];
            NanoData->TopSonicSensor = PShortData[4];
            #ifdef ENABLE_LOGGING
            RAW_LOG(INFO, "NanoController: got data - front = %d, right = %d, back = %d, left = %d, top = %d",PShortData[0],PShortData[1],PShortData[2],PShortData[3],PShortData[4]);
            #endif
            #ifdef NANO_FPS_TEST
            if (count4 == 0){
              FirstData = time(NULL);
            }else{
              if (count4 % 64 == 63){
                time_t cur = time(NULL);
                printf("%f\n",(float)count4/difftime(cur,FirstData));
              }
            }
            count4++;
            #endif
            #ifdef NANO_TELEMETRY_TEST
            printf("%d %d %d %d %d\n",PShortData[0],PShortData[1],PShortData[2],PShortData[3],PShortData[4]);
            #endif
            NanoData->Time = time(NULL);
            buffer->Enqueue(NanoData);
          }
        }
      }
      lastReadTime = time(NULL);
    }else{
      if (difftime(time(NULL),lastReadTime)>NANO_SECONDS_TO_RECONNECT){
        nanoCom->~SerialCom();
        #ifdef ENABLE_LOGGING
        RAW_LOG(INFO, "NanoController: reconnecting...");
        #endif
        char *cstr = new char[nanoPort.length() + 1];
        strcpy(cstr, nanoPort.c_str());
        nanoCom = new SerialCom(cstr, NANO_BAUD_RATE);
        lastReadTime = time(NULL);
        stage = -4;
      }
    }
  }
}

void NanoController::FakeStart(void){
  int i = 0;
  while (true){
    boost::shared_ptr<NanoReceived> NanoData (new NanoReceived());
    NanoData->FrontSonicSensor = (i % 5)*10;
    NanoData->RightSonicSensor = ((i+1)% 5)*10;
    NanoData->BackSonicSensor = ((i+2)% 5)*10;
    NanoData->LeftSonicSensor = ((i+3)% 5)*10;
    NanoData->TopSonicSensor = ((i+4)% 5)*10;
    NanoData->Time = time(NULL);
    buffer->Enqueue(NanoData);
    i = (i + 1)%5;
    Sleep(103);
  }
}