#include <Boards.h>
#include <Firmata.h>

#include <RS232Com.h>
#include <GPSSignalSender.h>
#include <GPSSignalMaker.h>
#include <UltrasonicArray.h>

#define SERIAL_BUFF_SIZE 128

GPSSignalMaker *M = new GPSSignalMaker(7,8);
UltrasonicArray *A = new UltrasonicArray(2);
char *NewData = new char[SERIAL_BUFF_SIZE];
char stage;
char xor_summ;
char last_byte;
char this_byte;
int next_byte;
int *distantion;

void setup(){
  Serial.begin(9600);
  stage = 0;
  A->SetSonic(0,3,4);
  A->SetSonic(1,5,6);
}

void loop(){
  while(Serial.available()){
    this_byte = Serial.read();
    if ((this_byte == '$')&&(stage == 0)){
      stage = 1;
      next_byte = 0;
      xor_summ = 0;
      NewData[next_byte++] = this_byte;  
    }else{
      if ((this_byte == '*')&&(stage == 1)){
        if (last_byte == xor_summ){
          M->ChangeMessage(NewData);
        }
        stage = 0;
      }
      if ((stage == 1)&&(next_byte<SERIAL_BUFF_SIZE)){
        NewData[next_byte++] = this_byte;
      }else{
        stage = 0;
      }
    }
    xor_summ ^= last_byte;
    last_byte = this_byte;
  }
  M->SendMessage();
  A->Run();
  distantion = A->GetDist();
  Serial.print(distantion[0]);
  Serial.write(' ');
  Serial.print(distantion[1]);
  Serial.write('\r');
  Serial.write('\n');
  delay(100);
}
