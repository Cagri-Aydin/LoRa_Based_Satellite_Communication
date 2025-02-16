#ifndef BOARD_H
#define BOARD_H

#include <string>
#include "oledScreenConfig.h"
#include "LoRaModuleConfig.h"

using namespace std;

class Board {
private:
    string name;               // Board name
    int id;                    // Board ID
    oledScreenConfig oledCfg;  // OLED screen settings
    LoRaModuleConfig loraCfg;  // LoRa module settings

public:
    //Constructor
    Board(const string& boardName, int boardId,
          const oledScreenConfig& oled,
          const LoRaModuleConfig& lora)
        : name(boardName), id(boardId), oledCfg(oled), loraCfg(lora) {}

    // Getters
    string getName() const { return name; }
    int getId() const { return id; }
    oledScreenConfig getOledConfig() const { return oledCfg; }
    LoRaModuleConfig getLoraConfig() const { return loraCfg; }

    // Setters
    void setName(const string& newName) { name = newName; }
    void setId(int newId) { id = newId; }
    void setOledConfig(const oledScreenConfig& newOled) { oledCfg = newOled; }
    void setLoraConfig(const LoRaModuleConfig& newLora) { loraCfg = newLora; }
};

#endif // BOARD_H
