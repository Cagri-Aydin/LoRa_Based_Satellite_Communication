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
#include "DoubleLinkedList.h"
#include "ClassifyMessageForG.h"

DoublyLinkedList datasToBeComplete;
DoublyLinkedList datasReadyToSend;
DoublyLinkedList datasSendend;
String IamInboundText = "S" + String(myBoard.getId()) + "-"+"GD"+"-Inbound";
bool messageProcess = false;
void setup() {
  Serial.begin(115200);
  delay(1000);
  initializeOLED();  // **OLED ekranı başlat**
  initializeLoRa();  // **LoRa modülünü başlat**
}

void loop() {
  sendSignal(IamInboundText);
  String recivedMessage = listenLoRaPackets(); 
  if (!recivedMessage.equals(""))
  {
    messageProcess = analyzeMessage(recivedMessage);
  }
  
}