/*
    1) Listens on port 8888
    2) Expects to receive a UDP Packet with no payload (this will identify the 
        sender)
*/


#include <iostream>
#include <sstream>
#include <map>
#include <cstring>
#include <unistd.h>
#include "ListeningSocket.h"
#include "SendingSocket.h"
#include "Packet.h"
#include "UDPPacket.h"
#include "BigBuckPacket.h"
#include "Utility.h"
#include "HostAndPort.h"
#include "PacketConstants.h"

using namespace std;

const int POLLING_RATE_IN_SECONDS = 0.2; // so we don't do a busy wait

// Sends the Host and Port of the Master to the node with the specified id
void sendMasterHapToNode( HostAndPort & self, HostAndPort & destHap, HostAndPort & masterHap, unsigned short nodeId);

// Acquires the node id for the specified hap. If it is already known, returns 
// the current id that is stored. Otherwise, assigns it the next available id
// and returns it.
int acquireNodeId( unsigned short & nextNodeIdToAssign, 
    map<HostAndPort, unsigned short> & sensorNodes, 
    const HostAndPort & hap );


int main(int argc, char** argv){
    try{    
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
    
        // This is kind of hacky - but we identify if we're a dns-finable host (such as 
        // cedar.cs.wisc.edu) by asking getOwnHostName(). Otherwise, fall back to 
        // our wlan0 address.
        unsigned long ownIPAddress = 0;
        if (getOwnHostName() == "cedar"){
            ownIPAddress = htonl(getOwnIPAddress());
        }
        else{
            ownIPAddress = getOwnWlanIpAddress();
        }
        HostAndPort self(ownIPAddress, NAME_SERVER_PORT);
        HostAndPort masterHap;
        bool masterIsKnown = false;
    
        cout << "Own Hap: " << self << endl;
    
        // Always listen on port 8888
        ListeningSocket listenSock( NAME_SERVER_PORT );

        map<HostAndPort, unsigned short> sensorNodes;
        unsigned short nextNodeIdToAssign = 1;
    
    
        //loop to receive and forward packets
        while(1){
            // SEE IF PACKET IS AVAILABLE
            if(listenSock.isPacketWaiting()){               
                UDPPacket* incomingPkt = listenSock.receivePacket(0.1);
                incomingPkt->print();
                
                BigBuckPacket* innerPkt = BigBuckPacket::create(incomingPkt->getPayload());

                unsigned short senderId = innerPkt->getSrcNodeId();
                HostAndPort senderHap = incomingPkt->getSrc();
                
                cout << "Packet Received!" << endl;
                
                if (innerPkt->getPacketType() == PKT_LETTER_REGISTRATION){
                    cout << "Registration!" << endl;
                    
                    unsigned short nodeId = acquireNodeId( nextNodeIdToAssign, sensorNodes, senderHap);
                    cout << "Node Id is " << nodeId << endl;
                    
                    // Respond with node Id
                    SendingSocket sock(htonl(senderHap.getIP()), senderHap.getPort());
                    cout << "Sending response to " << senderHap << endl;

                    // Send the response and clean up
                    BigBuckPacket* responseInnerPkt = BigBuckPacket::create( 
                        'A', DEFAULT_NODE_ID, nodeId, NO_SEQUENCE, NO_PAYLOAD, EMPTY_PAYLOAD );
                    Packet* responseOuterPkt = UDPPacket::create(
                        self, senderHap, responseInnerPkt->c_str_length(), responseInnerPkt->c_str());
                        
                    sock.sendPacket(responseOuterPkt);
                    delete responseInnerPkt;
                    delete responseOuterPkt;
                    
                    if ( masterIsKnown ){
                        sendMasterHapToNode( self, senderHap, masterHap, nodeId);
                    } 
                }
                else if(innerPkt->getPacketType() == PKT_LETTER_HEARTBEAT){
                    cout << "Heartbeat!" << endl;
                    cout << "\tNode Id: " << senderId << endl;
                    cout << "\tNode Hap: " << senderHap << endl;
                }
                else if(innerPkt->getPacketType() == PKT_LETTER_MASTER){
                    cout << "Master Registration!" << endl;
                    cout << "\tNode Id: " << senderId << endl;
                    cout << "\tNode Hap: " << senderHap << endl;
                    
                    masterHap = senderHap;
                    masterIsKnown = true;

                    // Contact all of the nodes and inform them of the master's 
                    // location
                    typedef map<HostAndPort, unsigned short>::iterator HapIter;
                    for( HapIter it = sensorNodes.begin(); it != sensorNodes.end(); ++it){
                        unsigned short nodeId = it->second;
                        HostAndPort hap = it->first;
                        
                        sendMasterHapToNode( self, hap, senderHap, nodeId );
                    }
                    
                    // Send ACK to master
                    SendingSocket sock(htonl(senderHap.getIP()), senderHap.getPort());
                    BigBuckPacket* responseInnerPkt = BigBuckPacket::create( 
                        'A', DEFAULT_NODE_ID, DEFAULT_NODE_ID, NO_SEQUENCE, NO_PAYLOAD, EMPTY_PAYLOAD );
                    Packet* responseOuterPkt = UDPPacket::create(
                        self, senderHap, responseInnerPkt->c_str_length(), responseInnerPkt->c_str());
                    sock.sendPacket(responseOuterPkt);
                }
                else if(innerPkt->getPacketType() == PKT_LETTER_INFO){
                    cout << "Info Request!" << endl;
                    cout << "\tNode Id: " << senderId << endl;
                    cout << "\tNode Hap: " << senderHap << endl;
                    
                    // Construct payload full of our own Hap, as well as
                    // the Haps of all the other nodes
                    ostringstream oss;
                    oss << "Own Hap: " << self << endl;
                    typedef map<HostAndPort, unsigned short>::iterator HapIter;
                    for( HapIter it = sensorNodes.begin(); it != sensorNodes.end(); ++it){
                        unsigned short nodeId = it->second;
                        HostAndPort hap = it->first;
                        
                        oss << "Node: " << nodeId << " " << hap << endl;
                    }
                    
                    const char* payload = oss.str().c_str();
                    
                    SendingSocket sock(htonl(senderHap.getIP()), senderHap.getPort());
                    BigBuckPacket* responseInnerPkt = BigBuckPacket::create( 
                        'A', DEFAULT_NODE_ID, DEFAULT_NODE_ID, NO_SEQUENCE, strlen(payload) + 1, payload );
                    Packet* responseOuterPkt = UDPPacket::create(
                        self, senderHap, responseInnerPkt->c_str_length(), responseInnerPkt->c_str());
                    sock.sendPacket(responseOuterPkt);
                }
            }
        
        usleep(static_cast<int>(POLLING_RATE_IN_SECONDS * 1000000));
        }
    }
    catch(Error & e){
        cout << e.getMsg() << endl;
        //listenSock.closeSocket();
    }
    return 0;
}

