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

using namespace std;

const int PORT = 5001;

int main(int argc, char** argv){
    char* destIpAddress = 0;
    unsigned short destPort = 0;
    if ( argc != 3 ){
        cout << "Usage: Main <Dot-Separated IP Address> <port>" << endl;
        exit( 0 );
    }
    else{
        destIpAddress = argv[1];
        destPort = atoi(argv[2]);
    }

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
