/*
    1) Listens on port 8888
    2) Expects to receive a UDP Packet with no payload (this will identify the 
        sender)
*/


#include <iostream>
#include "ListeningSocket.h"
#include "SendingSocket.h"
#include "Packet.h"
#include "UDPPacket.h"
#include "BigBuckPacket.h"
#include "Utility.h"
#include "HostAndPort.h"

using namespace std;

const int PORT = 8888;

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
    
    HostAndPort self(getOwnIPAddress(), PORT);
    
    // Always listen on port 8888
    ListeningSocket listenSock( PORT );
    
    // UDPPacket* setupPkt = listenSock.receivePacket();
    //HostAndPort srcHap = setupPkt->getSrc();
    //destIpAddress = srcHap.getIPAsStr().c_str();
    //destPort = srcHap.getPort();
    
    //cout << "Dest IP Address: " << destIpAddress << endl;
    //cout << "Dest Port: " << destPort << endl;
    
    //delete setupPkt;
    //listenSock.closeSocket();

    try{    
        //loop to receive and forward packets
        while(1){
            // SEE IF PACKET IS AVAILABLE
            if(listenSock.isPacketWaiting()){               
                UDPPacket* incomingPkt = listenSock.receivePacket(0.1);
            
                cout << "Packet Received!" << endl;
                
                HostAndPort senderHap = incomingPkt->getSrc();

                SendingSocket sock(senderHap.getIP(), senderHap.getPort());
                sock.sendPacket(UDPPacket::create(self, senderHap, 0, 0));
            }
        }
    }
    catch(Error & e){
        cout << e.getMsg() << endl;
    }
    listenSock.closeSocket();
    return 0;
}
