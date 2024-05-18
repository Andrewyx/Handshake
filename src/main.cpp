#include <Arduino.h>
#include <ArduinoJson.h>
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
#define LEFT_SERVO_PIN 22
#define RIGHT_SERVO_PIN 23
#define UPDATE_REMOTE_STATUS_MILLISECONDS 300000

#if defined(ESP32) || defined(ESP8266) || defined(ARDUINO_RASPBERRY_PI_PICO_W)
#include <WiFiClientSecure.h>
WiFiClientSecure ssl_client;
#elif defined(ARDUINO_ARCH_SAMD) || defined(ARDUINO_UNOWIFIR4) || defined(ARDUINO_GIGA) || defined(ARDUINO_PORTENTA_C33) || defined(ARDUINO_NANO_RP2040_CONNECT)
#include <WiFiSSLClient.h>
WiFiSSLClient ssl_client;
#endif

Adafruit_SH1106 display(21, 19);
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
String robotIDPath;

uint32_t milliscounter = 0; 
uint32_t previousMillis = 0;
const int updateActivityInterval = UPDATE_REMOTE_STATUS_MILLISECONDS;

Servo leftServo;
Servo rightServo;
 
String firebaseMessage;
String robotID;
object_t json;
JsonWriter writer;
JsonDocument parsedJson;

void drawUI() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(2, 2);
  display.println("Handshakes's Information");
}

void setupWifi() {
  Serial.println("Press BOOT button to configure Wifi");

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

  leftServo.attach(LEFT_SERVO_PIN);
  rightServo.attach(RIGHT_SERVO_PIN);
  robotIDPath = (String)"/robotIDs/" + robotID;
}

void loop() {

    // The async task handler should run inside the main loop
    // without blocking delay or bypassing with millis code blocks.

    app.loop();

    Database.loop();

    if (app.ready())
    {
      milliscounter = millis();

      if (Database.existed(aClient, robotIDPath)) 
      {

        String resultJson = Database.get<String>(aClient, robotIDPath);
        deserializeJson(parsedJson, resultJson);

        leftMotorNum = static_cast<float>(parsedJson["leftmotor"]);
        rightMotorNum = static_cast<float>(parsedJson["rightmotor"]);
        firebaseMessage = static_cast<const char*>(parsedJson["message"]);

        if (milliscounter - previousMillis >  updateActivityInterval || previousMillis == 0) {
          writer.create(json, "lastactive", getTime());
          Database.update(aClient, robotIDPath, json);
          previousMillis = milliscounter;
        }
      } 
      else 
      {
        Database.set<float>(aClient, robotIDPath + "/leftmotor", MOTOR_STOP_PWM);
        Database.set<float>(aClient, robotIDPath + "/rightmotor", MOTOR_STOP_PWM);
        Database.set<String>(aClient, robotIDPath + "/message", "");
        Database.set<uint32_t>(aClient, robotIDPath + "/lastactive", getTime());
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
    display.println(leftMotorNum);
    display.print("Msg: " + firebaseMessage);
    display.display();
  
}