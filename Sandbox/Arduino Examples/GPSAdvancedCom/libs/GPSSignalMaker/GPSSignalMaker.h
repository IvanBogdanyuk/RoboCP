#ifndef GPSSignalMaker_h
#define GPSSignalMaker_h

#include <GPSSignalSender.h>

class GPSSignalMaker
{
  public:
    GPSSignalMaker(int rxPin, int txPin);
    ~GPSSignalMaker();
    void ChangeMessage(char *Data);
    void UseDefaultMessage();
    void SendMessage();
  private:            
    GPSSignalSender *sender;  
    char *dataToSend;
};                                    

#endif