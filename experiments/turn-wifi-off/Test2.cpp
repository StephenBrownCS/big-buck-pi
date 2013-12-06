#include <cstdlib>
#include <iostream>
#include <chrono>
#include <thread>

using std::this_thread::sleep_for;
using std::chrono::milliseconds;
using namespace std;

int main(){
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
   

    return 0;
}
