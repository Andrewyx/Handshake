#include <Arduino.h>
#include <Wire.h>
#include <virtuabotixRTC.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH1106.h>

#if defined(ESP32)
  #include <WiFi.h>
#elif defined(ESP8266)
  #include <ESP8266WiFi.h>
#endif
#include <Firebase_ESP_Client.h>

//Provide the token generation process info.
#include "addons/TokenHelper.h"
//Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

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

//Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
int count = 0;
bool signupOK = false;
float leftMotorNum;
float rightMotorNum;


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
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  /* Assign the api key (required) */
  config.api_key = API_KEY;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  /* Sign up */
  if (Firebase.signUp(&config, &auth, "", "")){
    Serial.println("ok");
    signupOK = true;
  }
  else{
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h
  
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  pinMode(5, INPUT_PULLUP);
  display.begin(SH1106_SWITCHCAPVCC, 0x3C);
  display.setTextColor(WHITE);
  drawUI();
  display.display();
}

void loop() {


    if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 15000 || sendDataPrevMillis == 0)){
    sendDataPrevMillis = millis();
    leftMotorNum = random(0,360);
    rightMotorNum = random(0,360);
    
    myClock.updateTime();
    drawUI();
    display.setTextColor(WHITE);
    display.setTextSize(1);
    display.setCursor(1, 19);
    display.print("Right: ");
    display.print(rightMotorNum);
    display.print("Left: ");
    display.print(leftMotorNum);
    display.display();
    // Write an Float number on the database path robots/leftmotor
    if (Firebase.RTDB.setInt(&fbdo, "robots/leftmotor", leftMotorNum)){
      Serial.println("PASSED");
      Serial.print("PATH: ");
      Serial.println(fbdo.dataPath());
      Serial.print("TYPE: ");
      Serial.println(fbdo.dataType());
    }
    else {
      Serial.println("FAILED");
      Serial.print("REASON: ");
      Serial.println(fbdo.errorReason());
    }
    
    // Write an Float number on the database path robots/rightmotor
    if (Firebase.RTDB.setFloat(&fbdo, "robots/rightmotor", rightMotorNum)){
      Serial.println("PASSED");
      Serial.print("PATH: ");
      Serial.println(fbdo.dataPath());
      Serial.print("TYPE: "); 
      Serial.println(fbdo.dataType());
    }
    else {
      Serial.println("FAILED");
      Serial.println("REASON: ");
      Serial.println(fbdo.errorReason());
    }
  }
}
