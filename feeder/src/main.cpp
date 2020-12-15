#include <Arduino.h>

#include "wifiManager.h"
#include "settings.h"
#include "sdManager.h"
#include "serverManager.h"

WifiManager *wifiManager = new WifiManager();
Settings settings;
SdManager *sdManager = new SdManager();
ServerManager *serverManager = new ServerManager();
Settings getSettings();
void setSettings(Settings);

void setup() {
  Serial.begin(115200);
  delay(10); 
  //sdManager->readSettings();
  Serial.println("Setup Test Line 13");
  
  //  settings = {
  //       .ssid = "Cicso05019",
  //       .password = "MarSdoras",
  //       .name = "Foo"
  //   };
  wifiManager->begin(settings);
  serverManager->onGetSettings(std::bind(&SdManager::readSettings, sdManager));  // Pass the function getSettings() as param.
  serverManager->onSetSettings(setSettings);
  serverManager->begin();
}

void loop() {
  wifiManager->checkStatus();
  serverManager->handleClient();
}

Settings getSettings() {
  return sdManager->readSettings();
}

void setSettings(Settings newSettings) {
  if(newSettings.name.empty()) {
    newSettings.name = settings.name;
  }
  if(newSettings.ssid.empty()) {
    newSettings.ssid = settings.ssid;
  }
  if(newSettings.password.empty()) {
    newSettings.password = settings.password;
  }
  settings = newSettings;
  sdManager->writeSettings(settings);
  wifiManager->begin(settings);
}
