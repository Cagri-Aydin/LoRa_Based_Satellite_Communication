#ifndef OLED_SCREEN_CONFIG_H
#define OLED_SCREEN_CONFIG_H

#include <string>
using namespace std;

class oledScreenConfig {
private:
    int sdaPin, sclPin, rstPin;
    int width, height;
    string address;

public:
    // Constructor
    oledScreenConfig(int sda, int scl, int rst, int w, int h, string addr)
        : sdaPin(sda), sclPin(scl), rstPin(rst), width(w), height(h), address(addr) {}

    // Getters
    int getSdaPin()     const { return sdaPin; }
    int getSclPin()     const { return sclPin; }
    int getRstPin()     const { return rstPin; }
    int getWidth()      const { return width; }
    int getHeight()     const { return height; }
    string getAddress() const { return address; }

    // Setters
    void setSdaPin(int pin)               { sdaPin = pin; }
    void setSclPin(int pin)               { sclPin = pin; }
    void setRstPin(int pin)               { rstPin = pin; }
    void setWidth(int w)                  { width = w; }
    void setHeight(int h)                 { height = h; }
    void setAddress(const string& addr)   { address = addr; }
};

#endif // OLED_SCREEN_CONFIG_H
