#include <cstdlib>
#include <iostream>
#include <chrono>
#include <thread>

using std::this_thread::sleep_for;
using std::chrono::milliseconds;
using namespace std;

int main(){

    // Can do this all with one system command - using ifdown, which turns off 
    // the wifi driver
    int ret = -1;
    int numTries = 0;
    while ( ret < 0 ){
        ret = system("ifdown wlan0");
        if ( ret < 0 ){
            cout << "turn off failed" << endl;
            sleep_for(milliseconds( 5 * 1000 ));
        } 
        
        numTries++;
        if ( numTries > NUM_TRIES_TO_TURN_OFF_WIFI){
            throw Error("Unable to turn off wlan0 using ifdown");
        }
    }
    cout << "Successfully shut down wifi" << endl;

    return 0;
}
