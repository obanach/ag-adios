#include "AGDataPackage.h"

AGDataPackage::AGDataPackage() {
    timestamp = time(nullptr);
    temperature = 0.f;
    airHumidity = 0;
    dirtHumidity = 0;
    voltage = 0.f;
    batteryPercentage = 0.f;
}

String AGDataPackage::toJSON() {
    StaticJsonDocument<200> doc;

    doc["timestamp"] = formatTimestamp(timestamp);
    doc["temperature"] = temperature;
    doc["airHumidity"] = airHumidity;
    doc["dirtHumidity"] = dirtHumidity;
    doc["voltage"] = voltage;
    doc["batteryPercentage"] = batteryPercentage;

    String output;
    serializeJson(doc, output);
    return output;
}

String AGDataPackage::formatTimestamp(time_t rawTime) {
    char buffer[30];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", localtime(&rawTime));
    return String(buffer);
}