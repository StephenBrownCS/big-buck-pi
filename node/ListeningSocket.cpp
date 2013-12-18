/**
 * Creates a socket for listening based off of a given port.
 *
 * @author Stephen Brown
 */

#include <strings.h>
#include <stdlib.h>
#include <iostream>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <signal.h>
#include <unistd.h>

#include "ListeningSocket.h"
#include "Packet.h"
#include "UDPPacket.h"
#include "Utility.h"
#include "UDPPacket.h"

const int MAX_PACKET_SIZE = 1024 + 9;
using namespace std;

/**
 * Constructor for the Listening Socket
 *
 * @param portNumber: port to listen on
 * 
 */
ListeningSocket::ListeningSocket(int portNumber){
    // THIS PREVENTS SOME ERRORS
    signal(SIGPIPE, SIG_IGN);
    
    //GET A SOCKET
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP); 
    if(sock < 0){
        throw Error("Socket Creation Problem in ListeningSocket()");
    }
    
    //MAKE PORT NUMBER REUSABLE
    unsigned int option = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &option, sizeof (option)) == -1){
        throw Error("setsockopt error");
    }
    
    //BIND SOCKET TO PORT NUMBER
    sockaddr_in addr;
    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(portNumber);  
    if ( bind (sock,(struct sockaddr *) &addr, sizeof(addr)) < 0){
        throw Error("Local port already in use");
    }
    
    //GET PORT NUMBER IF WE HAD SYSTEM ASSIGN IT
    if(portNumber == 0){
        socklen_t length = (socklen_t)sizeof addr;
        if (getsockname(sock,(sockaddr *) &addr, &length) < 0){
            throw Error("getsockname didn't work");
        }
    }
    portNumber = htons(addr.sin_port);
    cout << "Listening on Port " << portNumber << endl;
    
    //FINISH CONFIGURING THE SOCKET
    listen(sock, 10);
}


ListeningSocket::~ListeningSocket(){
    closeSocket();
}

void ListeningSocket::closeSocket(){
    close(sock);
}


bool ListeningSocket::isPacketWaiting(){
    fd_set socks;
    timeval t;
    FD_ZERO(&socks);
    FD_SET(sock, &socks);
       
    t.tv_usec = 1;   
    t.tv_sec = 0;
    
    // Select will wait for t.tv_usec microseconds
    // Return true if select detects a packet
    // False otherwise
    if (select(sock + 1, &socks, NULL, NULL, &t)){
        return true;
    }
    else{
        return false;
    }
}



UDPPacket* ListeningSocket::receivePacket(double timeoutInMilliSeconds){
    char packetHeader[MAX_PACKET_SIZE];
    receive_c_string(packetHeader, static_cast<int>(timeoutInMilliSeconds * 1000)); //timeout is in milliseconds
    return UDPPacket::create(packetHeader);
}



/** 
    Receive a C-string from the socket (don't abstract it into a packet yet)
    It is the job of other functions to figure out what this c-string is
*/
void ListeningSocket::receive_c_string(char* buffer, int timeoutInMicroSeconds){
    fd_set socks;
    timeval t;
    FD_ZERO(&socks);
    FD_SET(sock, &socks);
       
    t.tv_usec = timeoutInMicroSeconds % 1000000;   
    t.tv_sec = timeoutInMicroSeconds / 1000000;
    
    // If timeoutInSeconds is zero, that means wait until packet comes
    // If timeout specified, select() will wait until the timeout is reached
    if (timeoutInMicroSeconds != 0 && ! select(sock + 1, &socks, NULL, NULL, &t)){
        throw SocketTimeoutException();
    }

    // RECEIVE THE DATA
    sockaddr_in si_other;
    socklen_t slen = sizeof(si_other);

    if ( recvfrom(sock, buffer, MAX_PACKET_SIZE, 0, (sockaddr*) &si_other, &slen) < 0){
        throw Error("recvfrom didn't work ");
    }

    // RECORD THE SOURCE IP ADDRESS
    sourceIPAddress = si_other.sin_addr.s_addr;
    
}



