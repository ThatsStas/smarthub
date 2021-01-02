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
        Serial.print("DD: Updating value: "); Serial.println(updateValue);
        memset(originalValue, 0, MAX_SIZE);
        memcpy(originalValue, updateValue, strlen(updateValue));
    }
    else
    {   
        Serial.println("WW: Returning old data.");
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

bool DataHandler::setData(JsonObject& obj)
{
    for (JsonPair p: obj)
    {
        auto key = p.key().c_str();
        JsonVariant value = p.value();


        Serial.print("Key: "); Serial.println(key);
        Serial.print("Value: "); Serial.println(value.as<const char*>());

        if(!strcmp(key, "hostname"))
            hostname(value.as<const char*>());
        else if(!strcmp(key, "wifi-ssid"))
            wifiSSID(value.as<const char*>());
        else if(!strcmp(key, "wifi-password"))
            wifiPass(value.as<const char*>());
        else if(!strcmp(key, "broker-address"))
            brokerAddress(value.as<const char*>());
        else if(!strcmp(key, "broker-user"))
            brokerUser(value.as<const char*>());
        else if(!strcmp(key, "broker-password"))
            brokerPassword(value.as<const char*>());
        else if(!strcmp(key, "broker-topic"))
            brokerTopic(value.as<const char*>());
        else if(!strcmp(key, "broker-update-interval"))
            updateInterval(value.as<uint32>());
        else
        {
            Serial.print("EE: Unknown key: "); Serial.println(key);
            return false;
        }
    }

    return writeData();
}

JsonObject DataHandler::getData() 
{
    StaticJsonDocument<1024> buf;
    buf["hostname"] = hostname();
    buf["wifi-ssid"] = wifiSSID();
    buf["wifi-password"] = wifiPass();
    buf["broker-address"] = brokerAddress();
    buf["broker-user"] = brokerUser();
    buf["broker-password"] = brokerPassword();
    buf["broker-topic"] = brokerTopic();
    buf["broker-update-interval"] = updateInterval();
    return buf.as<JsonObject>();
}

char* DataHandler::getSerializedJson() {
    auto obj = getData();
    
    static char data[1024];
    
    serializeJson(obj, data);

    Serial.println("=== Serialized: ");
    Serial.println(data);

    return data;
}
