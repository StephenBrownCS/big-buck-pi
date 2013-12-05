#ifndef RF_RADIO_H_
#define RF_RADIO_H_

// Forward Reference
class gnublin_gpio;

class RFRadio{
public:
    static RFRadio* create();
    ~RFRadio();
    void sendPulse();
    bool pulseDetected();
    static void monitorForPulses(RFRadio* radio);
private:
    gnublin_gpio* gpio;
    bool pulseDetectedRecently;
    pthread_mutex_t* pulseFlagLock;
    
    RFRadio();
    
    // No copy or assignment
    RFRadio( const RFRadio & );
    RFRadio & operator=(const RFRadio & );
};

#endif
