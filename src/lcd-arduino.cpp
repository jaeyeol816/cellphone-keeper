#include <Arduino.h>
#include <Adafruit_GFX.h>    // Core graphics library
#include <MCUFRIEND_kbv.h>   // Hardware-specific library


#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSans12pt7b.h>
#include <Fonts/FreeSans18pt7b.h>
#include <Fonts/FreeSerif12pt7b.h>
#include <Fonts/FreeSansBold12pt7b.h>
#include <Fonts/FreeMonoBold12pt7b.h>

#include <FreeDefaultFonts.h>

#include <Adafruit_I2CDevice.h>

#define BLACK   0x0000
#define RED     0xF800
#define GREEN   0x07E0
#define WHITE   0xFFFF
#define GREY    0x8410
#define BLUE    0x001F

MCUFRIEND_kbv tft;
char nextStatus = 'a';

void setup(){
  Serial.begin(115200);
  uint16_t ID = tft.readID();
  if (ID == 0xD3D3) ID = 0x9481; //force ID if write-only display
  tft.begin(ID);
  tft.setRotation(0);
}


void showmsgXY(int x, int y, int sz, const GFXfont *f, const char *msg, uint16_t color)
{
    int16_t x1, y1;
    uint16_t wid, ht;
    // tft.drawFastHLine(0, y, tft.width(), WHITE);
    tft.setFont(f);
    tft.setCursor(x, y);
    tft.setTextColor(GREEN);
    tft.setTextSize(sz);
    tft.setTextColor(color, BLACK);
    tft.print(msg);
    delay(10);
}

