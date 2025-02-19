#ifndef DATA_PROCESS_H
#define DATA_PROCESS_H

#include <Arduino.h>
#include <SatelliteControl.h>

void processDirectDataRequest(String valuePart) {
    int dataValue = valuePart.toInt();

    Serial.print("Processing direct data value: ");
    Serial.println(dataValue);

    
    //processDirectData(dataValue);
}

#endif // DATA_PROCESS_H
