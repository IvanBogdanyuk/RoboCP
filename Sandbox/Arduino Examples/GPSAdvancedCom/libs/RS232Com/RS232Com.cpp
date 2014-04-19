#include "RS232Com.h"

#define BIT_DELAY 21  //baud rate 38400
#define HALF_BIT_DELAY 10.5
               
RS232Communicator::RS232Communicator(int rxPin, int txPin)
{                         
  pinMode(rxPin,INPUT);
  pinMode(txPin,OUTPUT);
  digitalWrite(txPin,HIGH);                       
  rx = rxPin;
  tx = txPin; 
}
                 
void RS232Communicator::Write(byte Data)
{
  byte mask;
  digitalWrite(tx,LOW);
  delayMicroseconds(BIT_DELAY);
  for (mask = 0x01; mask>0; mask <<= 1) {
    if (Data & mask){ 
     digitalWrite(tx,HIGH); 
    }
    else{
     digitalWrite(tx,LOW); 
    }
    delayMicroseconds(BIT_DELAY);
  }
  digitalWrite(tx, HIGH);
  delayMicroseconds(BIT_DELAY);
}    

byte RS232Communicator::Read()
{
  byte value = 0;
  while (digitalRead(rx));
  if (digitalRead(rx) == LOW) {
    delayMicroseconds(HALF_BIT_DELAY);
    for (int offset = 0; offset < 8; offset++) {
     delayMicroseconds(BIT_DELAY);
     value |= digitalRead(rx) << offset;
    }
    delayMicroseconds(2*BIT_DELAY); 
    return value;
  }
}
