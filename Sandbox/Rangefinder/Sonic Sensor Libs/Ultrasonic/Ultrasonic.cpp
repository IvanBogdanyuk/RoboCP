#include "Ultrasonic.h"
               
Ultrasonic::Ultrasonic(int trigPin, int echoPin)
{                     
   mTrig=trigPin;
   mEcho=echoPin;
   pinMode(mTrig, OUTPUT);
   pinMode(mEcho, INPUT);
}

int Ultrasonic::getRange()
{
  digitalWrite(mTrig, LOW);
  delayMicroseconds(2);
  digitalWrite(mTrig, HIGH);
  delayMicroseconds(10);
  digitalWrite(mTrig, LOW);      
  return pulseIn(mEcho, HIGH)/58;
}
                 