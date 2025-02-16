#ifndef SATELLITE_CONTROL_H
#define SATELLITE_CONTROL_H

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <LoRa.h>
#include "oledScreenConfig.h"
#include "LoRaModuleConfig.h"
#include "Board.h"

// **Board nesnesi tanımlanıyor**
Board myBoard(
    "MyBoard",
    1,
    oledScreenConfig(21, 22, -1, 128, 64, "0x3C"),
    LoRaModuleConfig(5, 19, 27, 18, 14, 26, "868E6") // LoRa için pinler ve bant
);

// **OLED Ekran Nesnesi**
Adafruit_SSD1306 display(
    myBoard.getOledConfig().getWidth(),
    myBoard.getOledConfig().getHeight(),
    &Wire,
    myBoard.getOledConfig().getRstPin()
);

// **OLED Başlatma Fonksiyonu**
void initializeOLED() {
    Wire.begin(myBoard.getOledConfig().getSdaPin(), myBoard.getOledConfig().getSclPin());

    uint8_t i2cAddress = (uint8_t) strtol(myBoard.getOledConfig().getAddress().c_str(), NULL, 16);
    if (!display.begin(SSD1306_SWITCHCAPVCC, i2cAddress)) {
        Serial.println("OLED başlatılamadı!");
        for (;;);
    }
    
    Serial.println("OLED başlatıldı.");
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println("LoRa Terminal Kontrolü");
    display.display();
}

// **LoRa Başlatma Fonksiyonu**
void initializeLoRa() {
    Serial.println("LoRa başlatılıyor...");
    LoRa.setPins(
        myBoard.getLoraConfig().getSs(),
        myBoard.getLoraConfig().getRst(),
        myBoard.getLoraConfig().getDi0()
    );

    if (!LoRa.begin(atof(myBoard.getLoraConfig().getBand().c_str()))) {
        Serial.println("LoRa başlatılamadı!");
        for (;;);
    }

    Serial.println("LoRa Başlatıldı! Sürekli Dinleme Modunda...");
    LoRa.receive(); // LoRa'yı sürekli dinleme moduna al
}

// **LoRa Paketlerini Dinleme Fonksiyonu**
void listenLoRaPackets() {
    int packetSize = LoRa.parsePacket(); // Gelen paket var mı kontrol et
    if (packetSize) {
        Serial.print("Yeni LoRa Paketi Alındı: ");
        String received = "";
        while (LoRa.available()) {
            received += (char)LoRa.read();
        }

        Serial.println(received);

        // OLED ekrana yaz
        display.clearDisplay();
        display.setCursor(0, 0);
        display.println("LoRa Mesajı:");
        display.println(received);
        display.display();
    }
}

// **LoRa Mesaj Gönderme Fonksiyonu**
void sendLoRaMessage(String message) {
    Serial.print("LoRa Mesaj Gönderiliyor: ");
    Serial.println(message);

    LoRa.beginPacket();
    LoRa.print(message);
    LoRa.endPacket();

    Serial.println("Mesaj Gönderildi!");

    // OLED Ekrana Gönderildiğini Yaz
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("Gönderilen Mesaj:");
    display.println(message);
    display.display();

    // Tekrar dinleme moduna dön
    LoRa.receive();
}

// **Seri Monitörden Kullanıcı Girişi Okuma**
void checkSerialInput() {
    if (Serial.available() > 0) {
        char input = Serial.read(); // Kullanıcının girdiği karakteri oku

        if (input == '1') {
            sendLoRaMessage("Merhaba, LoRa!"); // Terminalden '1' basıldığında mesaj gönder
        }
    }
}

#endif // SATELLITE_CONTROL_H
