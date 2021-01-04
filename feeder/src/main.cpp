#include <Arduino.h>
#include <string>

#include "wifiManager.h"
#include "settings.h"
#include "sdManager.h"
#include "serverManager.h"
#include "feeding.h"
#include "motorManager.h"

WifiManager *wifiManager = new WifiManager();
Settings settings;
SdManager *sdManager = new SdManager();
ServerManager *serverManager = new ServerManager();
Feeding feeding;
MotorManager *motorManager = new MotorManager();

const double minFeedingAmount = 0.125;

double getMinFeedingAmount();
Settings getSettings();
void setSettings(Settings);
void deleteSettings();
void setFeeding(Feeding);
double getDispenseNumberOfTimes(double);

void setup() {
  Serial.begin(115200);
  delay(10);

  sdManager->begin(); 
  settings = sdManager->readSettings();

  wifiManager->begin(settings);

  serverManager->onGetSettings(std::bind(&SdManager::readSettings, sdManager));  // Pass the function getSettings() as param.
  serverManager->onSetSettings(setSettings);
  serverManager->onDeleteSettings(deleteSettings);
  serverManager->onGetMinFeedingAmount(getMinFeedingAmount);
  serverManager->onSetFeeding(setFeeding);
  serverManager->begin();

  //motorManager->onGetDispenseNumberOfTimes();
  //motorManager->begin();
}

void loop() {
  wifiManager->checkStatus();
  serverManager->handleClient();
}

double getMinFeedingAmount() {
  return minFeedingAmount;
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

void setFeeding(Feeding newFeeding) {
  String tempCupString = String(newFeeding.feedingAmount, 3);
  Serial.print("Cups Entered: ");
  Serial.println(tempCupString);
  Serial.print("Amount Motor turns:  ");
  String tempTurnString = String(getDispenseNumberOfTimes(newFeeding.feedingAmount), 0);
  Serial.println(tempTurnString);
}

double getDispenseNumberOfTimes(double newAmount) {
  return (newAmount / minFeedingAmount);
}
