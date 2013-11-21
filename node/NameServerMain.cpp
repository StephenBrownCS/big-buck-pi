/*
    1) Listens on port 8888
    2) Expects to receive a UDP Packet with no payload (this will identify the 
        sender)
*/


#include <iostream>
#include <sstream>
#include <map>
#include "ListeningSocket.h"
#include "SendingSocket.h"
#include "Packet.h"
#include "UDPPacket.h"
#include "BigBuckPacket.h"
#include "Utility.h"
#include "HostAndPort.h"

using namespace std;

const int PORT = 8888;

const char PKT_LETTER_REGISTRATION = 'R';
const char PKT_LETTER_ACK = 'A';
const char PKT_LETTER_HEARTBEAT = 'H';
const char PKT_LETTER_MASTER = 'M';

const int DEFAULT_NODE_ID = 0;
const int NO_SEQUENCE = 0;
const int NO_PAYLOAD = 0;

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
    
    HostAndPort self(getOwnIPAddress(), PORT);
    
    // Always listen on port 8888
    ListeningSocket listenSock( PORT );

    map<unsigned short, HostAndPort> sensorNodes;
    unsigned short nextNodeIdToAssign = 0;

    try{    
        //loop to receive and forward packets
        while(1){
            // SEE IF PACKET IS AVAILABLE
            if(listenSock.isPacketWaiting()){               
                UDPPacket* incomingPkt = listenSock.receivePacket(0.1);
                BigBuckPacket* innerPkt = BigBuckPacket::create(incomingPkt->getPayload());
                unsigned short senderId = innerPkt->getSrcNodeId();
                HostAndPort senderHap = incomingPkt->getSrc();
                
                cout << "Packet Received!" << endl;
                
                if (innerPkt->getPacketType() == PKT_LETTER_REGISTRATION){
                    cout << "Registration!" << endl;
                    
                    // Assign it a node Id
                    unsigned short nodeId = nextNodeIdToAssign++;
                    sensorNodes[nodeId] = senderHap;
                    
                    // Respond with node Id
                    SendingSocket sock(senderHap.getIP(), senderHap.getPort());

                    // Send the response and clean up
                    BigBuckPacket* responseInnerPkt = BigBuckPacket::create( 
                        'A', DEFAULT_NODE_ID, nodeId, NO_SEQUENCE, 0, NO_PAYLOAD );
                    Packet* responseOuterPkt = UDPPacket::create(
                        self, senderHap, responseInnerPkt->c_str_length(), responseInnerPkt->c_str());
                    sock.sendPacket(responseOuterPkt);
                    delete responseInnerPkt;
                    delete responseOuterPkt;
                    
                    
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

                    // Contact all of the nodes and inform them of the master's 
                    // location
                    typedef map<unsigned short, HostAndPort>::iterator HapIter;
                    for( HapIter it = sensorNodes.begin(); it != sensorNodes.end(); ++it){
                        unsigned short nodeId = it->first;
                        HostAndPort hap = it->second;
                        SendingSocket sock(hap.getIP(), hap.getPort());
                        
                        // Serialize hap into a string and send it
                        ostringstream os;
                        os << senderHap;
                        string str = os.str();
                        BigBuckPacket* responseInnerPkt = BigBuckPacket::create(
                            PKT_LETTER_MASTER, DEFAULT_NODE_ID, nodeId, NO_SEQUENCE, str.length(), str.c_str());
                        Packet* responseOuterPkt = UDPPacket::create(
                            self, senderHap, responseInnerPkt->c_str_length(), responseInnerPkt->c_str());
                        sock.sendPacket(responseOuterPkt); 
                    }
                }
                
                
            }
        }
    }
    catch(Error & e){
        cout << e.getMsg() << endl;
    }
    listenSock.closeSocket();
    return 0;
}
