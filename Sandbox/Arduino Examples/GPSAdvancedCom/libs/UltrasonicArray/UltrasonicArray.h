#ifndef UltrasonicArray_h
#define UltrasonicArray_h

#include "Arduino.h"

class UltrasonicArray
{
  public:
    UltrasonicArray(int Size);
    ~UltrasonicArray();
    void Run();
    void SetSonic(int Id, int TrigPin, int EchoPin);
    int *GetDist();
  private:
    int *tPin;
    int *ePin;
    int *dist;
    int size;
    int iterations;
};

#endif