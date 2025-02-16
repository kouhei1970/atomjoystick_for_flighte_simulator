/*
* MIT License
* 
* Copyright (c) 2025 Kouhei Ito
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/

#include "Arduino.h"
#include "USB.h"
#include "USBHIDGamepad.h"

#include <M5AtomS3.h>
#include <atoms3joy.h>

USBHIDGamepad Gamepad;

volatile uint8_t Loop_flag = 0;

//周期カウンタ割り込み関数
hw_timer_t * timer = NULL;
void IRAM_ATTR onTimer() 
{
  Loop_flag = 1;
}


void setup() {
  //USB-HID Gamepdad初期化
  //pinMode(0, INPUT_PULLUP); 
  Gamepad.begin();
  USB.begin();

  //M5AtomS3初期化
  M5.begin();
  Wire1.begin(38, 39, 400*1000);
  //USBSerial.begin(115200);
  M5.update();
  M5.Lcd.setRotation( 2 );
  M5.Lcd.setTextFont(2);
  M5.Lcd.setTextColor(WHITE, BLUE);
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.fillScreen(BLUE);
  M5.Lcd.drawCentreString("StampFly Sim", 64, 0, 2);
  M5.Lcd.drawCentreString("Joystick", 64, 16, 2);

  //ジョイスティック更新
  joy_update();

  //割り込み設定
  timer = timerBegin(1, 80, true);
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, 10000, true);
  timerAlarmEnable(timer);
  delay(100);

}

void loop() {
  uint8_t throttle;// = getThrottle();
  uint8_t phi;// = getAileron();
  uint8_t theta;// = getElevator();
  uint8_t psi;// = getRudder();
  //static uint8_t cnt = 0;

  while(Loop_flag==0);//周期割り込み待ち
  Loop_flag = 0;//周期セマフォクリア

  M5.update();//M5AtomS3更新
  joy_update();//ジョイスティック更新
  throttle = (uint8_t)((float)getThrottle()/4095.0*255.0);
  phi = (uint8_t)((float)getAileron()/4095.0*255.0) ;
  theta = (uint8_t)((float)getElevator()/4095.0*255.0);
  psi = (uint8_t)((float)getRudder()/4095.0*255.0);
  Gamepad.leftStick(phi, theta);      // X Axis, Y Axis
  Gamepad.rightStick(psi, throttle);  // Z Axis, Z Rotation
  //Gamepad.leftTrigger(11);
  //Gamepad.rightTrigger(22);
  //Gamepad.hat(3);
  //Gamepad.pressButton(4); 
  //M5.Lcd.setCursor(0, 0);
  //M5.Lcd.printf("cnt:%03d",cnt);
  //cnt++;

}
