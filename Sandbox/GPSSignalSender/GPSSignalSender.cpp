#include "GPSSignalSender.h"
               
GPSSignalSender::GPSSignalSender(int rxPin, int txPin)
{            
  Comm = new RS232Communicator(rxPin,txPin);
}                        

GPSSignalSender::~GPSSignalSender()
{
  Comm->~RS232Communicator();
}

void GPSSignalSender::Send(char *Data)
{
  int i = 0;
  while (Data[i]!='\0')
  {
    Comm->Write(Data[i]);
    i++;
  }
}