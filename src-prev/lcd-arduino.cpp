#include <Arduino.h>
#include <Adafruit_GFX.h>    // Core graphics library
#include <MCUFRIEND_kbv.h>   // Hardware-specific library


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

MCUFRIEND_kbv tft;
char nextStatus = 'a';

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
  if (Serial.available() > 0) {
    char letter = (char)Serial.read();

    if (letter == 'a') {   // START (a)
      tft.fillScreen(BLACK);
      showmsgXY(20, 80, 2, &FreeSans12pt7b, "Start Menu!");

      while (true) {
        if (Serial.available() > 0) {
          char btn = (char)Serial.read();

          if (btn == 'x') break;
        }
      }

    } 
    else if (letter == 'b') {   // TIMER_CONFIG (b)

      showmsgXY(20, 80, 2, &FreeSans12pt7b, "Timer Config!");
      while (true) {
        if (Serial.available() > 0) {
          int h = Serial.parseInt();
          int m = Serial.parseInt();
          int s = Serial.parseInt();

          if (Serial.read() == '\n') {    //시, 분, 초 다 넘어옴.
            char buf[12];
            sprintf(buf, "%d : %d : %d", h, m, s);
            showmsgXY(20, 100, 2, &FreeSans12pt7b, buf);
            break;
          }
        }
      } //while

      int endSignal = false;
      while (true) {    // H, M, S에 모두 걸리는 while문
        if (endSignal) break;

        if (Serial.available() > 0) {
          char unit = (char)Serial.read();

          if (unit == 'H') {
            while (true) {
              if (Serial.available() > 0) { 
                int h = Serial.parseInt();
                if ((char)h == 'x') { endSignal = true; break; }
                else if ((char)h == 'o') {break;}
                int m = Serial.parseInt();
                if ((char)m == 'x') { endSignal = true; break; }
                else if ((char)m == 'o') {break;}
                int s = Serial.parseInt();
                if ((char)s == 'x') { endSignal = true; break; }
                else if ((char)s == 'o') {break;}

                if (Serial.read() == '\n') {
                  char buf[12];
                  sprintf(buf, "%d : %d : %d", h, m, s);
                  showmsgXY(20, 100, 2, &FreeSans12pt7b, buf);
                }
              }
            } // while of H

          }
          else if (unit == 'M') {

            while (true) {
              if (Serial.available() > 0) { 
                int h = Serial.parseInt();
                if ((char)h == 'x') { endSignal = true; break; }
                else if ((char)h == 'o') {break;}
                int m = Serial.parseInt();
                if ((char)m == 'x') { endSignal = true; break; }
                else if ((char)m == 'o') {break;}
                int s = Serial.parseInt();
                if ((char)s == 'x') { endSignal = true; break; }
                else if ((char)s == 'o') {break;}

                if (Serial.read() == '\n') {
                  char buf[12];
                  sprintf(buf, "%d : %d : %d", h, m, s);
                  showmsgXY(20, 100, 2, &FreeSans12pt7b, buf);
                }
              }
            } // while of M

          }
          else if (unit == 'S') {

            while (true) {
              if (Serial.available() > 0) { 
                int h = Serial.parseInt();
                if ((char)h == 'x') { endSignal = true; break; }
                else if ((char)h == 'o') {break;}
                int m = Serial.parseInt();
                if ((char)m == 'x') { endSignal = true; break; }
                else if ((char)m == 'o') {break;}
                int s = Serial.parseInt();
                if ((char)s == 'x') { endSignal = true; break; }
                else if ((char)s == 'o') {break;}

                if (Serial.read() == '\n') {
                  char buf[12];
                  sprintf(buf, "%d : %d : %d", h, m, s);
                  showmsgXY(20, 100, 2, &FreeSans12pt7b, buf);
                }
              }
            } // while of S

          }
          else if (unit == 'o') {

          }
          else if (unit == 'x') {
            endSignal = true;
            break;
          }

        }
      } //while

    }
    else if (letter == 'c') {
      showmsgXY(20, 80, 2, &FreeSans12pt7b, "Timer");
      while (true) {
        if (Serial.available() > 0) {
          int h = Serial.parseInt();
          if ((char)h == 'x') break;
          int m = Serial.parseInt();
          if ((char)m == 'x') break;
          int s = Serial.parseInt();
          if ((char)s == 'x') break;

          if (Serial.read() == '\n') {
            char buf[12];
            sprintf(buf, "%d : %d : %d", h, m, s);
            showmsgXY(20, 100, 2, &FreeSans12pt7b, buf);
          }

        }

      }
     

    }
  


  }




} //loop


