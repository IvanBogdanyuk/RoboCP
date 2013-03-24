#include "UltrasonicArray.h"


UltrasonicArray::UltrasonicArray(int size)
{
  mDist = new int[size];
  mTPin = new int[size];
  mEPin = new int[size];
  mSize = size;
  mIterations = 20000/5/size;
}

UltrasonicArray::~UltrasonicArray()
{
  delete [] mDist;
  delete [] mTPin;
  delete [] mEPin;
}

void UltrasonicArray::Run()
{
  long *Duration = new long[mSize];
  bool *flag = new bool[mSize];
  for (int i = 0; i<mSize; i++){
    Duration[i] = 0;
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
	    if (Duration[j] == 0){
		  flag[j] = false;
		  Duration[j] = i;
		}
		if (flag[j] == false){
		  Duration[j]++;
		}
	  }else{
	    flag[j] = true;
	  }
	}
  }
  for (int i = mSize-1; i >= 0; i--){
    mDist[i] = (20*i + Duration[i]*6*mSize)/58;
  }
  delete [] Duration;
  delete [] flag;
}

void UltrasonicArray::SetSonic(int id, int trigPin, int echoPin)
{
  mTPin[id] = trigPin;
  mEPin[id] = echoPin;
  pinMode(trigPin,OUTPUT);
  pinMode(echoPin,INPUT);
  digitalWrite(trigPin,LOW);
}

int *UltrasonicArray::GetDist()
{
  return mDist;
}