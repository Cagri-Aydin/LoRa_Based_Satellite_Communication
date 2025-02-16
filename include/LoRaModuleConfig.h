#ifndef LORA_MODULE_CONFIG_H
#define LORA_MODULE_CONFIG_H

#include <string>
using namespace std;

class LoRaModuleConfig {
private:
    int sck;
    int miso;
    int mosi;
    int ss;
    int rst;
    int di0;
    string band;  

public:
    // Constructor
    LoRaModuleConfig(int sck, int miso, int mosi, int ss, int rst, int di0, const string& band)
        : sck(sck), miso(miso), mosi(mosi), ss(ss), rst(rst), di0(di0), band(band) {}

    // Getters
    int getSck()  const { return sck; }
    int getMiso() const { return miso; }
    int getMosi() const { return mosi; }
    int getSs()   const { return ss; }
    int getRst()  const { return rst; }
    int getDi0()  const { return di0; }
    string getBand() const { return band; }

    // Setters
    void setSck(int val)  { sck = val; }
    void setMiso(int val) { miso = val; }
    void setMosi(int val) { mosi = val; }
    void setSs(int val)   { ss = val; }
    void setRst(int val)  { rst = val; }
    void setDi0(int val)  { di0 = val; }
    void setBand(const string& val) { band = val; }
};

#endif // LORA_MODULE_CONFIG_H
