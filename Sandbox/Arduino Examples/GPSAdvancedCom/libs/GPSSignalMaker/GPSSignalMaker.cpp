#include "GPSSignalMaker.h"

#define SENDING_BUFFER_SIZE 128

char *DefaultMessage = "$,0000.0005,00000.0005,,,";

GPSSignalMaker::GPSSignalMaker(int rxPin, int txPin)
{            
  mSender = new GPSSignalSender(rxPin,txPin);
  mDataToSend = new char[SENDING_BUFFER_SIZE];
  ChangeMessage(DefaultMessage);
}                        

GPSSignalMaker::~GPSSignalMaker()
{
  mSender->~GPSSignalSender();
  delete [] mDataToSend;
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
  mSender->Send(mDataToSend);
}

void GPSSignalMaker::UseDefaultMessage()
{
  ChangeMessage(DefaultMessage);
}

//$UTC,Latitude,Longtitude,GSpeed,SpeedAngle,X*
void GPSSignalMaker::ChangeMessage(char *Data)
{
  mDataToSend[0] = '$';
  mDataToSend[1] = 'G';
  mDataToSend[2] = 'P';
  mDataToSend[3] = 'R';
  mDataToSend[4] = 'M';
  mDataToSend[5] = 'C';
  mDataToSend[6] = ','; 
  int next = 7;
  int i = 1;
  while (Data[i] != ','){  //UTC
    mDataToSend[next++] = Data[i++];
  }
  i++;
  mDataToSend[next++]=',';
  mDataToSend[next++]='A';  
  mDataToSend[next++]=',';
  while (Data[i] != ','){  //Latitude
    mDataToSend[next++] = Data[i++];
  }
  i++;
  mDataToSend[next++]=',';
  mDataToSend[next++]='N';  
  mDataToSend[next++]=',';
  while (Data[i] != ','){  //Longtitude
    mDataToSend[next++] = Data[i++];
  }
  i++;
  mDataToSend[next++]=',';
  mDataToSend[next++]='E';
  mDataToSend[next++]=',';
  while (Data[i] != ','){  //Ground speed
    mDataToSend[next++] = Data[i++];
  }
  i++;
  mDataToSend[next++]=',';
  while (Data[i] != ','){  //Velocity angle
    mDataToSend[next++] = Data[i++];
  }
  i++;
  mDataToSend[next++]=',';
  //skipped current date
  mDataToSend[next++]=',';
  //skipped magnetic variation
  mDataToSend[next++]=',';
  //skipped E/W for variation
  mDataToSend[next]='*';
  char c = 0;
  for (int i = 1; i<next; i++){
    c = (c^mDataToSend[i]);
  }
  next++;
  mDataToSend[next++] = ByteToHex(c / 16);
  mDataToSend[next++] = ByteToHex(c % 16);
  mDataToSend[next++] = '\r';
  mDataToSend[next++] = '\n';
  mDataToSend[next] = '\0';
}