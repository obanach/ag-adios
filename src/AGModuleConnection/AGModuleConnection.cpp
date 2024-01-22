// AGModuleConnection.cpp
#include "AGModuleConnection.h"

AGModuleConnection* AGModuleConnection::instance = nullptr;

void writeMacAddressToEEPROM(const String& macAddress) {
    Serial.println("Writing MAC address to EEPROM: " + macAddress);
    for (unsigned int i = 0; i < macAddress.length(); i++) 
        EEPROM.write(i, macAddress[i]);
    EEPROM.write(macAddress.length(), '\0');
    EEPROM.commit();
}

String readMacAddressFromEEPROM() {
    String macAddress;
    for (unsigned int i = 0; i < 18; i++) {
        byte readByte = EEPROM.read(i);
        if (readByte == 0xFF) return "";
        if (readByte == '\0') break;
        macAddress += char(readByte);
    }
    Serial.println("Read MAC address from EEPROM: \"" + macAddress + "\"");
    return macAddress;
}

AGModuleConnection::AGModuleConnection(AGDataManager& dataManagerRef) : dataManager(dataManagerRef) {
    instance = this;
    hubMacAddress = readMacAddressFromEEPROM();
    if(hubMacAddress == "") {
        Serial.println("No MAC address found in EEPROM.");
    } else {
        Serial.println("MAC address found in EEPROM: " + hubMacAddress);
    }
}

bool AGModuleConnection::sendMessage(const String& message, const String& macAddress) {
    uint8_t mac[6];
    sscanf(macAddress.c_str(), "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx", &mac[0], &mac[1], &mac[2], &mac[3], &mac[4], &mac[5]);

    if(esp_now_is_peer_exist(mac) == false)
        esp_now_add_peer(mac, ESP_NOW_ROLE_COMBO, 1, NULL, 0);

    int error = esp_now_send(mac, (uint8_t *)message.c_str(), message.length() + 1);
    if (error == 0)
        Serial.println("Message " + message + " sent to " + macToString(mac) + "");
    else
        Serial.println("Error sending message " + message + ": " + String(error));
    return error == 0;
}

void AGModuleConnection::initEspNow() {
    WiFi.disconnect();
    delay(100);
    
    WiFi.mode(WIFI_STA);
    if (esp_now_init() != 0) {
        Serial.println("Error initializing ESP-NOW");
        esp_now_deinit();
        if(esp_now_init() != 0) {
            Serial.println("Error initializing ESP-NOW after deinitializing");
            return;
        } else {
            Serial.println("ESP-NOW initialized after deinitializing");
        }
    }

    hubMacAddress = readMacAddressFromEEPROM();
    Serial.println("ESP-NOW initialized");
    esp_now_set_self_role(ESP_NOW_ROLE_COMBO);
    esp_now_register_recv_cb(OnDataRecv);

    uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}; // Broadcast address
    if(esp_now_is_peer_exist(broadcastAddress) == false)
        esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_COMBO, 1, NULL, 0);
}

void AGModuleConnection::OnDataRecv(uint8_t *mac, uint8_t *incomingData, uint8_t len) {
    AGPacket packet((char*)incomingData);
    Serial.println("Data received: " + packet.toString());
    bool messageFromHub = instance->hubMacAddress == macToString(mac);
    bool emptyHubMacAddress = instance->hubMacAddress == "";
    Serial.println("Message from hub: " + String(messageFromHub) + ", empty hub MAC address: " + String(emptyHubMacAddress) + ".");
    if (!instance) return;

    if (packet.header == "DISCOVER" && (emptyHubMacAddress || messageFromHub)) {
        Serial.println("Discovery request received");
        AGModuleInfo moduleInfo;
        AGPacket response("MODULE", moduleInfo);
        instance->sendMessage(response, macToString(mac));
    } else if(packet.header == "PAIR" && (emptyHubMacAddress || messageFromHub)) {
        Serial.println("Pairing request received.");
        if(emptyHubMacAddress || messageFromHub) {
            Serial.println("No hub MAC address found in EEPROM or message comes from the same device. Request accepted.");
            writeMacAddressToEEPROM(macToString(mac));
            instance->hubMacAddress = readMacAddressFromEEPROM();
            Serial.println("Hub MAC address saved to EEPROM: " + instance->hubMacAddress);
            AGModuleInfo moduleInfo;
            AGPacket response("PAIR_OK", moduleInfo);
            instance->sendMessage(response, macToString(mac));
        } else {
            Serial.println("Hub MAC address found in EEPROM. Request rejected.");
            AGModuleInfo moduleInfo;
            AGPacket response("PAIR_KO", moduleInfo);
            instance->sendMessage(response, macToString(mac));
        }
    } else if(packet.header == "UNPAIR") {
        Serial.println("Unpairing request received.");
        writeMacAddressToEEPROM("");
        instance->hubMacAddress = "";
        Serial.println("Hub MAC address removed from EEPROM.");
        AGModuleInfo moduleInfo;
        AGPacket response("UNPAIR_OK", moduleInfo);
        instance->sendMessage(response, macToString(mac));
    } else if(packet.header == "PACKAGE_SEND" && messageFromHub) {
        Serial.println("Package request received.");
        instance->fetchAndSendPackage();
    } else {
        Serial.println("Unknown message received");
    }
}

void AGModuleConnection::fetchAndSendPackage() {
    if(hubMacAddress == "") {
        hubMacAddress = readMacAddressFromEEPROM();
        if(hubMacAddress == "") {
            Serial.println("No hub MAC address found in EEPROM. Cannot send package.");
            return;
        }
    }

    if(dataManager.getPackagesAmount() == 0) {
        Serial.println("No packages to send.");
        sendMessage(AGPacket("NO_PACKAGES"), hubMacAddress);
        return;
    }
    
    AGDataPackage package = dataManager.getPackage();
    String json = package.toJSON();
    Serial.println("Sending package: " + json);
    AGPacket response("DATA", json);
    sendMessage(response, hubMacAddress);
}