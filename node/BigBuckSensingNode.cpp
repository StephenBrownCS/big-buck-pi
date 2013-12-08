#include <iostream>
#include <cstring>
#include <sstream>
#include <chrono> // for milliseconds()
#include <thread> // for sleep_for()
#include <cstdlib> // for system()
#include "BigBuckSensingNode.h"
#include "BigBuckPacket.h"
#include "Sensor.h"
#include "Communicator.h"
#include "Packet.h"
#include "UDPPacket.h"
#include "PacketConstants.h"
#include "Timer.h"
#include "RFRadio.h"

#define USING_RADIOS
//#define RUNNING_HOTSPOT

using namespace std;

using std::this_thread::sleep_for;
using std::chrono::milliseconds;

const int BASE_STATION_ID = 7777;
const int POLLING_RATE = 100; //milliseconds
const int MILLISECONDS_TO_WAIT_AFTER_POWERING_UP_WIFI = 1000 * 60; // 1 minute
const int HELLO_PACKET_TIMEOUT_IN_MILLISECONDS = 15000; // 15 seconds
const int WIFI_RADIO_POWERDOWN_TIMEOUT_IN_MILLISECONDS = 60000 * 1; // 1 minute
const int WIFI_RADIO_POWERUP_TIMEOUT_IN_MILLISECONDS = 60000 * 5; //5 minutes
const int MILLISECONDS_TO_WAIT_AFTER_IFCONFIG_FAILURE = 5 * 1000; // 5 seconds
const int NUM_TRIES_TO_TURN_ON_WIFI = 10;
const int NUM_TRIES_TO_TURN_OFF_WIFI = 10;

BigBuckSensingNode* BigBuckSensingNode::create( Logger& logger_, Communicator* communicator_, Sensor* sensor_, short id_ ){
    return new BigBuckSensingNode( logger_, communicator_, sensor_, id_ );
}

BigBuckSensingNode::BigBuckSensingNode(Logger& logger_, Communicator* communicator_, Sensor* sensor_, short id_):
    logger(logger_),
    communicator( communicator_ ),
    sensor( sensor_ ),
    id( id_),
    nextSequence(0),
    wifiRadioIsOn(true)
{
    rfRadio = RFRadio::create();
}

BigBuckSensingNode::~BigBuckSensingNode(){
    delete communicator;
    delete sensor;
}

void BigBuckSensingNode::sensingLoop(){
    int currentState = 0;
    int previousState = 0;
    
    Timer helloPacketTimeoutTimer;
    helloPacketTimeoutTimer.startCountdown( HELLO_PACKET_TIMEOUT_IN_MILLISECONDS );
    Timer wifiRadioPowerDownTimer;
    wifiRadioPowerDownTimer.startCountdown( WIFI_RADIO_POWERDOWN_TIMEOUT_IN_MILLISECONDS );
    Timer wifiRadioPowerUpTimer;
    while( true ){
        if (communicator->isPacketWaiting()){
            logger << "Packet Received!";
            Packet* pkt = communicator->receivePacket();
            UDPPacket* udpPkt = dynamic_cast<UDPPacket *>( pkt );
            if ( udpPkt ){
                handleReceivedPacket(udpPkt);
            }
            delete pkt;
        }
        
        if( rfRadio->pulseDetected() ){
            rfRadio->sendPulse();
            if ( !wifiRadioIsOn ){
                turnWifiRadioOn();
            }
        }
    
        currentState = sensor->getCurrentState() * 100;
        if ( currentState != previousState ){
            logger << "New State: " << currentState;
            sendSensorState( currentState );
            
            // Reset the timer, since hello packet is unnecessary if we send sensor state
            helloPacketTimeoutTimer.startCountdown( HELLO_PACKET_TIMEOUT_IN_MILLISECONDS );
            wifiRadioPowerDownTimer.startCountdown( WIFI_RADIO_POWERDOWN_TIMEOUT_IN_MILLISECONDS );
        }
        
        if ( wifiRadioIsOn && helloPacketTimeoutTimer.hasExpired()){
            logger << "Hello timer has expired, sending current state again.";
            sendSensorState( currentState );
            helloPacketTimeoutTimer.startCountdown( HELLO_PACKET_TIMEOUT_IN_MILLISECONDS );
        }
        
        // if no activity in the last minute or so, power down the Wifi radio
        #ifdef USING_RADIOS
        if (wifiRadioIsOn && wifiRadioPowerDownTimer.hasExpired()){
            logger << "Wifi Powerdown timer has expired, powering down wifi.";
            turnWifiRadioOff();
            wifiRadioPowerUpTimer.startCountdown( WIFI_RADIO_POWERUP_TIMEOUT_IN_MILLISECONDS  );
        }

        if (! wifiRadioIsOn && wifiRadioPowerUpTimer.hasExpired()){
            turnWifiRadioOn();
            wifiRadioPowerDownTimer.startCountdown( WIFI_RADIO_POWERDOWN_TIMEOUT_IN_MILLISECONDS  );
        }
        #endif
        
        previousState = currentState;
        sleep_for(milliseconds( POLLING_RATE ));
    }
}


