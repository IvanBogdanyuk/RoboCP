#include <Ultrasonic.h>
#include <Boards.h>
#include <Firmata.h>

int Trig_1 = A0;
int Echo_1 = A1;
int Trig_2 = A3;
int Echo_2 = A2;
int dist = 16;
int arctg_depth = 20;

Ultrasonic sonic1(Trig_1, Echo_1), sonic2(Trig_2, Echo_2);

float arctg(float x){
  float a_x = x;
  float summ = 0;
  for (int n = 0; n<arctg_depth; n++){
    summ += a_x/(2*n + 1);
    a_x = a_x*x*x*(-1);
  }
  return summ;
}

void setup() {
  Serial.begin(9600);
}

void loop() {
  int l1 = sonic1.getRange();
  delay(100);
  int l2 = sonic2.getRange();
  float angle = arctg((float)(abs(l2 - l1))/dist);
  float height = (l1+l2)*cos(angle)/2;
  angle = angle/PI*180; 
  Serial.println(angle);
  Serial.println(height);
  delay(500);
}
