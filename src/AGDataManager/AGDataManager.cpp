#include "AGDataManager.h"

AGDataManager::AGDataManager() {
    mux.begin();
    dht.begin();
}

float AGDataManager::mapFloat(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void AGDataManager::createPackage() {
    AGDataPackage package;
    packDHT(package);
    packDirtHumidity(package);
    packVoltage(package);
    packages.push_back(package);

    while (packages.size() >= 10) {
        Serial.println("Package limit reached, removing oldest package");
        packages.erase(packages.begin());
    }

    Serial.println("Package created");
}

AGDataPackage AGDataManager::getPackage() {
    if(packages.size() > 0) {
        AGDataPackage package = packages.front();
        packages.erase(packages.begin());
        return package;
    }
    else {
        return AGDataPackage();
    }
}

int AGDataManager::getPackagesAmount() {
    return packages.size();
}

void AGDataManager::packDHT(AGDataPackage& package) {
    package.temperature = dht.getTemperature();
    package.airHumidity = dht.getHumidity();
}

void AGDataManager::packDirtHumidity(AGDataPackage& package) {
    mux.setForDHT();
    package.dirtHumidity = analogRead(ANALOG_PIN) / 1024.f;
}

void AGDataManager::packVoltage(AGDataPackage& package) {
    mux.setForVoltage();
    package.voltage = analogRead(ANALOG_PIN) / 1024.f;
    package.batteryPercentage = mapFloat(package.voltage, 2.8, 4.15, 0, 100);
}
