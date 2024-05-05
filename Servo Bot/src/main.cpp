#include <Arduino.h>
#include <ESP32Servo.h>

Servo myservo1;
Servo myservo2; 

void setup() {
  myservo1.attach(32);
  myservo1.write(0);

  myservo2.attach(18);
  myservo2.write(0);

}

void loop() {
  myservo1.write(0);
  delay(500);
  myservo1.write(360);
  delay(500);

  myservo2.write(0);
  delay(500);
  myservo2.write(360);
  delay(500);
}
