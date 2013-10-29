#include <iostream>
#include "ListeningSocket.h"
#include "Packet.h"
#include "UDPPacket.h"
#include "BigBuckPacket.h"
#include "Utility.h"
#include "HostAndPort.h"
//#include <chrono>
//#include <thread>

//using std::this_thread::sleep_for;
//using std::chrono::milliseconds;

using namespace std;

const int PORT = 55019;

int main(){
    try{
        ListeningSocket listenSock( PORT );
        
        unsigned int ownIPAddress = ntohl(getOwnIPAddress());
        cout << "Own IP Address: " << convertIntToIPAddressString(ownIPAddress) << endl;
        
        while( true ){
            if (listenSock.isPacketWaiting() ){
                UDPPacket* pkt = listenSock.receivePacket();
                cout << "Received!" << endl;
                pkt->print();
                Packet* bigBuckPkt = BigBuckPacket::create(pkt->getPayload());
                bigBuckPkt->print();
            }
            sleep(1);
        }
    }
    catch(Error & e){
        cout << "Error!" << endl;
        cout << e.getMsg() << endl;
    }
    return 0;
}
