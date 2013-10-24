/*
    ICMP Packet Factory
    
    Uses the Factory Design Pattern to elminiate unnecessary
    dependency between the Receiving Socket and the various
    ICMP Packet types. It only knows about the ICMP Packet
    and the ICMP Packet Factory.
    
    It allows creation of any of them from a rawString
    through one method

*/


#ifndef Packet_Factory_H_
#define Packet_Factory_H_

class Packet;

class PacketFactory{
public:
    static Packet* createPacket(char* rawString);
};

#endif
