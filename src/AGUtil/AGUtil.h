#ifndef AG_UTIL_H
#define AG_UTIL_H

#include <ESP8266WiFi.h>

String macToString(const uint8_t* mac);
bool stringToMac(const String& macStr, uint8_t* mac);

#endif // AG_UTIL_H