#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_SPITFT.h>
#include <Adafruit_GrayOLED.h>

const int button1 = 2;
const int button2 = 3;

bool toggle1 = false;
bool toggle2 = false;

void setup() {
  Serial.begin(9600); 
  pinMode(button1, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);
}



bool debounce(int button) {   // 인수로 들어온 버튼이 클릭되었으면 true반환, 아니면 false반환.
  if (digitalRead(button) == LOW) {
    delay(5);
    if (digitalRead(button) == LOW) return true;
    else return false;
  }
  else return false;
}

void loop() {
  if (debounce(button1)) {
    if (toggle1) toggle1 = false;
    else toggle1 = true;
  }
  else if (debounce(button2)) {
    if (toggle2) toggle2 = false;
    else toggle2 = true;
  }
  Serial.print("toggle1: ");
  Serial.print(toggle1);
  Serial.print(" toggle2: ");
  Serial.println(toggle2);
  delay(500);

}
