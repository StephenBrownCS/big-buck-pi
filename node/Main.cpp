/*
    1) Listens on port 8888
    2) Expects to receive a UDP Packet with no payload (this will identify the 
        sender)
*/


#include <iostream>
#include "SendingSocket.h"
#include "Packet.h"
#include "UDPPacket.h"
#include "BigBuckPacket.h"
#include "Utility.h"
#include "HostAndPort.h"
#include "PIRSensor.h"
#include "WifiCommunicator.h"
#include "BigBuckSensingNode.h"
#include "ListeningSocket.h"

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
    
    // Always listen on port 8888
    ListeningSocket listenSock( PORT );
    
    // Get the first packet that will tell us where to send our packets to
    UDPPacket* setupPkt = listenSock.receivePacket();
    HostAndPort srcHap = setupPkt->getSrc();
    destIpAddress = srcHap.getIPAsStr().c_str();
    destPort = srcHap.getPort();
    
    cout << "Dest IP Address: " << destIpAddress << endl;
    cout << "Dest Port: " << destPort << endl;
    
    delete setupPkt;
    listenSock.closeSocket();

    try{    
        Communicator* communicator = WifiCommunicator::create( destIpAddress, destPort, PORT );
        Sensor* sensor = PIRSensor::create();
        BigBuckSensingNode* sensingNode = BigBuckSensingNode::create(communicator, sensor, 0);
        
        communicator->sendPacket(BigBuckPacket::create('H', 22, 33, 28, 0, 0));
        
        sensingNode->sensingLoop();
        
        delete sensingNode;
    }
    catch(Error & e){
        cout << e.getMsg() << endl;
    }
    return 0;
}
