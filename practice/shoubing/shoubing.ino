#include <Keyboard.h>
#include <Mouse.h>
#include <IRremote.h>

IRrecv irrecv(6);
decode_results results;

int uiStep()         //切歌
{
  if (analogRead(A0) < 100)  //Y-up,play
  {
    delay(50);       //延迟50毫秒输出
    if (analogRead(A0) < 100)      //
      return 2;      //回到动作1
  }
  if (analogRead(A0) > 900)
  {
    delay(50);
    if (analogRead(A0) > 900)
      return 1;
  }

  if (analogRead(A1) < 100)    //
  {
    delay(50);       //延迟50毫秒输出
    if (analogRead(A1) < 100)  //X-Right 下一曲
      return 3;      //回到动作2
  }
  if (analogRead(A1) > 900)    //X-Left 上一曲
  {
    delay(50);       //延迟50毫秒输出
    if (analogRead(A1) > 900)  //
      return 4;      //回到动作3
  }
  return 0;
}

void setup() {
  // put your setup code here, to run once:
  Keyboard.begin();
  Mouse.begin();
  Serial.begin(9600);
  irrecv.enableIRIn();
}

void loop() {
  // put your main code here, to run repeatedly:
  int vol = uiStep();
//  Serial.println(analogRead(A0));
//  Serial.println(analogRead(A1));
//  Serial.println();
//  delay(1000);
  
  if(!vol)
  {
    Keyboard.release(KEY_RIGHT_ARROW);
    Keyboard.release(KEY_LEFT_ARROW);
    Keyboard.release(KEY_UP_ARROW);
    Keyboard.release(KEY_DOWN_ARROW);
  }
  else if(vol == 1)
  {
    Keyboard.press(KEY_UP_ARROW);
  }
  else if(vol == 2)
  {
    Keyboard.press(KEY_DOWN_ARROW);
  }
  else if(vol == 3)
  {
    Keyboard.press(KEY_LEFT_ARROW);
  }
  else if(vol == 4)
  {
    Keyboard.press(KEY_RIGHT_ARROW);
  }

  if(irrecv.decode(&results))
  {
    irrecv.resume();
    Serial.println(results.value, HEX);
    if(results.value == 0x1FE807F)
    {
      Mouse.press(MOUSE_LEFT);
      Serial.println("A");
    }
    else if(results.value == 0x1FE40BF)
    {
      Mouse.press(MOUSE_RIGHT);
      Serial.println("B");
    }
    else
    {
      Serial.println("c");
      Mouse.release(MOUSE_LEFT);
      Mouse.release(MOUSE_RIGHT);
    }
  }
  
  delay(100);
}
