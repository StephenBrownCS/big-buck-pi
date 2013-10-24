
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

#include "packet.h"
#define MAX_PAYLOAD_LENGTH 5000

class UDP_Packet : public Packet{
public: 
     /**
     * Constructor for the UDP_Packet object
     *
     * @param UDP_PacketType: request, data, or end UDP_Packet
     * @param sequence: denotes sequence number
     * @param length: number of bytes in payload
     * @param payload: data of the UDP_Packet
     */
    UDP_Packet(char UDP_PacketType_, unsigned int sequence_, unsigned int length_, const char* payload_);
    
    // Copy Constructor
    UDP_Packet(const UDP_Packet & p);
    
    // Constructor from C string
    explicit UDP_Packet(const char* c_string);
    
    //Dtor, deallocate malloc'd data
    ~UDP_Packet();
    
    // Getters for all relevant UDP_Packet data
    char getPacketType() const { return UDP_PacketType; }
    unsigned int getSequence() const { return sequence; }
    unsigned int getPayloadLength() const { return payloadLength; }
    
    // Get Payload as c-string, returns 0 if no payload
    char* getPayload() const { return payload; }
    
    //Produces a c string in network order for send()
    const char * c_str() const { return UDP_Packet_as_str; }
    
    //Returns the length of the c-string form of this UDP_Packet 
    int c_str_length() const;
    
    // Prints contents to standard out (for debugging)
    void print() const;

private:
    char UDP_PacketType;
    unsigned int sequence;
    unsigned int payloadLength;
    char* payload;
    char* UDP_Packet_as_str;
    
    // Initialized payload based on the other member variables    
    void initPayload(const char* c_string);
    
    // Creates a string from the member data
    void assemblePacketString();
    
    // No assignment
    const UDP_Packet & operator==(const UDP_Packet & p);
};

#endif
