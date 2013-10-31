#include <chrono>
#include <thread>
#include "BigBuckSensingNode.h"
#include "BigBuckPacket.h"
#include "Sensor.h"
#include "Communicator.h"

using namespace std;

//using std::this_thread::sleep_for;
//using std::chrono::milliseconds;

const int BASE_STATION_ID = 7777;
const int POLLING_RATE = 100; //milliseconds

BigBuckSensingNode* BigBuckSensingNode::create( Communicator* communicator_, Sensor* sensor_, short id_ ){
    return new BigBuckSensingNode( communicator_, sensor_, id_ );
}

BigBuckSensingNode::BigBuckSensingNode(Communicator* communicator_, Sensor* sensor_, short id_):
    communicator( communicator_ ),
    sensor( sensor_ ),
    id( id_),
    nextSequence(0)
{
}

BigBuckSensingNode::~BigBuckSensingNode(){
    delete communicator;
    delete sensor;
}

void BigBuckSensingNode::sensingLoop(){
    int currentState = 0;
    int previousState = 0;
    while( true ){
        currentState = sensor->getCurrentState();
        if ( currentState != previousState ){
            sendSensorState( currentState );
        }
        previousState = currentState;
        sleep_for(milliseconds( POLLING_RATE ));
    }
}

void BigBuckSensingNode::sendSensorState( int currentState ){
    BigBuckPacket* pkt = BigBuckPacket::create( 'S', id, BASE_STATION_ID, nextSequence++, 0, 0 );
    communicator->sendPacket( pkt );
    delete pkt;
}

