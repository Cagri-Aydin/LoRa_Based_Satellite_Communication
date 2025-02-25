#ifndef CLASSIFY_MESSAGE_FOR_D_H
#define CLASSIFY_MESSAGE_FOR_D_H

#include <Arduino.h>
#include <SatelliteControl.h>

void ClassifyMessageForD(String sender, String thisDeviceReciverId, String message){
    char commandType = message.charAt(0);
    String valuePart = message.substring(1);
    
    if(commandType == 'M') //M -> Meta-Data, means device send us the data's capacity 
    {
        MetaDataProcess(sender,thisDeviceReciverId,valuePart);
    }
    


}







#endif