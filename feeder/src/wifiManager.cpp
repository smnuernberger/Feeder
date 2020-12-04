#include "wifiManager.h"

#include <sstream>
#include <Wifi.h>
#include <ESPmDNS.h>

const int NUMBER_OF_CONNECTION_ATTEMPTS = 10;

WifiManager::WifiManager() {

    std::stringstream ss;
    ss << "petfeeder_" << random(9999);
    defaultName = ss.str();

    settings = {
        .ssid = "",
        .password = "",
        .name = ""
    };

    WiFi.mode(WIFI_STA);
    WiFi.setAutoConnect(true);
    WiFi.setAutoReconnect(true);
}

void WifiManager::begin(const Settings settings) {
    const char *ssid = settings.ssid.c_str();
    const char *password = settings.password.c_str();
    int attempt = 0;
    if(attempt < NUMBER_OF_CONNECTION_ATTEMPTS) {
        WiFi.begin(ssid, password);
        delay(500);
        attempt++;
    }
    if(WiFi.status() == WL_CONNECTED) {
        WiFi.softAPdisconnect(true);
    }
}

void WifiManager::checkStatus() {
    if(WiFi.status() != WL_CONNECTED) {
        WiFi.softAP("temp AP");
        WifiManager::begin(settings);
    }
}
