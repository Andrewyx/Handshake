#include <Wire.h>
#include <virtuabotixRTC.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH1106.h>

virtuabotixRTC clock(3, 4, 2);
Adafruit_SH1106 display(-1);
int state = 1;

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
      display.println("Clock will turn");
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
    clock.updateTime();
    drawUI();
    display.setTextColor(WHITE);
    display.setTextSize(3);
    display.setCursor(1, 19);
    if (clock.hours < 10) {
      display.print("0");
    }
    display.print(clock.hours);
    display.print(":");
    if (clock.minutes < 10) {
      display.print("0");
    }
    display.print(clock.minutes);
    display.setTextSize(2);
    display.print(":");
    if (clock.seconds < 10) {
      display.print("0");
    }
    display.print(clock.seconds);
    display.setTextSize(1);
    display.setCursor(26, 48);
    if (clock.dayofweek == 1) {
      display.print("Monday,");
    }
    if (clock.dayofweek == 2) {
      display.print("Tuesday,");
    }
    if (clock.dayofweek == 3) {
      display.print("Wednesday,");
    }
    if (clock.dayofweek == 4) {
      display.print("Thursday,");
    }
    if (clock.dayofweek == 5) {
      display.print("Friday,");
    }
    if (clock.dayofweek == 6) {
      display.print("Saturday,");
    }
    if (clock.dayofweek == 7) {
      display.print("Sunday,");
    }
    display.setCursor(26, 56);
    if (clock.dayofmonth < 10) {
      display.print("0");
    }
    display.print(clock.dayofmonth);
    display.print(".");
    if (clock.month < 10) {
      display.print("0");
    }
    display.print(clock.month);
    display.print(".");
    display.print(clock.year);
    display.display();
    delay(999);
  }
}

void drawUI() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(2, 2);
  display.println(" Adam's  Clock  Sleep");
  display.drawLine(0, 11, 128, 11, WHITE);
  display.drawLine(94, 0, 94, 10, WHITE);
  display.drawLine(26, 44, 102, 44, WHITE);
}