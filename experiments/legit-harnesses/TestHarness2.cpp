// Experiment to detect a run of consecutive 1's

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

const int CONSECUTIVE_ONES_THRESHOLD = 12;
const int SLEEP_INTERVAL = 25; // milliseconds

int main()
{
    wiringPiSetup();

    pinMode( RADIO_INPUT, INPUT );

    int consecutiveOnes = 0;
   
    while( true ) {
	int a = analogRead( RADIO_INPUT );
        if ( a == 5){
             consecutiveOnes++;
        }
        else{
             if ( consecutiveOnes > CONSECUTIVE_ONES_THRESHOLD ){
                  cout << "PULSE DETECTED" << consecutiveOnes << endl;
             }
             consecutiveOnes = 0;
        }
        sleep_for( milliseconds( SLEEP_INTERVAL ) );
     }
}
