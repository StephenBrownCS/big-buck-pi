// Experiment for determining the current rate

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

const int SLEEP_INTERVAL = 25; // milliseconds
const int NUM_SAMPLES_PER_SECOND = 1000 / SLEEP_INTERVAL;

int main()
{
    wiringPiSetup();

    pinMode( RADIO_INPUT, INPUT );

    int previous = 0;
    int counter = 0;
    int numTransitions = 0;
    while( true ) {
	int current = analogRead( RADIO_INPUT );
        if ( previous != current ){
            numTransitions++;
        }
        previous = current;

        counter++;
        // Check if a full second has elapsed
        if ( counter >= NUM_SAMPLES_PER_SECOND ){
             counter = 0;
             cout << "Num Transitions per s: " << numTransitions << endl;
             numTransitions = 0;
        }

        sleep_for( milliseconds( SLEEP_INTERVAL ) );
     }
}
