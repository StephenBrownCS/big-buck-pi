/*
    Link State Packet Object

    This module encapsulates an Link State Packet.
    It consists of:
        8 bit type
        32 bit time to live
        32 bit sequence number
        32 bit src IP
        16 bit src Port
        List of neighbors as  <dot-separated IP string , port>
            example: 1.1.1.1,4 3.3.3.3,2

    
    It can be constructed from disparate parts (the ones listed above) or from a 
    c_string (the form it would be received from the network)

    Once constructed, you can access its member variables
    through the public interface, which provides getters for all of them.
    You can also use the c_str() function to get a c-string representing the IP Packet
    c_str_length() will give you the length of the c_string representing the IP Packet
    
    Example Usage:
 
 (From component parts)    
    // Need to use named static constructor instead of normal constructor
    LinkState_Packet* pkt = 
        LinkState_Packet::create( ???? TBD ??? ); 
        
    sending_socket.sendPacket(*p);
    
  (From c-string)
    LinkState_Packet* pkt = LinkState_Packet::create(c-string); 
    
    Because we're using the named constructor, these objects will
    ALWAYS be created on the heap, and consequently, will ALWAYS need to be
    deleted:
        delete pkt;
*/

#ifndef LinkState_Packet__H_
#define LinkState_Packet__H_

#include <list>

#include "Packet.h"
#include "HostAndPort.h"

const char LINK_STATE_PACKET_CHAR = 'L';

class LinkStatePacket : public Packet{
public: 
    //Named Constructor Idiom, all packets are to be created on the heap
    static LinkStatePacket* create(unsigned int timeToLive, 
                                    unsigned int seqno,
                                    const HostAndPort & srcHostAndPort,
                                    const std::list<HostAndPort> & liveNeighbors_);
    
    static LinkStatePacket* create(const char* rawString);
    
    //Dtor, deallocate malloc'd data
    ~LinkStatePacket();
    
    // Getters for all relevant LinkState_Packet data
    char getType() { return type; }
    unsigned int getTTL() { return timeToLive; }
    unsigned int getSeqNo() {return sequenceNumber;}
    
    // Getters for the source - three options
    HostAndPort getSrc();
    unsigned int getSrcIP() { return srcIP; }
    unsigned short getSrcPort() { return srcPort; }
    std::list<HostAndPort> getLiveNeighbors(){ return liveNeighbors; }
    
    //Produces a c string in network order for send()
    const char *c_str() const { return packet_as_str; }
    
    //Returns the length of the c-string form of this UDP_Packet 
    int c_str_length() const;
    
    // Prints contents to standard out (for debugging)
    void print() const;

private:
    // Constructor from disparate parts
    LinkStatePacket(unsigned int timeToLive, 
                     unsigned int seqno,
                     const HostAndPort & hostAndPort,
                     const std::list<HostAndPort> & liveNeighbors_);
    
    // Constructor from a c_string, as when received from a socket
    explicit LinkStatePacket( const char* c_string);

    // Creates a string from the member data
    // Used only by constructors
    void assemblePacketString();
    
    // No copy
    LinkStatePacket(const LinkStatePacket & p);
    // No assignment
    const LinkStatePacket & operator==(const LinkStatePacket & p);


    // **************** PRIVATE VARIABLES **************    
    //internal c-string representation
    char* packet_as_str;

    char type;
    unsigned int timeToLive;
    unsigned int sequenceNumber;
    unsigned int srcIP; // IP in host-byte order
    unsigned short srcPort; // Port in host-byte order
    std::list<HostAndPort> liveNeighbors;
    
    int payloadLength;
};

#endif
