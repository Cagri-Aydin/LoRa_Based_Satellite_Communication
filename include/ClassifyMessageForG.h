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
                String confirmationString = requestDataConfirmation(thisDeviceReciverId+"-"+sender+"-"+data);
                
                //Moving data to Sended datas
                if (!data.equals("No-Data") && confirmationString.equals("Data Recived"))
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
                String confirmationString = requestDataConfirmation(thisDeviceReciverId+"-"+sender+"-"+data);
                
            
                //Moving data to Sended datas
                if (!data.equals("No-Data") && confirmationString.equals("Data Recived"))
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
            try
            {
                String deviceID = valuePart.substring(1);
                String data = datasReadyToSend.findNodeBySender(deviceID)->data;
                String confirmationString = "";
                if (data != nullptr)
                {
                    confirmationString = requestDataConfirmation(thisDeviceReciverId+"-"+sender+"-"+data);
                    if (confirmationString.equals("Data Recived"))
                    {
                        datasSendend.insertAtTail(datasReadyToSend.findNodeBySender(deviceID)->dataLength, datasReadyToSend.findNodeBySender(deviceID)->senderDevice,datasReadyToSend.findNodeBySender(deviceID)->data);
                        datasReadyToSend.deleteNodeBySender(datasReadyToSend.findNodeBySender(deviceID)->senderDevice);
                    }
                    
                }else{
                    confirmationString = requestDataConfirmation(thisDeviceReciverId+"-"+sender+"-"+"No Device Data");
                    if (confirmationString.equals("Ask Sats"))//asking other satillites for the same data
                    {
                        sendLoRaMessage(thisDeviceReciverId+"S"+valuePart);
                    }
                    
                }
                
            }
            catch(const std::exception& e)
            {
                sendLoRaMessage(thisDeviceReciverId+"-"+sender+"-"+"ND");
            }
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

String requestDataConfirmation(String msg) {
    sendLoRaMessage(msg); // Send the initial request

    int maxRetries = 6; // Run for 60 seconds (6 x 10 seconds)

    for (int attempt = 0; attempt < maxRetries; attempt++) {
        unsigned long startTime = millis(); // Get the start time
        int timeout = 10000; // 10-second timeout
        String receivedData = "";

        while (millis() - startTime < timeout) {
            if (LoRa.available()) { // Check if data is available
                receivedData = LoRa.readString(); // Read the received message
                Serial.print("Received: ");
                Serial.println(receivedData);

                // Split the received message by "-"
                std::vector<String> messageParts = splitString(receivedData, '-');

                // Ensure the vector has at least 2 elements
                if (messageParts.size() == 3) {
                    String messageReciver = messageParts.at(1);

                    // If the message is intended for this device, return it
                    if (thisDeviceReciverId.equals(messageReciver)) {
                        return receivedData;
                    } else {
                        Serial.println("Message not for this device, continuing to listen...");
                    }
                }
            }
        }

        // If no message is received within 10 seconds, resend the request
        Serial.println("Timeout! Resending request...");
        sendLoRaMessage(msg);
    }

    return ""; // Return an empty string if no valid message is received within 60 seconds
}

#endif