#ifndef BIG_BUCK_SENSOR_H_
#define BIG_BUCK_SENSOR_H_

//Forward References
class Communicator;
class Sensor;

class BigBuckSensingNode{
public:
    // Named Constructor
    static BigBuckSensingNode* create( Communicator* communicator_, Sensor* sensor_, short id_ );
    
    void sensingLoop();
    
    ~BigBuckSensingNode();
private:
    Communicator* communicator;
    Sensor* sensor;
    short id;
    int nextSequence;
    
    // Private Ctor, use named constructors instead
    BigBuckSensingNode( Communicator* communicator_, Sensor* sensor_, short id_ );
    
    void sendSensorState( int currentState);
    
    // No copy or assignment
    BigBuckSensingNode & operator=( const BigBuckSensingNode & );
    BigBuckSensingNode( const BigBuckSensingNode & );
};

#endif
