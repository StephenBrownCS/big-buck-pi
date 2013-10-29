#include <iostream>
#include "SendingSocket.h"
#include "Packet.h"
#include "UDPPacket.h"
#include "BigBuckPacket.h"
#include "Utility.h"
#include "HostAndPort.h"

using namespace std;

int main(){
    try{    
        unsigned int destIP = ipAddressStrToLong("25.14.87.94");
        int destPort = 55019;//44444;//55003;
        cout << "destIP: " << destIP << endl;
        cout << "destPort: " << destPort << endl;
        
        unsigned int ownIPAddress = (unsigned int) getOwnIPAddress();
        string srcIP = convertIntToIPAddressString(ownIPAddress);
        int srcPort = 55004;
        cout << "Own IP Address: " << srcIP << endl;
        
        HostAndPort srcHap( srcIP, srcPort );
        HostAndPort destHap( destIP, destPort );
        SendingSocket sendSock(destIP, destPort);
        
        Packet* bigBuckPkt = BigBuckPacket::create('H', 0, 1, 1, 5, "hello");
        bigBuckPkt->print();
        BigBuckPacket::create(bigBuckPkt->c_str())->print();
        
        UDPPacket* udpPkt = UDPPacket::create(
            srcHap, destHap, bigBuckPkt->c_str_length(), bigBuckPkt->c_str());
                
        for(int i = 0; i < 10; i++){
            cout << "Sending packet" << endl;
            sendSock.sendPacket(udpPkt);
        }
        delete udpPkt;
        delete bigBuckPkt;
    }
    catch(Error & e){
        cout << e.getMsg() << endl;
    }
    return 0;
}
