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
        unsigned int destIP = ipAddressStrToLong("128.105.34.152");
        int destPort = 8888;//44444;//55003;
        cout << "destIP: " << destIP << endl;
        cout << "destPort: " << destPort << endl;
        
        // For testing name server
        HostAndPort self(getOwnIPAddress(), destPort);
        
        unsigned int ownIPAddress = (unsigned int) getOwnIPAddress();
        string srcIP = convertIntToIPAddressString(ownIPAddress);
        int srcPort = 55004;
        cout << "Own IP Address: " << srcIP << endl;
        
        
        // Register with the name server
        unsigned long nameServerIp = ntohl(getIPAddressForHostname("cedar.cs.wisc.edu"));
        unsigned short nameServerPort = 8888;
    
        cout << "Name Server IP: " << nameServerIp << endl;
    
        HostAndPort nameServerHap(nameServerIp, nameServerPort);
        SendingSocket sock(nameServerIp, nameServerPort);
        sock.sendPacket(UDPPacket::create(self, nameServerHap, 0, 0));    
    
        cout << "Sent the packet to name server!" << endl;
        
        HostAndPort srcHap( srcIP, srcPort );
        HostAndPort destHap( destIP, destPort );
        SendingSocket sendSock(destIP, destPort);
        
        Packet* bigBuckPkt = BigBuckPacket::create('H', 0, 1, 1, 5, "hello");
        bigBuckPkt->print();
        BigBuckPacket::create(bigBuckPkt->c_str())->print();
        
        UDPPacket* udpPkt = UDPPacket::create(
            srcHap, destHap, bigBuckPkt->c_str_length(), bigBuckPkt->c_str());
                
        for(int i = 0; i < 1; i++){
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
