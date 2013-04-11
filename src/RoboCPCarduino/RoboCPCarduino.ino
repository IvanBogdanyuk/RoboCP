#include <Boards.h>
#include <Firmata.h>

#include <RS232Com.h>
#include <GPSSignalSender.h>
#include <GPSSignalMaker.h>
#include <UltrasonicArray.h>

//////////
//CONFIG//
//////////
#define GPS_SIGNAL_MAKER_RX 7
#define GPS_SIGNAL_MAKER_TX 8
#define SERIAL_BUFF_SIZE 128
#define SERIAL_BAUD_RATE 115200
#define FRONT_SONIC_TRIG 3
#define FRONT_SONIC_ECHO 4
#define RIGHT_SONIC_TRIG 5
#define RIGHT_SONIC_ECHO 6
#define BACK_SONIC_TRIG 5
#define BACK_SONIC_ECHO 6
#define LEFT_SONIC_TRIG 5
#define LEFT_SONIC_ECHO 6
#define TOP_SONIC_TRIG 5
#define TOP_SONIC_ECHO 6
///////
//END//
///////

GPSSignalMaker *SignalMaker = new GPSSignalMaker(GPS_SIGNAL_MAKER_RX, GPS_SIGNAL_MAKER_TX);
UltrasonicArray *SonicArray = new UltrasonicArray(2);
char *NewData = new char[SERIAL_BUFF_SIZE];
byte *DataToSend;
char Stage;
char XorSumm;
char LastByte;
char ThisByte;
int NextByte;
unsigned int *Distantion;

void setup(){
  Serial.begin(SERIAL_BAUD_RATE);
  Stage = 0;
  SonicArray->SetSonic(0,FRONT_SONIC_TRIG,FRONT_SONIC_ECHO);
  SonicArray->SetSonic(1,RIGHT_SONIC_TRIG,RIGHT_SONIC_ECHO);
  //SonicArray->SetSonic(2,BACK_SONIC_TRIG,BACK_SONIC_ECHO);
  //SonicArray->SetSonic(3,LEFT_SONIC_TRIG,LEFT_SONIC_ECHO);
  //SonicArray->SetSonic(4,TOP_SONIC_TRIG,TOP_SONIC_ECHO);
}

void loop(){
  while(Serial.available()){
    ThisByte = Serial.read();
    if ((ThisByte == '$')&&(Stage == 0)){
      Stage = 1;
      NextByte = 0;
      XorSumm = 0;
      NewData[NextByte++] = ThisByte;  
    }else{
      if ((ThisByte == '*')&&(Stage == 1)){
        if (LastByte == XorSumm){
          SignalMaker->ChangeMessage(NewData);
        }
        Stage = 0;
      }
      if ((Stage == 1)&&(NextByte<SERIAL_BUFF_SIZE)){
        NewData[NextByte++] = ThisByte;
      }else{
        Stage = 0;
      }
    }
    XorSumm ^= LastByte;
    LastByte = ThisByte;
  }
  SignalMaker->SendMessage();
  SonicArray->Run();
  Distantion = SonicArray->GetDist();
  Serial.write(131);
  Serial.write(130);
  Serial.write(129);
  Serial.write(128);
  DataToSend = (byte *)&Distantion[0];
  Serial.write(DataToSend, 2);
  DataToSend = (byte *)&Distantion[1];
  Serial.write(DataToSend, 2);
  DataToSend = (byte *)&Distantion[0];
  Serial.write(DataToSend, 2);
  DataToSend = (byte *)&Distantion[0];
  Serial.write(DataToSend, 2);
  DataToSend = (byte *)&Distantion[1];
  Serial.write(DataToSend, 2);
}
