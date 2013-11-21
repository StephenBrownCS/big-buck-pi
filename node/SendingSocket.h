/*
    SENDING SOCKET

    This class in an abstraction of the a UDP socket meant for sending data
    The constructor takes a hostname and destination port number as arguments
    
    Packets can be sent one by one using sendPacket() 

*/

#ifndef SENDING_SOCKET_H_
#define SENDING_SOCKET_H_

#include <string>
#include <stdlib.h>

// Forward References
class Packet;
struct sockaddr_in;

class SendingSocket{
public:
    //CONSTRUCTORS
    //Creates a sending socket for a specified destination IP and port
    // destinationIPAddress must be in network-byte order
    SendingSocket(long destinationIPAddress, int destinationPortNumber);
    
    //Creates a sending socket for a specified host and port
    SendingSocket(std::string hostName, int destinationPortNumber);

    // closes the socket
    ~SendingSocket();

    // Sends a single packet
    void sendPacket(const Packet * p);
    
private:
    // Initialization function used by constructors
    void setupSocketConnection(long destinationIPAddress, int destinationPortNumber);
    
    //No Copy or Assignment
    SendingSocket(const SendingSocket &);
    SendingSocket & operator=(const SendingSocket &);
    
    //Socket Descriptor Number
    int sock;

    // SockAddr Structure which configures the sendto function
    sockaddr_in* dest_sockaddr;

    // Size of Sockaddr Structure, used by sendto
    int dest_sockaddr_size;
};

#endif
