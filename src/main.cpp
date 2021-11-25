#include <Arduino.h>
#include <EEPROM.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_SPITFT.h>
#include <Adafruit_GrayOLED.h>

#define START 1
#define TIMER_CONFIG 2
#define TIMER 3
#define OPTION 4
#define STATISTICS 5
#define STOPPED 6
#define ENDED  7

#define NO_COLOR 1
#define RED 2
#define GREEN 3
#define BLUE 4
#define YELLOW 5
#define PURPLE 6
#define ORANGE 7

#define NUM_OF_PIXELS 7

/*
핀번호
2: start버튼
~3: stop버튼
4: +버튼
~5: -버튼
~6: ok버튼
7: option버튼
8: 
~9: 스피커
~10: LED1
~11: LED2
12: LED3
13: LED4
A0: 압전센서
A1: 모션센서1(?)
A2: 모션센서2(?)
A3: 
A4: 
*/

const int startButton = 2;
const int stopButton = 3;
const int plusButton = 4;
const int minusButton = 5;
const int okButton = 6;
const int optionButton = 7;

const int led1Pin = 10;
const int led2Pin = 11;

const int weightSensorPin = 0;   // A0


//현재 모드 상태
int status = START;
int prevStatus = START;   //돌아왔을때를 위해
int timeLeft[3] = {0, 0, 0};  
int time[3] = {0, 0, 0};
int volume = 5;  
int count[2] = {0, 0};    //접근횟수, 꺼낸횟수 세기
int eepromAddress = 0;    // EEPROM에 writing할 주소.
bool correctEnding = false;   //정상종료시(타이머가 다 되어서 종료되었을 시에만) true됨.

