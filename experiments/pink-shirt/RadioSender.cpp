#include <wiringPi.h>
//#include "Timer.h"
#include <iostream>
#include <chrono>
#include <thread>

using std::this_thread::sleep_for;
using std::chrono::milliseconds;
using namespace std;

const int RADIO_INPUT = 23;
const int RADIO_OUTPUT = 24;

int main()
{
    wiringPiSetup();

    pinMode( RADIO_INPUT, INPUT );

    cout<<"Sending RF Signals"<<endl;

    //Timer flipTimer;
    //flipTimer.startCountdown( 1000 * 5 );
   
   ///gpio.digitalWrite( RADIO_OUTPUT, 1 );

    int ret = 0;
    int stateToWrite = 0;  
    {
        // digitalWrite( 0, LOW );
        
	int a = analogRead( RADIO_INPUT );
        cout << a << endl;
//if ( flipTimer.hasExpired() )
            //flipTimer.startCountdown( 1000 * 5 );
        //}
     }
    
     /* 
     while ( true ){
           sleep_for(milliseconds(10000));
     }
     */
}
