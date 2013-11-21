/*
    Represents a high-level Big Buck Application packet
    Encapsulates the following pieces of data:
        packetType (1 byte)
        source node Id ( 2 bytes )
        dest node Id (2 bytes)
        sequence # (8 bytes)
        payload length (4 bytes)
        payload (flexible length)
        
    Can be constructed from either the 
    component parts (packet type, source node ID, etc.) or
    from a c-string in network byte order.
    
    Once constructed, you can access its member variables
    through the public interface, which provides getters for all of them.
    You can also use the c_str() function to get a c-string representing the UDP_Packet
    
    Sample Usage:
        BigBuckPacket* responseInnerPkt = BigBuckPacket::create( 
            'A', DEFAULT_NODE_ID, nodeId, NO_SEQUENCE, NO_PAYLOAD, EMPTY_PAYLOAD );
        Packet* responseOuterPkt = UDPPacket::create(
            self, senderHap, responseInnerPkt->c_str_length(), responseInnerPkt->c_str());
        sock.sendPacket(responseOuterPkt);
        delete responseInnerPkt;
        delete responseOuterPkt;
*/

#ifndef BIG_BUCK_PACKET__H_
#define BIG_BUCK_PACKET__H_

#include "Packet.h"
#define MAX_PAYLOAD_LENGTH 5000

class BigBuckPacket : public Packet{
public: 
    static BigBuckPacket* create(char packetType, unsigned short srcNodeId, 
                                 unsigned short destNodeId, unsigned int sequence, 
                                 unsigned int length, const char* payload);
    
    static BigBuckPacket* create(const char* rawString);
    
    //Dtor, deallocate malloc'd data
    ~BigBuckPacket();
    
    // Getters for all relevant BigBuckPacket data
    char getPacketType() const { return packetType; }
    unsigned short getSrcNodeId() const{ return srcNodeId; }
    unsigned short getDestNodeId() const{ return destNodeId; }
    unsigned int getSequence() const { return sequence; }
    unsigned int getPayloadLength() const { return payloadLength; }
    
    // Get Payload as c-string, returns 0 if no payload
    char* getPayload() const { return payload; }
    
    //Produces a c string in network order for send()
    const char * c_str() const { return packet_as_str; }
    
    //Returns the length of the c-string form of this BigBuckPacket 
    int c_str_length() const;
    
    // Prints contents to standard out (for debugging)
    void print() const;

private:
    char packetType;
    unsigned short srcNodeId;
    unsigned short destNodeId;
    unsigned int sequence;
    unsigned int payloadLength;
    char* payload;
    char* packet_as_str;
    
    BigBuckPacket(char packetType_, unsigned short srcNodeId_, 
                    unsigned short destNodeId_, unsigned int sequence_, 
                    unsigned int length_, const char* payload_);
    
    // Copy Constructor
    BigBuckPacket(const BigBuckPacket & p);
    
    // Constructor from C string
    explicit BigBuckPacket(const char* c_string);
    
    // Initialized payload based on the other member variables    
    void initPayload(const char* c_string);
    
    // Creates a string from the member data
    void assemblePacketString();
    
    // No assignment
    const BigBuckPacket & operator==(const BigBuckPacket & p);
};

#endif