#ifndef AG_DATA_PACKAGE_H
#define AG_DATA_PACKAGE_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <ctime>

class AGDataPackage {
public:
    AGDataPackage();
    time_t timestamp;
    float temperature;
    int airHumidity;
    int dirtHumidity;
    float voltage;
    float batteryPercentage;
    String toJSON();

private:
    String formatTimestamp(time_t rawTime);
};

#endif // AG_DATA_PACKAGE_H
