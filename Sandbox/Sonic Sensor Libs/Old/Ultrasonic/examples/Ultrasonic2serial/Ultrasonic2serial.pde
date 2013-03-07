//
// Ultrasonic HR-SC04 sensor test
//
// http://robocraft.ru
//

#include "Ultrasonic.h"

// sensor connected to:
// Trig - 12, Echo - 13
Ultrasonic ultrasonic(12, 13);

void setup()
{
  Serial.begin(9600); 						// start the serial port
}

void loop()
{
  float dist_cm = ultrasonic.Ranging(CM); 	// get distance
  Serial.println(dist_cm); 					// print the distance
  
  delay(100); 								// arbitary wait time.
}