Adafruit_NeoPixel pixels1 = Adafruit_NeoPixel(NUM_OF_PIXELS, led1Pin, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels2 = Adafruit_NeoPixel(NUM_OF_PIXELS, led2Pin, NEO_GRB + NEO_KHZ800);


void setup() {
  Serial.begin(115200);   //연결된 lcd용 아두이노와 통신하기 위해

  pinMode(startButton, INPUT_PULLUP);
  pinMode(stopButton, INPUT_PULLUP);
  pinMode(plusButton, INPUT_PULLUP);
  pinMode(minusButton, INPUT_PULLUP);
  pinMode(okButton, INPUT_PULLUP);
  pinMode(optionButton, INPUT_PULLUP);
  

}



void set_led(int color) {
  switch (color) {
    case NO_COLOR:
      break;
    case RED:
      break;

    case BLUE:
      for(int i=0; i<NUM_OF_PIXELS; i++){
        // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
        pixels1.setPixelColor(i, pixels1.Color(0,0,150)); 
        pixels2.setPixelColor(i, pixels2.Color(0,0,150)); 
    
        pixels1.show(); // This sends the updated pixel color to the hardware.
        pixels2.show();
    
        delay(10); // Delay for a period of time (in milliseconds).
      }
      break;
    
    case GREEN:


  }
}

bool debounce(int button) {   // 인수로 들어온 버튼이 클릭되었으면 true반환, 아니면 false반환.
  if (digitalRead(button) == HIGH) {
    delay(5);
    if (digitalRead(button) == HIGH) return true;
    else return false;
  }
  else return false;
}

void send_time_info() {
  Serial.print(timeLeft[0]);
  Serial.print(",");
  Serial.print(timeLeft[1]);
  Serial.print(",");
  Serial.print(timeLeft[2]);
}

void loop() {
 
  switch (status) {
    case START:
      //시작 메뉴 출력
      Serial.print("a");  //연결된 lcd용 아두이노에게 a 신호를 보낸다. (시작 메뉴를 출력하도록 하기위해)
      set_led(BLUE);
      //무한루프
      while(true) {
        if (debounce(startButton) == true) {  //start버튼 클릭 시
          status = TIMER_CONFIG;
          break;
        }
        else if (debounce(optionButton) == true) {  //option버튼 클릭 시
          status = OPTION;
          break;
        }
        else if (debounce(plusButton) == true) {    // +버튼 클릭시
          status = STATISTICS;
          break;
        }
      }
      prevStatus = START;
      break;

    case TIMER_CONFIG:
      bool endSignal = false;    // 0 --> 계속 무한루프 돈다. 1 --> 탈출

      //타이머 설정 화면 출력
      Serial.print("b");  //연결된 lcd용 아두이노에게 b 신호를 보낸다. (타이머설정화면메뉴를 출력하도록 하기 위해)
      set_led(PURPLE);

      // LCD에게 timeLeft 정보 보내기.
      send_time_info();

      // 무한루프
      while (true) {
        
        while (true) {    // 시(hour) 조절 무한루프 (ok 또는 start또는 stop버튼 눌려야 탈출)
          if (debounce(plusButton) == true) {
            timeLeft[0]++;
            
            send_time_info();
          }
          else if (debounce(minusButton) == true) {
            timeLeft[0]--;

            send_time_info();
          }
          else if (debounce(okButton) == true) {    // 분(min)조절로 넘어감.
            break;
          }
          else if (debounce(startButton) == true) {   // start 클릭시 타이머 시작으로
            status = TIMER;
            endSignal = true;
            break;
          }
          else if (debounce(stopButton) == true) {    // stop 클릭시 지금까지 맞춰놓은 것 초기화. 그리고 시작 화면으로.
            timeLeft[0] = 0;
            timeLeft[1] = 0;
            timeLeft[2] = 0;
            status = prevStatus;
            endSignal = true;
            break;
          }
          if (endSignal) break;

          while (true) {    // 분(min) 조절 무한루프 (ok 또는 start 또는 stop 버튼 눌러야 탈출)
            if (debounce(plusButton) == true) {
              timeLeft[1]++;
              
              send_time_info();
            }
            else if (debounce(minusButton) == true) {
              timeLeft[1]--;

              send_time_info();
            }
            else if (debounce(okButton) == true) {    // 초(sec)조절로 넘어감.
              break;
            }
            else if (debounce(startButton) == true) {   // start 클릭시 타이머 시작으로
              status = TIMER;
              endSignal = true;
              break;
            }
            else if (debounce(stopButton) == true) {    // stop 클릭시 지금까지 맞춰놓은 것 초기화. 그리고 시작 화면으로.
              timeLeft[0] = 0;
              timeLeft[1] = 0;
              timeLeft[2] = 0;
              status = prevStatus;
              endSignal = true;
              break;
            }
          }
          if (endSignal) break;

          while (true) {    // 초(sec) 조절 무한루프 (ok 또는 start 또는 stop버튼 눌러야 탈출)
            if (debounce(plusButton) == true) {
              timeLeft[2]++;
              
              send_time_info();
            }
            else if (debounce(minusButton) == true) {
              timeLeft[2]--;

              send_time_info();
            }
            else if (debounce(okButton) == true) {    // 시(hour)조절로 넘어감.
              break;
            }
            else if (debounce(startButton) == true) {   // start 클릭시 타이머 시작으로
              status = TIMER;
              endSignal = true;
              break;
            }
            else if (debounce(stopButton) == true) {  // stop 클릭시 지금까지 맞춰놓은 것 초기화. 그리고 시작 화면으로.
              timeLeft[0] = 0;
              timeLeft[1] = 0;
              timeLeft[2] = 0;
              status = prevStatus;
              endSignal = true;
              break;
            }
          }
          if (endSignal) break;

        } //while
      } //while

      //끝날 때까지 보관할 시간 정보.
      time[0] = timeLeft[0];
      time[1] = timeLeft[1];
      time[2] = timeLeft[2];
    
      //prevStatus는 TIMER_CONFIG으로!
      prevStatus = TIMER_CONFIG;
      break;


    case TIMER:
      Serial.print("c");
      set_led(GREEN);
      
      bool countMotionAlready = false;
      bool countWeightAlready = false;

      while (true) {
        // 추가할 내용: 모션센서 관련 내용.


        if (analogRead(weightSensorPin) < 10) {   // 무게센서에서 휴대폰 꺼낸 거라고 반응될때,
          set_led(RED);
          if (countWeightAlready == false) {
            count[1]++;
            countWeightAlready = true;
          }
        }
        else {    //꺼냄이 종료되었거나 꺼내지 않았을 때,
          set_led(GREEN);
          countWeightAlready = false;
        }

        send_time_info();

        delay(995); //1초 기다림. (딜레이 고려해서 995ms으로 지정함.)

        //아래는 남은 시간 1초 줄이는 과정
        if (timeLeft[2] == 0) {   //0초일 때,
          if (timeLeft[0] == 0 && timeLeft[1] == 0) {   //0시 0분 0초일 때,
            status = ENDED;   // 타이머 종료 화면으로 넘어감.
            break;
          }
          else if (timeLeft[1] != 0) {    // n분 0초일 때,
            timeLeft[2] = 59;
            timeLeft[1]--;
          }
          else {    // n시 0분 0초일 때,
            timeLeft[2] = 59;
            timeLeft[1] = 59;
            timeLeft[0]--;
          }
        }
        else {  // n초일 때
          timeLeft[2]--;
        }

        //아래는 버튼 클릭되었나 확인하는 과정
        if (debounce(stopButton)) {
          status = STOPPED;   // 일시 중지 화면으로 넘어감.
          break;
        }
        else if (debounce(optionButton)) {
          status = OPTION;  // 옵션 화면으로 넘어감.
          break;
        }

      }
      prevStatus = TIMER;
      break;


    case OPTION:
      
      //환경설정 화면 출력
      //환경설정 후에는 최근에 들어있던 모드로 복귀해야 함. (데이터 손상 없이)
    

    case STATISTICS:
      //사용량 통계 출력
      //EEPROM에서 데이터 읽어야 함.
    

    case STOPPED:
      //일시정지시 화면 출력
      Serial.print("f");
      send_time_info();
      set_led(ORANGE);
      while (true) {
        if (debounce(okButton)) {
          status = TIMER;
          break;
        }
        else if (debounce(stopButton)) {
          status = ENDED;
          break;
        }
        else if (debounce(optionButton)) {
          status = OPTION;
          break;
        }
      }
      prevStatus = STOPPED;
      break;
    

    case ENDED:
      //타이머 종료시 화면 출력
      Serial.print("g");
      set_led(ORANGE);

      // 접근, 꺼낸 기록 전송
      Serial.print(count[0]);
      Serial.print(",");
      Serial.print(count[1]);

      // 시간 정보 전송
      Serial.print(time[0]);
      Serial.print(",");
      Serial.print(time[1]);
      Serial.print(",");
      Serial.print(time[2]);

      if (correctEnding) {    //정상 종료일 경우(타이머 다 되서 종료)
        //EEPROM에 기록. 한 타이머 기록 단위 당 10바이트 쓸 예정
        EEPROM.write(eepromAddress, (char)time[0]);
        eepromAddress++;
        EEPROM.write(eepromAddress, (char)time[1]);
        eepromAddress++;
        EEPROM.write(eepromAddress, (char)time[2]);
        eepromAddress++;
        EEPROM.write(eepromAddress, (char)count[0]);
        eepromAddress++;
        EEPROM.write(eepromAddress, (char)count[1]);
        eepromAddress += 5;

        correctEnding = false;
      }
      timeLeft[0] = 0;
      timeLeft[1] = 0;
      timeLeft[2] = 0;
      time[0] = 0;
      time[1] = 0;
      time[2] = 0;

      while (true) {
        if (debounce(okButton) || debounce(stopButton) || debounce(startButton)) break; 
      }
      status = START;
      prevStatus = ENDED;
    
    default:


  }

  
}