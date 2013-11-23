
#ifndef UTILITY__H__
#define UTILITY__H__

#include <iostream>

const int MIN_PORT_NUM = 1025;  // Lowest valid port number
const int MAX_PORT_NUM = 65535; //Highest valid port number

//An Exception class
class Error {
public:
	Error(std::string msg_) : msg(msg_) {}
	std::string getMsg() const { return msg; }
private:
	std::string msg;
};


//Converts an IP address in integer form to IP dot notation as a string
std::string convertIntToIPAddressString(unsigned long ipAddress);

// Takes in a . separated IP Address and returns a long
// in network-byte order
unsigned long ipAddressStrToLong(std::string ipAddress);

// Returns the current time in millisecond granularity
std::string getCurrentTimeStr();

/**
 * Calculates current time in milliseconds.
 *
 * @return time in milliseconds
 */
int getMilliCount();

/**
 * Calculates the amount of time passed in milliseconds.
 *
 * @return difference in current time vs. start time
 */
 int getMilliSpan(int start);
 

// Returns the name of the host this program is running on
std::string getOwnHostName();


// Returns the IPAddress of the host this program is running on
// in network-byte order. To convert, use ntohl()
int getOwnIPAddress();

// Returns ip address of host in network-byte order.
// To convert, use ntohl()
long getIPAddressForHostname(std::string hostName);

//Returns the hostname of the ip Address
//ipAddress must be in network byte order
std::string getHostForIPAddress(int ipAddress);

}

#endif
