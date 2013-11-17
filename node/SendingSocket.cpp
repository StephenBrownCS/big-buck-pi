/**
 * Creates a socket for sending packets.
 *
 * <p>Bugs: none known
 *
 * @author Stephen Brown and Brian Hunter
 */

#include <stdlib.h>
#include <iostream>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>
#include <netdb.h>
#include <signal.h>
#include <unistd.h>

#include "SendingSocket.h"
#include "Packet.h"
#include "Utility.h"

using namespace std;


SendingSocket::SendingSocket(long destinationIPAddress, int destinationPortNumber){
    setupSocketConnection(destinationIPAddress, destinationPortNumber);
}


SendingSocket::SendingSocket(std::string hostName, int destinationPortNumber){
    long destinationIPAddress = getIPAddressForHostname(hostName);
    setupSocketConnection(destinationIPAddress, destinationPortNumber);
}

SendingSocket::~SendingSocket(){
    close(sock);
    delete dest_sockaddr;
}

void SendingSocket::setupSocketConnection(long destinationIPAddress, int destinationPortNumber){
    // THIS PREVENTS SOME ERRORS
    signal(SIGPIPE, SIG_IGN);
    
    // Create the socket  
    if ((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1){
        throw Error("socket creation error");
    }
       
    // Set up the dest_sockaddr struct and slen
    dest_sockaddr = new sockaddr_in();
    dest_sockaddr_size = sizeof(*dest_sockaddr);
    memset((char *) dest_sockaddr, 0, dest_sockaddr_size);
    dest_sockaddr->sin_family = AF_INET;
    dest_sockaddr->sin_port = htons(destinationPortNumber);
    dest_sockaddr->sin_addr.s_addr = destinationIPAddress;
}

void SendingSocket::sendPacket(const Packet * p){  
    if (sendto(sock, p->c_str(), p->c_str_length(), 0, (sockaddr *) dest_sockaddr, (socklen_t) dest_sockaddr_size) == -1){
        throw Error("sendto error in SendingSocket::sendPacket");
    }
}
