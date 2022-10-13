#include <Arduino.h>

#if defined(ARDUINO_ARCH_RP2040)
void setup() {}
void loop() {}

#else
#if defined(ESP32)
    #include <WiFi.h>
#else
    #include <ESP8266WiFi.h>
#endif
#include "handel.h"

#ifndef STASSID
#define STASSID "janxland"
#define STAPSK  "88888888"
#endif

void setup()
{
  // pinMode(25,OUTPUT);digitalWrite(25,HIGH);
  pinMode(25,INPUT_PULLDOWN);digitalWrite(2,LOW);
  pinMode(27,INPUT_PULLDOWN);digitalWrite(27,LOW);
  pinMode(14,INPUT_PULLDOWN);digitalWrite(14,LOW);
  Serial.begin(115200);
  delay(1000);
  Serial.println("Connecting to WiFi");
  WiFi.disconnect();
  WiFi.softAPdisconnect(true);
  WiFi.mode(WIFI_STA);
  WiFi.begin(STASSID, STAPSK);
  // Try forever
  while (WiFi.status() != WL_CONNECTED) {
    Serial.println("...Connecting to WiFi");
    delay(1000);
  }
  Serial.println("Connected");
  pushMusic("Taylor",1);
  delay(2000);
  init_lvgl();
}


void loop()
{
  handleIndex();
  flush_screen();
  if(paused){

  } else {
    loop_music();
  }
  delay(3);
}
#endif
