#include <string>
#include <ctime>

#include "feederTime.h"


#ifndef TIME_MANAGER  
#define TIME_MANAGER

using namespace std;

class TimeManager {
public:
    TimeManager();
    
    string convertToLocalTime(string);
    string convertToUTC(string);

private:

    bool isTimeValid(string);
    bool isHourValid(int);
    bool isMinutesValid(int);

    string padTime(string);
    
};

#endif