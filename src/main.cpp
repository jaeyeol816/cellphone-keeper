#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#define START 1
#define TIMER_CONFIG 2
#define TIMER 3
#define USER_SETUP 4
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

/*
핀번호
2: 
~3: 
4: 
~5: 
~6: 
7: 
8: 멀티플렉서 s0
~9: 멀티플렉서 s1
~10: 멀티플렉서 s2
~11: 멀티플렉서 s3
12: LED_1_PIN
13: LED_2_PIN
A0: 멀티플렉서 SIG_PIN
A1: 
A2:  
A3: 
A4: 
*/

// Write할 LED: 시프트레지스터 2개 사용해 핀 확장해야 함-->총16개 LED가능 (shiftregister PWM 알아보기)
// <Shifty.h> 이용. https://youtu.be/fKjff9-7Mq8

// 시프트레지스터 이용 안해도 될수도?
// 하드웨어는 RGB 3색 LED스트립 사용
const int LED_1_PIN = 7;
const int LED_2_PIN = 13;

// 버튼,센서는 멀티플렉서(Multiplexer, MUX, 여러 아날로그 또는 디지털 입력 신호 중 하나를 선택하여 선택된 입력을 하나의 라인에 전달하는 장치)
// 를 사용하여 핀 확장하여 인풋값 받기.
//MUX Control Pins
const int SIG_PIN = 0;
const int s0 = 8; // A0, MUX 출력핀
const int s1 = 9;
const int s2 = 10;
const int s3 = 11;

//현재 모드 상태
int status = START;

int controlPin[] = {s0, s1, s2, s3};
int muxChannel[16][4] = { {0,0,0,0},
  {1,0,0,0}, //channel 1  -> 버튼1
  {0,1,0,0}, //channel 2  -> 버튼2
  {1,1,0,0}, //channel 3  -> 버튼3
  {0,0,1,0}, //channel 4  -> 버튼4
  {1,0,1,0}, //channel 5  -> 버튼5
  {0,1,1,0}, //channel 6  -> 스피커
  {1,1,1,0}, //channel 7  -> 모션센서1
  {0,0,0,1}, //channel 8  -> 모션센서2
  {1,0,0,1}, //channel 9  -> 압전센서
  {0,1,0,1}, //channel 10 
  {1,1,0,1}, //channel 11 
  {0,0,1,1}, //channel 12 
  {1,0,1,1}, //channel 13 
  {0,1,1,1}, //channel 14 
  {1,1,1,1}  //channel 15
};

void setup() {
  //MUX
  pinMode(s0, INPUT);
  pinMode(s1, OUTPUT);
  pinMode(s2, OUTPUT);
  pinMode(s3, OUTPUT);

  digitalWrite(s0, LOW);
  digitalWrite(s1, LOW);
  digitalWrite(s2, LOW);
  digitalWrite(s3, LOW);

}



void set_led(int color) {
  switch (color) {
    case NO_COLOR:
      break;
    case RED:
      break;

  }
}

void loop() {
  while (true) {
    switch (status) {
      case START:
        set_led(BLUE);
        //시작 메뉴 출력

      case TIMER_CONFIG:
        // 
        //타이머 설정 화면 출력
        //
      case TIMER:
        set_led(GREEN);
        //메인 로직 (핵심적인 부분)
        //
        //
        //
      case USER_SETUP:
        set_led(ORANGE);
        //환경설정 화면 출력
      
      case STATISTICS:
        //사용량 통계 출력
        //EEPROM에서 데이터 읽어야 함.
      
      case STOPPED:
        //일시정지시 화면 출력
        //
      
      case ENDED:
        //타이머 종료시 화면 출력
        //EEPROM에 데이터 쓰기
      
      default:


  }
  }
  
}