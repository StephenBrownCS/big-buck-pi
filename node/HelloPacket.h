
/*
    Hello Packet, for notifying neighbors that a node is up
    Only contains a type character, a SRC IP and SRC Port
*/

#ifndef HELLO_PACKET__H_
#define HELLO_PACKET__H_

#include "Packet.h"
#include "HostAndPort.h"

class HelloPacket : public Packet {
public:
    //Named Constructor Idiom, all packets are to be created on the heap
    static HelloPacket* create(const HostAndPort & srcHap);
    
    static HelloPacket* create(const char* rawString);
    
    //Dtor, deallocate malloc'd data
    ~HelloPacket();
    
    // Getters for all relevant LinkState_Packet data
    char getType() { return type; }
    HostAndPort getSrc();
    unsigned int getSrcIP() { return srcIP; }
    unsigned short getSrcPort() { return srcPort; }
    
    //Produces a c string in network order for send()
    const char *c_str() const { return packet_as_str; }
    
    //Returns the length of the c-string form of this UDP_Packet 
    int c_str_length() const;
    
    // Prints contents to standard out (for debugging)
    void print() const;

private:
    // Constructor from disparate parts
    HelloPacket(const HostAndPort & srcHap);
    
    // Constructor from a c_string, as when received from a socket
    explicit HelloPacket( const char* c_string);

    // Creates a string from the member data
    // Used only by constructors
    void assemblePacketString();
    
    // No copy
    HelloPacket(const HelloPacket & p);
    // No assignment
    const HelloPacket & operator==(const HelloPacket & p);


    // **************** PRIVATE VARIABLES **************    
    //internal c-string representation
    char* packet_as_str;

    char type;
    unsigned int srcIP; // IP in host-byte order
    unsigned short srcPort; // Port in host-byte order
};

#endif
