
#include "PacketFactory.h"
#include "LinkStatePacket.h"
#include "HelloPacket.h"
#include "Utility.h"

using namespace std;

Packet* PacketFactory::createPacket(char* rawString){
    switch(rawString[0]){
        case 'L':
            return LinkStatePacket::create(rawString);
            break;
        case 'H':
            return HelloPacket::create(rawString);
            break;
        default:
            throw Error(string("Unknown Packet submitted to the factory"));
            return 0;
            break;
    }
}
