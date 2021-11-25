#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_SPITFT.h>
#include <Adafruit_GrayOLED.h>

#define PIN 1

void setup() {
  pinMode(7, INPUT);
  Serial.begin(9600);
}

void loop() {
  int val = analogRead(PIN);

  Serial.println(val);

  delay(500);
}





