//Libaries
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFi.h>

//Classes
#include "oledScreenConfig.h"
#include "LoRaModuleConfig.h"
#include "Board.h"

//Declaring Board
Board myBoard(
    "MyBoard",                      // Board name
    1,                              // Board ID
    oledScreenConfig(21, 22, -1, 128, 64, "0x3C"),
    LoRaModuleConfig(5, 19, 27, 18, 14, 26, "868E6")
);

// 2) Adafruit_SSD1306 nesnesini, Board içindeki OLED config'den çekiyoruz
Adafruit_SSD1306 display(
    myBoard.getOledConfig().getWidth(),
    myBoard.getOledConfig().getHeight(),
    &Wire,
    myBoard.getOledConfig().getRstPin()
);

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("WiFi taramasi + OLED + LoRa Config ornegi basliyor...");

  // -- OLED Ayarları --
  // I2C başlat (SDA, SCL) => Board içindeki OLED config'den alıyoruz
  Wire.begin(myBoard.getOledConfig().getSdaPin(), myBoard.getOledConfig().getSclPin());

  // OLED I2C adresini string'den integer (hex) değere dönüştür (örnek: "0x3C" -> 0x3C)
  uint8_t i2cAddress = (uint8_t) strtol(myBoard.getOledConfig().getAddress().c_str(), NULL, 16);

  // OLED'i başlat
  if (!display.begin(SSD1306_SWITCHCAPVCC, i2cAddress)) {
    Serial.println("OLED baslatilamadi. Adres ya da baglantilari kontrol edin!");
    while (true) { }
  }
  Serial.println("OLED baslatildi.");

  // Ekranı temizleyip bir mesaj yazalım
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("WiFi Taramasi...");
  display.display();

  // -- LoRa Ayarları (isteğe bağlı olarak görüntülemek) --
  // Board içindeki LoRa konfigürasyonunu seri monitörde gösteriyoruz
  Serial.println("=== LoRa Config ===");
  Serial.print("SCK  : ");  Serial.println(myBoard.getLoraConfig().getSck());
  Serial.print("MISO : ");  Serial.println(myBoard.getLoraConfig().getMiso());
  Serial.print("MOSI : ");  Serial.println(myBoard.getLoraConfig().getMosi());
  Serial.print("SS   : ");  Serial.println(myBoard.getLoraConfig().getSs());
  Serial.print("RST  : ");  Serial.println(myBoard.getLoraConfig().getRst());
  Serial.print("DI0  : ");  Serial.println(myBoard.getLoraConfig().getDi0());
  Serial.print("BAND : ");  Serial.println(myBoard.getLoraConfig().getBand().c_str());
  Serial.println("===================");

  // -- WiFi Ayarları (Tarama) --
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  Serial.println("WiFi ag taramasi baslatiliyor...");
  int n = WiFi.scanNetworks();
  Serial.println("Tarama tamamlandi.");

  if (n == 0) {
    Serial.println("Hic WiFi agi bulunamadi.");

    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("WiFi agi bulunamadi.");
    display.display();
    
  } else {
    Serial.print("Bulunan ag sayisi: ");
    Serial.println(n);

    display.clearDisplay();
    display.setCursor(0, 0);
    display.printf("Bulunan %d ag:\n", n);

    // Yalnızca ilk 5 tanesini gösterelim
    int maxDisplay = (n < 5) ? n : 5;
    for (int i = 0; i < maxDisplay; i++) {
      // Seri monitöre yaz
      Serial.print(i + 1);
      Serial.print(") ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (RSSI: ");
      Serial.print(WiFi.RSSI(i));
      Serial.println(")");

      // OLED ekran
      display.print(i + 1);
      display.print(") ");
      display.println(WiFi.SSID(i));
    }
    display.display();
  }
}

void loop() {
  // Sürekli döngü içinde ek bir işlem yapmıyoruz
}
