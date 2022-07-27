

//#include <Adafruit_LEDBackpack.h>

//#include <Arduino.h>
#include "WiFi.h"
//#include <WiFi.h>
//#include <WiFiMulti.h>
#include <Adafruit_GFX.h>
#include <Adafruit_LEDBackpack.h>
//#include "Adafruit_LEDBackpack.h"
#include <HTTPClient.h>

#define Serial Serial
#define LED2 2

unsigned long previousTime = 0;
const int buttonOne = 16; 
int buttonState = 0; 
int setTime = 0;
//unsigned long previousTime = 0;
int start = 0;
String queue;

unsigned long lastTime = 0;
unsigned long timerDelay = 600000;

//WiFiMulti wifiMulti;
Adafruit_8x16minimatrix matrix;

TaskHandle_t Task1;
TaskHandle_t Task2;

#define WIFI_STA_NAME "TP-golf"
#define WIFI_STA_PASS "0841931809"


//Task1code: blinks an LED every 1000 ms
void Task1code( void * pvParameters ){
  Serial.print("Task1 running on core ");
  Serial.println(xPortGetCoreID());

  for(;;){

    for (int8_t x = 16; x >= -55; x--) {
              matrix.clear();
              matrix.setCursor(x, 0);
              matrix.print(queue);
              matrix.writeDisplay();
              delay(100);

}
  
  } 
}

void setup() {

    Serial.begin(115200);
    pinMode(buttonOne, INPUT_PULLUP);
    pinMode(LED_BUILTIN, OUTPUT);
    Serial.println();

    
  matrix.clear();
  matrix.begin(0x70);
  matrix.setRotation(1);
  matrix.setTextSize(1);
  matrix.setTextColor(LED_ON);
  matrix.setTextWrap(false);
  
    
    Serial.print("Connecting to ");
    Serial.println(WIFI_STA_NAME);
    
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_STA_NAME, WIFI_STA_PASS);
    
for (int8_t x = 16; x >= -25; x--) {
              matrix.clear();
              matrix.setCursor(x, 0);
              matrix.print("...");
              matrix.writeDisplay();
              delay(100);
}

    while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  }


              matrix.clear();
              matrix.setCursor(3, 0);
              matrix.print("OK");
              matrix.writeDisplay();
              delay(1000); 
              matrix.clear();


  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
    
xTaskCreatePinnedToCore(
                    Task1code,   /* Task function. */
                    "Task1",     /* name of task. */
                    10000,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    1,           /* priority of the task */
                    &Task1,      /* Task handle to keep track of created task */
                    0); 
delay(5000); 



}

void loop() {

    if(WiFi.status()== WL_CONNECTED){
      WiFiClient client;
      HTTPClient http;

        Serial.print("[HTTP] begin...\n");
        String url = "https://admob-api.herokuapp.com/get";
        http.begin(url.c_str());
        int httpCode = http.GET(); 
    Serial.print("HTTP Status Code: ");
    Serial.println(httpCode);

String payload;

if (httpCode>0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpCode);
    payload = http.getString();
    Serial.println("..Payload : .. "+payload);
queue = payload;

if(queue == ""){
  queue = "-";
 }
}
  else {
    Serial.print("Error code: ");
    Serial.println(httpCode);
  }
http.end();
//delay(50000); 

//////////////////////
}
    else {
      Serial.println("WiFi Disconnected");
    }


delay(3600000); 
}
    
