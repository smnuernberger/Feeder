#include <Arduino.h>

#include "wifiManager.h"
#include "settings.h"
#include "sdManager.h"

WifiManager *wifiManager = new WifiManager();
Settings settings;
SdManager *sdManager = new SdManager();

void setup() {
  Serial.begin(115200);
  delay(10); 
  sdManager->readSettings();
  Serial.println("Setup Test Line 13");
  
   settings = {
        .ssid = "Cicso05019",
        .password = "MarSdoras",
        .name = "Foo"
    };
  wifiManager->begin(settings);
}

void loop() {
  wifiManager->checkStatus();
}
