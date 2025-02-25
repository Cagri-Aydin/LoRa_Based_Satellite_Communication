#ifndef CLASSIFY_MESSAGE_FOR_G_H
#define CLASSIFY_MESSAGE_FOR_G_H

#include <Arduino.h>
#include <SatelliteControl.h>

void ClassifyMessageForG(String sender, String thisDeviceReciverId, String message){
    char commandType = message.charAt(0);
    String valuePart = message.substring(1);

    if (commandType =='C')
    {
        if (valuePart.charAt(0) == 'B' ) //Sending the firts node in the list
        {
            
            try
            {
                //Sending Data to ground station
                String data = (datasReadyToSend.getFirstNode()->senderDevice + "," + datasReadyToSend.getFirstNode()->data);
                sendLoRaMessage(thisDeviceReciverId+"-"+sender+"-"+data);
                
                //Moving data to Sended datas
                datasSendend.insertAtTail(datasReadyToSend.getFirstNode()->dataLength, datasReadyToSend.getFirstNode()->senderDevice, datasReadyToSend.getFirstNode()->data);
                datasReadyToSend.deleteFirstNode();
            }
            catch(const std::exception& e)
            {
                sendLoRaMessage(thisDeviceReciverId+"-"+sender+"-"+"ND");
            }
        }
        else if (valuePart.charAt(0) == 'E') //Sending the last node in the list
        {
            try
            {
                //Sending Data to ground station
                String data = (datasReadyToSend.getLastNode()->senderDevice + "," + datasReadyToSend.getLastNode()->data);
                sendLoRaMessage(thisDeviceReciverId+"-"+sender+"-"+data);
            
                //Moving data to Sended datas
                datasSendend.insertAtTail(datasReadyToSend.getLastNode()->dataLength, datasReadyToSend.getLastNode()->senderDevice, datasReadyToSend.getLastNode()->data);
                datasReadyToSend.deleteLastNode();
            }
            catch(const std::exception& e)
            {
                sendLoRaMessage(thisDeviceReciverId+"-"+sender+"-"+"ND");
            }
            
            
        }else if (valuePart.charAt(0) == 'D')
        {
            //spesifik cihazın datasını getirme yoksa istek gönderme
        }
        
        
    }
    

}

#endif