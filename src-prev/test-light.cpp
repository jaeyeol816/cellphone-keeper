#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_SPITFT.h>
#include <Adafruit_GrayOLED.h>


#define PIN            7                              // 제어 신호핀
#define NUMPIXELS      4                          // 제어하고 싶은 LED 개수
 
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
int delayval = 500; // delay for half a second
 
void setup() {
  pixels.begin(); // This initializes the NeoPixel library.
}
 
void loop() {
  for(int i=0;i<NUMPIXELS;i++){
    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    pixels.setPixelColor(i, pixels.Color(0,150,0)); // Moderately bright green color.
 
    pixels.show(); // This sends the updated pixel color to the hardware.
 
    delay(delayval); // Delay for a period of time (in milliseconds).
  }
}