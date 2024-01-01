#include "AGDataManager.h"

AGDataManager::AGDataManager() {
    Serial.println("AGDataManager created");
    packages.clear();
    mux.begin();
    dht = Bonezegei_DHT22(DHT_PIN);
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
    dht.getData();
    package.temperature = dht.getTemperature();
    package.airHumidity = dht.getHumidity();
    Serial.println("Temperature: " + String(package.temperature));
    Serial.println("Humidity: " + String(package.airHumidity));
}

void AGDataManager::packDirtHumidity(AGDataPackage& package) {
    mux.setForDirt();
    float value = analogRead(ANALOG_PIN);
    package.dirtHumidity = value / 1024.f;
    Serial.println("Dirt humidity: " + String(analogRead(ANALOG_PIN) / 1024.f));
}

void AGDataManager::packVoltage(AGDataPackage& package) {
    mux.setForVoltage();
    float value = analogRead(ANALOG_PIN);
    package.voltage = (((value * 3.7) / 1024) * 2 + 0);
    package.batteryPercentage = mapFloat(package.voltage, 2.8, 4.15, 0, 100);
    Serial.println("Voltage: " + String(package.voltage));
    Serial.println("Battery percentage: " + String(package.batteryPercentage));
}
