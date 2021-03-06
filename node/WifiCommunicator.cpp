#include "WifiCommunicator.h"
#include "ListeningSocket.h"
#include "UDPPacket.h"
#include "SendingSocket.h"
#include "Utility.h"

using namespace std;

WifiCommunicator*  WifiCommunicator::create( const char* baseIPAddress_, const int basePort_, const int ownPort_ ){
    return new WifiCommunicator( baseIPAddress_, basePort_, ownPort_ );
}

WifiCommunicator::WifiCommunicator( const char* baseIPAddress_, const int basePort_, const int ownPort_ ){
    baseHap.setIP(ipAddressStrToLong( baseIPAddress_ ) );
    baseHap.setPort( basePort_ );
    ownHap.setIP( (unsigned int) getOwnWlanIpAddress() );
    ownHap.setPort( ownPort_ );

    cout << "self: " << ownHap << endl;
    cout << "dest: " << baseHap << endl;
    sendSock = new SendingSocket( ntohl(baseHap.getIP()), baseHap.getPort());
    listenSock = new ListeningSocket( ownHap.getPort() );
}

WifiCommunicator::~WifiCommunicator(){
    delete sendSock;
    delete listenSock;
}

void WifiCommunicator::sendPacket( Packet* pkt ){
    sendSock->sendPacket( UDPPacket::create( ownHap, baseHap, pkt->c_str_length(), pkt->c_str() ) );
}

bool WifiCommunicator::isPacketWaiting(){
    return listenSock->isPacketWaiting();
}

Packet* WifiCommunicator::receivePacket(){
    return listenSock->receivePacket();
}
