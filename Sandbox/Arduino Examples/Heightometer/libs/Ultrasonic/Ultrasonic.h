#ifndef Ultrasonic_h
#define Ultrasonic_h

#include "Arduino.h"
                                    
class Ultrasonic
{
  public:
    Ultrasonic(int trigPin, int echoPin);
    int getRange();
  private:
    int mTrig;       
    int mEcho;                                 
};                                    

#endif