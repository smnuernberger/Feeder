#include <Arduino.h>
#include "wifiManager.h"
#include "settings.h"
#include "sdManager.h"

WifiManager *wifiManager = new WifiManager();
Settings settings;
SdManager *sdManager = new SdManager();

void setup() {
  sdManager->readSettings();
  wifiManager->begin(settings);
}

void loop() {
  wifiManager->checkStatus();
}