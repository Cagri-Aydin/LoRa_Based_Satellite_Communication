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
#include <vector>
#include <SD.h>
#include <SPI.h>


#define SD_CS_PIN 10
// **Board nesnesi tanımlanıyor**
Board myBoard(
    "Satellite001",
    1,
    oledScreenConfig(21, 22, -1, 128, 64, "0x3C"),
    LoRaModuleConfig(5, 19, 27, 18, 14, 26, "868E6") // LoRa için pinler ve bant
);
String thisDeviceReciverId = ("S" + String(myBoard.getId()));
// **OLED Ekran Nesnesi**
Adafruit_SSD1306 display(
    myBoard.getOledConfig().getWidth(),
    myBoard.getOledConfig().getHeight(),
    &Wire,
    myBoard.getOledConfig().getRstPin()
);

// **OLED Başlatma Fonksiyonu** Sadece
void initializeOLED() {
    Wire.begin(myBoard.getOledConfig().getSdaPin(), myBoard.getOledConfig().getSclPin());

    uint8_t i2cAddress = (uint8_t) strtol(myBoard.getOledConfig().getAddress().c_str(), NULL, 16);
    if (!display.begin(SSD1306_SWITCHCAPVCC, i2cAddress)) {
        Serial.println("OLED started");
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
String listenLoRaPackets() {
    unsigned long startTime = millis(); //Start time
    String receivedMessage = ""; 

    while (millis() - startTime < 30000) { // 30 sec control
        int packetSize = LoRa.parsePacket(); // Packet control

        if (packetSize) {
            Serial.print("Yeni LoRa Paketi Alındı: ");
            receivedMessage = ""; // Önceki mesajları temizle

            while (LoRa.available()) {
                receivedMessage += (char)LoRa.read();
            }

            Serial.println(receivedMessage);

            // OLED display for testing
            display.clearDisplay();
            display.setCursor(0, 0);
            display.println("LoRa Mesajı:");
            display.println(receivedMessage);
            display.display();

            return receivedMessage; 
        }
    }

    Serial.println("No LoRa packet recived.");
    LoRa.sleep(); 

    return "";
}

std::vector<String> splitString(String data, char delimiter) {
    std::vector<String> result;
    int startIndex = 0, endIndex = 0;

    while ((endIndex = data.indexOf(delimiter, startIndex)) != -1) {
        result.push_back(data.substring(startIndex, endIndex));
        startIndex = endIndex + 1;
    }

    result.push_back(data.substring(startIndex)); 
    return result;
}

String requestDataFromDevice(String msg) {
    sendLoRaMessage(msg); // Send the initial request

    int maxRetries = 6; // Run for 60 seconds (6 x 10 seconds)

    for (int attempt = 0; attempt < maxRetries; attempt++) {
        unsigned long startTime = millis(); // Get the start time
        int timeout = 10000; // 10-second timeout
        String receivedData = "";

        while (millis() - startTime < timeout) {
            if (LoRa.available()) { // Check if data is available
                receivedData = LoRa.readString(); // Read the received message
                Serial.print("Received: ");
                Serial.println(receivedData);

                // Split the received message by "-"
                std::vector<String> messageParts = splitString(receivedData, '-');

                // Ensure the vector has at least 2 elements
                if (messageParts.size() > 1) {
                    String messageReciver = messageParts.at(1);

                    // If the message is intended for this device, return it
                    if (thisDeviceReciverId.equals(messageReciver)) {
                        return receivedData;
                    } else {
                        Serial.println("Message not for this device, continuing to listen...");
                    }
                }
            }
        }

        // If no message is received within 10 seconds, resend the request
        Serial.println("Timeout! Resending request...");
        sendLoRaMessage(msg);
    }

    return ""; // Return an empty string if no valid message is received within 60 seconds
}





bool analyzeMessage(String message) {
    std::vector<String> messageParts = splitString(message, '-');

    if (messageParts.size() != 3) {
        Serial.print("Received corrupted data!");
        sendLoRaMessage(thisDeviceReciverId + "-" + messageParts.at(0) + "-" + "CDataCorrupted");
        return false;
    }

    String messageReciver = messageParts.at(1);

    if (thisDeviceReciverId.equals(messageReciver)) {
        Serial.print("Message in process..");

        String senderDevice = messageParts.at(0);
        String dataRecived = messageParts.at(2);

        if (dataRecived.length() > 1 && isDigit(dataRecived.charAt(1))) { // Sayısal veri olduğundan emin ol
            
            int memorySize = dataRecived.substring(1).toInt(); // İlk karakteri at, kalanını sayıya çevir
            
            if (memorySize > 0) { // Check if size is valid
                // Request data from the device
                String data = requestDataFromDevice(thisDeviceReciverId + "-" + messageParts.at(0) + "-" + "DR");
        
                if (!data.equals("")) { // If data is received, save it to file
                    saveToFile(data);
                } else {
                    Serial.println("No data received");
                }
                //Buffer'a göre iletimi yarım kalan dataların daha sonradan aktarımı sağlanacak...
            } else {
                Serial.print("Invalid memory size in received data");
            }
        } else {
            Serial.print("Invalid data format");
        }
    }
    return true;
}

void saveToFile(String data) {
    File file = SD.open("data.txt", FILE_WRITE); // Open file in append mode

    if (file) {
        file.println(data); // Write the data to the file
        file.close(); // Close the file to save changes
        Serial.println("Data saved to data.txt");
    } else {
        Serial.println("Error opening data.txt!");
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
