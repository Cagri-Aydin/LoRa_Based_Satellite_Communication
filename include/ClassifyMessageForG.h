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
                String data = dataValue(datasReadyToSend,'F');
                sendLoRaMessage(thisDeviceReciverId+"-"+sender+"-"+data);
                
                //Moving data to Sended datas
                if (!data.equals("No-Data"))
                {
                    datasSendend.insertAtTail(datasReadyToSend.getFirstNode()->dataLength, datasReadyToSend.getFirstNode()->senderDevice, datasReadyToSend.getFirstNode()->data);
                    datasReadyToSend.deleteFirstNode();
                }
                
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
                String data = dataValue(datasReadyToSend,'L');
                sendLoRaMessage(thisDeviceReciverId+"-"+sender+"-"+data);
            
                //Moving data to Sended datas
                if (!data.equals("No-Data"))
                {
                    datasSendend.insertAtTail(datasReadyToSend.getLastNode()->dataLength, datasReadyToSend.getLastNode()->senderDevice, datasReadyToSend.getLastNode()->data);
                    datasReadyToSend.deleteLastNode();
                }   
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

String dataValue(DoublyLinkedList DLL, char order){
    String data;
    if (order == 'F') //First node
    {
        String sender = DLL.getFirstNode()-> senderDevice;
        String data = DLL.getFirstNode()->data;

        if (data == nullptr || sender == nullptr)
        {
            data = "No-Data";
        }else{
            data = sender + "," + data;
        }
        return data;

    }else if (order == 'L') //Last Node
    {
        String sender = DLL.getLastNode()-> senderDevice;
        String data = DLL.getLastNode()->data;

        if (data == nullptr || sender == nullptr)
        {
            data = "No-Data";
        }else{
            data = sender + "," + data;
        }
        return data;
    }
}

#endif