#include <string>
#include <cstdlib>


#include "timeManager.h"

TimeManager::TimeManager() {

}

bool isTimeValid(string userTime) {
    if(userTime.length() == 4 || userTime.length() == 5) {
        if(userTime.length() == 4) {
            userTime = padTime(userTime);
        }
        
        int newHour = atoi(userTime.substr(0,2).c_str());
        int newMinute = atoi(userTime.substr(3).c_str());

        if(isHourValid(newHour) && isMinutesValid(newMinute)) {
            return true;
        } else {
            return false;
        }

    } else {
        return false;
    }
}

string padTime(string userTime) {
    return "0" + userTime;
}

bool isHourValid(int userHour) {
    if(userHour < 0 || userHour > 23) {
        return false;
    } else {
        return true;
    }
}

bool isMinutesValid(int userMinutes) {
    if(userMinutes < 0 || userMinutes > 59) {
        return false;
    } else {
        return true;
    }
}


