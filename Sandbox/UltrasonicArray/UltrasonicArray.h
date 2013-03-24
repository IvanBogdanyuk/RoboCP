#ifndef UltrasonicArray_h
#define UltrasonicArray_h

#include "Arduino.h"

class UltrasonicArray
{
  public:
    UltrasonicArray(int size);
    ~UltrasonicArray();
    void Run();
    void SetSonic(int id, int trigPin, int echoPin);
    int *GetDist();
  private:
    int *mTPin;
    int *mEPin;
    int *mDist;
    int mSize;
    int mIterations;
};

#endif