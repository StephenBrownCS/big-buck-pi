#include <iostream>
#include "SendingSocket.h"
#include "Packet.h"
#include "UDP_Packet.h"
#include "utility.h"
#include "HostAndPort.h"

using namespace std;

int main(){
    try{
        int destIP = ipAddressStrToLong("192.168.0.0");
        int destPort = 1025;
        SendingSocket sendSock(destIP, destPort);
        UDP_Packet pkt('A', 1, 0, 0);
        sendSock.sendPacket(pkt);
    }
    catch(Error & e){
        cout << e.getMsg() << endl;
    }
    return 0;
}