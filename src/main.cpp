#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH1106.h>
#include <FirebaseClient.h>
#include <ESP32Servo.h>
#include "wifiTools.h"
#include "time.h"

#if defined(ESP32)
  #include <WiFi.h>
#elif defined(ESP8266)
  #include <ESP8266WiFi.h>
#endif

// Insert Firebase project API Key
#define API_KEY "AIzaSyDqATxGy0ATTYjuo-K6fOB9AUAG1AWzspU"
// Insert RTDB URLefine the RTDB URL */
#define DATABASE_URL "https://handshake-664b7-default-rtdb.firebaseio.com/" 

#define MOTOR_STOP_PWM 95

#if defined(ESP32) || defined(ESP8266) || defined(ARDUINO_RASPBERRY_PI_PICO_W)
#include <WiFiClientSecure.h>
WiFiClientSecure ssl_client;
#elif defined(ARDUINO_ARCH_SAMD) || defined(ARDUINO_UNOWIFIR4) || defined(ARDUINO_GIGA) || defined(ARDUINO_PORTENTA_C33) || defined(ARDUINO_NANO_RP2040_CONNECT)
#include <WiFiSSLClient.h>
WiFiSSLClient ssl_client;
#endif

Adafruit_SH1106 display(21, 22);
int state = 1;
DefaultNetwork network;
NoAuth no_auth;
FirebaseApp app;

void asyncCB(AsyncResult &aResult);
void printResult(AsyncResult &aResult);

AsyncClientClass aClient(ssl_client, getNetwork(network));
RealtimeDatabase Database;

bool taskComplete = false;
float leftMotorNum = MOTOR_STOP_PWM;
float rightMotorNum = MOTOR_STOP_PWM;

const char* ntpServer = "pool.ntp.org";

uint32_t milliscounter= 0; 
uint32_t previousMillis = 0;
const int updateActivityInterval = 300000;

Servo leftServo;
Servo rightServo;
 
String robotID;
object_t json;
JsonWriter writer;

void drawUI() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(2, 2);
  display.println("Handshakes's Motors");
}

void setupWifi() {
  Serial.print("Jump pins ");
  Serial.print(WIFI_SET_PIN);
  Serial.print(" and ");
  Serial.print(WIFI_SET_PIN_GROUND);
  Serial.println("to set up Wifi Network");
  if(wifi_set_main())
  {
    Serial.println("Connect WIFI SUCCESS");
  }
  else
  {
    Serial.println("Connect WIFI FAULT");
  }
}

unsigned long getTime() {
  time_t now;
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    //Serial.println("Failed to obtain time");
    return(0);
  }
  time(&now);
  return now;
}

void setup(){
  Serial.begin(115200);
  robotID = WiFi.macAddress().substring(0, 5);
  Serial.print("Robot MAC Address: ");
  Serial.println(robotID);

  setupWifi();

  Firebase.printf("Firebase Client v%s\n", FIREBASE_CLIENT_VERSION);
  
  Serial.println("Initializing app...");

  #if defined(ESP32) || defined(ESP8266) || defined(PICO_RP2040)
      ssl_client.setInsecure();
  #if defined(ESP8266)
      ssl_client.setBufferSizes(4096, 1024);
  #endif
  #endif

  initializeApp(aClient, app, getAuth(no_auth));

  app.getApp<RealtimeDatabase>(Database);

  Database.url(DATABASE_URL);

  Serial.println("App ready");
  pinMode(5, INPUT_PULLUP);
  display.begin(SH1106_SWITCHCAPVCC, 0x3C);
  display.setTextColor(WHITE);
  drawUI();
  display.display();

  configTime(0, 0, ntpServer);

  leftServo.attach(33);
  rightServo.attach(32);
}

void loop() {

    // The async task handler should run inside the main loop
    // without blocking delay or bypassing with millis code blocks.

    app.loop();

    Database.loop();

    if (app.ready())
    {
      milliscounter = millis();
      
      if (Database.existed(aClient, "/robotIDs/"+robotID)) 
      {
        leftMotorNum = Database.get<float>(aClient, "/robotIDs/"+robotID+"/leftmotor");
        rightMotorNum = Database.get<float>(aClient, "/robotIDs/"+robotID+"/rightmotor");
        if (milliscounter - previousMillis >  updateActivityInterval || previousMillis == 0) {
          writer.create(json, "lastactive", getTime());
          Database.update(aClient, "/robotIDs/"+robotID, json);
          previousMillis = milliscounter;
        }
      } 
      else 
      {
        Database.set<float>(aClient, "/robotIDs/"+robotID+"/leftmotor", MOTOR_STOP_PWM);
        Database.set<float>(aClient, "/robotIDs/"+robotID+"/rightmotor", MOTOR_STOP_PWM);
        Database.set<uint32_t>(aClient, "/robotIDs/"+robotID+"/lastactive", getTime());
      }

      leftServo.write(leftMotorNum);
      rightServo.write(rightMotorNum);
    }
    drawUI();
    display.setTextColor(WHITE);
    display.setTextSize(1);
    display.setCursor(1, 19);
    display.print("RobotID: ");
    display.println(robotID);
    display.print("Right: ");
    display.println(rightMotorNum);
    display.print("Left: ");
    display.print(leftMotorNum);
    display.display();
  
}