void loop(){
  if (Serial.available() > 0) {
    int letter = Serial.read();

    if (letter == 'a') {   // START (a)
      tft.fillScreen(WHITE);
      showmsgXY(10, 60, 2, &FreeMonoBold12pt7b, "Start Menu", BLUE);
      showmsgXY(0, 120, 1, &FreeSans9pt7b, "Click 'START' button to start timer!", BLUE);
      // showmsgXY(5, 180, 1, &FreeSans12pt7b, "Click '+' button to show statistics", BLUE);

      while (true) {
        if (Serial.available() > 0) {
          char btn = Serial.read();

          if (btn == 'x') break;
        }
      }

    } 
    else if (letter == 'b') {   // TIMER_CONFIG (b)
      tft.fillScreen(WHITE);
      showmsgXY(10, 60, 2, &FreeMonoBold12pt7b, "Set Timer", BLUE);
      while (true) {
        if (Serial.available() > 0) {
          int h = Serial.read();
          h = h - '0';
          while (Serial.available() <= 0);
          int m = Serial.read();
          m = m - '0';
          while (Serial.available() <= 0);
          int s = Serial.read();
          s = s - '0';
          while (Serial.available() <= 0);

          if (Serial.read() == 'x') break;    //p 읽음.

          char buf[15];
          sprintf(buf, "%d : %d : %d", h, m, s);
          tft.fillScreen(WHITE);
          showmsgXY(10, 60, 2, &FreeMonoBold12pt7b, "Set Timer", BLUE);
          delay(5);
          showmsgXY(20, 150, 2, &FreeSans12pt7b, buf, BLUE);
          delay(5);
          showmsgXY(20, 170, 2, &FreeSerif12pt7b, "__", BLUE);
          delay(5);
          showmsgXY(10, 220, 1, &FreeSans9pt7b, "change hours-> +, - button", BLACK);
          delay(5);
          showmsgXY(10, 240, 1, &FreeSans9pt7b, "manipulate 'minutes'-> OK button", BLACK);
          delay(5);
          showmsgXY(10, 260, 1, &FreeSans9pt7b, "start timer-> START button", BLACK);
          delay(5);
          showmsgXY(10, 280, 1, &FreeSans9pt7b, "stop timer-> STOP button", BLACK);
          delay(5);

          break;

        }
      } //while

      int endSignal = false;
      while (true) {    // H, M, S에 모두 걸리는 while
        if (endSignal) break;

        if (Serial.available() > 0) {
          char unit = Serial.read();

          if (unit == 'H') {

            showmsgXY(20, 170, 2, &FreeSerif12pt7b, "__", BLUE);
            while (true) {
              if (Serial.available() > 0) { 
                int h = Serial.read();
                if (h == 'o') break;
                else if (h == 'x') {endSignal = true; break;}
                h = h - '0';
                while (Serial.available() <= 0);

                int m = Serial.read();
                if (m == 'o') break;
                else if (m == 'x') {endSignal = true; break;}
                m = m - '0';
                while (Serial.available() <= 0);

                int s = Serial.read();
                if (s == 'o') break;
                else if (s == 'x') {endSignal = true; break;}
                s = s - '0';
                while (Serial.available() <= 0);

                Serial.read();    // p 읽음.

               
                char buf[15];
                sprintf(buf, "%d : %d : %d", h, m, s);
                tft.fillScreen(WHITE);
                showmsgXY(10, 60, 2, &FreeMonoBold12pt7b, "Set Timer", BLUE);
                delay(5);
                showmsgXY(20, 150, 2, &FreeSans12pt7b, buf, BLUE);
                delay(5);
                showmsgXY(20, 170, 2, &FreeSerif12pt7b, "__", BLUE);
                delay(5);
                showmsgXY(10, 220, 1, &FreeSans9pt7b, "change hours-> +, - button", BLACK);
                delay(5);
                showmsgXY(10, 240, 1, &FreeSans9pt7b, "manipulate 'minutes'-> OK button", BLACK);
                delay(5);
                showmsgXY(10, 260, 1, &FreeSans9pt7b, "start timer-> START button", BLACK);
                delay(5);
                showmsgXY(10, 280, 1, &FreeSans9pt7b, "stop timer-> STOP button", BLACK);
                delay(10);
                
              }
            } // while of H

          }
          else if (unit == 'M') {
            showmsgXY(20, 170, 2, &FreeSerif12pt7b, "     __", BLUE);
            while (true) {
              if (Serial.available() > 0) { 
                int h = Serial.read();
                if (h == 'o') break;
                else if (h == 'x') {endSignal = true; break;}
                h = h - '0';
                while (Serial.available() <= 0);

                int m = Serial.read();
                if (m == 'o') break;
                else if (m == 'x') {endSignal = true; break;}
                m = m - '0';
                while (Serial.available() <= 0);

                int s = Serial.read();
                if (s == 'o') break;
                else if (s == 'x') {endSignal = true; break;}
                s = s - '0';
                while (Serial.available() <= 0);

                Serial.read();    // p 읽음.

               
                char buf[15];
                sprintf(buf, "%d : %d : %d", h, m, s);
                tft.fillScreen(WHITE);
                showmsgXY(10, 60, 2, &FreeMonoBold12pt7b, "Set Timer", BLUE);
                delay(5);
                showmsgXY(20, 150, 2, &FreeSans12pt7b, buf, BLUE);
                delay(5);
                showmsgXY(20, 170, 2, &FreeSerif12pt7b, "     __", BLUE);
                delay(5);
                showmsgXY(10, 220, 1, &FreeSans9pt7b, "change minutes-> +, - button", BLACK);
                delay(5);
                showmsgXY(10, 240, 1, &FreeSans9pt7b, "manipulate 'seconds'-> OK button", BLACK);
                delay(5);
                showmsgXY(10, 260, 1, &FreeSans9pt7b, "start timer-> start button", BLACK);
                delay(5);
                showmsgXY(10, 280, 1, &FreeSans9pt7b, "stop timer-> stop button", BLACK);
                delay(10);
              }
            } // while of M

          }
          else if (unit == 'S') {

            showmsgXY(20, 170, 2, &FreeSerif12pt7b, "         __", BLUE);
            while (true) {
              if (Serial.available() > 0) { 
                int h = Serial.read();
                if (h == 'o') break;
                else if (h == 'x') {endSignal = true; break;}
                h = h - '0';
                while (Serial.available() <= 0);

                int m = Serial.read();
                if (m == 'o') break;
                else if (m == 'x') {endSignal = true; break;}
                m = m - '0';
                while (Serial.available() <= 0);

                int s = Serial.read();
                if (s == 'o') break;
                else if (s == 'x') {endSignal = true; break;}
                s = s - '0';
                while (Serial.available() <= 0);

                if (Serial.read() == 'o') break;    // p 읽음.

               
                char buf[15];
                sprintf(buf, "%d : %d : %d", h, m, s);
                tft.fillScreen(WHITE);
                showmsgXY(10, 60, 2, &FreeMonoBold12pt7b, "Set Timer", BLUE);
                delay(5);
                showmsgXY(20, 150, 2, &FreeSans12pt7b, buf, BLUE);
                delay(5);
                showmsgXY(20, 170, 2, &FreeSerif12pt7b, "         __", BLUE);
                delay(5);
                showmsgXY(10, 220, 1, &FreeSans9pt7b, "change seconds-> +, - button", BLACK);
                delay(5);
                showmsgXY(10, 240, 1, &FreeSans9pt7b, "manipulate 'hour'-> OK button", BLACK);
                delay(5);
                showmsgXY(10, 260, 1, &FreeSans9pt7b, "start timer-> start button", BLACK);
                delay(5);
                showmsgXY(10, 280, 1, &FreeSans9pt7b, "stop timer-> stop button", BLACK);
                delay(10);
              }
            } // while of S

          }
          else if (unit == 'o') {

          }
          else if (unit == '\n') {

          }
          else if (unit == 'x') {
            endSignal = true;
            break;
          }

        }
      } //while

    }
    else if (letter == 'c') {
      tft.fillScreen(WHITE);
      showmsgXY(10, 50, 2, &FreeMonoBold12pt7b, "Be away of phone!", BLUE);
      showmsgXY(10, 150, 1, &FreeMonoBold12pt7b, "Timer", BLUE);
      while (true) {
        if (Serial.available() > 0) {
          int h = Serial.read();
          if (h == 'x') break;
          h = h - '0';
          while (Serial.available() <= 0);

          int m = Serial.read();
          if (h == 'x') break;
          m = m - '0';
          while (Serial.available() <= 0);

          int s = Serial.read();
          if (h == 'x') break;
          s = s - '0';
          while (Serial.available() <= 0);

          if (Serial.read() == 'x') break;    // p 읽음.

          
          char buf[15];
          sprintf(buf, "%d : %d: %d", h, m, s);
          tft.fillScreen(WHITE);
          showmsgXY(10, 50, 2, &FreeMonoBold12pt7b, "Be away of phone!", BLUE);
          showmsgXY(10, 150, 1, &FreeMonoBold12pt7b, "Timer", BLUE);
          showmsgXY(20, 200, 2, &FreeSans12pt7b, buf, BLUE);
          showmsgXY(10, 240, 1, &FreeSans9pt7b, "pause/stop timer-> stop button", BLACK);

        }
      } //while
    

    }
    // else if (letter == 'e') {   // STATISTICS (e)



    // }
    else if (letter == 'f') {   // STOPPED (f)
      tft.fillScreen(WHITE);
      showmsgXY(10, 60, 2, &FreeMonoBold12pt7b, "Paused", BLUE);

      while (true) {
        if (Serial.available() > 0) {
          int h = Serial.read();
          if (h == 'x') break;
          h = h - '0';
          while (Serial.available() <= 0);

          int m = Serial.read();
          if (h == 'x') break;
          m = m - '0';
          while (Serial.available() <= 0);

          int s = Serial.read();
          if (h == 'x') break;
          s = s - '0';
          while (Serial.available() <= 0);

          if (Serial.read() == 'x') break;    // p 읽음.

          
          char buf[15];
          sprintf(buf, "%d : %d : %d left.", h, m, s);
          tft.fillScreen(WHITE);
          showmsgXY(10, 60, 2, &FreeMonoBold12pt7b, "Paused", BLUE);
          showmsgXY(20, 150, 2, &FreeSans12pt7b, buf, BLUE);
          showmsgXY(10, 220, 1, &FreeSans9pt7b, "resume timer-> start button", BLACK);
          showmsgXY(10, 250, 1, &FreeSans9pt7b, "terminate timer-> stop button", BLACK);

        }
      }  // while

    }
    else if (letter == 'g') {   // ENDED (g)

      tft.fillScreen(WHITE);
      showmsgXY(10, 60, 2, &FreeMonoBold12pt7b, "End!", BLUE);

      while (true) {
        if (Serial.available()) {
          int closeCount = Serial.read();
          if (closeCount == 'x') break;
          closeCount = closeCount - '0';
          while (Serial.available() <= 0);

          int outCount = Serial.read();
          if (outCount == 'x') break;
          outCount = outCount - '0';
          while (Serial.available() <= 0);

          int h = Serial.read();
          if (h == 'x') break;
          h = h - '0';
          while (Serial.available() <= 0);

          int m = Serial.read();
          if (h == 'x') break;
          m = m - '0';
          while (Serial.available() <= 0);

          int s = Serial.read();
          if (h == 'x') break;
          s = s - '0';
          while (Serial.available() <= 0);

          if (Serial.read() == 'x') break;    // p 읽음.


          tft.fillScreen(WHITE);
          showmsgXY(10, 60, 2, &FreeMonoBold12pt7b, "End!", BLUE);

          char buf1[25];
          sprintf(buf1, "approach: %d times\n  take out: %d times", closeCount, outCount);
          
          char buf2[25];
          sprintf(buf2, "time: %dh %dm %ds", h, m, s);

          showmsgXY(20, 150, 1, &FreeSerif12pt7b, buf1, RED);
          showmsgXY(20, 220, 1, &FreeSerif12pt7b, buf2, BLACK);
          showmsgXY(5, 250, 1, &FreeSerif12pt7b, "move to start menu-> start button", BLUE);

        }

      }

    }
  }




} //loop


