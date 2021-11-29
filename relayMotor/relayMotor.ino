#include "Servo.h"
#define RelayPlusPin D1
#define RelayMinusPin D2
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

//openECHO 나중에 바꿔야함!!

int pushTRIG = D7;    // 트리거 핀 선언
int pushECHO = D6;    // 에코 핀 선언
int openTRIG = D4;    // SD3 나중에 바꿔야함
int openECHO = D5;
int servoPin = D8;

//확정
int servoStart = 0, servoEnd = 90, servoDelay = 5000;

//D3,4-> D4에서 input 으로 설정해주면 에러가 나는 것 같다!(input 설정이 안되는 듯)
//D3 도 output trig 했는데 안됨
//빨강이 위, 갈색이 아래

//
int Clock = 1, pushDelayTime = 230, pullDelayTime = 335, count = 0;
int pushPower = 900, pullPower = 1020;
int distanceDelayTime = 100;
int vol = 25;

const char *ssid = "INS_phone";
const char *password = "kkkkkkkk";

IPAddress ip (192, 168, 119, 101); // 연결할, 고정 IP 주소
IPAddress gateway (192, 168, 119, 135); // 게이트웨이 주소
IPAddress subnet (255, 255, 255, 0); // 서브마스크 주소
WiFiServer server(80);
WiFiClient client;
Servo myServo;

SoftwareSerial mySoftwareSerial(3, 1); // RX, TX
DFRobotDFPlayerMini myDFPlayer;

int countDistance(int modeChoice, int runMode);

//모터 동작 멈추기
void stop(){
  digitalWrite(RelayPlusPin, LOW);
  digitalWrite(RelayMinusPin, LOW);  
}

//모터 press
void push(){
  analogWrite(RelayPlusPin,1024);
  digitalWrite(RelayMinusPin,LOW); // 회전
  delay(pushDelayTime);
  stop();
}

//모터 pull
void pull(){
  digitalWrite(RelayPlusPin,LOW); 
  analogWrite(RelayMinusPin,1024);
  delay(pullDelayTime);
  stop();
}

//모터 사이클 동작
void motorCycle(){
  //push 음성안내 음성 출력
  myDFPlayer.play(1);
  delay(2500);
  Serial.println("push");
  push();
      
  delay(2000);
  
  //pull 음성안내 출력
  myDFPlayer.play(2);
  delay(1000);
  Serial.println("pull");
  pull();
}

void servoRun(){
  //문 Open 음성안내 출력
  myDFPlayer.play(4);
  
  myServo.write(servoStart);
  delay(servoDelay);
  myServo.write(servoEnd);
}

int countDistance(int modeChoice, int runMode){
  //modeChoice - mode == 0 -> push 거리, mode == 1 open 거리
  //runMode - runMode == 0 -> 작동 x, runMode == 1 -> 작동 o
  int TRIG = 0, ECHO = 0;
  
  //
  if(modeChoice == 0){ 
    Serial.println("press Distance");
    TRIG = pushTRIG;
    ECHO = pushECHO;
  }
  else{ 
    Serial.println("Open Distance");
    TRIG = openTRIG;
    ECHO = openECHO;
  }
  
  //open 일 경우 0, press 일 경우 1
  long duration, distance;
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  //물체에 반사되어돌아온 초음파의 시간을 변수에 저장합니다.
  duration = pulseIn (ECHO, HIGH); 
  distance = duration * 17 / 1000; 

  //측정된 물체로부터 거리값(cm값)을 보여줍니다.
  Serial.print(distance); 
  Serial.println(" Cm");

  delay(distanceDelayTime);
  count++;


  //프레스동작 - 0, 오픈동작 - 1
  if(distance < 20 && runMode == 1){ 
    if(modeChoice == 0){
      
      motorCycle();
    }
    else{
      stop();
      
      servoRun();
    }
  }
  
  return distance;
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  delay(10);
  pinMode(RelayPlusPin, OUTPUT);
  pinMode(RelayMinusPin, OUTPUT);

  Serial.println("1. push, 2.stop, 3.pull");
  Serial.println("qw : push +-, ty : pull +-");

  stop();
  
  pinMode(pushTRIG, OUTPUT);    // 트리거 핀 출력으로 선언
  pinMode(pushECHO, INPUT);     // 에코 핀 입력으로 선언

  pinMode(openTRIG, OUTPUT);    // 트리거 핀 출력으로 선언
  pinMode(openECHO, INPUT);     // 에코 핀 입력으로 선언

  myServo.attach(servoPin);

  WiFi.disconnect (true);
  delay(1000);

  Serial.print("Connecting...");
  Serial.println(ssid);
  WiFi.config (ip, gateway, subnet);
  WiFi.begin(ssid, password);
  while(1)
  {
    if(WiFi.status()==WL_CONNECTED)break;
    Serial.print(".");
    delay(500);
  }
  Serial.printf("Connected!\r\n");

  server.begin();
  Serial.println("server Start");

  Serial.println(WiFi.localIP());
//
  if (!myDFPlayer.begin(Serial)) {
    Serial.println("Unable to begin:");
    Serial.println("1.Please recheck the connection!");
    Serial.println("2.Please insert the SD card!");
  }
  Serial.println("DFPlayer Mini online.");
   
  myDFPlayer.volume(vol);
}

