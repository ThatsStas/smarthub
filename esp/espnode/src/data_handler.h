#ifndef DATA_HANDLER_H
#define DATA_HANDLER_H

#include <Arduino.h>
#include "ArduinoJson.h"

class DataHandler
{
public:
    DataHandler();
    ~DataHandler();

    char *hostname(const char *hostname = nullptr);
    char *wifiSSID(const char *wifiSSID = nullptr);
    char *wifiPass(const char *wifiPass = nullptr);
    char *brokerAddress(const char *brokerAddress = nullptr);
    char *brokerUser(const char *brokerUser = nullptr);
    char *brokerPassword(const char *brokerPassword = nullptr);
    char *brokerTopic(const char *brokerTopic = nullptr);
    uint32 updateInterval(const uint32 updateInterval = 0);

    bool setData(JsonObject &obj);
    char *getSerializedJson();

private:
    static const uint16 MAX_SIZE = 64;
    static const uint16 BASE_ADDRESS = 0;
    static const uint16 MAX_EEPROM_SIZE = 512;
    static const uint32 MAGIC = 0xdead;
    char EMPTY_STR[2] = {' '};

    struct dataStructure
    {
        char hostname[MAX_SIZE];
        char wifiSSID[MAX_SIZE];
        char wifiPass[MAX_SIZE];
        char brokerAddress[MAX_SIZE];
        char brokerUser[MAX_SIZE];
        char brokerPassword[MAX_SIZE];
        char brokerTopic[MAX_SIZE];
        uint32 updateInterval;
        uint32 magic;
    } configurationData;

    char *updateData(char *originalValue, const char *updateValue);
    bool writeData();
    JsonObject getData();
};

#endif