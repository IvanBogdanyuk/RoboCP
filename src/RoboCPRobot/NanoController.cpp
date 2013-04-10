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
}

void NanoController::Start(void)
{
  char *ReadenData;
  while (true){
    ReadenData = nanoCom->Read();
    if (nanoCom->GetOutSize() > 0){
      lastReadTime = time(NULL);
      for (int i = 0; i<nanoCom->GetOutSize(); i++){
        printf("%c",ReadenData[i]);
      }
    }else{
      if (difftime(time(NULL),lastReadTime)>SECONDS_TO_RECONNECT){
        nanoCom->~SerialCom();
        nanoCom = new SerialCom(NANO_COM_PORT, NANO_BAUD_RATE);
        lastReadTime = time(NULL);
      }
    }
  }
}
