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
#include <cstring>
#include <sstream>
#include <sys/types.h>
#include <ifaddrs.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <chrono>
#include <thread>
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
#include "Logger.h"

using namespace std;
using std::this_thread::sleep_for;
using std::chrono::milliseconds;

const int OWN_LISTEN_PORT = 8888;
const char* NAME_SERVER_NAME = "cedar.cs.wisc.edu";

// The hotspot bounces around between two static IP's
const char* NAME_SERVER_HOTSPOT_STATIC_IP_1 = "10.0.0.2";
const char* NAME_SERVER_HOTSPOT_STATIC_IP_2 = "10.0.0.9";

unsigned short registerWithNameServer(HostAndPort & self, HostAndPort & masterHap, Logger & logger, HostAndPort & nameServerHap, unsigned long nameServerIp, unsigned short nameServerPort);
unsigned long getNameServerIP();

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
    string curTime = string(getCurrentHumanReadableTime());
    // Trim off the newline character
    curTime = curTime.substr(0, curTime.length() - 1);
    Logger logger(string("/home/pi/big-buck-node-") + curTime + string(".txt"));
    
    while( true ){
        try{ 
            HostAndPort self(getOwnWlanIpAddress(), OWN_LISTEN_PORT);
            HostAndPort masterHap;

            cout << "Okay" << endl;
            logger << "Own Hap: " << self << "\n";
                        
            // Register with the name server
            unsigned long nameServerIp = getNameServerIP();
            unsigned short nameServerPort = NAME_SERVER_PORT;
            logger << "Name Server IP: " << convertIntToIPAddressString(nameServerIp);
    
            HostAndPort nameServerHap(nameServerIp, nameServerPort);
            logger << nameServerHap;
            
            unsigned int ownNodeId = registerWithNameServer(self, masterHap, logger, nameServerHap, nameServerIp, nameServerPort);
            
            Communicator* communicator = 
                WifiCommunicator::create( 
                    masterHap.getIPAsStr().c_str(), masterHap.getPort(), OWN_LISTEN_PORT
                );
            
            Sensor* sensor = PIRSensor::create();
        
            BigBuckSensingNode* sensingNode = 
                BigBuckSensingNode::create(
                    logger, communicator, sensor, ownNodeId
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
            logger << e.getMsg() << "\n";
        }
        catch(SocketTimeoutException & e){
            logger << "Socket Timeout Exception ";
        }
        catch(MasterResetException & e){
            logger << "Caught MasterResetException";
        }
    
        sleep_for(milliseconds( 10000 ));
        logger << "Retrying" << "\n";
    }
    return 0;
}


unsigned short registerWithNameServer(HostAndPort & self, HostAndPort & masterHap, Logger & logger, HostAndPort & nameServerHap, unsigned long nameServerIp, unsigned short nameServerPort){

    
    SendingSocket sock(nameServerIp, nameServerPort);
    BigBuckPacket* registrationPkt = 
        BigBuckPacket::create(
            'R', DEFAULT_NODE_ID, DEFAULT_NODE_ID, NO_SEQUENCE, NO_PAYLOAD, EMPTY_PAYLOAD
        );
    UDPPacket* outerPkt = UDPPacket::create(
            self, nameServerHap, registrationPkt->c_str_length(), registrationPkt->c_str()
        );
        
    outerPkt->print();

    sock.sendPacket(
        outerPkt
    );
        
    delete registrationPkt;
    delete outerPkt;
    
    logger << "Sent the packet to name server!";
    
    // RECEIVE NODE ID FROM THE REGISTRATION RESPONSE    
    ListeningSocket listenSock( OWN_LISTEN_PORT );
    // Listen for 10 seconds
    outerPkt = listenSock.receivePacket( 10000 );
    BigBuckPacket* innerPkt = BigBuckPacket::create(outerPkt->getPayload());
    
    unsigned int ownNodeId = innerPkt->getDestNodeId();
    logger << "Assigned Node ID: " << ownNodeId;
    
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
            logger << "Master Hap: " << masterHap;
            waitingForMasterPacket = false;
        }
        delete outerPkt;
        delete innerPkt;
    }
    
    listenSock.closeSocket();
    
    return ownNodeId;   
}

unsigned long getNameServerIP(){
    static int counter = 0;
    counter++;

    if( counter % 3 == 0 ){
        return getIPAddressForHostname(NAME_SERVER_NAME);
    }
    else if (counter % 3 == 1){
        return ntohl(ipAddressStrToLong( NAME_SERVER_HOTSPOT_STATIC_IP_1 ));
    }
    else{
        return ntohl(ipAddressStrToLong( NAME_SERVER_HOTSPOT_STATIC_IP_2 ));
    }
}



