#include <Boards.h>
#include <Firmata.h>

#include <RS232Com.h>
#include <GPSSignalSender.h>
#include <GPSSignalMaker.h>
#include <UltrasonicArray.h>

#define SERIAL_BUFF_SIZE 128
#define FRONT_SONIC_TRIG 3
#define FRONT_SONIC_ECHO 4
#define RIGHT_SONIC_TRIG 5
#define RIGHT_SONIC_ECHO 6
/*
#define BACK_SONIC_TRIG 5
#define BACK_SONIC_ECHO 6
#define LEFT_SONIC_TRIG 5
#define LEFT_SONIC_ECHO 6
#define TOP_SONIC_TRIG 5
#define TOP_SONIC_ECHO 6
*/


GPSSignalMaker *SignalMaker = new GPSSignalMaker(7,8);
UltrasonicArray *SonicArray = new UltrasonicArray(2);
char *NewData = new char[SERIAL_BUFF_SIZE];
char Stage;
char XorSumm;
char LastByte;
char ThisByte;
int NextByte;
int *Distantion;

void setup(){
  Serial.begin(9600);
  Stage = 0;
  SonicArray->SetSonic(0,FRONT_SONIC_TRIG,FRONT_SONIC_ECHO);
  SonicArray->SetSonic(1,RIGHT_SONIC_TRIG,RIGHT_SONIC_ECHO);
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
  Serial.print(Distantion[0]);
  Serial.print(' ');
  Serial.print(Distantion[1]);
  Serial.print('\r');
  Serial.print('\n');
}
