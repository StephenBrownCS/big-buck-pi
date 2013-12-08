/*
    A Communicator subclass taylor-made for sending packets over Wifi
    Encapsulates a sending socket and a listening socket, which are completely
    managed internally by this class. To use this, simply create it, and send/receive
    packets.
    
    Encapsulates all packets sent through it with a UDP Packet containing 
    src/dest IPs and ports. 
    
    All received packets are NOT decapsulated - we do not want to couple an 
    implementation with the specific packet types.
*/


#ifndef WIFI_COMMUNICATOR_H_
#define WIFI_COMMUNICATOR_H_

#include "Communicator.h"
#include "HostAndPort.h"

// Forward References
class SendingSocket;
class ListeningSocket;
class Packet;
class UDPPacket;

class WifiCommunicator : public Communicator{
public:
    static WifiCommunicator* create( const char* baseIPAddress_, const int basePort_, const int ownPort_ );
    ~WifiCommunicator();
    
    // Sends packet directly to the master node
    void sendPacket( Packet* pkt );
    
    bool isPacketWaiting();
    Packet* receivePacket();

private:
    WifiCommunicator( const char* baseIPAddress_, const int basePort_, const int ownPort_ );

    // Socket for sending directly to the master
    SendingSocket* sendSock;
    
    // Socket for receiving at our own port
    ListeningSocket* listenSock;
    HostAndPort ownHap;
    HostAndPort baseHap;
};

#endif
