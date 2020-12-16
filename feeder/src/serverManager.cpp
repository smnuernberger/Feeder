#include <WebServer.h>

#include "serverManager.h"


WebServer *webserver = nullptr;

std::function<Settings()> onGetSettingsCallback;
std::function<void(Settings)> onSetSettingsCallback;
std::function<void()> onDeleteSettingsCallback;
ServerManager::ServerManager() {
    webserver = new WebServer(80); // HTTP default to port 80 :: HTTPS defaults to port 443
}

void ServerManager::begin() {
    webserver->begin();
    webserver->on("/settings", HTTP_GET, std::bind(&ServerManager::handleGETSettings, this));
    webserver->on("/settings", HTTP_PUT, std::bind(&ServerManager::handlePUTSettings, this));
    webserver->on("/settings", HTTP_DELETE, std::bind(&ServerManager::handleDELETESettings, this));
    
    //All class function have a hidden first param that is their class.  This overwrites it and forces it to have one param. 
}

void ServerManager::handleGETSettings() {
    Serial.println(settings.ssid.c_str());
    Serial.println(settings.name.c_str());
    Settings settings = onGetSettingsCallback();
    std::string myString = "{ \"ssid\" : \"" + settings.ssid + "\", \"name\" : \"" + settings.name + "\" }";
    webserver->send(200, "application/json", myString.c_str());
}

void ServerManager::handlePUTSettings() {
    Settings newSettings; 
    newSettings = {
        .ssid = webserver->arg("ssid").c_str(),
        .password = webserver->arg("password").c_str(),
        .name = webserver->arg("name").c_str()
    };
    webserver->send(201);
    onSetSettingsCallback(newSettings);
}

void ServerManager::handleDELETESettings() { 
    webserver->send(204);
    onDeleteSettingsCallback();
    
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

void ServerManager::onDeleteSettings(std::function<void()> callback) {
    onDeleteSettingsCallback = callback;
}

void ServerManager::handleBadRequest(string text) {
    webserver->send(400, "text/plain", text.c_str());
}
