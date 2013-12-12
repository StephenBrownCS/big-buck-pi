
// Experiment to see what analog read we see
// from the input pin

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

    for(int i = 0; i < 10; i++) {
	int a = analogRead( RADIO_INPUT );
        cout << a << endl;
     }
}
