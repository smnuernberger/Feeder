// #include <SPI.h>
// #include <SD.h>
// #include <iostream>
// #include <fstream>
// #include <string>

// #include "sdManager.h"
// #include "settings.h"

// using namespace std;

// SdManager::SdManager() {
//     cout << "Initializing SD card...";
//     if (!SD.begin(10)) {
//         cout << "Initialization failed.";
//         while(1);
//     }
//     cout << "Initialization Completed";

//     //myFile = SD.open("feeder.txt",FILE_WRITE);
// }

// Settings SdManager::readSettings() {
//     File myFile;
//     Settings settings;
//     myFile = SD.open("feeder_settings.txt",FILE_READ);
//     if(myFile) {
//         while (myFile.available()) {
//             return {
//                 .ssid = std::string(myFile.readStringUntil('\n').c_str()),
//                 .password = std::string(myFile.readStringUntil('\n').c_str()),
//                 .name = std::string(myFile.readStringUntil('\n').c_str()),
//             };
//         }
//     }
//     myFile.close();
// }

// void SdManager::writeSettings(const Settings settings) {
//     File myFile;
//     SD.remove("feeder_settings.txt");
//     myFile = SD.open("feeder_settings.txt",FILE_WRITE);
//     if(myFile) {
//         myFile.println(settings.ssid.c_str());
//         myFile.println(settings.password.c_str());
//         myFile.println(settings.name.c_str());
//     }
//     myFile.close(); 
// }
