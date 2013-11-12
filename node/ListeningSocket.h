/*
    LISTENING SOCKET
    
    This class is an abstraction of a UDP socket meant for receiving packets
    Its constructor specifies a port to listen on.
    Packets can be received by receivePacket() - these are custom "UDP Packets"
    which are our own custom emulation of a real UDP Packet.
*/

#ifndef LISTENING_SOCKET_H_
#define LISTENING_SOCKET_H_

// Forward References
class UDPPacket;

// When thrown, signifies a TimeoutException
struct SocketTimeoutException{};

class ListeningSocket{
    public:
        //Constructor
        //Creates Listening Socket to listen at portNumber
        ListeningSocket(int portNumber);
        
        // closes the socket
        ~ListeningSocket();
        
        // closes the socket
        void closeSocket();
        
        //Receive a single Packet
        //timeoutInMilliSeconds specifies how long the socket should listen for before giving up
        //if timeoutInMilliSeconds is 0, it will block until it receives
        UDPPacket* receivePacket(double timeoutInMilliSeconds = 0);
        //IP_Packet receive_IP_Packet(double timeoutInMilliSeconds = 0);
        //ICMP_Packet* receive_ICMP_Packet(double timeoutInMilliSeconds = 0);
        
        // Checks in a non-blocking way to see if a packet is ready to be received
        // Returns true iff there is one ready
        bool isPacketWaiting();

        // Returns the source IP Address
        // Requires: receivePacket must have been called at least once
        int getSourceIPAddress() const { return sourceIPAddress; }
        
    private:
        //Socket Descriptor
        int sock;

        //IP Address of Source (valid after receivePacket is called)
        int sourceIPAddress;
        
        //Used by receive UDP/IP Packets
        //Waits for a c-string to arrive over the socket and places it in buffer
        void receive_c_string(char * buffer, int timeoutInSeconds);
               
        //No Copy or Assignment
        ListeningSocket(const ListeningSocket &);
        ListeningSocket & operator=(const ListeningSocket &);
};

#endif
