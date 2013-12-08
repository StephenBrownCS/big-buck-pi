#define BOARD RASPBERRY_PI
#include "gnublin.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <pthread.h>

using std::this_thread::sleep_for;
using std::chrono::milliseconds;

const int RADIO_INPUT = 23;
const int RADIO_OUTPUT = 24;

void* monitorReceiving(void* foo);

int main()
{
    gnublin_gpio gpio;
    
    gpio.pinMode( RADIO_INPUT, INPUT );
    gpio.pinMode( RADIO_OUTPUT, OUTPUT);

    cout<<"Waiting to settle"<<endl;
    
    pthread_t pulseMonitoringThread;
    int ret = pthread_create(&pulseMonitoringThread, NULL, 
                           monitorReceiving, (void *) &gpio);

    sleep_for(milliseconds(1000000000));
}

void* monitorReceiving(void* foo){
    gnublin_gpio* gpio = static_cast<gnublin_gpio *>(foo);

    int previousState = 0;
    int currentState = 0;

    int consecutiveZeroes = 0;

    while( 1 ){
        currentState = gpio->analogRead( RADIO_INPUT );
        if( currentState < 0 ){
            cout << "Digital Read failure" << endl;
        }
        else{
            // cout << currentState << endl;
            if ( currentState == 0 ){
                consecutiveZeroes++;
            }
            else{
                cout << "Consecutive Zeroes: " << consecutiveZeroes << endl;
                consecutiveZeroes = 0;
            }
        }
        
	    previousState = currentState;
    }
}


void* monitorReceivingWithBuffer(void* foo){
    gnublin_gpio* gpio = static_cast<gnublin_gpio *>(foo);

    int previousState = 0;
    int currentState = 0;

    int BUFFER_SIZE = 100;
    bool historyBuffer[BUFFER_SIZE];
    for(int i = 0; i < BUFFER_SIZE; i++){
        historyBuffer[i] = 0;
    }
    int nextSpot = 0;

    while( 1 ){
        currentState = gpio->analogRead( RADIO_INPUT );
        if( currentState < 0 ){
            cout << "Digital Read failure" << endl;
        }
        else{
            // cout << currentState << endl;
            historyBuffer[nextSpot] = currentState;
            nextSpot = (nextSpot + 1) % BUFFER_SIZE;
        }
        
        int numZeroes = 0;
        for(int i = 0; i < BUFFER_SIZE; i++){
            if ( historyBuffer[i] ){
                numZeroes++;
            }
        }
        printf("Proportion of Zeroes: %d", numZeroes);
        
	    previousState = currentState;
    }
}




