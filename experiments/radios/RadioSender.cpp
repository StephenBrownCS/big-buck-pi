#define BOARD RASPBERRY_PI
#include "gnublin.h"
#include "Timer.h"
#include <iostream>
#include <chrono>
#include <thread>

using std::this_thread::sleep_for;
using std::chrono::milliseconds;

const int RADIO_INPUT = 23;
const int RADIO_OUTPUT = 24;

int main()
{
    gnublin_gpio gpio;
    
    gpio.pinMode( RADIO_INPUT, INPUT );
    gpio.pinMode( RADIO_OUTPUT, OUTPUT);

    cout<<"Sending RF Signals"<<endl;

    Timer flipTimer;
    flipTimer.startCountdown( 1000 * 5 );
   
   ///gpio.digitalWrite( RADIO_OUTPUT, 1 );


    int ret = 0;               
    int stateToWrite = 0;  
    while( 1 ){
        ret = gpio.digitalWrite( RADIO_OUTPUT, stateToWrite );
        if ( ret < 0 ){
            cout << "Digital Write failure" << endl;
        }
        sleep_for(milliseconds(1000 * 10 ));
        
        //if ( flipTimer.hasExpired() ){
            stateToWrite = stateToWrite ? 0 : 1;
            cout << "Toggling to " << stateToWrite << endl;
            //flipTimer.startCountdown( 1000 * 5 );
        //}
     }
     
     while ( true ){
           sleep_for(milliseconds(10000));
     }
}
