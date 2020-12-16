#include <WebServer.h>

#include "serverManager.h"


WebServer *webserver = nullptr;
//string foo;
std::function<Settings()> onGetSettingsCallback;
std::function<void(Settings)> onSetSettingsCallback;
ServerManager::ServerManager() {
    webserver = new WebServer(80); // HTTP default to port 80 :: HTTPS defaults to port 443
    
}

void ServerManager::begin() {
    webserver->begin();
    webserver->on("/settings", HTTP_GET, std::bind(&ServerManager::handleGETSettings, this));
    webserver->on("/settings", HTTP_PUT, std::bind(&ServerManager::handleSETSettings, this));
     
    //webserver->on("/string", HTTP_GET, std::bind(&ServerManager::getString, this));
    //All class function have a hidden first param that is their class.  This overwrites it and forces it to have one param. 
    //webserver->on("/string", HTTP_POST, std::bind(&ServerManager::setString, this));
}

void ServerManager::handleGETSettings() {
    Serial.println("In handleGETSettings()");
    Serial.println(settings.ssid.c_str());
    Serial.println(settings.name.c_str());
    Settings settings = onGetSettingsCallback();
    std::string myString = "{ \"ssid\" : \"" + settings.ssid + "\", \"name\" : \"" + settings.name + "\" }";
    webserver->send(200, "application/json", myString.c_str());
}

void ServerManager::handleSETSettings() {
    Settings newSettings; 
    newSettings = {
        .ssid = webserver->arg("ssid").c_str(),
        .password = webserver->arg("password").c_str(),
        .name = webserver->arg("name").c_str()
    };
    onSetSettingsCallback(newSettings);
    webserver->send(201);
}

void ServerManager::handleClient() {
    webserver->handleClient();
}

void ServerManager::onGetSettings(std::function<Settings()> callback) {
    onGetSettingsCallback = callback;
}

void ServerManager::onSetSettings(std::function<void(Settings)> callback) {
    onSetSettingsCallback = callback;
}
