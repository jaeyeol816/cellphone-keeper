#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_SPITFT.h>
#include <Adafruit_GrayOLED.h>


const int speaker = 9;

void setup() {

}

void loop() {
  tone(speaker, 300, 500);
  delay(500);
}