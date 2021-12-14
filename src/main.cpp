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
#define SKY 8

#define NUM_OF_PIXELS 7

/*
핀번호
2: start버튼
~3: stop버튼
4: +버튼
~5: -버튼
~6: ok버튼
7: 
8: 
~9: 스피커
~10: LED1
~11: LED2
12: LED3
13: LED4
A0: 압전센서
A1: 근접센서
A2: 
A3: 
A4: 
*/

const int startButton = 2;
const int stopButton = 3;
const int plusButton = 4;
const int minusButton = 5;
const int okButton = 6;

const int speakerPin = 9;

const int led1Pin = 10;
const int led2Pin = 11;

const int weightSensorPin = 0;   // .A0

const int motionSensorPin = 8;   // D8


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
  // Serial.begin(9600);

  pinMode(startButton, INPUT_PULLUP);
  pinMode(stopButton, INPUT_PULLUP);
  pinMode(plusButton, INPUT_PULLUP);
  pinMode(minusButton, INPUT_PULLUP);
  pinMode(okButton, INPUT_PULLUP);

  pinMode(speakerPin, OUTPUT);

  pixels1.begin();
  pixels2.begin();
  

}



void set_led(int color) {
  switch (color) {
    case NO_COLOR:
      for (int i = 0; i < NUM_OF_PIXELS; i++) {
        pixels1.setPixelColor(i, pixels1.Color(0,0,0)); 
        pixels2.setPixelColor(i, pixels2.Color(0,0,0)); 

        pixels1.show(); 
        pixels2.show();
      }
      break;
    
    case RED:
      for (int i = 0; i < NUM_OF_PIXELS; i++) {
        pixels1.setPixelColor(i, pixels1.Color(200,0,0)); 
        pixels2.setPixelColor(i, pixels2.Color(200,0,0)); 

        pixels1.show(); 
        pixels2.show();
      }
      break;

    case BLUE:
      for(int i=0; i<NUM_OF_PIXELS; i++){
        // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
        pixels1.setPixelColor(i, pixels1.Color(0,0,200)); 
        pixels2.setPixelColor(i, pixels2.Color(0,0,200)); 
    
        pixels1.show(); // This sends the updated pixel color to the hardware.
        pixels2.show();
        delay(10);
      }
      break;
    
    case GREEN:
      for (int i = 0; i < NUM_OF_PIXELS; i++) {
        pixels1.setPixelColor(i, pixels1.Color(0,200,0)); 
        pixels2.setPixelColor(i, pixels2.Color(0,200,0)); 

        pixels1.show(); 
        pixels2.show();
      }
      break;
    
    case ORANGE:
      for (int i = 0; i < NUM_OF_PIXELS; i++) {
        pixels1.setPixelColor(i, pixels1.Color(200,100,0)); 
        pixels2.setPixelColor(i, pixels2.Color(200,100,0)); 

        pixels1.show(); 
        pixels2.show();
      }
      break;
    case PURPLE:
      for (int i = 0; i < NUM_OF_PIXELS; i++) {
        pixels1.setPixelColor(i, pixels1.Color(150,0,150)); 
        pixels2.setPixelColor(i, pixels2.Color(150,0,150)); 

        pixels1.show(); 
        pixels2.show();
      }
      break;

    case YELLOW:
      for (int i = 0; i < NUM_OF_PIXELS; i++) {
        pixels1.setPixelColor(i, pixels1.Color(150,150,0)); 
        pixels2.setPixelColor(i, pixels2.Color(150,150,0)); 

        pixels1.show(); 
        pixels2.show();
      }
      break;
    case SKY:
      for (int i = 0; i < NUM_OF_PIXELS; i++) {
        pixels1.setPixelColor(i, pixels1.Color(0,150,150)); 
        pixels2.setPixelColor(i, pixels2.Color(0,150,150)); 

        pixels1.show(); 
        pixels2.show();
      }

    default:
      break;
  }
}

