#include <iostream>
#include "SendingSocket.h"
#include "Packet.h"
#include "HelloPacket.h"
#include "Utility.h"
#include "HostAndPort.h"

using namespace std;

int main(){
    try{    
        unsigned int destIP = ipAddressStrToLong("128.105.32.253");
        int destPort = 55003;;//44444;//55003;
        cout << "destIP: " << destIP << endl;
        
        unsigned int ownIPAddress = (unsigned int) getOwnIPAddress();
        string ipAddrString = convertIntToIPAddressString(ownIPAddress);
        cout << "Own IP Address: " << ipAddrString << endl;
        
        SendingSocket sendSock(destIP, destPort);
        HostAndPort hap( ipAddrString, destPort );
        HelloPacket* pkt = HelloPacket::create(hap);
        for(int i = 0; i < 10; i++){
            cout << "Sending packet" << endl;
            sendSock.sendPacket(pkt);
        }
    }
    catch(Error & e){
        cout << e.getMsg() << endl;
    }
    return 0;
}
