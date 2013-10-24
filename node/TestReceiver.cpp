#include <iostream>
#include "ListeningSocket.h"
#include "Packet.h"
#include "HelloPacket.h"
#include "Utility.h"
#include "HostAndPort.h"
//#include <chrono>
//#include <thread>

//using std::this_thread::sleep_for;
//using std::chrono::milliseconds;

using namespace std;

int main(){
    try{
        const int PORT = 55003;
        ListeningSocket listenSock( PORT );
        
        unsigned int ownIPAddress = getOwnIPAddress();
        cout << "Own IP Address: " << convertIntToIPAddressString(ownIPAddress) << endl;
        
        while( true ){
            if (listenSock.isPacketWaiting() ){
                Packet* pkt = listenSock.receivePacket();
                cout<<"Received!"<<endl;
            }
            sleep(1);
        }
    }
    catch(Error & e){
        cout << "Error!" << endl;
        cout << e.getMsg() << endl;
    }
    return 0;
}
