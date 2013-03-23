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

void GPSSignalSender::TestDataSend()
{
  char *Q = new char[256];
  char Q2[] = "$GPRMC,125504.049,A,5542.2389,N,03741.6063,E,0.06,25.82,200906,,,*17?";
  int i = 0;
  while (Q2[i]!='?')
  {
    Q[i] = Q2[i];
    i++;
  }
  Q[i] = 0x0D;
  Q[i+1] = 0x0A;
  Q[i+2] = '\0';
  Send(Q);
  delete [] Q;
}