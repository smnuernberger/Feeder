#include <string>

#include "settings.h"
#include "schedule.h"

#ifndef CARD_SETTINGS  
#define CARD_SETTINGS

class SdManager {
public: 
    SdManager();

    Settings readSettings();
    void writeSettings(Settings settings);

    Schedule readSchedule();
    void writeSchedule(Schedule schedule);

    void readFeeding();
    void writeFeeding();

private:
    Settings settings;
    File myFile;

    std::vector<std::string> read(std::string fileName, char delimiter);
    void write(std::string fileName, std::vector<std::string> myData, char delimiter);

    std::string settingsFile;
    std::string scheduleFile;
    std::string feedingFile;
};

#endif
