// Libraries
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFi.h>
#include <LoRa.h>
#include <string>

// Classes
#include "oledScreenConfig.h"
#include "LoRaModuleConfig.h"
#include "Board.h"
#include "SatelliteControl.h"
#include "SatelliteInboundSignal.h"

String IamInboundText = "S" + String(myBoard.getId()) + "-"+"GD"+"Inbound";
boolean recived = false;

void setup() {
  Serial.begin(115200);
  delay(1000);
  initializeOLED();  // **OLED ekranı başlat**
  initializeLoRa();  // **LoRa modülünü başlat**
}

void loop() {
  sendSignal(IamInboundText);
  recived = listenLoRaPackets(); // **Gelen LoRa paketlerini dinle**
  if (recived == true)
  {
    
  }
  
}