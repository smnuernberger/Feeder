#include <SPI.h>
#include <SD.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <EEPROM.h>
#include <vector>

#include "sdManager.h"
#include "settings.h"

using namespace std;

SdManager::SdManager() {
    //Settings settings; 
    settingsFile = "settings.txt";
    scheduleFile = "schedule.txt";
    feedingFile = "feeding.txt";
}

void SdManager::begin() {
    Serial.println("Initializing SD card...");
    // Param 22 is the pin that the 'CS' is plugged into. 
    if (!SD.begin(22)) {
        Serial.println("Initialization failed.");
        while(1);
    }
    Serial.println("Initialization Completed");
}

Settings SdManager::readSettings() {
    Serial.println("Line 32: readSettings()");
    string fileNameWithSlash = "/" + settingsFile;
    if(SD.open(fileNameWithSlash.c_str())) {
        Serial.println("Line 34: readSettings()");
        std::vector<string> settingsData = read(settingsFile, '\n');
        return {
            .ssid = settingsData[0],
            .password = settingsData[1],
            .name =  settingsData[2]
        };
    } else {
        Serial.println("Line 43: readSettings()");
        return {
            .ssid = "",
            .password = "",
            .name =  ""
        };
    }
}

void SdManager::writeSettings(const Settings settings) {
    if(SD.exists(settingsFile.c_str())) {
        deleteFile(settingsFile);
    }

    std::vector<string> settingsData;
    settingsData.push_back(settings.ssid);
    settingsData.push_back(settings.password);
    settingsData.push_back(settings.name);
   
    write(settingsFile, settingsData, '\n');
} 

void SdManager::deleteSettings() {
    deleteFile(settingsFile);
}

std::vector<std::string> SdManager::read(std::string fileName, char delimiter) {
    string fileNameWithSlash = "/" + fileName;
    File myFile = SD.open(fileNameWithSlash.c_str(), FILE_READ);
    Serial.println("Line 64 in read()");
    std::vector<string> myData;
    std::string line; 

    Serial.println("Line 67 read()");
    while(myFile.available()) {
        line = myFile.readStringUntil(delimiter).c_str();
        Serial.println(line.c_str());
        myData.push_back(line);
    }
    myFile.close();
    return myData;
}

void SdManager::write(std::string fileName, std::vector<std::string> myData, char delimiter) {
    File myFile;
    string fileNameWithSlash = "/" + fileName;
    myFile = SD.open(fileNameWithSlash.c_str(), FILE_WRITE);
    
    std::string line; 
    std::string lineToFile;
    int myDataLength = myData.size();

    for(int i=0; i<myDataLength; i++) {
        lineToFile += myData[i];
        lineToFile += delimiter;
    }
    if(myFile) {
        myFile.println(lineToFile.c_str());
        Serial.println(lineToFile.c_str());
        Serial.println("Write to File Completed");
    } else {
        Serial.println("Did not write to File");
    }
    myFile.close();
}

void SdManager::deleteFile(std::string fileName) {
    SD.remove(fileName.c_str());
}
