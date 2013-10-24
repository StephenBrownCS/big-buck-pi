#include <stdlib.h>
#include <arpa/inet.h>

#include "HostAndPort.h"
#include "utility.h"

using namespace std;


unsigned int HostAndPort::getIP() const{
    return htonl(ipAddressStrToLong(ipAddr));
}

ostream& operator<< (ostream& os, const HostAndPort& hap){
    os << hap.getIPAsStr() << "," << hap.getPort();
    return os;
}

istream& operator>> (istream& is, HostAndPort & hap){
    string entry;
    is >> entry;
    
    // Get position of comma, and split string based on that
    int comma_index = entry.find_first_of(',');
    if (comma_index == string::npos){
        return is;
    }
    
    string ip_addr = entry.substr(0, comma_index);
    string port_str = entry.substr(comma_index + 1, entry.length() - comma_index - 1);
    
    // Update the host and port
    hap.setIPStr(ip_addr);
    hap.setPort(atoi(port_str.c_str()));
    
    return is;
}
