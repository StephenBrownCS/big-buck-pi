#ifndef BIG_BUCK_SENSOR_H_
#define BIG_BUCK_SENSOR_H_

#include "Logger.h"

//Forward References
class Communicator;
class Sensor;

// An exception which gets thrown when the name server notifies this node of a 
// new master that just connected
class MasterResetException{};

class BigBuckSensingNode{
public:
    // Named Constructor
    static BigBuckSensingNode* create( Logger& logger_, Communicator* communicator_, Sensor* sensor_, short id_ );
    
    void sensingLoop();
    
    ~BigBuckSensingNode();
private:
    Communicator* communicator;
    Sensor* sensor;
    Logger& logger;
    short id;
    int nextSequence;
    
    // Private Ctor, use named constructors instead
    BigBuckSensingNode( Logger & logger, Communicator* communicator_, Sensor* sensor_, short id_ );
    
    void sendSensorState( int currentState);
    
    // No copy or assignment
    BigBuckSensingNode & operator=( const BigBuckSensingNode & );
    BigBuckSensingNode( const BigBuckSensingNode & );
};

#endif
