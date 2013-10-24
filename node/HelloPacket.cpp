#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

#include "HelloPacket.h"

using namespace std;

const char HELLO_STATE_PACKET_CHAR = 'H';
const int HELLO_PACKET_HEADER_LENGTH_IN_BYTES = 7;



// ***************  PUBLIC NAMED CONSTRUCTORS **************

HelloPacket* HelloPacket::create(const HostAndPort & srcHap){
    return new HelloPacket(srcHap);
}

HelloPacket* HelloPacket::create(const char* rawString){
    return new HelloPacket(rawString);
}




// ***************  DESTRUCTOR **************
HelloPacket::~HelloPacket(){
    free(packet_as_str);
}



// ************** PUBLIC ACCESSORS *******************
HostAndPort HelloPacket::getSrc(){
    return HostAndPort(srcIP, srcPort);    
}





// ***************  PUBLIC OVERRIDDEN FUNCTIONS **************

//Returns the length of the c-string form of this UDP_Packet 
int HelloPacket::c_str_length() const{
    return strlen(packet_as_str);
}


// Prints contents to standard out (for debugging)
void HelloPacket::print() const{
    cout << endl << endl << "\t***** PRINTING HELLO PACKET INFORMATION *****" << endl;
    cout << "\t\tType: " << type << endl;
    cout << "\t\tSRC IP: " << convertIntToIPAddressString(htonl(srcIP)) << endl;
    cout << "\t\tSRC Port: " << srcPort << endl;
    cout << endl;
}





// ***************   PRIVATE CONSTRUCTORS **************

HelloPacket::HelloPacket(const HostAndPort & srcHap):
    type(HELLO_STATE_PACKET_CHAR),
    srcIP(srcHap.getIP()),
    srcPort(srcHap.getPort())
{
    assemblePacketString();
}

HelloPacket::HelloPacket(const char* rawString){
    //EXTRACT INFORMATION FROM THE C_STRING
       
    type = rawString[0];
    srcIP = ntohl(*(int *)(rawString + 1));
    srcPort = ntohs(*(short *)(rawString + 5));
    
    assemblePacketString();
}





// ***************  OTHER PRIVATE FUNCTIONS **************

void HelloPacket::assemblePacketString(){
    
    packet_as_str = (char *)malloc(HELLO_PACKET_HEADER_LENGTH_IN_BYTES + 1);
    
    //place the type
    packet_as_str[0] = type;
    
    //place the src IP address
    int* int_ptr = (int *)(packet_as_str + 1);
    *int_ptr = htonl(srcIP);
    
    //place the src 16-bit port
    short* short_ptr = (short *)(packet_as_str + 5);
    *short_ptr = htons(srcPort);    

    packet_as_str[HELLO_PACKET_HEADER_LENGTH_IN_BYTES] = '\0';
}

