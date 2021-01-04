#include <string>
#include <bits/stdc++.h>

#include "settings.h"
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
    void onGetMinFeedingAmount(std::function<double()> callback);
    void onSetFeeding(std::function<void(Feeding)> callback);

private:
    Settings settings;
    Feeding feeding;

    void handleGETSettings();
    void handlePUTSettings();
    void handleDELETESettings();

    void handleGETFeedings();
    void handlePOSTFeeding();
    void handleDELETEFeeding();

    bool isTimeValid(string time);
    bool isAmountValid(string amount);
    bool isValidFeedingSize(double amount);

    string convertToString(char* array, int size); 

    void handleBadRequest(string text); 
    void handleNotAcceptable(string text); 

};

#endif

