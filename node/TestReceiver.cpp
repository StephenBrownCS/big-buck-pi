#include <iostream>
#include <arpa/inet.h>
/*
    A program for posing as a master tablet - used for testing the nodes and 
    name server.
*/
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

//#include <chrono>
//#include <thread>

//using std::this_thread::sleep_for;
//using std::chrono::milliseconds;

using namespace std;

int main(){
    try{
        HostAndPort self(ntohl(getOwnIPAddress()), OWN_LISTEN_PORT);

       cout << "self: " << self << endl;
    
        // Register with the name server
        #ifdef USING_CEDAR
        unsigned long nameServerIp = getIPAddressForHostname(NAME_SERVER_NAME);
        #else
        unsigned long nameServerIp = ntohl(ipAddressStrToLong( NAME_SERVER_HOTSPOT_STATIC_IP ));
        #endif
        unsigned short nameServerPort = NAME_SERVER_PORT;
    
        HostAndPort nameServerHap(nameServerIp, nameServerPort);
        cout << "Name Server IP: " << nameServerHap << endl;

        SendingSocket sock(nameServerIp, nameServerPort);
        BigBuckPacket* registrationPkt = 
        BigBuckPacket::create(
            'M', DEFAULT_NODE_ID, DEFAULT_NODE_ID, NO_SEQUENCE, NO_PAYLOAD, EMPTY_PAYLOAD
        );
        sock.sendPacket(
        UDPPacket::create(
            self, nameServerHap, registrationPkt->c_str_length(), registrationPkt->c_str()
            )
        );    
    
        ListeningSocket listenSock( OWN_LISTEN_PORT );
        
        unsigned int ownIPAddress = ntohl(getOwnIPAddress());
        cout << "Own IP Address: " << convertIntToIPAddressString(ownIPAddress) << endl;
        
        while( true ){
            if (listenSock.isPacketWaiting() ){
                UDPPacket* pkt = listenSock.receivePacket();
                cout << getCurrentHumanReadableTime();
                cout << "Received!" << endl;
                pkt->print();
                Packet* bigBuckPkt = BigBuckPacket::create(pkt->getPayload());
                bigBuckPkt->print();
            }
            //sleep(1);
        }
    }
    catch(Error & e){
        cout << "Error!" << endl;
        cout << e.getMsg() << endl;
    }
    return 0;
}
