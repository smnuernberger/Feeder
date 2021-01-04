#include <WebServer.h>

#include "serverManager.h"


WebServer *webserver = nullptr;

std::function<Settings()> onGetSettingsCallback;
std::function<void(Settings)> onSetSettingsCallback;
std::function<void()> onDeleteSettingsCallback;
std::function<double()> onGetMinFeedingAmountCallback;
std::function<void(Feeding)> onSetFeedingCallback;
ServerManager::ServerManager() {
    webserver = new WebServer(80); // HTTP default to port 80 :: HTTPS defaults to port 443
}

void ServerManager::begin() {
    webserver->begin();
    webserver->on("/settings", HTTP_GET, std::bind(&ServerManager::handleGETSettings, this));
    webserver->on("/settings", HTTP_PUT, std::bind(&ServerManager::handlePUTSettings, this));
    webserver->on("/settings", HTTP_DELETE, std::bind(&ServerManager::handleDELETESettings, this));
    webserver->on("/feed", HTTP_POST, std::bind(&ServerManager::handlePOSTFeeding, this));
    
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

void ServerManager::handlePOSTFeeding() {
    Feeding newFeeding;
    string timeString = webserver->arg("time").c_str();
    string amountString = webserver->arg("cups").c_str();

    if(isTimeValid(timeString)) {
        newFeeding.feedingTime = timeString;
    }

    if(isAmountValid(amountString)) {
        newFeeding.feedingAmount = strtod(amountString.c_str(), NULL); 
        webserver->send(204);
        onSetFeedingCallback(newFeeding);
    }
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

void ServerManager::onSetFeeding(std::function<void(Feeding)>callback) {
    onSetFeedingCallback = callback; 
}

void ServerManager::handleBadRequest(string text) {
    webserver->send(400, "text/plain", text.c_str());
}

void ServerManager::handleNotAcceptable(string text) {
    webserver->send(406, "text/plain", text.c_str());
}

void ServerManager::onGetMinFeedingAmount(std::function<double()> callback) {
    onGetMinFeedingAmountCallback = callback;
}

bool ServerManager::isTimeValid(string timeString) {

}

bool ServerManager::isAmountValid(string amountString) {

    if(amountString.empty()) {
        string errorMessage = "Please enter a value in cups.";
        handleBadRequest(errorMessage);
        return false;
    }
    double amountDouble = strtod(amountString.c_str(), NULL);
    if(amountDouble > 0) {
        if(isValidFeedingSize(amountDouble)) {
            return true;
        } else {
            string errorMessage = "Pleaes enter a value as a multiple of 1/8th cup.";
            handleNotAcceptable(errorMessage);
            return false;
        }
    } else {
        string errorMessage = "Please enter a positive numerical value.";
        handleNotAcceptable(errorMessage);
        return false;
    }
}

bool ServerManager::isValidFeedingSize(double amount) {
    double feedingSize = amount / onGetMinFeedingAmountCallback();
    return floor(feedingSize) == feedingSize;
}

