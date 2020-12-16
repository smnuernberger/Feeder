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
    Serial.println("Initializing SD card...");
    // Param 22 is the pin that the 'CS' is plugged into. 
    if (!SD.begin(22)) {
        Serial.println("Initialization failed.");
        while(1);
    }
    Serial.println("Initialization Completed");

    Settings settings; 
    string feederName = settings.name.c_str();
    settingsFile = "settings.txt";
    scheduleFile = "schedule.txt";
    feedingFile = "feeding.txt";
}

Settings SdManager::readSettings() {
    if(SD.exists(settingsFile.c_str())) {
        std::vector<string> settingsData = read(settingsFile, '\n');
        return {
            .ssid = settingsData[0],
            .password = settingsData[1],
            .name =  settingsData[2]
        };
    } else {
        return {
            .ssid = "",
            .password = "",
            .name =  ""
        };
    }
}

void SdManager::writeSettings(const Settings settings) {
    if(SD.exists(settingsFile.c_str())) {
        SD.remove(settingsFile.c_str());
    }

    std::vector<string> settingsData;
    settingsData.push_back(settings.ssid);
    settingsData.push_back(settings.password);
    settingsData.push_back(settings.name);
    Serial.println("Line 57: from vector");
    Serial.println(settingsData[0].c_str());
    Serial.println(settingsData[2].c_str());
    write(settingsFile, settingsData, '\n');
} 

std::vector<std::string> SdManager::read(std::string fileName, char delimiter) {
    string fileNameWithSlash = "/" + fileName;
    File myFile = SD.open(fileNameWithSlash.c_str(), FILE_READ);
    std::vector<string> myData;
    string line;
    stringstream fullLine(line);
    string temp;
    while(myFile.available()) {
        line = myFile.read();
    }
    if(line.length() > 0) {
        while(getline(fullLine, temp, delimiter)) {
            myData.push_back(temp);
        }
    }
    return myData;
    myFile.close();
}

void SdManager::write(std::string fileName, std::vector<std::string> myData, char delimiter) {
    File myFile;
    string fileNameWithSlash = "/" + fileName;
    myFile = SD.open(fileNameWithSlash.c_str(), FILE_WRITE);
    Serial.print("Line 83 in write(): ");
    Serial.println(fileName.c_str());
    std::string line; 
    std::string lineToFile;
    int myDataLength = myData.size();

    for(int i=0; i<myDataLength; i++) {
        lineToFile += myData[i];
        lineToFile += delimiter;
    }
    Serial.println("Line 94  ");
    Serial.println(lineToFile.c_str());
    if(myFile) {
        Serial.println("Line 94 SDManager  ");
        Serial.println(lineToFile.c_str());
        myFile.println(lineToFile.c_str());
        Serial.println(lineToFile.c_str());
        Serial.println("Write to File Completed");
    } else {
        Serial.println("Did not write to File");
    }
    myFile.close();
}
