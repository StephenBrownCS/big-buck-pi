/*
    Entry point for talking with our RF Radio modules. It includes theoretical 
    facilities for sending a pulse and listening for pulses. We weren't able to 
    get it to work. See the test harnesses in the experments folder. We are able 
    to set the radio input voltage to high and low, but were unable to read from 
    the receiver's output - it requires an Analog-Digital Converter.
    
    Also the use case may have changed - instead of sending pulse - it may make more 
    sense to turn the pulse on and off, rather than give it a syncrhonous burst.
*/

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
    static void* monitorForPulses(void* radio);
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
