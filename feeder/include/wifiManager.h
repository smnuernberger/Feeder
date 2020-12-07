#include <string>
#include <IPAddress.h>

#include "settings.h"

#ifndef PET_WIFI  
#define PET_WIFI

class WifiManager {
public:
    WifiManager();
    void begin(Settings settings);
    
    void checkStatus();
private:
    std::string defaultName;

    Settings settings;
    bool createAccessPoint();
};

#endif