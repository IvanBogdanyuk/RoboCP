#include "RS232Com.h"

#define bitDelay 21  //baud rate 38400
#define halfBitDelay 10.5
               
RS232Communicator::RS232Communicator(int rxPin, int txPin)
{                         
  pinMode(rxPin,INPUT);
  pinMode(txPin,OUTPUT);
  digitalWrite(txPin,HIGH);                       
  rx = rxPin;
  tx = txPin; 
}
                 
void RS232Communicator::Write(byte data)
{
  byte mask;
  digitalWrite(tx,LOW);
  delayMicroseconds(bitDelay);
  for (mask = 0x01; mask>0; mask <<= 1) {
    if (data & mask){ 
     digitalWrite(tx,HIGH); 
    }
    else{
     digitalWrite(tx,LOW); 
    }
    delayMicroseconds(bitDelay);
  }
  digitalWrite(tx, HIGH);
  delayMicroseconds(bitDelay);
}    

byte RS232Communicator::Read()
{
  byte value = 0;
  while (digitalRead(rx));
  if (digitalRead(rx) == LOW) {
    delayMicroseconds(halfBitDelay);
    for (int offset = 0; offset < 8; offset++) {
     delayMicroseconds(bitDelay);
     value |= digitalRead(rx) << offset;
    }
    delayMicroseconds(2*bitDelay); 
    return value;
  }
}
