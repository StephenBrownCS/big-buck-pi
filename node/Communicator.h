/*
    Represents an abstract Communicator class, for use in sending information 
    to the master node.
*/

#ifndef COMMUNICATOR_H_
#define COMMUNICATOR_H_

//Forward Reference
class Packet;

class Communicator{
public:
    virtual ~Communicator(){}
    
    // Send packt to master node
    virtual void sendPacket( Packet* pkt ) = 0;
    
    // check to see if a packet is waiting (this would be a call to select 
    // for a concrete wifi communicator)
    virtual bool isPacketWaiting() = 0;
    
    // Synchronously wait for and receive a packet
    virtual Packet* receivePacket() = 0;
};

#endif
