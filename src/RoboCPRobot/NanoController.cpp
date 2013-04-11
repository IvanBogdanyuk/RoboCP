#include "NanoController.h"


NanoReceivedBuffer *NanoController::GetBuffer(void)
{
  return buffer;
}

NanoController::NanoController(NanoReceivedBuffer *buf)
{
  buffer = buf;
  lastReadTime = time(NULL);
  nanoCom = new SerialCom(NANO_COM_PORT, NANO_BAUD_RATE);
}


NanoController::~NanoController(void)
{
  nanoCom->~SerialCom();
}

void NanoController::Start(void)
{
  /*int count4 = 0;
  time_t FirstData;*/

  unsigned char *ReadenData;
  unsigned short *PShortData;
  unsigned char DataPacket[10];
  int NextByte;
  int stage = -4;
  while (true){
    ReadenData = (unsigned char *)nanoCom->Read();
    if (nanoCom->GetOutSize() > 0){
      for (int i = 0; i<nanoCom->GetOutSize(); i++){
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
            /*if (count4 == 0){
              FirstData = time(NULL);
            }else{
              if (count4 % 64 == 63){
                time_t cur = time(NULL);
                printf("%f\n",(float)count4/difftime(cur,FirstData));
              }
            }
            count4++;*/
            //printf("%d %d %d %d %d\n",PShortData[0],PShortData[1],PShortData[2],PShortData[3],PShortData[4]);
            NanoData->Time = time(NULL);
            buffer->Enqueue(NanoData);
          }
        }
      }
      lastReadTime = time(NULL);
    }else{
      if (difftime(time(NULL),lastReadTime)>NANO_SECONDS_TO_RECONNECT){
        nanoCom->~SerialCom();
        nanoCom = new SerialCom(NANO_COM_PORT, NANO_BAUD_RATE);
        lastReadTime = time(NULL);
        stage = -4;
      }
    }
  }
}
