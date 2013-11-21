/*
    HostAndPort class
    Represents an IP address and a port. Has methods to
    access the IP Address as a raw number for use with sockets,
    and also to get it as a .-separated string.
    
    Has the operators ==, !=, <, <<, and >> overridden
    Has a HostAndPort_Comparator class as well

*/



#ifndef HostAndPort__H__
#define HostAndPort__H__

#include <iostream>
#include <arpa/inet.h>

#include "Utility.h"

// Defines a point of contact in a network
// A host and port, used for communicating with RoutingTable
class HostAndPort{
public:
    // Constructor from an IP .-separated string and a port
    HostAndPort(std::string ipAddr_ = "", unsigned short port_ = 0):
        ipAddr(ipAddr_), port(port_) {}
        
    // Constructor from an unsigned int IP
    // ip and port must both be in network byte order
    HostAndPort(unsigned int ip, unsigned short port_){
        ipAddr = convertIntToIPAddressString(ntohl(ip));
        port = port_;
    }

    // Getters and Setters for IP as a .-separated IPv4 string in host order
    std::string getIPAsStr() const{ return ipAddr; }
    void setIPStr(std::string ipAddr_) { ipAddr = ipAddr_; }
    
    // Getter for IP as an unsigned int in network-byte order
    unsigned int getIP() const;
    
    // Takes in IP in network-byte order
    void setIP(long ip){ ipAddr = convertIntToIPAddressString(ntohl(ip)); }
    
    // Getters and Setters for short port
    unsigned short getPort() const { return port; }
    void setPort(unsigned short port_){ port = port_; }

    bool operator== (HostAndPort other) const{
        return other.ipAddr == ipAddr && other.port == port;
    }
    
    bool operator!= (HostAndPort other) const{
        return !(*this == other);
    }
    
    bool operator< (HostAndPort other) const{
        return ipAddr < other.ipAddr ? 1 : port < other.port;
    }

private:
    std::string ipAddr; /// Always in the correct human-readable order
    unsigned short port;
};



// HELPER FUNCTION OBJECT FOR FORWARDING TABLE
// Comparator used to compare HostAndPorts
struct HostAndPort_Comparator{
    bool operator()(const HostAndPort & a, const HostAndPort & b) const{
        if (a.getIPAsStr() != b.getIPAsStr()){
            return a.getIPAsStr() < b.getIPAsStr();
        }
        else{
            return a.getPort() < b.getPort();
        }
    }
};


// Overloaded for convenience when reading/writing from/to a file,
// or from.to a string

// Output Stream Operator Overloaded
std::ostream& operator<< (std::ostream& os, const HostAndPort& hap);

//Input Stream Operator Overloaded
std::istream& operator>> (std::istream& is, HostAndPort & hap);


#endif
