#include <Arduino.h>
#include <Wire.h>
#include <virtuabotixRTC.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH1106.h>
#include <FirebaseClient.h>
#include <ESP32Servo.h>

#if defined(ESP32)
  #include <WiFi.h>
#elif defined(ESP8266)
  #include <ESP8266WiFi.h>
#endif

// Insert your network credentials
#define WIFI_SSID "TELUS3280"
#define WIFI_PASSWORD "m3BcxPDvF4F2"

// Insert Firebase project API Key
#define API_KEY "AIzaSyDqATxGy0ATTYjuo-K6fOB9AUAG1AWzspU"

// Insert RTDB URLefine the RTDB URL */
#define DATABASE_URL "https://handshake-664b7-default-rtdb.firebaseio.com/" 

virtuabotixRTC myClock(3, 4, 2);
Adafruit_SH1106 display(21, 22);
int state = 1;
DefaultNetwork network;
NoAuth no_auth;
FirebaseApp app;

void asyncCB(AsyncResult &aResult);

void printResult(AsyncResult &aResult);

#if defined(ESP32) || defined(ESP8266) || defined(ARDUINO_RASPBERRY_PI_PICO_W)
#include <WiFiClientSecure.h>
WiFiClientSecure ssl_client;
#elif defined(ARDUINO_ARCH_SAMD) || defined(ARDUINO_UNOWIFIR4) || defined(ARDUINO_GIGA) || defined(ARDUINO_PORTENTA_C33) || defined(ARDUINO_NANO_RP2040_CONNECT)
#include <WiFiSSLClient.h>
WiFiSSLClient ssl_client;
#endif

using AsyncClient = AsyncClientClass;

AsyncClient aClient(ssl_client, getNetwork(network));

RealtimeDatabase Database;

bool taskComplete = false;
float leftMotorNum = 5;
float rightMotorNum = 5;

Servo leftServo;
Servo rightServo;

void drawUI() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(2, 2);
  display.println(" Handshakes's  Clock  Sleep");
  display.drawLine(0, 11, 128, 11, WHITE);
  display.drawLine(94, 0, 94, 10, WHITE);
  display.drawLine(26, 44, 102, 44, WHITE);
}

void setup(){
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.print("Connecting to Wi-Fi");
  unsigned long ms = millis();
  while (WiFi.status() != WL_CONNECTED)
  {
      Serial.print(".");
      delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

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

  leftServo.attach(33);
  rightServo.attach(32);
}

void loop() {

    // The async task handler should run inside the main loop
    // without blocking delay or bypassing with millis code blocks.

    app.loop();

    Database.loop();

    if (app.ready() && !taskComplete)
    {
      leftMotorNum = static_cast<float>(Database.get(aClient, "/robots/leftmotor"));
      rightMotorNum = static_cast<float>(Database.get(aClient, "/robots/rightmotor"));
      leftServo.write(leftMotorNum);
      rightServo.write(rightMotorNum);
    }

    myClock.updateTime();
    drawUI();
    display.setTextColor(WHITE);
    display.setTextSize(1);
    display.setCursor(1, 19);
    display.print("Right: ");
    display.println(rightMotorNum);
    display.print("Left: ");
    display.print(leftMotorNum);
    display.display();
  
}