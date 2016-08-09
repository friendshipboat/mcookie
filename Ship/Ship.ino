#include <Adafruit_NeoPixel.h>
#include <ESP8266.h>
#include <Servo.h>

#define LEDPIN 8  //led引脚
#define CUTNUM 20 //最大忍受的丢包数量
#define SSID "luoyicheng"     //AP名
#define PASSWD "18811796676"  //AP密码
#define HOST_PORT 8080        //约定端口
#define PIN_MOTOR_B1 8   //马达引脚
#define PIN_MOTOR_B2 6
#define PIN_MOTOR_A1 7
#define PIN_MOTOR_A2 5

int lost_cnt = CUTNUM; //丢包计数
uint8_t udp_id = 0; //udp号，默认0
byte trigPin = 12;   //超声波触发引脚
byte echoPin = 13;   //超声波接收引脚
uint8_t pos = 90;   //舵机位置
uint8_t vel = 255;    //马达转速

Adafruit_NeoPixel strip(1, LEDPIN, NEO_RGB+NEO_KHZ800);
ESP8266 wifi(Serial1);
Servo duo;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  delay(1000);
  Serial.println("test");

  //设置led
  strip.begin();
  strip.setPixelColor(0, strip.Color(0, 255, 0));
  strip.show();

  //设置wifi通信
//  while(!wifi.setOprToStationSoftAP());
//  Serial.println("softap ok");
//  Serial.println(wifi.getAPList().c_str());
//  strip.setPixelColor(0, strip.Color(0, 0, 255));
//  strip.show();
//  while(!wifi.joinAP(SSID, PASSWD));
//  Serial.println("Join AP success");
//  strip.setPixelColor(0, strip.Color(0, 0, 0));
//  strip.show();
//  Serial.println(wifi.getStationIp());
//  while(!wifi.enableMUX());
//  Serial.println("multiple ok");
//  Serial.println(wifi.getAPIp());
//  while(!wifi.registerUDP(udp_id, "192.168.137.1", HOST_PORT));
//  Serial.println("regUDP ok");
  
  //设置超声波模块
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  digitalWrite(trigPin, LOW);

  //设置舵机
  duo.attach(SDA);

  //设置马达
  pinMode(PIN_MOTOR_B1, OUTPUT);
  pinMode(PIN_MOTOR_B2, OUTPUT);
  pinMode(PIN_MOTOR_A1, OUTPUT);
  pinMode(PIN_MOTOR_A2, OUTPUT);

  strip.setPixelColor(0, strip.Color(255, 0, 0));
}

#define TIMEOUT_DIST 100000  //测距离的可忍受最大延时
#define TIMEOUT_WIFI 1000  //通信包传输的最大可接受延时
#define BUFLEN 50   //数据包大小

void loop() {
  // put your main code here, to run repeatedly:

  //LED指示控制状态
  if(lost_cnt < CUTNUM)
    strip.setPixelColor(0, strip.Color(255, 0, 0));
  else
    strip.setPixelColor(0, strip.Color(0, 255, 0));
  strip.show();
  Serial.println(lost_cnt);

  //超声波测距
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long dist = pulseIn(echoPin, HIGH, TIMEOUT_DIST);
  Serial.print("dist:");
  Serial.println(dist);

  //将距离发送，写在包的前四个字节
//  static uint8_t buf_send[BUFLEN] = {65};
//  memcpy(buf_send+1, (const char*)&dist, sizeof(long));
//  wifi.send(udp_id, buf_send, BUFLEN);
//
//  //接收控制指令
//  static uint8_t buf_recv[BUFLEN] = {1, 2, 3, 4, 5};
//  if(!wifi.recv(udp_id, buf_recv, BUFLEN, TIMEOUT_WIFI))
//  {
//    lost_cnt++;
//    return;
//  }
//  else
//    lost_cnt = 0;
//  switch(buf_recv[0])
//  {
//    case 'a':
//      if(pos > 0)
//        pos--;
//       break;
//    case 'd':
//      if(pos < 180)
//        pos++;
//      break;
//    case 'w':
//      if(vel < 255)
//        vel = 255;
//      break;
//    //case 'd':
//      if(vel > 0)
//        vel = 0;
//      break;
//  }
  duo.write(pos);
  analogWrite(PIN_MOTOR_B1, vel);
  digitalWrite(PIN_MOTOR_B2, LOW);
  analogWrite(PIN_MOTOR_A1, vel);
  digitalWrite(PIN_MOTOR_A2, LOW);
//  Serial.print((char)buf_recv[0]);
  Serial.println(pos);
}
