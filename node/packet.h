
/*  A Packet Object
    Represents a UDP Packet, can be constructed from either the 
    component parts (packetType, sequence, length, and payload) or
    from a c-string in network order.
    
    Once constructed, you can access its member variables
    through the public interface, which provides getters for all of them.
    You can also use the c_str() function to get a c-string representing the packet
    
    Example Usage:
 
 (From component parts)
    char filename[20] = "hello-world.cpp";
    
    Packet p('D', 24, strlen(filename) + 1, filename);
    cout << p.getPacketType() << endl;
    cout << p.getSequence() << endl;
    cout << p.getPayloadLength() << endl;
    cout << p.getPayload() << endl;
    
    send(sock, p.c_str(), p.c_str_length(), 0);
*/

#ifndef PACKET_H_
#define PACKET_H_

#define MAX_PAYLOAD_LENGTH 5000

class Packet{
public:    
    //Virtual Dtor
    virtual ~Packet() {}
    
    //Produces a c string in network order for send()
    virtual const char * c_str() const = 0;
    
    //Returns the length of the c-string form of this packet 
    virtual int c_str_length() const = 0;
    
    // Prints contents to standard out (for debugging)
    virtual void print() const = 0;

private:     
    // No assignment
    const Packet & operator==(const Packet & p);
};

#endif
