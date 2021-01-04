#include <string>
#include <bits/stdc++.h>

#ifndef MOTOR_SETTINGS  
#define MOTOR_SETTINGS

using namespace std;

class MotorManager {
public: 
    MotorManager();
    std::function<int()> onGetDispenseNumberOfTimes();
    
    void begin();
   
    
private:
    

};

#endif

