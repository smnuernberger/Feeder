#include <SPI.h>
#include <SD.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <EEPROM.h>

#include "sdManager.h"
#include "settings.h"

using namespace std;

SdManager::SdManager() {
    cout << "Initializing SD card...";
    if (!SD.begin(10)) {
        cout << "Initialization failed.";
        while(1);
    }
    cout << "Initialization Completed";

    Settings settings; 
    string feederName = settings.name.c_str();
    settingsFile = feederName + "_settings.txt";
    scheduleFile = feederName + "_schedule.txt";
    feedingFile = feederName + "_feeding.txt";
}

Settings SdManager::readSettings() {
    File myFile;
    Settings settings;
    myFile = SD.open(settingsFile.c_str(),FILE_READ);
    if(myFile) {
        settings = {
            .ssid = std::string(myFile.readStringUntil('\n').c_str()),
            .password = std::string(myFile.readStringUntil('\n').c_str()),
            .name = std::string(myFile.readStringUntil('\n').c_str()),
        };
    }
    myFile.close();
    return settings;
}

void SdManager::writeSettings(const Settings settings) {
    File myFile;
    SD.remove(settingsFile.c_str());
    myFile = SD.open(settingsFile.c_str(),FILE_WRITE);
    if(myFile) {
        myFile.println(settings.ssid.c_str());
        myFile.println(settings.password.c_str());
        myFile.println(settings.name.c_str());
    }
    myFile.close(); 
}
