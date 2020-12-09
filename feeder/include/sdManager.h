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

    template<typename T> T read();
    template<typename T> void write(T object);

    std::string settingsFile;
    std::string scheduleFile;
    std::string feedingFile;
};

#endif
