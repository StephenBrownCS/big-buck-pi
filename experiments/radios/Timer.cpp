
#include "Utility.h"
#include "Timer.h"

Timer::Timer(){
    startTime = 0;
    isRunning = false;
}
    
void Timer::startCountdown(int numMilliseconds){
    startTime = getMilliCount();
    duration = numMilliseconds;
    isRunning = true;
}
    
bool Timer::hasExpired(){
    if (isRunning)
        return getMilliSpan(startTime) > duration;
    else
        return true;
}
