#ifndef WIFI_COMMUNICATOR_H_
#define WIFI_COMMUNICATOR_H_

#include "Communicator.h"
#include "HostAndPort.h"

// Forward References
class SendingSocket;
class ListeningSocket;
class Packet;

class WifiCommunicator : public Communicator{
public:
    static WifiCommunicator* create( const char* baseIPAddress_, const int basePort_, const int ownPort_ );
    ~WifiCommunicator();
    
    void sendPacket( Packet* pkt );
    bool isPacketWaiting();
    Packet* receivePacket();

private:
    WifiCommunicator( const char* baseIPAddress_, const int basePort_, const int ownPort_ );

    SendingSocket* sendSock;
    ListeningSocket* listenSock;
    HostAndPort ownHap;
    HostAndPort baseHap;
};

#endif
