/**
 * Creates the UDP_Packet as an object based off of a given type, sequence,
 * length, and payload.
 *
 * <p>Bugs: none known
 *
 * @author Stephen Brown and Brian Hunter
 */

#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "UDP_packet.h"
#include "utility.h"

const int MAX_FILENAME_SIZE = 1024;
using namespace std;

/**
 * Constructor for the UDP_Packet object
 *
 * @param UDP_PacketType: request, data, or end UDP_Packet
 * @param sequence: denotes sequence number
 * @param length: number of bytes in payload
 * @param payload: data of the UDP_Packet
 */
UDP_Packet::UDP_Packet(char UDP_PacketType_, unsigned int sequence_, unsigned int length_, const char* payload_):
    UDP_PacketType(UDP_PacketType_), sequence(sequence_), payloadLength(length_) { 
    
    initPayload(payload_);
    assemblePacketString();
}

UDP_Packet::UDP_Packet(const UDP_Packet & p):
    UDP_PacketType(p.UDP_PacketType), sequence(p.sequence), payloadLength(p.payloadLength)
{
    initPayload(p.payload);
    assemblePacketString();
}

UDP_Packet::UDP_Packet(const char* c_string){
    //EXTRACT INFORMATION FROM THE PACKET WE RECEIVED
    UDP_PacketType = c_string[0];
    sequence = ntohl(*(int *)(c_string + 1));
    payloadLength = ntohl(*(int *)(c_string + 5));    
    
    initPayload(c_string + 9);
    assemblePacketString();
}


UDP_Packet::~UDP_Packet(){
    if (payload)
        free(payload);
    free(UDP_Packet_as_str);
}

int UDP_Packet::c_str_length() const { 
    switch(UDP_PacketType){
    case 'D':
    return 9 + payloadLength;
        break;
    case 'R': 
        // Request UDP_Packets MUST send the NULL Byte
        return 9 + strlen(payload) + 1;
        break;
    case 'E':
    case 'A':
        return 9;
        break;
    default:
        throw Error("Error: Invalid UDP_Packet Type");
        return -1;
    }
}


void UDP_Packet::print() const{
    cout << endl << endl << "\t***** PRINTING UDP_Packet INFORMATION *****" << endl;
    cout << "\t\tUDP_Packet Type: " << getPacketType() << endl;
    cout << "\t\tsequenceNumber: " << getSequence() << endl;
    cout << "\t\tpayloadLength: " << getPayloadLength() << endl;
    if (payload)
        cout << "\t\tPayload : " << getPayload() << endl << endl;
    else
        cout << "\t\tPayload : None! " << endl << endl;
}




// ************** PRIVATE MEMBER FUNCTION DEFINITIONS *****************

void UDP_Packet::initPayload(const char* c_string){
    // Data UDP_Packet payloads are not null terminated
    // Request UDP_Packet payloads are null terminated
    // End UDP_Packets do not have payloads
    switch(UDP_PacketType){
    case 'D':
        payload = (char*) malloc(payloadLength + 1);
        for(int i = 0; i < payloadLength; i++)
            payload[i] = c_string[i];
        payload[payloadLength] = '\0';
        break;
    case 'R':
       payload = (char *)malloc(strlen(c_string) + 1);
       strcpy(payload, c_string);
        break;
    case 'E':
    case 'A':
       payload = 0;
        break;
    default:
        throw Error("Invalid UDP_Packet Type! ");
    }
}


/**
 * Assembles the internal UDP_Packet string using the other member variables.
 */
void UDP_Packet::assemblePacketString(){
    if (payload)
        UDP_Packet_as_str = (char *)malloc(9 + strlen(payload) + 1);
    else
        UDP_Packet_as_str = (char *)malloc(9 + 1);
    
    //place the UDP_Packet type letter in the buffer
    UDP_Packet_as_str[0] = UDP_PacketType;
    
    // place the sequence number
    int* ptr = (int *)(UDP_Packet_as_str + 1);
    *ptr = htonl(sequence);
    
    // place the length
    ptr = (int *)(UDP_Packet_as_str + 5);
    *ptr = htonl(payloadLength);
    
    if (payload)
        strcpy(UDP_Packet_as_str + 9, payload);
    else
        UDP_Packet_as_str[9] = '\0';
}
