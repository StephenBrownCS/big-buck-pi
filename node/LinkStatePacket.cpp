#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <iostream>
#include <sstream>
#include <iterator>
#include "LinkStatePacket.h"

using namespace std;

const int LINK_STATE_PACKET_HEADER_LENGTH_IN_BYTES = 15;



// ***************  PUBLIC NAMED CONSTRUCTORS **************

LinkStatePacket* LinkStatePacket::create(
                     unsigned int timeToLive_, 
                     unsigned int seqno,
                     const HostAndPort & hostAndPort,
                     const list<HostAndPort> & liveNeighbors_){
    return new LinkStatePacket(timeToLive_, seqno, hostAndPort, liveNeighbors_);
}

LinkStatePacket* LinkStatePacket::create(const char* rawString){
    return new LinkStatePacket(rawString);
}




// ***************  DESTRUCTOR **************
LinkStatePacket::~LinkStatePacket(){
    free(packet_as_str);
}



// ************** PUBLIC ACCESSORS *******************
HostAndPort LinkStatePacket::getSrc(){
    return HostAndPort(srcIP, srcPort);    
}





// ***************  PUBLIC OVERRIDDEN FUNCTIONS **************

//Returns the length of the c-string form of this UDP_Packet 
int LinkStatePacket::c_str_length() const{
    return LINK_STATE_PACKET_HEADER_LENGTH_IN_BYTES + payloadLength;
}


// Prints contents to standard out (for debugging)
void LinkStatePacket::print() const{
    cout << endl << endl << "\t***** PRINTING LINK STATE PACKET INFORMATION *****" << endl;
    cout << "\t\tType: " << type << endl;
    cout << "\t\tTTL: " << timeToLive << endl;
    cout << "\t\tSeq #: " << sequenceNumber << endl;
    cout << "\t\tSRC IP: " << convertIntToIPAddressString(htonl(srcIP)) << endl;
    cout << "\t\tSRC Port: " << srcPort << endl;
    cout << "\t\tLive Neighbors: " << endl;
    
    cout << "\t\t\t";
    ostream_iterator<HostAndPort> out_iter(cout, "\n\t\t\t");
    copy(liveNeighbors.begin(), liveNeighbors.end(), out_iter);
    cout << endl;
}





// ***************   PRIVATE CONSTRUCTORS **************

LinkStatePacket::LinkStatePacket(unsigned int timeToLive_, 
                                   unsigned int seqno,
                                   const HostAndPort & hostAndPort,
                                   const std::list<HostAndPort> & liveNeighbors_):
    type(LINK_STATE_PACKET_CHAR),
    timeToLive(timeToLive_),
    sequenceNumber(seqno),
    srcIP(hostAndPort.getIP()),
    srcPort(hostAndPort.getPort()),
    liveNeighbors(liveNeighbors_)
{
    assemblePacketString();
}

LinkStatePacket::LinkStatePacket(const char* rawString){
    //EXTRACT INFORMATION FROM THE C_STRING
       
    type = rawString[0];
    timeToLive = ntohl(*(int *)(rawString + 1));
    sequenceNumber = ntohl(*(int *)(rawString + 5));
    srcIP = ntohl(*(int *)(rawString + 9));
    srcPort = ntohs(*(short *)(rawString + 13));
    
    // Extract the payload (list of active neighbors)
    string rawList(rawString + 15);
    istringstream iss(rawList);
    istream_iterator<HostAndPort> in_iter(iss);
    istream_iterator<HostAndPort> eof_iter;
    copy(in_iter, eof_iter, back_inserter(liveNeighbors));
    
    assemblePacketString();
}





// ***************  OTHER PRIVATE FUNCTIONS **************

void LinkStatePacket::assemblePacketString(){

    // Create the payload string, and determine its length
    ostringstream oss;
    ostream_iterator<HostAndPort> out_iter(oss, " ");
    copy(liveNeighbors.begin(), liveNeighbors.end(), out_iter);
    string payload = oss.str();
    
    payloadLength = payload.length();
    
    packet_as_str = (char *)malloc(LINK_STATE_PACKET_HEADER_LENGTH_IN_BYTES + 
                                    payloadLength + 1);
    
    
    //place the type
    packet_as_str[0] = type;
    
    //place the TTL
    int* int_ptr = (int *)(packet_as_str + 1);
    *int_ptr = htonl(timeToLive);
    
    //place the Sequence #
    int_ptr = (int *)(packet_as_str + 5);
    *int_ptr = htonl(sequenceNumber);
    
    //place the src IP address
    int_ptr = (int *)(packet_as_str + 9);
    *int_ptr = htonl(srcIP);
    
    //place the src 16-bit port
    short* short_ptr = (short *)(packet_as_str + 13);
    *short_ptr = htons(srcPort);
    
    // place the payload (list of active neighbors)
    strcpy(packet_as_str + 15, payload.c_str());    
    

    packet_as_str[LINK_STATE_PACKET_HEADER_LENGTH_IN_BYTES + payloadLength] = '\0';
}
