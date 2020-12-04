#include <string>

#ifndef CARD_SETTINGS  
#define CARD_SETTINGS

class SdManager {
public: 
    SdManager();

    Settings readSettings();
    void writeSettings(Settings settings);

    void readSchedule();
    void writeSchedule();
};

#endif
