
/*
    Wrapper class for getMilliseconds() and get MilliSpan()
    Works more intuitively like a timer

*/

#ifndef TIMER__H__
#define TIMER__H__

class Timer{
public:
    Timer();
    
    // Starts timer in background
    // When numMilliseconds has passed, will
    // enter expired state
    void startCountdown(int numMilliseconds);
    
    // Returns true if countdown has not begun
    // Otherwise, returns true if and only if the countdown
    // time has passed
    bool hasExpired();
    
private:
    int startTime;
    int duration;
    bool isRunning;
};

#endif
