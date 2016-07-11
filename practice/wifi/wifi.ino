#include "U8glib.h"
#include "ESP8266.h"
#include "I2Cdev.h"
#include <Wire.h>
#include <Rtc_Pcf8563.h>
//#include <AM2321.h>
#include "rtc.h"


#define SSID        "ldd"
#define PASSWORD    "18811796676"
#define HOST_NAME   "pool.ntp.org"
#define HOST_PORT   (123)

#define INTERVAL_NET             20000 
#define INTERVAL_LCD             200             //定义OLED刷新时间间隔  
#define INTERVAL_SENSOR          1000  

ESP8266 wifi(Serial1);

String wifiSetInfo="wifi ready...";

unsigned long net_time = millis();  
unsigned long lcd_time = millis();                 //OLED刷新时间计时器  

uint8_t buffer[128] = {0};
static uint8_t upd_id = 0;
uint32_t len=0;


U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);     //设置OLED型号  

//-------字体设置，大、中、小
#define setFont_L u8g.setFont(u8g_font_7x13)
#define setFont_M u8g.setFont(u8g_font_fixed_v0r)
#define setFont_S u8g.setFont(u8g_font_fixed_v0r)

#define setFont_SS u8g.setFont(u8g_font_fub25n)

void setup(void)
{
  Serial.begin(9600);

  //while (!Serial); // wait for Leonardo enumeration, others continue immediately

  Serial.print("FW Version:");
  Serial.println(wifi.getVersion().c_str());

  if (wifi.setOprToStationSoftAP()) {
    Serial.print("to station + softap ok\r\n");
    wifiSetInfo="softap ok";
  } else {
    Serial.print("to station + softap err\r\n");
    wifiSetInfo="softap err";
  }

  if (wifi.joinAP(SSID, PASSWORD)) {
    Serial.print("Join AP success\r\n");
    Serial.print("IP: ");
    Serial.println(wifi.getLocalIP().c_str());
    wifiSetInfo="Join AP success";
  } else {
    Serial.print("Join AP failure\r\n");
    wifiSetInfo="Join AP failure";
  }

  if (wifi.enableMUX()) {
    Serial.print("multiple ok\r\n");
  } else {
    Serial.print("multiple err\r\n");
  }

  Serial.print("setup end\r\n");

  wifiSetInfo="Getting Time...";
  updateTimeData();
}

void loop(void)
{

  if (lcd_time > millis()) lcd_time = millis();
  if (millis() - lcd_time > INTERVAL_LCD) {
    ret = getRtcTimeString();

    //getRTC();
    volcd();                       //调用显示库

    //serialShowDateTime();
    lcd_time = millis();
  }

}





void updateTimeData() {
  do {
      delay(200);
      registerUDPAndSendRecvData();
      if(len>0) {
        getTimeStampAndSetRTC();
        unregisterUDP();
      } else {
        unregisterUDP();
      }
  } while(!len);
}



void registerUDPAndSendRecvData() {
  if (wifi.registerUDP(upd_id, HOST_NAME, HOST_PORT)) {
    Serial.print("register udp ");
    Serial.print(upd_id);
    Serial.println(" ok");
  } else {
    Serial.print("register udp ");
    Serial.print(upd_id);
    Serial.println(" err");
  }

  static const char PROGMEM
  timeReqA[] = { 227,  0,  6, 236 }, timeReqB[] = {  49, 78, 49,  52 };
  // Assemble and issue request packet
  uint8_t       buf[48];
  memset(buf, 0, sizeof(buf));
  memcpy_P( buf    , timeReqA, sizeof(timeReqA));
  memcpy_P(&buf[12], timeReqB, sizeof(timeReqB));

  wifi.send(upd_id, (const uint8_t*)buf, 48);
  //uint32_t len = wifi.recv(upd_id, buffer, sizeof(buffer), 10000);
  len = wifi.recv(upd_id, buffer, sizeof(buffer), 10000);

  for(int i=0; i<48; i++)
    Serial.println((int)buf[i]);
  for(int i=0; i<128; i++)
    Serial.println(buffer[i], HEX);
}

void getTimeStampAndSetRTC() {
      Serial.print("Received:[");

    unsigned long t = (((unsigned long)buffer[40] << 24) |
                       ((unsigned long)buffer[41] << 16) |
                       ((unsigned long)buffer[42] <<  8) |
                       (unsigned long)buffer[43]) - 2208988800UL;

    Serial.print("Unix timestamp:");
    Serial.print(t);
    Serial.print("]\r\n");

    getDateStamp(t);
    setRTC();

}


void unregisterUDP() {
    if (wifi.unregisterUDP(upd_id)) {
      Serial.print("unregister udp ");
      Serial.print(upd_id);
      Serial.println(" ok");
    } else {
      Serial.print("unregister udp ");
      Serial.print(upd_id);
      Serial.println(" err");
    }
}

//显示函数 
void volcd() {
  //pkj-=4;
  u8g.firstPage();
  do {
      setFont_L;
      u8g.setPrintPos(4, 16);
      u8g.print(rtc.formatDate(RTCC_DATE_US));
      u8g.print("    ");
      switch (rtc.getWeekday()) {
        case 1:
          u8g.print("Mon");
          break;
        case 2:
          u8g.print("Tue");
          break;
        case 3:
          u8g.print("Wed");
          break;
        case 4:
          u8g.print("Thu");
          break;
        case 5:
          u8g.print("Fri");
          break;
        case 6:
          u8g.print("Sat");
          break;
        //case 7:
        case 0:
          u8g.print("Sun");
          break;
      }

      setFont_SS;
      //u8g.setPrintPos(18, 49);
      u8g.setPrintPos(18, 60);

      //u8g.print(rtc.getHour());

      if (rtc.getHour() < 10)
      {
        u8g.print("0");
      } 

      u8g.print(rtc.getHour());

      //u8g.setPrintPos(55, 46);
      u8g.setPrintPos(55, 55);
      if (rtc.getSecond() % 2 == 0)
        u8g.print(":");
      else
        u8g.print(" ");
      //u8g.setPrintPos(68, 48);
      u8g.setPrintPos(68, 60);
      if (rtc.getMinute() < 10)
      {
        u8g.print("0");
        u8g.print(rtc.getMinute());
      }
      else
        u8g.print(rtc.getMinute());
  }
  while( u8g.nextPage() );
} 
