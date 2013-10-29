/**
 * Creates the BigBuckPacket as an object based off of a given type, sequence,
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

#include "BigBuckPacket.h"

const int MAX_FILENAME_SIZE = 1024;
const int BIG_BUCK_PACKET_HEADER_SIZE = 13;

using namespace std;

BigBuckPacket* BigBuckPacket::create(char packetType, unsigned short srcNodeId, 
                                 unsigned short destNodeId, unsigned int sequence, 
                                 unsigned int length, const char* payload){
    return new BigBuckPacket(packetType, srcNodeId, destNodeId, sequence, length, payload);                                 
}
    
BigBuckPacket* BigBuckPacket::create(const char* rawString){
    return new BigBuckPacket( rawString );
}


BigBuckPacket::BigBuckPacket(char BigBuckPacketType_, unsigned short srcNodeId_, 
        unsigned short destNodeId_, unsigned int sequence_, unsigned int length_, 
        const char* payload_):
    packetType(BigBuckPacketType_), 
    srcNodeId( srcNodeId_), 
    destNodeId( destNodeId_), 
    sequence(sequence_), 
    payloadLength(length_) { 
    
    initPayload(payload_);
    assemblePacketString();
}

BigBuckPacket::BigBuckPacket(const BigBuckPacket & p):
    packetType(p.packetType), 
    srcNodeId( p.srcNodeId), 
    destNodeId( p.destNodeId), 
    sequence(p.sequence), 
    payloadLength(p.payloadLength)
{
    initPayload(p.payload);
    assemblePacketString();
}

BigBuckPacket::BigBuckPacket(const char* rawString){
    //EXTRACT INFORMATION FROM THE PACKET WE RECEIVED
    packetType = rawString[0];
    srcNodeId = ntohs(*(short *)(rawString + 1));
    destNodeId = ntohs(*(short *)(rawString + 3));
    sequence = ntohl(*(int *)(rawString + 5));
    payloadLength = ntohl(*(int *)(rawString + 9));    
    
    initPayload(rawString + BIG_BUCK_PACKET_HEADER_SIZE);
    assemblePacketString();
}


BigBuckPacket::~BigBuckPacket(){
    if (payload)
        free(payload);
    free(packet_as_str);
}

int BigBuckPacket::c_str_length() const { 
    return BIG_BUCK_PACKET_HEADER_SIZE + payloadLength;
}


void BigBuckPacket::print() const{
    cout << endl << endl << "\t***** PRINTING BigBuckPacket INFORMATION *****" << endl;
    cout << "\t\tBigBuckPacket Type: " << getPacketType() << endl;
    cout << "\t\tSource Node ID: " << srcNodeId << endl;
    cout << "\t\tDest Node ID: " << destNodeId << endl;
    cout << "\t\tsequenceNumber: " << getSequence() << endl;
    cout << "\t\tpayloadLength: " << getPayloadLength() << endl;
    if (payload)
        cout << "\t\tPayload : " << getPayload() << endl << endl;
    else
        cout << "\t\tPayload : None! " << endl << endl;
}




// ************** PRIVATE MEMBER FUNCTION DEFINITIONS *****************

void BigBuckPacket::initPayload(const char* c_string){
    // Data BigBuckPacket payloads are not null terminated
    // Request BigBuckPacket payloads are null terminated
    // End BigBuckPackets do not have payloads
    if (payloadLength){
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
 * Assembles the internal BigBuckPacket string using the other member variables.
 */
void BigBuckPacket::assemblePacketString(){
    if (payload)
        packet_as_str = (char *)malloc(BIG_BUCK_PACKET_HEADER_SIZE + payloadLength + 1);
    else
        packet_as_str = (char *)malloc(BIG_BUCK_PACKET_HEADER_SIZE + 1);
    
    //place the BigBuckPacket type letter in the buffer
    packet_as_str[0] = packetType;
    
    short* short_ptr = (short *)(packet_as_str + 1);
    *short_ptr = htonl(srcNodeId);
    
    short_ptr = (short *)(packet_as_str + 3);
    *short_ptr = htonl(destNodeId);
    
    // place the sequence number
    int* int_ptr = (int *)(packet_as_str + 5);
    *int_ptr = htonl(sequence);
    
    // place the length
    int_ptr = (int *)(packet_as_str + 9);
    *int_ptr = htonl(payloadLength);
    
    if (payload){
        strcpy(packet_as_str + BIG_BUCK_PACKET_HEADER_SIZE, payload);
        for(int i = 0; i < payloadLength; i++){
            packet_as_str[BIG_BUCK_PACKET_HEADER_SIZE + i] = payload[i];
        }
        packet_as_str[BIG_BUCK_PACKET_HEADER_SIZE + payloadLength] = '\0';
    }
    else{
        packet_as_str[BIG_BUCK_PACKET_HEADER_SIZE] = '\0';
    }
}
