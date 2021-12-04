#include <Adafruit_GFX.h>    // Core graphics library
#include <MCUFRIEND_kbv.h>   // Hardware-specific library
#include <Wire.h>

#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSans12pt7b.h>
#include <Fonts/FreeSans18pt7b.h>
#include <Fonts/FreeSerif12pt7b.h>
#include <Fonts/FreeSansBold12pt7b.h>
#include <Fonts/FreeMonoBold12pt7b.h>

#include <FreeDefaultFonts.h>

#define BLACK   0x0000
#define RED     0xF800
#define GREEN   0x07E0
#define WHITE   0xFFFF
#define GREY    0x8410
#define BLUE    0x001F

MCUFRIEND_kbv tft;

void setup(void)
{
    Serial.begin(9600);
    uint16_t ID = tft.readID();
    Serial.println("Example: Font_simple");
    Serial.print("found ID = 0x");
    Serial.println(ID, HEX);
    if (ID == 0xD3D3) ID = 0x9481; //force ID if write-only display
    tft.begin(ID);
    tft.setRotation(0);
}

void showmsgXY(int x, int y, int sz, const GFXfont *f, const char *msg)
{
    int16_t x1, y1;
    uint16_t wid, ht;
    // tft.drawFastHLine(0, y, tft.width(), WHITE);
    tft.setFont(f);
    tft.setCursor(x, y);
    tft.setTextColor(GREEN);
    tft.setTextSize(sz);
    tft.setTextColor(BLUE, BLACK);
    tft.print(msg);
    // delay(10);
}


void loop(void)
{
    tft.fillScreen(WHITE);
    showmsgXY(10, 60, 2, &FreeMonoBold12pt7b, "Start Menu");
    showmsgXY(5, 120, 1, &FreeSans12pt7b, "Click 'start' button to start timer!");
    showmsgXY(5, 180, 1, &FreeSans12pt7b, "Click '+' button to show statistics");
    char buf[15];
    sprintf(buf, "%d : %d : %d", 1, 2, 3);
    showmsgXY(20, 150, 2, &FreeSans12pt7b, buf);

    // showmsgXY(5, 200, 1, &FreeSevenSegNumFont, "01234");

   
    delay(10000);
}
