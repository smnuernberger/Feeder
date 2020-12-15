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
    std::vector<string> settingsData = read(settingsFile, '\n');

    return {
        .ssid = settingsData[0],
        .password = settingsData[1],
        .name =  settingsData [2]
    };
}

void SdManager::writeSettings(const Settings settings) {
    SD.remove(settingsFile.c_str());

    std::vector<string> settingsData;
    settingsData[0] = settings.ssid;
    settingsData[1] = settings.password;
    settingsData[2] = settings.name;
   
   write(settingsFile, settingsData, '\n');
}

std::vector<std::string> read(std::string fileName, char delimiter) {
    File myFile = SD.open(fileName.c_str(), FILE_READ);
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
        return myData;
    }
    myFile.close();
}

void write(std::string fileName, std::vector<std::string> myData, char delimiter) {
    File myFile = SD.open(fileName.c_str(), FILE_WRITE);
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
        Serial.print("Did not write to File");
    }
    myFile.close();
}
