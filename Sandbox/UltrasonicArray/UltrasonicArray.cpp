#include "UltrasonicArray.h"

#define DIGITAL_READ_ITERATIONS 10000
#define TIME_TO_LISTEN 20000

UltrasonicArray::UltrasonicArray(int Size)
{
  dist = new int[Size];
  tPin = new int[Size];
  ePin = new int[Size];
  size = Size;
  iterations = 0;
}

UltrasonicArray::~UltrasonicArray()
{
  delete [] dist;
  delete [] tPin;
  delete [] ePin;
}

void UltrasonicArray::Run()
{
  unsigned long *startEcho = new unsigned long[size];
  unsigned long *endEcho = new unsigned long[size];
  bool *flag = new bool[size];
  for (int i = 0; i<size; i++){
    startEcho[i] = 0;
    endEcho[i] = 0;
    flag[i] = true;
  }
  for (int i = 0; i<size; i++){
    digitalWrite(tPin[i], HIGH);
    delayMicroseconds(10);
    digitalWrite(tPin[i], LOW);
  }
  for (int i = 0; i<iterations; i++){
    for (int j = 0; j<size; j++){
      if (digitalRead(ePin[j])==HIGH){
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
  for (int i = 0; i < size; i++){
    if ((endEcho[i]-startEcho[i])/58 < 30000){
      dist[i] = (int)((endEcho[i]-startEcho[i])/58);
    }else{
      dist[i] = 800;
    }
  }
  delete [] startEcho;
  delete [] endEcho;
  delete [] flag;
}

void UltrasonicArray::SetSonic(int Id, int TrigPin, int EchoPin)
{
  tPin[Id] = TrigPin;
  ePin[Id] = EchoPin;
  pinMode(TrigPin,OUTPUT);
  pinMode(EchoPin,INPUT);
  digitalWrite(TrigPin,LOW);
  if (iterations == 0){
    int i = 0;
    unsigned long time = micros();
    while (i<DIGITAL_READ_ITERATIONS){	
      digitalRead(EchoPin);
      i++;
    }
    int timeToDigitalRead = (int)((micros()-time)/DIGITAL_READ_ITERATIONS);
    iterations = TIME_TO_LISTEN/(timeToDigitalRead*size);
  }
}

int *UltrasonicArray::GetDist()
{
  return dist;
}