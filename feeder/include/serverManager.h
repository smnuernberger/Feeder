#include <string>
#include <bits/stdc++.h>

#include "settings.h"
#include "schedule.h"
#include "feeding.h"

#ifndef SERVER_SETTINGS  
#define SERVER_SETTINGS

using namespace std;

class ServerManager {
public: 
    ServerManager();
    //void getString();
    //void setString();
    
    void begin();
    void handleClient();

    void onGetSettings(std::function<Settings()> callback);
    void onSetSettings(std::function<void(Settings)> callback);

private:
    Settings settings;
    Schedule schedule;
    Feeding feeding;

    void handleGETSettings();
    void handleSETSettings();
    void handleDELETESettings();

    void handleGETSchedules();
    void handleSETSchedule(); 
    void handleDELETESchedule();

    void handleGETFeedings();
    void handleSETFeeding();
    void handleDELETEFeeding();

    bool isValidTime(string time);
    bool isValidAmount(double amount);

    string convertToString(char* array, int size); 

};

#endif

