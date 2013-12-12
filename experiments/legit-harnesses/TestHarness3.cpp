// Experiment to detect a zero if it happens

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

int main()
{
    wiringPiSetup();

    pinMode( RADIO_INPUT, INPUT );
   
    while( true ) {
	int a = analogRead( RADIO_INPUT );
        if ( a == 0){
             cout << "ZERO" << endl;
        }
        sleep_for( milliseconds( SLEEP_INTERVAL ) );
     }
}
