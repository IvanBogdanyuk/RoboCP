#include "GPSSignalSender.h"

GPSSignalSender::GPSSignalSender(int rxPin, int txPin)
{            
  Comm = new RS232Communicator(rxPin,txPin);
}                        

GPSSignalSender::~GPSSignalSender()
{
  Comm->~RS232Communicator();
}

void GPSSignalSender::Send(char *data)
{
  int i = 0;
  while (data[i]!='\0')
  {
    Comm->Write(data[i]);
    i++;
  }
}