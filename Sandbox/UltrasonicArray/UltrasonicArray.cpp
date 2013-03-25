#include "UltrasonicArray.h"

#define DIGITAL_READ_ITERATIONS 10000

UltrasonicArray::UltrasonicArray(int size)
{
  mDist = new int[size];
  mTPin = new int[size];
  mEPin = new int[size];
  mSize = size;
  mIterations = 0;
}

UltrasonicArray::~UltrasonicArray()
{
  delete [] mDist;
  delete [] mTPin;
  delete [] mEPin;
}

void UltrasonicArray::Run()
{
  unsigned long *startEcho = new unsigned long[mSize];
  unsigned long *endEcho = new unsigned long[mSize];
  bool *flag = new bool[mSize];
  for (int i = 0; i<mSize; i++){
    startEcho[i] = 0;
    endEcho[i] = 0;
    flag[i] = true;
  }
  for (int i = 0; i<mSize; i++){
    digitalWrite(mTPin[i], HIGH);
    delayMicroseconds(10);
    digitalWrite(mTPin[i], LOW);
  }
  for (int i = 0; i<mIterations; i++){
    for (int j = 0; j<mSize; j++){
      if (digitalRead(mEPin[j])==HIGH){
        if (startEcho[j] == 0){
        flag[j] = false;
          startEcho[j] = micros();
        }
    	if (flag[j] == false){
          endEcho[j] = micros();
        }
      }else{
        flag[j] = true;
      }
    }
  }
  for (int i = 0; i < mSize; i++){
    if ((endEcho[i]-startEcho[i])/58 < 30000){
      mDist[i] = (int)((endEcho[i]-startEcho[i])/58);
    }else{
      mDist[i] = 800;
    }
  }
  delete [] startEcho;
  delete [] endEcho;
  delete [] flag;
}

void UltrasonicArray::SetSonic(int id, int trigPin, int echoPin)
{
  mTPin[id] = trigPin;
  mEPin[id] = echoPin;
  pinMode(trigPin,OUTPUT);
  pinMode(echoPin,INPUT);
  digitalWrite(trigPin,LOW);
  if (mIterations == 0){
    int i = 0;
    unsigned long time = micros();
    while (i<DIGITAL_READ_ITERATIONS){	
      digitalRead(echoPin);
     i++;
    }
    int mTimeToDigitalRead = (int)((micros()-time)/DIGITAL_READ_ITERATIONS);
    mIterations = 20000/(mTimeToDigitalRead*mSize);
  }
}

int *UltrasonicArray::GetDist()
{
  return mDist;
}