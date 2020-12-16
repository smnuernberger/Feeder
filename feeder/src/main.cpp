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
void deleteSettings();

void setup() {
  Serial.begin(115200);
  delay(10);

  sdManager->begin(); 
  settings = sdManager->readSettings();

  wifiManager->begin(settings);

  serverManager->onGetSettings(std::bind(&SdManager::readSettings, sdManager));  // Pass the function getSettings() as param.
  serverManager->onSetSettings(setSettings);
  serverManager->onDeleteSettings(deleteSettings);
  serverManager->begin();
}

void loop() {
  wifiManager->checkStatus();
  serverManager->handleClient();
}

void setSettings(Settings newSettings) {
  if(settings.ssid.compare(newSettings.ssid) != 0 && newSettings.password.empty()) {
    settings.password = "";
    Serial.print("old pw:  ");
    Serial.println(settings.password.c_str());
    Serial.print("new pw:  ");
    Serial.println(newSettings.password.c_str());
  }
  if(newSettings.name.empty()) {
    newSettings.name = settings.name;
  } 
  if(newSettings.password.empty()) {
    newSettings.password = settings.password;
  }
  if(newSettings.ssid.empty()) {
    newSettings.ssid = settings.ssid;
  }
  Serial.println("after IFs setSettings() Line 54");
  Serial.println(newSettings.ssid.c_str());
  Serial.println(newSettings.password.c_str());
  Serial.println(newSettings.name.c_str()); 

  settings = newSettings;
  sdManager->writeSettings(settings);
  
  wifiManager->begin(settings);
}

void deleteSettings() {
  sdManager->deleteSettings();
  settings = sdManager->readSettings();
  wifiManager->begin(settings);
}
