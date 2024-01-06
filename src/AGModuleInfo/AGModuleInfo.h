// AGModule.h
#ifndef AG_MODULE_INFO_H
#define AG_MODULE_INFO_H

#include <Arduino.h>
#include <vector>
#include <ESP8266WiFi.h>

class AGModuleInfo {
public:
    AGModuleInfo();
    AGModuleInfo(String name, String type, const uint8_t *macAddress);
    String name;
    String type;
    uint8_t macAddress[6];

    String toString();
    operator String();
};

#endif // AG_MODULE_INFO_H