#include <Arduino.h>
#include <EEPROM.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_SPITFT.h>
#include <Adafruit_GrayOLED.h>

void setup() {
  Serial.begin(9600);
  pinMode(7, INPUT);

}

void loop() {


  int val = digitalRead(7);   //가까이오면 0, 아니면 1

  Serial.println(val);

  delay(100);
}