#include <EEPROM.h>

#include "AGModuleConnection/AGModuleConnection.h"
#include "AGDataManager/AGDataManager.h"
#include "AGDataPackage/AGDataPackage.h"

AGDataManager dataManager;
AGModuleConnection moduleConnection(dataManager);
unsigned long previousMillis = 0;

void setup() {
    Serial.begin(115200);
    EEPROM.begin(512);
    dataManager = AGDataManager();
    moduleConnection.initEspNow();
}

void loop() {
    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= 20000) { // 20 seconds
        previousMillis = currentMillis;
        dataManager.createPackage();
    }
}
