#include "data_handler.h"

#include "EEPROM.h"

DataHandler::DataHandler() {
    EEPROM.begin(512);
    configurationData = (dataStructure*)getData(512);
}

char* DataHandler::getBrokerAddress() {

}

byte* DataHandler::getData(size_t numBytes) {
    return nullptr;
}