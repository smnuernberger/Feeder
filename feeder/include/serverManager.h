#include <string>
#include <bits/stdc++.h>

#include "settings.h"
#include "feeding.h"
#include "schedule.h"

#ifndef SERVER_SETTINGS  
#define SERVER_SETTINGS

using namespace std;

class ServerManager {
public: 
    ServerManager();
    
    void begin();
    void handleClient();

    void onGetSettings(std::function<Settings()> callback);
    void onSetSettings(std::function<void(Settings)> callback);
    void onDeleteSettings(std::function<void()> callback);
    void onGetMinFeedingAmount(std::function<double()> callback);
    void onSetFeeding(std::function<void(Feeding)> callback);
    void onGetSchedule(std::function<Schedule()> callback);
    void onSetSchedule(std::function<void(Schedule)> callback);

private:
    Settings settings;
    Feeding feeding;

    void handleGETSettings();
    void handlePUTSettings();
    void handleDELETESettings();

    void handleGETFeedings();
    void handlePOSTFeeding();
    void handleDELETEFeeding();

    void handleGetSchedule();
    void handlePOSTSchedule();

    bool isAmountValid(string);
    bool isValidFeedingSize(double);

    bool isTimeValid(string);
    bool isHourValid(int);
    bool isMinutesValid(int);

    string padTime(string);

    //string convertToString(char* array, int size); 

    void handleBadRequest(string); 
    void handleNotAcceptable(string); 

};

#endif

