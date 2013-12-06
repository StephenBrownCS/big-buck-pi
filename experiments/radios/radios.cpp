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

    if ( ret < 0 ){
        cout << "pthread failed" << endl;
    }        

                   
    while( 1 ){
        // cout << "Writing 1" << endl;
        //ret = gpio.digitalWrite( RADIO_OUTPUT, 1 );
        if ( ret < 0 ){
            cout << "Digital Write failure" << endl;
        }
        sleep_for(milliseconds(1000000000));
     }
     while ( true ){
           sleep_for(milliseconds(10000));
     }
}

void* monitorReceiving(void* foo){
    gnublin_gpio* gpio = static_cast<gnublin_gpio *>(foo);

    int previousState = 0;
    int currentState = 0;

    int numOnes = 0;
    int numZeroes = 0;

    while( 1 ){
        currentState = gpio->digitalRead( RADIO_INPUT );
        cout << currentState << " " << numOnes << ", " << numZeroes << endl;

        if ( currentState == 1){
                numOnes++;
        }
        else if (currentState == 0){
             numZeroes++;
        }

        //if( currentState == 1 && previousState == 0){
        //    cout<<"Motion!"<<endl;
        //}
        //else if ( currentState == 0 && previousState == 1){
        //    cout<<"None!"<<endl;
        //}
        
        if( currentState < 0 ){
            cout << "Digital Read failure" << endl;
        }
	previousState = currentState;
        sleep_for(milliseconds(100));
    }
}




