#include <Arduino.h>
#include "wifiManager.h"
#include "settings.h"
#include "sdManager.h"

WifiManager *wifiManager = new WifiManager();
Settings settings;
SdManager *sdManager = new SdManager();

void setup() {
  //sdManager->readSettings();
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