void BigBuckSensingNode::sendSensorState( int currentState ){

    if ( currentState ){
        rfRadio->sendPulse();
    }

    if ( ! wifiRadioIsOn ){
        // turn on wifi radio and wait, because it might take a while to 
        // actually turn on
        turnWifiRadioOn();
    }
    
    ostringstream convert;
    convert << currentState;
    const char* currentStateStr = convert.str().c_str();
    BigBuckPacket* pkt = 
        BigBuckPacket::create( 'S', id, BASE_STATION_ID, nextSequence++, 
                                strlen(currentStateStr), currentStateStr );
    communicator->sendPacket( pkt );
    delete pkt;
}

void BigBuckSensingNode::sendHelloPacket(){
    BigBuckPacket* pkt = 
        BigBuckPacket::create( 'H', id, BASE_STATION_ID, nextSequence++, 
                                NO_PAYLOAD, EMPTY_PAYLOAD );
    communicator->sendPacket( pkt );
    delete pkt;
}

void BigBuckSensingNode::handleReceivedPacket(UDPPacket* udpPkt){
    // The main thing we're checking for are packets from the name server 
    // saying that there's a new master in town
    BigBuckPacket* bigBuckPkt = BigBuckPacket::create(udpPkt->getPayload());
    if(bigBuckPkt->getPacketType() == PKT_LETTER_MASTER){
        logger << "Reset the Master!";
        delete bigBuckPkt;
        throw MasterResetException();
    }
    delete bigBuckPkt;    
}

void BigBuckSensingNode::turnWifiRadioOn(){
    // To make this work, we need to supply 3 system commands
    // turn on wifi driver, turn off wlan0 to reset it, turn on wlan0
    // We found that simply turning on the wifi driver is insufficient
    
    // STEP ONE: TURN ON WIFI DRIVER
    // This step is usally the most troublesome, and tends to need more tries 
    // than the others, which almost never fail
    bool wifiInterfaceIsUp = false;
    int ret = 0;
    int numTries = 0;
    while ( ! wifiInterfaceIsUp ){
        logger << "ifup wlan0";
        ret = system("ifup wlan0");
        logger << "... returned " << ret;
        wifiInterfaceIsUp = ret >= 0;
        if ( !wifiInterfaceIsUp ){
            sleep_for(milliseconds( MILLISECONDS_TO_WAIT_AFTER_IFCONFIG_FAILURE ));
        }
        
        numTries++;
        if ( numTries > NUM_TRIES_TO_TURN_ON_WIFI){
            throw Error("Unable to turn on wifi driver using ifup");
        }
    }

    // STEP TWO: TURN OFF WLAN0 INTERFACE TO RESET IT
    numTries = 0;
    ret = -1;
    while (ret < 0){
        logger << "ifconfig wlan0 down";
        ret = system("ifconfig wlan0 down");
        logger << "... returned " << ret;
        if ( ret < 0 ){
            sleep_for(milliseconds( MILLISECONDS_TO_WAIT_AFTER_IFCONFIG_FAILURE ));
        }
        numTries++;
        if ( numTries > NUM_TRIES_TO_TURN_ON_WIFI){
            throw Error("Unable to reset wlan0 using ifconfig wlan0 down");
        }
    }

    // STEP THREE: TURN ON WLAN0 INTERFACE
    numTries = 0;
    ret = -1;
    while ( ret < 0){
        logger << "ifconfig wlan0 up";
        ret = system("ifconfig wlan0 up");
        logger << "... returned " << ret;
        if ( ret < 0 ){
            sleep_for(milliseconds( MILLISECONDS_TO_WAIT_AFTER_IFCONFIG_FAILURE ));
        }
        numTries++;
        if ( numTries > NUM_TRIES_TO_TURN_ON_WIFI){
            throw Error("Unable to enable wlan0 using ifconfig wlan0 up");
        }
    }

    #ifdef RUNNING_HOTSPOT
    ret = system("dhcpd wlan0");
    if ( ret < 0 ){
         throw Error("Could not start dhcpd");
    }

    ret = system("hostapd /home/pi/hostap.conf");
    if ( ret < 0 ){
        throw Error("Could not start hostapd");
    }
    #endif
    
    sleep_for(milliseconds( MILLISECONDS_TO_WAIT_AFTER_POWERING_UP_WIFI));
    wifiRadioIsOn = true;
}

void BigBuckSensingNode::turnWifiRadioOff(){
    // Can do this all with one system command - using ifdown, which turns off 
    // the wifi driver
    int ret = 0;
    #ifdef RUNNING_HOTSPOT
    ret = system("pkill dhcpd");
    if ( ret < 0 ){
        throw Error("killing dhcpd failed");
    }
    ret = system("pkill hostapd");
    if ( ret < 0 ){
        throw Error("killing hostapd failed");
    }
    #endif

    ret = -1;
    int numTries = 0;
    while ( ret < 0 ){
        ret = system("ifdown wlan0");
        if ( ret < 0 ){
            logger << "turn off failed";
            sleep_for(milliseconds( 5 * 1000 ));
        } 
        
        numTries++;
        if ( numTries > NUM_TRIES_TO_TURN_OFF_WIFI){
            throw Error("Unable to turn off wlan0 using ifdown");
        }
    }
    logger << "Successfully shut down wifi";

    wifiRadioIsOn = false;
}
