#include <Arduino.h>
#include <Adafruit_GFX.h>    // Core graphics library
#include <MCUFRIEND_kbv.h>   // Hardware-specific library
MCUFRIEND_kbv tft;

#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSans12pt7b.h>
#include <Fonts/FreeSerif12pt7b.h>

#include <FreeDefaultFonts.h>

#include <Adafruit_I2CDevice.h>

#define BLACK   0x0000
#define RED     0xF800
#define GREEN   0x07E0
#define WHITE   0xFFFF
#define GREY    0x8410


void setup(){
  Serial.begin(115200);
  uint16_t ID = tft.readID();
  if (ID == 0xD3D3) ID = 0x9481; //force ID if write-only display
  tft.begin(ID);
  tft.setRotation(0);
}


void showmsgXY(int x, int y, int sz, const GFXfont *f, const char *msg)
{
    int16_t x1, y1;
    uint16_t wid, ht;
    tft.drawFastHLine(0, y, tft.width(), WHITE);
    tft.setFont(f);
    tft.setCursor(x, y);
    tft.setTextColor(GREEN);
    tft.setTextSize(sz);
    tft.print(msg);
    delay(1000);
}

void loop(){
  if(Serial.available()){
   char c = (char)Serial.read();
    if(c == 'a') {    // 시작화면일때
      tft.fillScreen(BLACK);
      showmsgXY(20, 80, 1, &FreeSans12pt7b, "START SCREEN!");

    } 
    else if (c == '2') {
      
    }
    else if (c == '3') {

    }
    else if (c == '4') {
    
    }
    else if (c == '5') {

    }

  }
}


