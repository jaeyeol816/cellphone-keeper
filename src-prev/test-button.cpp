#include <Arduino.h>
#include <Adafruit_SPITFT.h>
#include <Adafruit_GrayOLED.h>

void setup() {

  Serial.begin(9600);
  pinMode(2, INPUT_PULLUP);
  
}

void loop() {

  boolean now = digitalRead(2);

  if (now == LOW) {
    Serial.println("on");
  }
  else if (now == HIGH) {
    Serial.println("off");
  }
  delay(1000);
}

