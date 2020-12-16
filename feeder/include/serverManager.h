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
    void onDeleteSettings(std::function<void()> callback);

private:
    Settings settings;
    Schedule schedule;
    Feeding feeding;

    void handleGETSettings();
    void handlePUTSettings();
    void handleDELETESettings();

    void handleGETSchedules();
    void handlePUTSchedule(); 
    void handleDELETESchedule();

    void handleGETFeedings();
    void handlePUTFeeding();
    void handleDELETEFeeding();

    bool isValidTime(string time);
    bool isValidAmount(double amount);

    string convertToString(char* array, int size); 

    void handleBadRequest(string text); 

};

#endif

