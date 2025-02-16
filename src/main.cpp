// Libraries
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFi.h>
#include <LoRa.h>

// Classes
#include "oledScreenConfig.h"
#include "LoRaModuleConfig.h"
#include "Board.h"
#include "SatelliteControl.h"

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("ESP32 LoRa Terminal Kontrolü Başlıyor...");

  initializeOLED();  // **OLED ekranı başlat**
  initializeLoRa();  // **LoRa modülünü başlat**
}

void loop() {
  checkSerialInput();  // **Kullanıcıdan gelen veriyi kontrol et**
  listenLoRaPackets(); // **Gelen LoRa paketlerini dinle**
}