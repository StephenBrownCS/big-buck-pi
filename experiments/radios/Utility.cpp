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
#include <cstring>
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
#include <ctime>

#ifndef __APPLE__
#include <netpacket/packet.h>
#endif
#include <net/ethernet.h>
#include <ifaddrs.h>
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

    return result;
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

unsigned long getOwnWlanIpAddress(){
    struct ifaddrs *ifaddr, *ifa;
   int family = 0;
   int s = 0;
   char host[NI_MAXHOST];

   if (getifaddrs(&ifaddr) == -1) {
       perror("getifaddrs");
       exit(EXIT_FAILURE);
   }

   /* Walk through linked list, maintaining head pointer so we
      can free list later */

   for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
       if (ifa->ifa_addr == NULL)
           continue;

       family = ifa->ifa_addr->sa_family;

       /* Display interface name and family (including symbolic
          form of the latter for the common families) */

        /*
       printf("%s  address family: %d%s\n",
               ifa->ifa_name, family,
               (family == AF_PACKET) ? " (AF_PACKET)" :
               (family == AF_INET) ?   " (AF_INET)" :
               (family == AF_INET6) ?  " (AF_INET6)" : "");
        */

       /* For an AF_INET* interface address, display the address */

       if (family == AF_INET || family == AF_INET6) {
           s = getnameinfo(ifa->ifa_addr,
                   (family == AF_INET) ? sizeof(struct sockaddr_in) :
                                         sizeof(struct sockaddr_in6),
                   host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);
           if (s != 0) {
               printf("getnameinfo() failed: %s\n", gai_strerror(s));
               exit(EXIT_FAILURE);
           }
           // printf("\taddress: <%s>\n", host);
           if ( strcmp(ifa->ifa_name, "wlan0") == 0  ){
                freeifaddrs(ifaddr);
	            return ipAddressStrToLong(host);
           } 
       }
   }

   freeifaddrs(ifaddr);
   
   throw Error("No wlan0 Address");
   return -1;
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
    return result;
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

const char* getCurrentHumanReadableTime(){
  time_t now;
  time(&now);
 
  return ctime(&now);
}

