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
    settingsFile = "settings/settings.txt";
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
    delay(1000);
    SD.mkdir("settings");
}

Settings SdManager::readSettings() {
    string fileNameWithSlash = "/" + settingsFile;
    if(SD.open(fileNameWithSlash.c_str())) {
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

    std::vector<string> myData;
    std::string line; 

    while(myFile.available()) {
        line = myFile.readStringUntil(delimiter).c_str();
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
        Serial.println("Write to File Completed");
    } else {
        Serial.println("Did not write to File");
    }
    myFile.close();
}

void SdManager::deleteFile(std::string fileName) {
    string fileNameWithSlash = "/" + fileName;
    SD.remove(fileNameWithSlash.c_str());
    if(SD.exists(fileName.c_str())) {
        Serial.println("File not deleted");
    } else {
        Serial.println("File deleted"); 
    }
}

string SdManager::generateGUID() {
    std::string newGUID = ""; 
    int lenthOfGUID = 36;

    for(int i = 0; i < lenthOfGUID; i++) {
        if(i==8 || i==12 || i==16 || i==20) {
            newGUID += '-';
        }
        int randomNumberInDec = rand();
        char* randomNumberInHex = "";
        itoa(randomNumberInDec, randomNumberInHex, 16);
        if(i==13) {
            std::string temp = "4";
            strcpy(randomNumberInHex, temp.c_str());
        }
        if(i==17) {
            std::string shiftValue = "89AB";
            int randomShiftValue = 1 + (std::rand() % (4-1 +1));
            char* tempValue = new char(shiftValue[randomShiftValue]);
            //strcpy(randomNumberInHex, tempValue);
            randomNumberInHex = tempValue;
        }
        newGUID += randomNumberInHex;
    }
    return newGUID;
}
