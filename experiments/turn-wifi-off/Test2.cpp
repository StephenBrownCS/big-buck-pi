#include <cstdlib>
#include <iostream>
#include <chrono>
#include <thread>

using std::this_thread::sleep_for;
using std::chrono::milliseconds;
using namespace std;

const int MILLISECONDS_TO_WAIT_AFTER_IFCONFIG_FAILURE = 5 * 1000;
const int NUM_TRIES_TO_TURN_ON_WIFI = 10;

int main(){
    bool wifiInterfaceIsUp = false;
    int ret = 0;
    int numTries = 0;
    while ( ! wifiInterfaceIsUp ){
        cout << "ifup wlan0" << endl;
        ret = system("ifup wlan0");
        cout << "... returned " << ret << endl;
        wifiInterfaceIsUp = ret >= 0;
        if ( !wifiInterfaceIsUp ){
            sleep_for(milliseconds( MILLISECONDS_TO_WAIT_AFTER_IFCONFIG_FAILURE ));
        }
        
        numTries++;
        if ( numTries > NUM_TRIES_TO_TURN_ON_WIFI){
            cout << "Unable to turn on wifi driver using ifup" << endl;
        }
    }

    // STEP TWO: TURN OFF WLAN0 INTERFACE TO RESET IT
    numTries = 0;
    ret = -1;
    while (ret < 0){
        cout << "ifconfig wlan0 down";
        ret = system("ifconfig wlan0 down");
        cout << "... returned " << ret << endl;
        if ( ret < 0 ){
            sleep_for(milliseconds( MILLISECONDS_TO_WAIT_AFTER_IFCONFIG_FAILURE ));
        }
        numTries++;
        if ( numTries > NUM_TRIES_TO_TURN_ON_WIFI){
            cout << ("Unable to reset wlan0 using ifconfig wlan0 down") << endl;
        }
    }

    // STEP THREE: TURN ON WLAN0 INTERFACE
    numTries = 0;
    ret = -1;
    while ( ret < 0){
        cout << "ifconfig wlan0 up" << endl;
        ret = system("ifconfig wlan0 up");
        cout << "... returned " << ret << endl;
        if ( ret < 0 ){
            sleep_for(milliseconds( MILLISECONDS_TO_WAIT_AFTER_IFCONFIG_FAILURE ));
        }
        numTries++;
        if ( numTries > NUM_TRIES_TO_TURN_ON_WIFI){
            cout << "Unable to enable wlan0 using ifconfig wlan0 up" << endl;
        }
    }

/*
    cout << "ifup wlan0" << endl;
    
    bool wifiInterfaceIsUp = false;

    int ret = 0;

    while ( ! wifiInterfaceIsUp ){
        cout << "ifup wlan0" << endl;
        ret = system("ifup wlan0");
        cout << "... returned " << ret << endl;
        wifiInterfaceIsUp = ret == 0;
        if ( !wifiInterfaceIsUp )
            sleep_for(milliseconds( 5 * 1000 ));
    }

    // sleep_for(milliseconds(10 * 1000));

    cout << "ifconfig wlan0 down" << endl;
    ret = system("ifconfig wlan0 down");
    
    // sleep_for(milliseconds(5 * 1000));
    cout << "... returned " << ret << endl;
    cout << "ifconfig wlan0 up" << endl;
    ret = system("ifconfig wlan0 up");
    cout << "... returned " << ret << endl;
*/

    return 0;
}
