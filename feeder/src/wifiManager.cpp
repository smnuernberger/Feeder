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
    const char *ssid = settings.ssid.c_str();
    const char *password = settings.password.c_str();
    if((this->settings.ssid.compare(settings.ssid) != 0)||(this->settings.password.compare(settings.password) != 0)) {
        WiFi.disconnect();
        delay(10);
        if(!settings.ssid.empty()) {
            WiFi.begin(ssid, password);
            Serial.print("Connecting to '");
            Serial.print(settings.ssid.c_str());
            Serial.println("'...");
        }
    }
    this->settings = settings;
}

void WifiManager::checkStatus() {
    if(!WiFi.isConnected() && WiFi.getMode() != WIFI_AP_STA && WiFi.getMode() != WIFI_AP) {
        Serial.println("Setting up AP...");
        WiFi.softAP(defaultName.c_str());
        Serial.print("\tSSID:\t");
        Serial.println(WiFi.softAPgetHostname());
        Serial.print("IP Address:\t");
        Serial.println(WiFi.softAPIP());

    } else if(WiFi.isConnected() && WiFi.getMode() == WIFI_AP_STA) {
        Serial.print("Connected to:\t");
        Serial.println(WiFi.SSID());
        Serial.print("IP Address:\t");
        Serial.println(WiFi.localIP());

        WiFi.softAPdisconnect(true);
       
        delay(1000);
        if(MDNS.begin(defaultName.c_str())) {
            MDNS.addService("petfeeder", "tcp", 80);
            Serial.println("MDNS is running.");
        } else {
            Serial.println("MDNS failed.");
        }
    }
}
