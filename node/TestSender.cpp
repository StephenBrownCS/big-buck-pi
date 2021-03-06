#include <iostream>
#include <sstream>
#include <arpa/inet.h>
#include "ListeningSocket.h"
/*
    A program which poses as a sensing node - used for testing the name server.
*/


#include "SendingSocket.h"


#include "Packet.h"
#include "UDPPacket.h"
#include "BigBuckPacket.h"
#include "Utility.h"
#include "HostAndPort.h"
#include "PacketConstants.h"

using namespace std;

const int OWN_LISTEN_PORT = 4444;
const char* NAME_SERVER_NAME = "cedar.cs.wisc.edu";

unsigned short registerWithNameServer(HostAndPort & self, HostAndPort & masterHap );

int main(){
    try{    
        // For testing name server
        HostAndPort self(ntohl(getOwnIPAddress()), OWN_LISTEN_PORT);
        cout << self << endl;
        
        HostAndPort masterHap;
        
        registerWithNameServer(self, masterHap);
    }
    catch(Error & e){
        cout << e.getMsg() << endl;
    }
    return 0;
}

unsigned short registerWithNameServer(HostAndPort & self, HostAndPort & masterHap){
    // Register with the name server
    unsigned long nameServerIp = getIPAddressForHostname(NAME_SERVER_NAME);
    unsigned short nameServerPort = NAME_SERVER_PORT;
    
    HostAndPort nameServerHap(nameServerIp, nameServerPort);
    
    cout << "Name Server Hap: " << nameServerHap << endl;

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
    
    innerPkt->print();
    
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
        
    return ownNodeId;   
}
