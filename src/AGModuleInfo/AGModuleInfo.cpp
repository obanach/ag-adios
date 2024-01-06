#include "AGModuleInfo/AGModuleInfo.h"

AGModuleInfo::AGModuleInfo(String name, String type, const uint8_t *macAddress) : name(name), type(type) {
    memcpy(this->macAddress, macAddress, 6);
}

AGModuleInfo::AGModuleInfo() {
    WiFi.macAddress(macAddress);

    for (int i = 0; i < 4; ++i) {
        if (i > 0) name += "";
        char hexStr[3];
        sprintf(hexStr, "%02X", macAddress[i]);
        name += String(hexStr);
    }
    
    type = "ESP8266";
}

String AGModuleInfo::toString() {
    String moduleStr = name + ";" + type + ";";
    for (int i = 0; i < 6; ++i) {
        moduleStr += String(macAddress[i], HEX);
        if (i < 5) moduleStr += ":";
    }
    return moduleStr;
}

AGModuleInfo::operator String() {
    return toString();
}
