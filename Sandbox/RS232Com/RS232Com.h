#ifndef RS232Com_h
#define RS232Com_h

#include "Arduino.h"
                                    
class RS232Communicator
{
  public:
    RS232Communicator(int rxPin, int txPin);
    void Write(int data);
    int Read();
  private:                                     
    int rx;
    int tx;
};                                    

#endif