#include "data_handler.h"

#include "EEPROM.h"

DataHandler::DataHandler() {
    EEPROM.begin(MAX_EEPROM_SIZE);
    EEPROM.get(BASE_ADDRESS, configurationData);

    // first boot after flash: the EEPROM data is not initialized with sane values yet
    if (configurationData.magic != MAGIC)
    {
        Serial.println("DataHandler: INIT");
        hostname("");
        wifiSSID("");
        wifiPass("");
        brokerAddress("");
        brokerUser("");
        brokerPassword("");
        brokerTopic("");
        updateInterval(1);
        configurationData.magic = MAGIC;

        writeData();
    }

}

DataHandler::~DataHandler() {
    EEPROM.end();
}

char* DataHandler::hostname(const char* hostname) {
    Serial.println("II: Hostname");
    return updateData(configurationData.hostname, hostname);
}

char* DataHandler::wifiSSID(const char* wifiSSID) {
    Serial.println("II: WifiSSID");
    return updateData(configurationData.wifiSSID, wifiSSID);
}

char* DataHandler::wifiPass(const char* wifiPass) {
        Serial.println("II: WifiPassword");
    return updateData(configurationData.wifiPass, wifiPass);
}

char* DataHandler::brokerAddress(const char* brokerAddress) {
    Serial.println("II: BrokerAddress");
    return updateData(configurationData.brokerAddress, brokerAddress);
}

char* DataHandler::brokerUser(const char* brokerUser) {
    Serial.println("II: BrokerUser");
    return updateData(configurationData.brokerUser, brokerUser);
}

char* DataHandler::brokerPassword(const char* brokerPassword) {
    Serial.println("II: BrokerPassword");
    return updateData(configurationData.brokerPassword, brokerPassword);
}

char* DataHandler::brokerTopic(const char* brokerTopic) {
    Serial.println("II: BrokerTopic");
    return updateData(configurationData.brokerTopic, brokerTopic);
}

char* DataHandler::updateData(char* originalValue, const char* updateValue)
{
    if (updateValue != nullptr && strlen(updateValue) < MAX_SIZE) 
    {
        Serial.println("DD: Updating value");
        memset(originalValue, 0, MAX_SIZE);
        memcpy(originalValue, updateValue, strlen(updateValue));
    }
    else
    {   
        Serial.println("EE: Data is too large. Returning old data.");
    }
    return originalValue;
}

uint32 DataHandler::updateInterval(const uint32 updateInterval) {
    if (updateInterval != 0)
        configurationData.updateInterval = updateInterval;
    return configurationData.updateInterval;
}

bool DataHandler::writeData() {
    EEPROM.put(BASE_ADDRESS, configurationData);
    return EEPROM.commit();
}
