#include <Adafruit_NeoPixel.h>
#include <ESP8266.h>
#include <Servo.h>
//#include <MPU6050.h>

#define SSID "luoyicheng"     //AP名
#define PASSWD "18811796676"  //AP密码
#define HOST_PORT 8080    //约定端口
#define CUTNUM 20         //最大忍受的丢包数量
#define PIN_LED 10        //led引脚
#define PIN_MOTOR_B1 8    //马达引脚
#define PIN_MOTOR_B2 6
#define PIN_MOTOR_A1 7
#define PIN_MOTOR_A2 5
#define PIN_RADAR_TRIG 12 //雷达超声波触发引脚
#define PIN_RADAR_ECHO 13 //雷达超声波接收引脚
#define PIN_SONAR_TRIG 2  //水下超声波触发
#define PIN_SONAR_ECHO 3  //水深超声波接收
#define PIN_SERVO SDA     //方向舵机IIC,SDA=18
#define PIN_RADAR 19      //雷达舵机IIC
#define PIN_PIR A2        //人体传感器

const uint8_t ID_UDP = 0;   //udp通道编号,默认0
Adafruit_NeoPixel strip(1, PIN_LED, NEO_RGB+NEO_KHZ800);  //LED指示灯
ESP8266 wifi(Serial1);
Servo duo;
Servo rada;
//MPU6050 motion;

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  delay(1000);
  Serial.println("test");

  //设置led
  strip.begin();
  strip.setPixelColor(0, strip.Color(0, 255, 0));
  strip.show();

  //设置wifi通信
  while(!wifi.setOprToStationSoftAP());
  Serial.println("softap ok");
  Serial.println(wifi.getAPList().c_str());
  strip.setPixelColor(0, strip.Color(0, 0, 255));
  strip.show();
  while(!wifi.joinAP(SSID, PASSWD));
  Serial.println("Join AP success");
  strip.setPixelColor(0, strip.Color(0, 0, 0));
  strip.show();
  Serial.println(wifi.getStationIp());
  while(!wifi.enableMUX());
  Serial.println("multiple ok");
  Serial.println(wifi.getAPIp());
  while(!wifi.registerUDP(ID_UDP, "192.168.137.1", HOST_PORT));
  Serial.println("regUDP ok");
  
  //设置超声波模块
  pinMode(PIN_RADAR_TRIG, OUTPUT);
  pinMode(PIN_RADAR_ECHO, INPUT);
  digitalWrite(PIN_RADAR_TRIG, LOW);
  pinMode(PIN_SONAR_TRIG, OUTPUT);
  pinMode(PIN_SONAR_ECHO, INPUT);
  digitalWrite(PIN_SONAR_TRIG, LOW);

  //设置舵机
  duo.attach(PIN_SERVO);
  rada.attach(PIN_RADAR);

  //设置马达
  pinMode(PIN_MOTOR_B1, OUTPUT);
  pinMode(PIN_MOTOR_B2, OUTPUT);
  pinMode(PIN_MOTOR_A1, OUTPUT);
  pinMode(PIN_MOTOR_A2, OUTPUT);

  //设置生命传感器
  pinMode(PIN_PIR, INPUT);

  //设置陀螺仪
  //motion.initialize();

  strip.setPixelColor(0, strip.Color(255, 0, 0));
}

#define TIMEOUT_DIST 10000   //测距离的可忍受最大延时
#define TIMEOUT_WIFI 500    //通信包传输的最大可接受延时
#define BUFLEN 50           //数据包大小

int lost_cnt = CUTNUM; //丢包计数
uint8_t pos = 90;   //舵机位置
uint8_t rdd = 0;    //雷达所指方向
char rdv = -3;      //雷达转动速度
uint8_t vel = 0;    //马达转速
//int16_t v[6];       //陀螺仪数据 

void loop()
{
  // put your main code here, to run repeatedly:

  //LED指示控制状态
  if(lost_cnt < CUTNUM)
    strip.setPixelColor(0, strip.Color(255, 0, 0));
  else
    strip.setPixelColor(0, strip.Color(0, 255, 0));
  strip.show();
  Serial.println(lost_cnt);

  //超声波测距
  digitalWrite(PIN_RADAR_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_RADAR_TRIG, LOW);
  long dist = pulseIn(PIN_RADAR_ECHO, HIGH, TIMEOUT_DIST);
  digitalWrite(PIN_SONAR_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_SONAR_TRIG, LOW);
  long deep = pulseIn(PIN_SONAR_ECHO, HIGH, TIMEOUT_DIST);
  Serial.print("dist:");
  Serial.print(dist);
  Serial.print("\tdeep:");
  Serial.println(deep);

  //测速
  //motion.getMotion6(v, v+1, v+2, v+3, v+4, v+5);

  //发送数据，格式0A [0~3为超声波测得距离] [4~7为水深] 8舵方向 9雷达方向 10电机速度 11生命活动 //[12~23为陀螺仪数据]
  static uint8_t buf_send[BUFLEN] = {65};
  memcpy(buf_send, &dist, sizeof(long));
  memcpy(buf_send+4, &deep, sizeof(long));
  buf_send[8] = pos;
  buf_send[9] = rdd;
  buf_send[10] = vel;
  buf_send[11] = digitalRead(PIN_PIR);
  //memcpy(buf_send + 12, v, sizeof v);
for(int i=0;i<24;i++)Serial.print((int)buf_send[i]),Serial.print(" ");Serial.println("");
  wifi.send(ID_UDP, buf_send, BUFLEN);

  //更新雷达机
  if(rdd >= 180 || !rdd)
    rdv = -rdv;
  rdd += rdv;
  rada.write(rdd);
  Serial.print("rdd: ");
  Serial.println(rdd);

  //接收控制指令
  static uint8_t buf_recv[BUFLEN] = {1, 2, 3, 4, 5};
  if(!wifi.recv(ID_UDP, buf_recv, BUFLEN, TIMEOUT_WIFI))
  {
    lost_cnt++;
    return;
  }
  else
    lost_cnt = 0;
  switch((char)buf_recv[0])
  {
    case 'a':
      if(pos > 0)
        pos-=2;
       break;
    case 'd':
      if(pos < 180)
        pos+=2;
      break;
    case 'w':
      if(vel < 255)
        vel = 255;
      break;
    case 's':
      if(vel > 0)
        vel = 0;
      break;
  }
  duo.write(pos);
  analogWrite(PIN_MOTOR_B1, vel);
  digitalWrite(PIN_MOTOR_B2, LOW);
  analogWrite(PIN_MOTOR_A1, vel);
  digitalWrite(PIN_MOTOR_A2, LOW);
  Serial.print((char)buf_recv[0]);
  Serial.println(vel);
}
