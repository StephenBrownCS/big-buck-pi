
/*  A UDP_Packet Object
    Represents a UDP UDP_Packet, can be constructed from either the 
    component parts (UDP_PacketType, sequence, length, and payload) or
    from a c-string in network order.
    
    Once constructed, you can access its member variables
    through the public interface, which provides getters for all of them.
    You can also use the c_str() function to get a c-string representing the UDP_Packet
    
    Example Usage:
 
 (From component parts)
    char filename[20] = "hello-world.cpp";
    
    UDP_Packet p('D', 24, strlen(filename) + 1, filename);
    cout << p.getUDP_PacketType() << endl;
    cout << p.getSequence() << endl;
    cout << p.getPayloadLength() << endl;
    cout << p.getPayload() << endl;
    
    sending_socket.sendPacket(p);
*/

#ifndef UDP_Packet_H_
#define UDP_Packet_H_

#include "Packet.h"
#include "HostAndPort.h"
#define MAX_PAYLOAD_LENGTH 5000

class UDPPacket : public Packet{
public: 
    //Named Constructor Idiom, all packets are to be created on the heap
    static UDPPacket* create(const HostAndPort & src, const HostAndPort & dest, 
        unsigned int length_, const char* payload_);
    
    static UDPPacket* create(const char* rawString);
    
    //Dtor, deallocate malloc'd data
    ~UDPPacket();
    
    // Getters for all relevant UDP_Packet data
    HostAndPort getSrc() const{ return src; }
    HostAndPort getDest() const { return dest; }
    unsigned int getPayloadLength() const { return payloadLength; }
    
    // Get Payload as c-string, returns 0 if no payload
    char* getPayload() const { return payload; }
    
    //Produces a c string in network order for send()
    const char * c_str() const { return packet_as_str; }
    
    //Returns the length of the c-string form of this UDP_Packet 
    int c_str_length() const;
    
    // Prints contents to standard out (for debugging)
    void print() const;

private:
    HostAndPort src;
    HostAndPort dest;
    unsigned int payloadLength;
    char* payload;
    char* packet_as_str;
    
    UDPPacket(const HostAndPort & src, const HostAndPort & dest, 
        unsigned int length_, const char* payload_);
    
    // Copy Constructor
    UDPPacket(const UDPPacket & p);
    
    // Constructor from C string
    explicit UDPPacket(const char* c_string);    
    
    // Initialized payload based on the other member variables    
    void initPayload(const char* c_string);
    
    // Creates a string from the member data
    void assemblePacketString();
    
    // No assignment
    const UDPPacket & operator==(const UDPPacket & p);
};

#endif