void sendMasterHapToNode( HostAndPort & self, HostAndPort & destHap, HostAndPort & masterHap, unsigned short nodeId){
    
    SendingSocket sock(htonl(destHap.getIP()), destHap.getPort());
    
    // Serialize hap into a string and send it
    ostringstream os;
    os << masterHap;
    string str = os.str();
    
    BigBuckPacket* responseInnerPkt = BigBuckPacket::create(
        PKT_LETTER_MASTER, DEFAULT_NODE_ID, nodeId, NO_SEQUENCE, str.length(), str.c_str());
    Packet* responseOuterPkt = UDPPacket::create(
        self, destHap, responseInnerPkt->c_str_length(), responseInnerPkt->c_str());
    sock.sendPacket(responseOuterPkt); 
}

int acquireNodeId( unsigned short & nextNodeIdToAssign, 
    map<HostAndPort, unsigned short> & sensorNodes, 
    const HostAndPort & hap )
    { 
    
    typedef map<HostAndPort, unsigned short>::iterator HapIter;
    HapIter iter = sensorNodes.find(hap);
    
    if ( iter == sensorNodes.end() ){
        unsigned short nodeId = nextNodeIdToAssign++;
        sensorNodes[hap] = nodeId;
        return nodeId;
    }
    else{
        return iter->second;
    }
}

