#ifndef RS232Com_h
#define RS232Com_h

#include "Arduino.h"
                                    
class RS232Communicator
{
  public:
    RS232Communicator(int rxPin, int txPin);
    void Write(byte data);
    byte Read();
  private:                                     
    int rx;
    int tx;
};                                    

#endif