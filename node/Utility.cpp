/**
 * Used to obtain information when printing receipts, specifically time
 * and IP address.
 *
 * <p>Bugs: none known
 *
 * @author Stephen Brown and Brian Hunter
 */
#include <iostream>

#include <cmath>
#include <stdlib.h>
#include <string>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <stdio.h>
#include <netdb.h>
#include <sys/timeb.h>
#include <unistd.h>

#include "Utility.h"

using namespace std;

/**
 * Converts an ip address in int form to familiar string form.
 *
 * @param ipAddress: ip address in network-byte order
 * @return ip address in string form
 */
string convertIntToIPAddressString(unsigned long ipAddress){
    in_addr temp;
    temp.s_addr = ipAddress;
    return string(inet_ntoa(temp));
}


unsigned long ipAddressStrToLong(std::string ipAddress){
    int previousPeriodLocation = -1;
    int numProcessed = 0;
    unsigned long result = 0;
    for(int i = 0; i < ipAddress.length() && numProcessed < 3; i++){
        if(ipAddress[i] == '.'){
            string num = ipAddress.substr(previousPeriodLocation + 1,
                            i - previousPeriodLocation - 1);
            result += atoi(num.c_str()) * pow(256.0, 3 - numProcessed);
            previousPeriodLocation = i;
            numProcessed++;
        }
    }
    // Add the last number
    string last = ipAddress.substr(previousPeriodLocation + 1, 4);
    result += atoi(last.c_str());

    return ntohl(result);
}



/**
 * Gives current system time in string form.
 *
 * @return system time
 */
string getCurrentTimeStr(){
    timeval curTime;
    gettimeofday(&curTime, NULL);
    int milli = curTime.tv_usec / 1000;

    char buffer [80];
    strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", localtime(&curTime.tv_sec));

    char currentTime[84] = "";
    sprintf(currentTime, "%s:%d", buffer, milli);
    return string(currentTime);
}

// Returns the name of the host this program is running on
string getOwnHostName(){
    char hostname[1024];
    hostname[1023] = '\0';
    gethostname(hostname, 1023);
    
    string host(hostname);
    // Trim off the .cs.wisc.edu if it is part of hostName
    int pos = host.find(".cs.wisc.edu");
    if (pos != string::npos)
	    host = host.substr(0,pos);
    
    return host;
}

int getOwnIPAddress(){
    return getIPAddressForHostname(getOwnHostName());
}

long getIPAddressForHostname(string hostName){
    // PLACE HOST NAME IN A HOSTENT STRUCT
    hostent* hp = gethostbyname(hostName.c_str());
    if (! hp){
        throw Error("Invalid Host Name!");
    }
    //in_addr addr;
    //addr.s_addr = *(int *) hp->h_addr_list[0];
    //return addr.s_addr;
    int result = 0;
    result = *(int *)hp->h_addr_list[0];
    return ntohl(result);
}



string getHostForIPAddress(int ipAddress){
    sockaddr_in sa;
    in_addr address;
    address.s_addr = ipAddress;  
    sa.sin_addr = address;
    sa.sin_port = 0;
    sa.sin_family = AF_INET;
    char hostName [200];
    char service [100];
    getnameinfo((struct sockaddr*)&sa, 16, &hostName[0], 200, &service[0], 100, 0);
    
    string host(hostName);
    
    // Trim off the .cs.wisc.edu if it is part of hostName
    int pos = host.find(".cs.wisc.edu");
    if (pos != string::npos)
	    host = host.substr(0,pos);
    
    return host;
}
    
    
int getMilliCount(){
    timeb tb;
    ftime(&tb);
    int nCount = tb.millitm + (tb.time & 0xffffff) * 1000;
    return nCount;
}

int getMilliSpan(int start){
    int nSpan = getMilliCount() - start;
    if(nSpan < 0)
        nSpan += 0x100000 * 1000;
    return nSpan;
}

