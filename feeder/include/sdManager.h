#include <string>
#include <SD.h>

#include "settings.h"
#include "schedule.h"

#ifndef CARD_SETTINGS  
#define CARD_SETTINGS

class SdManager {
public: 
    SdManager();

    void begin();

    Settings readSettings();
    void writeSettings(Settings settings);
    void deleteSettings();

    Schedule readSchedule();
    void writeSchedule(Schedule schedule);

    void readFeeding();
    void writeFeeding();

    std::string generateGUID();
private:
    Settings settings;
    fs::File myFile;

    std::vector<std::string> read(std::string fileName, char delimiter);
    void write(std::string fileName, std::vector<std::string> myData, char delimiter);
    void deleteFile(std::string fileName);

    std::string settingsFile;
    std::string scheduleFile;
    std::string feedingFile;

};

#endif
