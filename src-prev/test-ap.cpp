#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_SPITFT.h>
#include <Adafruit_GrayOLED.h>

int value;

void setup() {
  Serial.begin(9600);
}

void loop() {
  value = analogRead(1);
  Serial.println(value);
  delay(1000);
}
