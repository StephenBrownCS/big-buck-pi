#define BOARD RASPBERRY_PI
#include "gnublin.h"
#include "SendingSocket.h"
#include "UDP_Packet.h"
#include <iostream>
#include <chrono>
#include <thread>

using std::this_thread::sleep_for;
using std::chrono::milliseconds;

const int PIR_INPUT = 7;

int senseThings( SendingSocket & sendSock )
{
    gnublin_gpio gpio;
    
    gpio.pinMode( PIR_INPUT, INPUT );

    cout<<"Waiting to settle"<<endl;
    while( gpio.digitalRead( PIR_INPUT) ){
       sleep(1);
    }
    cout<<"Ready"<<endl;

    int previousState = 0;
    int currentState = 0;

    while( 1 ){
        currentState = gpio.digitalRead( PIR_INPUT );
        if( currentState == 1 && previousState == 0){
            cout<<"Motion!"<<endl;
            UDP_Packet pkt('A', 1, 0, 0);
            sendSock.send(pkt);
        }
        else if ( currentState == 0 && previousState == 1){
            cout<<"None!"<<endl;
        }
	previousState = currentState;
        sleep_for(milliseconds(100));
    }
}