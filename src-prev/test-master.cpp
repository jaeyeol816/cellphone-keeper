#include <Arduino.h>
#include <Adafruit_SPITFT.h>
#include <Adafruit_GrayOLED.h>

void setup(){
  Serial.begin(115200);
}
 
void loop(){
  Serial.print("1");   // LED on
  delay(500);
  Serial.print("0");   // LED off
  delay(500);


    
}