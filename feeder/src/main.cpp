#include <Arduino.h>

#include "wifiManager.h"
#include "settings.h"
#include "sdManager.h"
#include "serverManager.h"

WifiManager *wifiManager = new WifiManager();
Settings settings;
//SdManager *sdManager = new SdManager();
ServerManager *serverManager = new ServerManager();
Settings getSettings();
void setSettings(Settings);

void setup() {
  Serial.begin(115200);
  delay(10); 
  //sdManager->readSettings();
  Serial.println("Setup Test Line 13");
  
   settings = {
        .ssid = "Cicso05019",
        .password = "MarSdoras",
        .name = "Foo"
    };
  wifiManager->begin(settings);
  serverManager->onGetSettings(getSettings);  // Pass the function getSettings() as param.
  serverManager->onSetSettings(setSettings);
  serverManager->begin();
}

void loop() {
  wifiManager->checkStatus();
  serverManager->handleClient();
}

Settings getSettings() {
  return settings;
}

void setSettings(Settings mySettings) {
  settings = mySettings;
}
