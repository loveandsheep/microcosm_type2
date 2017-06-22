#include <OSCMessage.h>
#include <OSCBundle.h>
#include <OSCData.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <WiFiUDP.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ArduinoOTA.h>
#include <WebSocketsServer.h>
#include <Hash.h>
#include <FS.h>
#include <SPI.h>

#define PIN_M1_D1 16
#define PIN_M1_D2 5
#define PIN_EN    4
#define PIN_M2_D1 2
#define PIN_M2_D2 0

#define ADC_ROT_A 0
#define ADC_ROT_B 1
#define ADC_PHOTO_ROOT 2
#define ADC_PHOTO_TIP 3

#define PIN_CS_ADC  15
#define PIN_MISO    12
#define PIN_MOSI    13
#define PIN_SCK     14

void setup() 
{
  
  pinMode(PIN_CS_ADC, OUTPUT);
  pinMode(PIN_MISO, OUTPUT);
  pinMode(PIN_MOSI, OUTPUT);
  pinMode(PIN_SCK, OUTPUT);

  pinMode(PIN_M1_D1, OUTPUT);
  pinMode(PIN_M1_D2, OUTPUT);
  pinMode(PIN_M2_D1, OUTPUT);
  pinMode(PIN_M2_D2, OUTPUT);
  pinMode(PIN_EN, OUTPUT);

  Serial.begin(74880);
  init_wifi();
  init_osc();
  ota_standby();
}

int lastCount = 0;

void loop() 
{
  ota_handle();
  server_update();
  update_osc();
  actuate();
}

