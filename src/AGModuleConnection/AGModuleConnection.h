// AGModuleConnection.h
#ifndef AGMODULECONNECTION_H
#define AGMODULECONNECTION_H

#include <ESP8266WiFi.h>
#include <espnow.h>
#include <EEPROM.h>
#include <vector>
#include "AGUtil/AGUtil.h"
#include "AGDataManager/AGDataManager.h"
#include "AGDataPackage/AGDataPackage.h"
#include "AGModuleInfo/AGModuleInfo.h"
#include "AGPacket/AGPacket.h"

class AGModuleConnection {
public:
    AGModuleConnection(AGDataManager& dataManagerRef);
    void initEspNow();
    bool sendMessage(const String& message, const String& macAddress);
    static void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len);
    void fetchAndSendPackage();
    void wifiConnectionSuccessful();
    void wifiConnectionFailed();
private:
    String hubMacAddress;
    static AGModuleConnection* instance;
    AGDataManager& dataManager;
};

#endif // AGMODULECONNECTION_H
