/**
 * Creates the UDPPacket as an object based off of a given type, sequence,
 * length, and payload.
 *
 * <p>Bugs: none known
 *
 * @author Stephen Brown
 */
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "UDPPacket.h"

const int MAX_FILENAME_SIZE = 1024;
const int UDP_PACKET_HEADER_SIZE = 16;
using namespace std;

UDPPacket* UDPPacket::create(const HostAndPort & src, const HostAndPort & dest, 
        unsigned int length, const char* payload){
        return new UDPPacket(src, dest, length, payload);
}
    
UDPPacket* UDPPacket::create(const char* rawString){
    return new UDPPacket( rawString );
}
    
UDPPacket::UDPPacket(const HostAndPort & src_, const HostAndPort & dest_, 
                     unsigned int length_, const char* payload_):
    src(src_), dest(dest_), payloadLength(length_) { 
    
    initPayload(payload_);
    assemblePacketString();
}

UDPPacket::UDPPacket(const UDPPacket & p):
    src(p.src), dest(p.dest), payloadLength(p.payloadLength)
{
    initPayload(p.payload);
    assemblePacketString();
}

UDPPacket::UDPPacket(const char* rawString){
    //EXTRACT INFORMATION FROM THE PACKET WE RECEIVED
    unsigned int srcIP = ntohl(*(int *)(rawString + 0));
    unsigned short srcPort = ntohs(*(short *)(rawString + 4)); 
    unsigned int destIP = ntohl(*(int *)(rawString + 6));
    unsigned short destPort = ntohs(*(short *)(rawString + 10));
    payloadLength = ntohl(*(int *)(rawString + 12));
    
    src.setIP(srcIP);
    src.setPort(srcPort);
    dest.setIP(destIP);
    dest.setPort(destPort);  

    initPayload(rawString + UDP_PACKET_HEADER_SIZE);
    assemblePacketString();
}


UDPPacket::~UDPPacket(){
    if (payload)
        free(payload);
    free(packet_as_str);
}

int UDPPacket::c_str_length() const { 
    return UDP_PACKET_HEADER_SIZE + payloadLength;
}


void UDPPacket::print() const{
    cout << endl << endl << "\t***** PRINTING UDPPacket INFORMATION *****" << endl;
    cout << "\t\tSource: " << getSrc() << endl;
    cout << "\t\tDestination: " << getDest() << endl;
    cout << "\t\tpayloadLength: " << getPayloadLength() << endl;
    if (payload)
        cout << "\t\tPayload : " << getPayload() << endl << endl;
    else
        cout << "\t\tPayload : None! " << endl << endl;
}




// ************** PRIVATE MEMBER FUNCTION DEFINITIONS *****************

void UDPPacket::initPayload(const char* c_string){
    // Data UDPPacket payloads are not null terminated
    // Request UDPPacket payloads are null terminated
    // End UDPPackets do not have payloads

    if(payloadLength){
        payload = (char*) malloc(payloadLength + 1);
        for(int i = 0; i < payloadLength; i++)
            payload[i] = c_string[i];
        payload[payloadLength] = '\0';
    }
    else{
        payload = 0;
    }
}


/**
 * Assembles the internal UDPPacket string using the other member variables.
 */
void UDPPacket::assemblePacketString(){
    if (payload)
        packet_as_str = (char *)malloc(UDP_PACKET_HEADER_SIZE + payloadLength + 1);
    else
        packet_as_str = (char *)malloc(UDP_PACKET_HEADER_SIZE + 1);
    
    //place the src IP address
    int* int_ptr = (int *)(packet_as_str + 0);
    *int_ptr = htonl(src.getIP());
    
    //place the src 16-bit port
    short* short_ptr = (short *)(packet_as_str + 4);
    *short_ptr = htons(src.getPort());   

    // place the dest IP address
    int_ptr = (int *)(packet_as_str + 6);
    *int_ptr = htonl(dest.getIP());
    
    // place the dest Port
    short_ptr = (short *)(packet_as_str + 10);
    *short_ptr = htons(dest.getPort());
    
    // place the length
    int_ptr = (int *)(packet_as_str + 12);
    *int_ptr = htonl(payloadLength);
    
    if (payload){
        strcpy(packet_as_str + UDP_PACKET_HEADER_SIZE, payload);
        for(int i = 0; i < payloadLength; i++){
            packet_as_str[UDP_PACKET_HEADER_SIZE + i] = payload[i];
        }
        packet_as_str[UDP_PACKET_HEADER_SIZE + payloadLength] = '\0';
    }
    else{
        packet_as_str[UDP_PACKET_HEADER_SIZE] = '\0';
    }
}
