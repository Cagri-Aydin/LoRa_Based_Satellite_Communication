#ifndef SATELLITE_INBOUND_SIGNAL_H
#define SATELLITE_INBOUND_SIGNAL_H

#include "SatelliteControl.h"

// LoRa sinyali gönderen fonksiyon
void sendSignal(const String& message) {
    sendLoRaMessage(message); // SatelliteControl.h içindeki fonksiyonu çağır
}

#endif // SATELLITE_INBOUND_SIGNAL_H
