#include <Wire.h>
#include <virtuabotixRTC.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH1106.h>

virtuabotixRTC myClock(3, 4, 2);
Adafruit_SH1106 display(21, 22);
int state = 1;

void drawUI() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(2, 2);
  display.println(" Handshakes's  myClock  Sleep");
  display.drawLine(0, 11, 128, 11, WHITE);
  display.drawLine(94, 0, 94, 10, WHITE);
  display.drawLine(26, 44, 102, 44, WHITE);
}

void setup() {
  pinMode(5, INPUT_PULLUP);
  display.begin(SH1106_SWITCHCAPVCC, 0x3C);
  display.setTextColor(WHITE);
  drawUI();
  display.display();
}

void loop() {
  if (digitalRead(5) == LOW) {
    if (state == 0) {
      delay(500);
      drawUI();
      state = 1;
    }else{
      delay(500);
      display.drawRect(4, 7, 120, 50, WHITE);
      display.fillRect(5, 8, 118, 48, BLACK);
      display.setTextSize(1);
      display.setCursor(6, 12);
      display.println("myClock will turn");
      display.println(" off in a moment.");
      display.println("");
      display.println(" To turn back on,");
      display.println(" press the button.");
      display.display();
      delay(3000);
      display.clearDisplay();
      display.display();
      state = 0;
    }
  }

  if (state == 1) {
    myClock.updateTime();
    drawUI();
    display.setTextColor(WHITE);
    display.setTextSize(3);
    display.setCursor(1, 19);
    if (myClock.hours < 10) {
      display.print("0");
    }
    display.print(myClock.hours);
    display.print(":");
    if (myClock.minutes < 10) {
      display.print("0");
    }
    display.print(myClock.minutes);
    display.setTextSize(2);
    display.print(":");
    if (myClock.seconds < 10) {
      display.print("0");
    }
    display.print(myClock.seconds);
    display.setTextSize(1);
    display.setCursor(26, 48);
    if (myClock.dayofweek == 1) {
      display.print("Monday,");
    }
    if (myClock.dayofweek == 2) {
      display.print("Tuesday,");
    }
    if (myClock.dayofweek == 3) {
      display.print("Wednesday,");
    }
    if (myClock.dayofweek == 4) {
      display.print("Thursday,");
    }
    if (myClock.dayofweek == 5) {
      display.print("Friday,");
    }
    if (myClock.dayofweek == 6) {
      display.print("Saturday,");
    }
    if (myClock.dayofweek == 7) {
      display.print("Sunday,");
    }
    display.setCursor(26, 56);
    if (myClock.dayofmonth < 10) {
      display.print("0");
    }
    display.print(myClock.dayofmonth);
    display.print(".");
    if (myClock.month < 10) {
      display.print("0");
    }
    display.print(myClock.month);
    display.print(".");
    display.print(myClock.year);
    display.display();
    delay(999);
  }
}
