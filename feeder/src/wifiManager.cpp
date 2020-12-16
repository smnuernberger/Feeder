#include <sstream>
#include <Wifi.h>
#include <ESPmDNS.h>

#include "wifiManager.h"

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
    Serial.println(settings.ssid.c_str());
    const char *ssid = settings.ssid.c_str();
    const char *password = settings.password.c_str();
    if((this->settings.ssid.compare(settings.ssid) != 0)||(this->settings.password.compare(settings.password) != 0)) {
        WiFi.begin(ssid, password);
        Serial.print("Connecting to...");
        Serial.println(ssid);
    }
    this->settings = settings;
}

void WifiManager::checkStatus() {
    if(!WiFi.isConnected() && WiFi.getMode() != WIFI_AP_STA && WiFi.getMode() != WIFI_AP) {
        WiFi.softAP(defaultName.c_str());
        Serial.println(defaultName.c_str());
        Serial.println(WiFi.softAPIP());
    } else if(WiFi.isConnected() && WiFi.getMode() == WIFI_AP_STA) {
        WiFi.softAPdisconnect(true);
        Serial.print("[Line 41] Connected to ");
        Serial.println(settings.ssid.c_str());
        delay(1000);
        Serial.println(WiFi.localIP());

    }
}
