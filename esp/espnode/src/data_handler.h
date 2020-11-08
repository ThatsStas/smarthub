#ifndef DATA_HANDLER_H
#define DATA_HANDLER_H

#include <Arduino.h>

class DataHandler {
public:
    DataHandler();
    ~DataHandler();
    bool writeData();
    char* getBrokerAddress();
    char* getBrokerUser();


private:
    struct dataStructure {
        char* hostname;
        char* wifiSSID;
        char* wifiPass;
        char* brokerAddress;
        char* brokerUser;
        char* brokerPassword;
        char* brokerTopic;
        uint32 updateInterval;
    } *configurationData;

    

    byte* getData(size_t numBytes);
};

#endif