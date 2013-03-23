#include <RS232Com.h>
#include <GPSSignalSender.h>

int rx_pin = 5;
int tx_pin = 6;

GPSSignalSender *S;

void setup() {
  // put your setup code here, to run once:
  S = new GPSSignalSender(rx_pin, tx_pin);
}

void loop() {
  // put your main code here, to run repeatedly: 
  S->TestDataSend();
  delay(1000);  
}
