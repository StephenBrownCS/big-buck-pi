/*
    Main Driver File for What is Run on Each Raspberry Pi Sensor
    
    This program runs the logic for each sensing node.
    Startup Protocol:
        1) Sends Registration Request to Name Server (location is known)
        2) Name Server responds with ACK which contains this node's Id assignment
        3) This node waits for a Master Registration request (which can come 
            from either the name server or the master node itself)
        4) Begin sensing and sending data
*/


#include <iostream>
#include <sstream>
#include "SendingSocket.h"
#include "Packet.h"
#include "UDPPacket.h"
#include "BigBuckPacket.h"
#include "Utility.h"
#include "HostAndPort.h"
#include "PIRSensor.h"
#include "WifiCommunicator.h"
#include "BigBuckSensingNode.h"
#include "ListeningSocket.h"
#include "PacketConstants.h"

using namespace std;

const int OWN_LISTEN_PORT = 8888;
const char* NAME_SERVER_NAME = "cedar.cs.wisc.edu";

unsigned short registerWithNameServer(HostAndPort & self, HostAndPort & masterHap );

int main(int argc, char** argv){
    const char* destIpAddress = 0;
    unsigned short destPort = 0;
    /*
    if ( argc != 3 ){
        cout << "Usage: Main <Dot-Separated IP Address> <port>" << endl;
        exit( 0 );
    }
    else{
        destIpAddress = argv[1];
        destPort = atoi(argv[2]);
    }
    */
    
    HostAndPort self(getOwnIPAddress(), OWN_LISTEN_PORT);
    HostAndPort masterHap;
    
    unsigned int ownNodeId = registerWithNameServer(self, masterHap);
    
    try{    
        Communicator* communicator = 
            WifiCommunicator::create( 
                masterHap.getIPAsStr().c_str(), masterHap.getPort(), OWN_LISTEN_PORT
            );
            
        Sensor* sensor = PIRSensor::create();
        
        BigBuckSensingNode* sensingNode = 
            BigBuckSensingNode::create(
                communicator, sensor, ownNodeId
            );
        
        // SEND HELLO TO THE MASTER
        communicator->sendPacket(
            BigBuckPacket::create(
                'H', ownNodeId, DEFAULT_NODE_ID, NO_SEQUENCE, NO_PAYLOAD, EMPTY_PAYLOAD
            )
        );
        
        sensingNode->sensingLoop();
        
        delete sensingNode;
    }
    catch(Error & e){
        cout << e.getMsg() << endl;
    }
    return 0;
}


unsigned short registerWithNameServer(HostAndPort & self, HostAndPort & masterHap){
    // Register with the name server
    unsigned long nameServerIp = ntohl(getIPAddressForHostname(NAME_SERVER_NAME));
    unsigned short nameServerPort = NAME_SERVER_PORT;
    
    cout << "Name Server IP: " << convertIntToIPAddressString(nameServerIp) << endl;
    
    HostAndPort nameServerHap(nameServerIp, nameServerPort);
    
    SendingSocket sock(nameServerIp, nameServerPort);
    BigBuckPacket* registrationPkt = 
        BigBuckPacket::create(
            'R', DEFAULT_NODE_ID, DEFAULT_NODE_ID, NO_SEQUENCE, NO_PAYLOAD, EMPTY_PAYLOAD
        );
    sock.sendPacket(
        UDPPacket::create(
            self, nameServerHap, registrationPkt->c_str_length(), registrationPkt->c_str()
            )
        );    
    
    cout << "Sent the packet to name server!" << endl;
    
    // RECEIVE NODE ID FROM THE REGISTRATION RESPONSE    
    ListeningSocket listenSock( OWN_LISTEN_PORT );
    UDPPacket* outerPkt = listenSock.receivePacket();
    BigBuckPacket* innerPkt = BigBuckPacket::create(outerPkt->getPayload());
    
    unsigned int ownNodeId = innerPkt->getDestNodeId();
    cout << "Assigned Node ID: " << ownNodeId << endl;
    
    delete outerPkt;
    delete innerPkt;
    
    // WAIT UNTIL WE GET A MASTER REGISTRATION PACKET 
    bool waitingForMasterPacket = true;
    while( waitingForMasterPacket ){
        UDPPacket* outerPkt = listenSock.receivePacket();
        BigBuckPacket* innerPkt = BigBuckPacket::create( outerPkt->getPayload() );
        if( innerPkt->getPacketType() == PKT_LETTER_MASTER){
            istringstream iss(innerPkt->getPayload());
            iss >> masterHap;
            cout << "Master Hap: " << masterHap << endl;
            waitingForMasterPacket = false;
        }
        delete outerPkt;
        delete innerPkt;
    }
    
    listenSock.closeSocket();
    
    return ownNodeId;   
}

