#ifndef GPSSignalSender_h
#define GPSSignalSender_h
                    
#include <RS232Com.h>

class GPSSignalSender
{
  public:
    GPSSignalSender(int rxPin, int txPin);
    ~GPSSignalSender();
    void Send(char *data);
  private:                      
    RS232Communicator *Comm;
};                                    

#endif