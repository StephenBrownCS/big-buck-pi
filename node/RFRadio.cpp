#include <pthread.h>
#include <chrono>
#include <thread>
#include "RFRadio.h"

#define BOARD RASPBERRY_PI
#include "gnublin.h"
#include "Utility.h"

const int PULSE_SAMPLING_INTERVAL = 1000;

using std::this_thread::sleep_for;
using std::chrono::milliseconds;

RFRadio* RFRadio::create(){
    return new RFRadio();
}

RFRadio::RFRadio(){
    gpio = new gnublin_gpio();
    pulseDetectedRecently = false;
    
    pulseFlagLock = new pthread_mutex_t();
    int ret = pthread_mutex_init(pulseFlagLock, NULL);
    if ( ret < 0 ){
        throw new Error("pthread_mutex_init failed.");
    }

    /*
    gpio->pinMode( PIR_INPUT, INPUT );

    if( gpio->digitalRead( PIR_INPUT) ){
    }
    */
    
    // Spin up a thread to monitor the gpio periodically - if something detected
    // set the pulseDetectedRecently to true
    pthread_t pulseMonitoringThread;
    ret = pthread_create(&pulseMonitoringThread, NULL, 
                           RFRadio::monitorForPulses, (void *) this);
                           

    if ( ret < 0 ){
        throw new Error("Pulse monitoring thread failed to get created");
    }
}

RFRadio::~RFRadio(){
    delete gpio;
}

void RFRadio::sendPulse(){
    // TODO
}

bool RFRadio::pulseDetected(){
    bool result;
    int ret = pthread_mutex_lock(pulseFlagLock);
    if( ret < 0){
        throw new Error( "Could not lock pulse detected recently lock!" );
    }
    result = pulseDetectedRecently;
    
    ret = pthread_mutex_unlock(pulseFlagLock);
    if( ret < 0){
        throw new Error( "Could not unlock queue lock!" );
    }
    return result;
}

void* RFRadio::monitorForPulses(void* radio){
    RFRadio* rfRadio = static_cast<RFRadio* >(radio);
    while( true ){
        // TODO
        sleep_for(milliseconds( PULSE_SAMPLING_INTERVAL ));
    }
    return 0;
}






