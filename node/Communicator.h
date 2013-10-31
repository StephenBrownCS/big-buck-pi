#ifndef COMMUNICATOR_H_
#define COMMUNICATOR_H_

//Forward Reference
class Packet;

class Communicator{
public:
    virtual ~Communicator(){}
    virtual void sendPacket( Packet* pkt ) = 0;
    virtual bool isPacketWaiting() = 0;
    virtual Packet* receivePacket() = 0;
};

#endif
