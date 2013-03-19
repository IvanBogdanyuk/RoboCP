#include "RS232Com.h"

#define bit9600Delay 84  
#define halfBit9600Delay 42
               
RS232Communicator::RS232Communicator(int rxPin, int txPin)
{                         
  pinMode(rxPin,INPUT);
  pinMode(txPin,OUTPUT);
  digitalWrite(txPin,HIGH);                       
  rx = rxPin;
  tx = txPin; 
}
                 
void RS232Communicator::Write(int data)
{
  int mask;
  digitalWrite(tx,LOW);
  delayMicroseconds(bit9600Delay);
  for (mask = 0x01; mask>0; mask <<= 1) {
    if (data & mask){ 
     digitalWrite(tx,HIGH); 
    }
    else{
     digitalWrite(tx,LOW); 
    }
    delayMicroseconds(bit9600Delay);
  }
  digitalWrite(tx, HIGH);
  delayMicroseconds(bit9600Delay);
}    

int RS232Communicator::Read()
{
  int value = 0;
  while (digitalRead(rx));
  if (digitalRead(rx) == LOW) {
    delayMicroseconds(halfBit9600Delay);
    for (int offset = 0; offset < 32; offset++) {
     delayMicroseconds(bit9600Delay);
     value |= digitalRead(rx) << offset;
    }
    delayMicroseconds(2*bit9600Delay); 
    return value;
  }
}
