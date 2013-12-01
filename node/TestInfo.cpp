/*
    A program for querying the name server for its current state. Sends an info
    request to the name server, and prints out the response.
*/

#include <iostream>
#include <sstream>
#include <arpa/inet.h>
#include "ListeningSocket.h"
#include "SendingSocket.h"
#include "Packet.h"
#include "UDPPacket.h"
#include "BigBuckPacket.h"
#include "Utility.h"
#include "HostAndPort.h"
#include "PacketConstants.h"

using namespace std;

const int OWN_LISTEN_PORT = 9999;
const char* NAME_SERVER_NAME = "cedar.cs.wisc.edu";
const char* NAME_SERVER_HOTSPOT_STATIC_IP = "10.0.0.2";
const bool USING_CEDAR = false;

unsigned long getNameServerIP();

int main(){
    try{    
        HostAndPort self(ntohl(getOwnIPAddress()), OWN_LISTEN_PORT);
        cout << self << endl;
        
        HostAndPort masterHap;

        // Contact the Name Server
        unsigned long nameServerIp = getNameServerIP();
        unsigned short nameServerPort = NAME_SERVER_PORT;
        HostAndPort nameServerHap(nameServerIp, nameServerPort);
    
        cout << "Name Server Hap: " << nameServerHap << endl;

        // Send the Info Request Packet
        SendingSocket sock(nameServerIp, nameServerPort);
        BigBuckPacket* registrationPkt = 
            BigBuckPacket::create(
                'I', DEFAULT_NODE_ID, DEFAULT_NODE_ID, NO_SEQUENCE, NO_PAYLOAD, EMPTY_PAYLOAD
            );
            
        UDPPacket* pkt = UDPPacket::create(
                self, nameServerHap, registrationPkt->c_str_length(), registrationPkt->c_str()
                );
            
        sock.sendPacket(pkt);    
        
        delete registrationPkt;
        delete pkt;
    
        cout << "Sent the packet to name server!\n" << endl;
    
        // RECEIVE NODE ID FROM THE REGISTRATION RESPONSE    
        ListeningSocket listenSock( OWN_LISTEN_PORT );
        UDPPacket* outerPkt = listenSock.receivePacket();
        BigBuckPacket* innerPkt = BigBuckPacket::create(outerPkt->getPayload());
    
        //innerPkt->print();
        const char* listOfNodes = innerPkt->getPayload();
        cout << listOfNodes << endl;
        
        delete outerPkt;
        delete innerPkt;
    }
    catch(Error & e){
        cout << e.getMsg() << endl;
    }
    return 0;
}

unsigned long getNameServerIP(){
    if( USING_CEDAR ){
        return getIPAddressForHostname(NAME_SERVER_NAME);
    }
    else{
        return ntohl(ipAddressStrToLong( NAME_SERVER_HOTSPOT_STATIC_IP ));
    }
}
