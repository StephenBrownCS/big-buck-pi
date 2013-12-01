#include <iostream>
#include <cstring>
#include <sstream>
#include <chrono>
#include <thread>
#include "BigBuckSensingNode.h"
#include "BigBuckPacket.h"
#include "Sensor.h"
#include "Communicator.h"
#include "Packet.h"
#include "UDPPacket.h"
#include "PacketConstants.h"
#include "Timer.h"

using namespace std;

using std::this_thread::sleep_for;
using std::chrono::milliseconds;

const int BASE_STATION_ID = 7777;
const int POLLING_RATE = 100; //milliseconds
const int HELLO_PACKET_TIMEOUT_IN_MILLISECONDS = 15000; // 15 seconds

BigBuckSensingNode* BigBuckSensingNode::create( Logger& logger_, Communicator* communicator_, Sensor* sensor_, short id_ ){
    return new BigBuckSensingNode( logger_, communicator_, sensor_, id_ );
}

BigBuckSensingNode::BigBuckSensingNode(Logger& logger_, Communicator* communicator_, Sensor* sensor_, short id_):
    logger(logger_),
    communicator( communicator_ ),
    sensor( sensor_ ),
    id( id_),
    nextSequence(0)
{
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
    
        currentState = sensor->getCurrentState() * 100;
        if ( currentState != previousState ){
            logger << "New State: " << currentState;
            sendSensorState( currentState );
            
            // Reset the timer, since hello packet is unnecessary if we send sensor state
            helloPacketTimeoutTimer.startCountdown( HELLO_PACKET_TIMEOUT_IN_MILLISECONDS );
        }
        
        if (helloPacketTimeoutTimer.hasExpired()){
            logger << "Hello timer has expired, sending current state again.";
            sendSensorState( currentState );
            helloPacketTimeoutTimer.startCountdown( HELLO_PACKET_TIMEOUT_IN_MILLISECONDS );
        }
        
        previousState = currentState;
        sleep_for(milliseconds( POLLING_RATE ));
    }
}

void BigBuckSensingNode::sendSensorState( int currentState ){
    ostringstream convert;
    convert << currentState;
    const char* currentStateStr = convert.str().c_str();
    BigBuckPacket* pkt = BigBuckPacket::create( 'S', id, BASE_STATION_ID, nextSequence++, strlen(currentStateStr), currentStateStr );
    communicator->sendPacket( pkt );
    delete pkt;
}

void BigBuckSensingNode::sendHelloPacket(){
    BigBuckPacket* pkt = BigBuckPacket::create( 'H', id, BASE_STATION_ID, nextSequence++, NO_PAYLOAD, EMPTY_PAYLOAD );
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