int whatMode = 1; //mode가 0 이면 push 초음파, 1 이면 open 초음파
//

void loop() {
  //1. 잔량
  //2. 문 열기
  //3. 압축 올리기
  //4. 압축 시작
  //modeChoice - mode == 0 -> push 거리, mode == 1 open 거리
  //runMode - runMode == 0 -> 작동 x, runMode == 1 -> 작동 o

  //press 부분 거리재기, runMode true
  countDistance(0, 1);

//  //open 부분 거리재기. runMode true
  countDistance(1, 1);
  
  client = server.available();
  if(client){
    if(client.connected())
    {
      //Serial.println("connect sucess");
      }
    else
    {
    Serial.println("접속 대기 중...");
    delay(500);
    client.stop();
    }
  }

// Response & Request Configure //
  //char* IP_Address = WiFi.localIP().toString().;
  String str1 = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n \r\n\r\n PRESS ON!";

  Serial.println("readString start : ");
  String REQ = client.readStringUntil('\r');
  Serial.println("readString end : ");
  Serial.println(REQ);

// WiFi Control Section //
  if(REQ.indexOf("/WIFI/1") != -1)
  {
    Serial.println("WiFi_Imform Submit");
    client.printf("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n \r\n\r\n WIFI");
    delay(20);
    client.printf("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n \r\n\r\n IP Adress is %s SSID is %c", WiFi.localIP().toString().c_str(), ssid);
    }
    
// REMAIN Check Section//
  else if(REQ.indexOf("/REMAIN/1") != -1)
  {
    int dis = countDistance(0,0);
    Serial.println(dis);
    client.printf("HTTP/1.1 201 OK\r\nContent-Type: text/html\r\n\r\n \r\n\r\n %d", dis);
    }
    
// Compression Control Section //      
  else if(REQ.indexOf("/PRESS/1") != -1){
    motorCycle(); 
    client.print(str1);
  }

  else if(REQ.indexOf("/PUSH/1") != -1){
    push();
    client.print("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n \r\n\r\n PUSH!");
  }

  else if(REQ.indexOf("/PULL/1") != -1){
    pull();
    client.print("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n \r\n\r\n PULL!");
  }
  

// Push&Pull Power Control Section //      
  else if(REQ.indexOf("/PULLUP/1") != -1){
  pullDelayTime += 50;
  Serial.println(pullDelayTime);
  client.printf("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n \r\n\r\n Press pull power %d", pullDelayTime);
  servoRun();

  
  } //PULL UP
  
  else if(REQ.indexOf("/PULLDOWN/1") != -1)
  {
  pullDelayTime -= 50;
  Serial.println(pullDelayTime);
  client.printf("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n \r\n\r\n Press pull power %d", pullDelayTime);
  }
  
  else if(REQ.indexOf("/PUSHUP/1") != -1){
  pushDelayTime += 50;
  Serial.println(pushDelayTime);
  client.printf("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n \r\n\r\n Press pull power %d", pushDelayTime);
  }
  
  else if(REQ.indexOf("/PUSHDOWN/1") != -1)
  {
  pushDelayTime += 50;
  Serial.println(pushDelayTime);
  client.printf("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n \r\n\r\n Press pull power %d", pushDelayTime);
  }

// Volume Control Section //
  else if(REQ.indexOf("/VOLU/1") != -1)
  {
  vol += 5;
  if(vol > 30) vol = 30;
  
  Serial.println(vol);
  client.printf("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n \r\n\r\n Volume Up %d", vol);
  }
  
  else if(REQ.indexOf("/VOLD/1") != -1)
  {
  vol -= 5;
  if(vol <0) vol = 0;
  Serial.println(vol);
  client.printf("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n \r\n\r\n Volume Down %d", vol);
  } 

  //버튼 누르면 오픈하기
  else if(REQ.indexOf("/OPEN/1") != -1)
  {
  servoRun();
  Serial.println("open");
  client.printf("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n \r\n\r\n open");
  } 

  // when Request Fail //      
  client.flush();

  if(Serial.available()){
      String inString = Serial.readStringUntil('\n');
      Serial.println(inString);
      if(inString == "1"){
        push();
        Serial.println("push");
      }
      else if(inString == "w"){
        pull();
        Serial.println("pull");
      }
  }
}
