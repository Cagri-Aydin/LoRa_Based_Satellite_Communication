#ifndef METADATA_PROCESS_H
#define METADATA_PROCESS_H

#include <Arduino.h>
#include <SatelliteControl.h>


void MetaDataProcess(String senderDevice, String thisDeviceReciverId, String valuePart) {
    int memorySize = valuePart.toInt();

    if (memorySize > 0) {
        Serial.print("Processing memory request with size: ");
        Serial.println(memorySize);

        //Requesting Data for this satellite
        String data = requestDataFromDevice(thisDeviceReciverId + "-" + senderDevice + "-" + "DR");

        if (!data.equals("")) {
            saveToFile(data); 
        } else {
            Serial.println("No data received");
        }
    } else {
        Serial.println("Invalid memory size in received data");
    }
}

#endif // METADATA_PROCESS_H
