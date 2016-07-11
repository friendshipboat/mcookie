#include <IRremote.h>
//#include <IRremoteInt.h>

//#include <SoftwareSerial.h>

int RECV_PIN = 6;
IRrecv irrecv(RECV_PIN);
decode_results results;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  irrecv.enableIRIn();
}

void loop() {
  // put your main code here, to run repeatedly:
 if(irrecv.decode(&results))
 {
    irrecv.resume();
    Serial.println("0");
    if(results.value == 0x1EF807F)
    {
      Mouse.press();
      Serial.println("1");
    }
    else if(results.value == 0x1FE40BF)
    {
      Mouse.press(MOUSE_RIGHT);
      Serial.println("2");
    }
    else
    {
      Serial.println("3");
      Mouse.release();
      Mouse.release(MOUSE_RIGHT);
    }
  }
  delay(1000);
}
