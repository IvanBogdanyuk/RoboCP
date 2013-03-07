#ifndef Rangefinder_h
#define Rangefinder_h

#include "Arduino.h"
#include "WProgram.h"
                                    
class Rangefinder
{
  public:
    Rangefinder(int trigPin, int echoPin);
    int getRange();
  private:
    int mTrig;       
    int mEcho;                                 
};                                    

#endif