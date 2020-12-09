#include <string>

#ifndef PET_SCHEDULE_MANAGER  
#define PET_SCHEDULE_MANAGER

class ScheduleManager {
public: 
    ScheduleManager();

    bool isValidTime(std::string time);
    bool isValidAmount(double amount);
    
    
};

#endif
