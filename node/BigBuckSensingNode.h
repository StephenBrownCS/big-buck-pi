#ifndef BIG_BUCK_SENSOR_H_
#define BIG_BUCK_SENSOR_H_

#include "Logger.h"

//Forward References
class Communicator;
class Sensor;
class Packet;

// An exception which gets thrown when the name server notifies this node of a 
// new master that just connected
class MasterResetException{};

class BigBuckSensingNode{
public:
    // Named Constructor
    static BigBuckSensingNode* create( Logger& logger_, Communicator* communicator_, Sensor* sensor_, short id_ );
    ~BigBuckSensingNode();

    void sensingLoop();
    
private:
    Communicator* communicator;
    Sensor* sensor;
    Logger& logger;
    short id;
    int nextSequence;
    
    // Sends a hello packet to the master, used for when haven't sensed something 
    // in a while
    void sendHelloPacket();
    
    // For handling packets received from the master or name server
    void handleReceivedPacket(Packet* pkt);
    
    // Private Ctor, use named constructors instead
    BigBuckSensingNode( Logger & logger, Communicator* communicator_, Sensor* sensor_, short id_ );
    
    void sendSensorState( int currentState);
    
    // No copy or assignment
    BigBuckSensingNode & operator=( const BigBuckSensingNode & );
    BigBuckSensingNode( const BigBuckSensingNode & );
};

#endif
