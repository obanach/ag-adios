#ifndef AG_DATA_MANAGER_H
#define AG_DATA_MANAGER_H

#include <Arduino.h>
#include <Bonezegei_DHT22.h>
#include <vector>
#include "AGMultiplexer/AGMultiplexer.h"
#include "AGDataPackage/AGDataPackage.h"

#define DHT_PIN D0
#define ANALOG_PIN A0

class AGDataManager {
public:
    AGDataManager();
    void createPackage();
    AGDataPackage getPackage();
    int getPackagesAmount();

private:
    void packDHT(AGDataPackage& package);
    void packDirtHumidity(AGDataPackage& package);
    void packVoltage(AGDataPackage& package);
    std::vector<AGDataPackage> packages;
    Bonezegei_DHT22 dht;
    AGMultiplexer mux;
    float mapFloat(float x, float in_min, float in_max, float out_min, float out_max);
};

#endif // AG_DATA_MANAGER_H