bool debounce(int button) {   // 인수로 들어온 버튼이 클릭되었으면 true반환, 아니면 false반환.
  if (digitalRead(button) == LOW) {
    delay(5);
    if (digitalRead(button) == LOW) return true;
    else return false;
  }
  else return false;
}

void send_time_info() {
  delay(10);
  Serial.print((char)('0' + timeLeft[0]));
  delay(5);
  Serial.print((char)('0' + timeLeft[1]));
  delay(5);
  Serial.print((char)('0' + timeLeft[2]));
  delay(5);
  Serial.print('p');
}

void loop() {
while(true){
  
  delay(100);
  
  // if start
  if (status == START) {
    //시작 메뉴 출력
    Serial.print("a");  //연결된 lcd용 아두이노에게 a 신호를 보낸다. (시작 메뉴를 출력하도록 하기위해)
    set_led(BLUE);


    //무한루프
    while(true) {
      if (debounce(startButton)) {  //start버튼 클릭 시
        status = TIMER_CONFIG;
        break;
      }
      // else if (debounce(plusButton)) {    // +버튼 클릭시
      //   status = STATISTICS;
      //   break;
      // }
    }
    prevStatus = START;
    Serial.print("x");

  }
  else if (status == TIMER_CONFIG) {

    delay(300);
    bool endSignal = false;    // 0 --> 계속 무한루프 돈다. 1 --> 탈출

    //타이머 설정 화면 출력
    Serial.print("b");  //연결된 lcd용 아두이노에게 b 신호를 보낸다. (타이머설정화면메뉴를 출력하도록 하기 위해)
    set_led(SKY);

    // LCD에게 timeLeft 정보 보내기.
    send_time_info();

    // 무한루프
    while (true) {
      
      Serial.print("H");
      while (true) {    // 시(hour) 조절 무한루프 (ok 또는 start또는 stop버튼 눌려야 탈출)
        
        if (debounce(plusButton)) {
          if (timeLeft[0] < 60) timeLeft[0]++;
          
          send_time_info();
          delay(300);
        }
        if (debounce(minusButton)) {
          if (timeLeft[0] > 0) timeLeft[0]--;

          send_time_info();
          delay(300);
        }
        if (debounce(okButton)) {    // 분(min)조절로 넘어감.
          delay(300);
          break;
        }
        if (debounce(startButton)) {   // start 클릭시 타이머 시작으로
          status = TIMER;
          endSignal = true;
          delay(300);
          break;
        }
        if (debounce(stopButton)) {    // stop 클릭시 지금까지 맞춰놓은 것 초기화. 그리고 시작 화면으로.
          timeLeft[0] = 0;
          timeLeft[1] = 0;
          timeLeft[2] = 0;
          status = prevStatus;
          endSignal = true;
          delay(300);
          break;
        }
      }
      Serial.print("o");
      if (endSignal) break;

      Serial.print("M");
      while (true) {    // 분(min) 조절 무한루프 (ok 또는 start 또는 stop 버튼 눌러야 탈출)
        
        if (debounce(plusButton)) {
          if (timeLeft[1] < 60) timeLeft[1]++;
          
          send_time_info();
          delay(300);
        }
        if (debounce(minusButton)) {
          if (timeLeft[1] > 0) timeLeft[1]--;

          send_time_info();
          delay(300);
        }
        if (debounce(okButton)) {    // 초(sec)조절로 넘어감.
          delay(300);
          break;
        }
        if (debounce(startButton) == true) {   // start 클릭시 타이머 시작으로
          status = TIMER;
          endSignal = true;
          delay(300);
          break;
        }
        if (debounce(stopButton) == true) {    // stop 클릭시 지금까지 맞춰놓은 것 초기화. 그리고 시작 화면으로.
          timeLeft[0] = 0;
          timeLeft[1] = 0;  
          timeLeft[2] = 0;
          status = prevStatus;
          endSignal = true;
          delay(300);
          break;
        }
      }
      Serial.print("o");
      if (endSignal) break;

      Serial.print("S");
      while (true) {    // 초(sec) 조절 무한루프 (ok 또는 start 또는 stop버튼 눌러야 탈출)
        
        if (debounce(plusButton)) {
          if (timeLeft[2] < 60) timeLeft[2]++;         
          
          send_time_info();
          delay(300);
        }
        if (debounce(minusButton)) {
          if (timeLeft[2] > 0) timeLeft[2]--;

          send_time_info();
          delay(300);
        }
        if (debounce(okButton)) {    // 시(hour)조절로 넘어감.
          delay(300);
          break;
        }
        if (debounce(startButton)) {   // start 클릭시 타이머 시작으로
          status = TIMER;
          endSignal = true;
          delay(300);
          break;
        }
        if (debounce(stopButton)) {  // stop 클릭시 지금까지 맞춰놓은 것 초기화. 그리고 시작 화면으로.
          timeLeft[0] = 0;
          timeLeft[1] = 0;
          timeLeft[2] = 0;
          status = START;
          endSignal = true;
          delay(300);
          break;
        }
      }  //while
      Serial.print("o");
      if (endSignal) break;

    } //while

    //끝날 때까지 보관할 시간 정보.
    time[0] = timeLeft[0];
    time[1] = timeLeft[1];
    time[2] = timeLeft[2];
  
    //prevStatus는 TIMER_CONFIG으로!
    prevStatus = TIMER_CONFIG;
    Serial.print("x");
    delay(100);

  }
  else if (status == TIMER) {

    Serial.print("c");
    set_led(GREEN);
    int color = GREEN;
    
    bool countMotionAlready = false;
    bool countWeightAlready = false;

    while (true) {
      
      if (digitalRead(motionSensorPin) == LOW) {  //모션센서에서 휴대폰에 접근했다고 반응될 때,
        set_led(YELLOW);
        color = YELLOW;
        tone(speakerPin, 200, 10);
        if (countMotionAlready == false) {
          count[0]++;
          countMotionAlready = true;
        }
      }
      else {    //접근이 종료되었거나 접근하지 않았을때
        if (color == YELLOW || color == GREEN) {
          set_led(GREEN);
          color = GREEN;
        }
        countMotionAlready = false;
      }

      if (analogRead(weightSensorPin) < 30) {   // 무게센서에서 휴대폰 꺼낸 거라고 반응될때,
        set_led(RED);
        color = RED;
        tone(speakerPin, 280, 10);
        if (countWeightAlready == false) {
          count[1]++;
          countWeightAlready = true;
        }
      }
      else {    //꺼냄이 종료되었거나 꺼내지 않았을 때,
        if (color == RED || color == GREEN) {
          set_led(GREEN);
          color = GREEN;
        }
        
        countWeightAlready = false;
      }

      

      send_time_info();

      delay(500); //1초 기다림 중 1부. 

      if (digitalRead(motionSensorPin) == 0) {  //모션센서에서 휴대폰에 접근했다고 반응될 때,
        set_led(YELLOW);
        color = YELLOW;
        tone(speakerPin, 200, 10);
        if (countMotionAlready == false) {
          count[0]++;
          countMotionAlready = true;
        }
      }
      else {    //접근이 종료되었거나 접근하지 않았을때
        if (color == YELLOW || color == GREEN) {
          set_led(GREEN);
          color == GREEN;
        }
        
        countMotionAlready = false;
      }

      if (analogRead(weightSensorPin) < 30) {   // 무게센서에서 휴대폰 꺼낸 거라고 반응될때,
        set_led(RED);
        color = RED;
        tone(speakerPin, 280, 10);
        if (countWeightAlready == false) {
          count[1]++;
          countWeightAlready = true;
        }
      }
      else {    //꺼냄이 종료되었거나 꺼내지 않았을 때,
        if (color == RED || color == GREEN) {
          set_led(GREEN);
        }
        
        countWeightAlready = false;
      }

      delay(480); //1초 기다림 중 2부.  (딜레이 고려해서 980ms으로 지정함.)




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

    }
    prevStatus = TIMER;
    Serial.print("x");

  }
  else if (status == STATISTICS) {

    //사용량 통계 출력
      //EEPROM에서 데이터 읽어야 함.
      Serial.print("e");
      set_led(BLUE);

      int i = 0;
      int sumOfClose = 0;
      int sumOfOut = 0;

      while (i < eepromAddress) {
        if (i%10 <= 6) {    //유효한 값이면,

          Serial.print(EEPROM.read(i));   //보낸다. (시,분,초,접근횟수,꺼낸횟수 순서대로 보내짐)

          Serial.print("\n");
          
          if (i / 10 == 5) sumOfClose += EEPROM.read(i);
          else if (i / 10 == 6) {
            sumOfOut += EEPROM.read(i);
            Serial.print("\n");
          }
        }
        i++;
      }
      Serial.print("y");
      Serial.print((char)(sumOfClose / (i / 10 + 1)));    // 1분당 접근횟수의 평균 보낸다.
      Serial.print("z");
      Serial.print((char)(sumOfOut / (i / 10 + 1)));    // 1분당 꺼낸횟수의 평균 보낸다.

      while (true) {
        if (debounce(okButton)) break;
        if (debounce(stopButton)) break;
      }

      status = prevStatus;
      prevStatus = STATISTICS;
      Serial.println("x");

  }
  else if (status == STOPPED) {

    //일시정지시 화면 출력
      Serial.print("f");
      send_time_info();
      set_led(ORANGE);
      delay(500);
      
      while (true) {
        if (debounce(startButton)) {
          status = TIMER;
          break;
        }
        else if (debounce(stopButton)) {
          status = ENDED;
          break;
        }
      }
      prevStatus = STOPPED;

      Serial.print("x");

  }
  else if (status == ENDED) {
    
    //타이머 종료시 화면 출력
      Serial.print("g");
      set_led(PURPLE);                                                                    

      // 접근, 꺼낸 기록 전송
      Serial.print((char)('0' + count[0]));
      Serial.print((char)('0' + count[1]));


      // 시간 정보 전송.
      
      Serial.print((char)('0' + time[0]));
      Serial.print((char)('0' + time[1]));
      Serial.print((char)('0' + time[2]));

      Serial.print('p');
      delay(300);

      // if (correctEnding) {    //정상 종료일 경우(타이머 다 되서 종료)
      //   //EEPROM에 기록. 한 타이머 기록 단위 당 10바이트 쓸 예정
      //   EEPROM.write(eepromAddress, (char)time[0]);
      //   eepromAddress++;
      //   EEPROM.write(eepromAddress, (char)time[1]);
      //   eepromAddress++;
      //   EEPROM.write(eepromAddress, (char)time[2]);
      //   eepromAddress++;
      //   EEPROM.write(eepromAddress, (char)count[0]);
      //   eepromAddress++;
      //   EEPROM.write(eepromAddress, (char)count[1]);
      //   eepromAddress ++;

      //   //1분당 접근횟수 계산
      //   int closePerMin = (int)(((double)count[0] / (time[2] + 60*time[1] + 3600*time[2])) * 60 + 0.5);
      //   EEPROM.write(eepromAddress, (char)closePerMin);

      //   //1분당 꺼낸횟수 계산
      //   int outPerMin = (int)(((double)count[1] / (time[2] + 60*time[1] + 3600*time[2])) * 60 + 0.5);
      //   EEPROM.write(eepromAddress, (char)outPerMin);

      //   correctEnding = false;
      // }
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
      Serial.print("x");
      prevStatus = ENDED;
      

  }

  
} //while
} //loop