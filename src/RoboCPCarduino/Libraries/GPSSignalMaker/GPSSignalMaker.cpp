#include "GPSSignalMaker.h"

#define SENDING_BUFFER_SIZE 128

char *DefaultMessage = "$,0000.0005,00000.0005,,,";

GPSSignalMaker::GPSSignalMaker(int rxPin, int txPin)
{            
  sender = new GPSSignalSender(rxPin,txPin);
  dataToSend = new char[SENDING_BUFFER_SIZE];
  ChangeMessage(DefaultMessage);
}                        

GPSSignalMaker::~GPSSignalMaker()
{
  sender->~GPSSignalSender();
  delete [] dataToSend;
}

char ByteToHex(char num)
{
  if (num<10){
    return '0'+num;
  }
  return 'A'+num-10;
}

void GPSSignalMaker::SendMessage()
{
  sender->Send(dataToSend);
}

void GPSSignalMaker::UseDefaultMessage()
{
  ChangeMessage(DefaultMessage);
}

//$UTC,Latitude,Longtitude,GSpeed,SpeedAngle,
void GPSSignalMaker::ChangeMessage(char *Data)
{
  dataToSend[0] = '$';
  dataToSend[1] = 'G';
  dataToSend[2] = 'P';
  dataToSend[3] = 'R';
  dataToSend[4] = 'M';
  dataToSend[5] = 'C';
  dataToSend[6] = ','; 
  int next = 7;
  int i = 1;
  while (Data[i] != ','){  //UTC
    dataToSend[next++] = Data[i++];
  }
  i++;
  dataToSend[next++]=',';
  dataToSend[next++]='A';  
  dataToSend[next++]=',';
  while (Data[i] != ','){  //Latitude
    dataToSend[next++] = Data[i++];
  }
  i++;
  dataToSend[next++]=',';
  dataToSend[next++]='N';  
  dataToSend[next++]=',';
  while (Data[i] != ','){  //Longtitude
    dataToSend[next++] = Data[i++];
  }
  i++;
  dataToSend[next++]=',';
  dataToSend[next++]='E';
  dataToSend[next++]=',';
  while (Data[i] != ','){  //Ground speed
    dataToSend[next++] = Data[i++];
  }
  i++;
  dataToSend[next++]=',';
  while (Data[i] != ','){  //Velocity angle
    dataToSend[next++] = Data[i++];
  }
  i++;
  dataToSend[next++]=',';
  //skipped current date
  dataToSend[next++]=',';
  //skipped magnetic variation
  dataToSend[next++]=',';
  //skipped E/W for variation
  dataToSend[next]='*';
  char c = 0;
  for (int i = 1; i<next; i++){
    c = (c^dataToSend[i]);
  }
  next++;
  dataToSend[next++] = ByteToHex(c / 16);
  dataToSend[next++] = ByteToHex(c % 16);
  dataToSend[next++] = '\r';
  dataToSend[next++] = '\n';
  dataToSend[next] = '\0';
}