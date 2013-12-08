/*
    Represents a sensing node, to be run on a Raspberry Pi equipped with 
    Wifi access to a master node, RF radios to communicate to other pis, and 
    a sensor for detecting motion.
    
    Originally, the design was to abstract away the communication module for cases 
    when we don't want to communicate over wifi. However, as it became clear 
    we would not do packet-based communication over RF radios, we moved away from that, 
    and this module would need refactoring if we decide to pursue that again. The 
    main dependencies on Wifi include the logic pertaining to turning the wifi
    radios on and off, based on sensor activity and received RF signals.
*/

#ifndef BIG_BUCK_SENSOR_H_
#define BIG_BUCK_SENSOR_H_

#include "Logger.h"

//Forward References
class Communicator;
class Sensor;
class Packet;
class UDPPacket;
class RFRadio;

// An exception which gets thrown when the name server notifies this node of a 
// new master that just connected
class MasterResetException{};

class BigBuckSensingNode{
public:
    // Named Constructor
    static BigBuckSensingNode* create( Logger& logger_, Communicator* communicator_, Sensor* sensor_, short id_ );
    ~BigBuckSensingNode();

    // The main loop of this module which will sense for activity, and send 
    // messages to the master node, turning wifi on and off as needed
    void sensingLoop();
    
    // Turn the Wifi Driver and wlan0 interface on using system commands to 
    // ifup and ifconfig. If any operation fails, retries the operation a few times. 
    // If unable to succeed after several tries, throws an Error
    // Upon success, will set the wifiRadioIsOn flag to true
    void turnWifiRadioOn();
    
    // Turn the Wifi Driver off using a system command to ifdown. Retries a few 
    // times if the operation fails. If unable to succeed after several tries, 
    // throws an Error
    // Upon success, will set the wifiRadioIsOn flag to false
    void turnWifiRadioOff();
    
private:
    // An abstract thing representing a means of communication to other nodes
    // The idea is that we could substitute in a radio-based packet solution instead 
    // of Wifi packets
    Communicator* communicator;
    
    // An abstract thing representing some sort of sensor - currently the only 
    // one we have is a passive infrared sensor
    Sensor* sensor;
    
    // A logging module used for both printing to the screen and writing to a log file
    Logger& logger;
    
    // The id of this node, as assigned from the name server
    short id;
    
    // The next sequence number to send in a packet
    int nextSequence;
    
    // This node may turn off its radio if it hasn't sensed any activity in 
    // some time - this flag indicates whether it is currently on or not
    bool wifiRadioIsOn;
    
    // The radio module, used for signaling to other pis that they should wake up
    // their wifi radios
    RFRadio* rfRadio;
    
    // Sends a hello packet to the master, used for when haven't sensed something 
    // in a while
    void sendHelloPacket();
    
    // For handling packets received from the master or name server
    void handleReceivedPacket(UDPPacket* udpPkt);
    
    // Private Ctor, use named constructors instead
    BigBuckSensingNode( Logger & logger, Communicator* communicator_, Sensor* sensor_, short id_ );
    
    // Send the current sensor state to the master
    // If currentState is nonzero, will also send a radio pulse
    // It will send the wifi packet immediately, if its own wifi radio is on.
    // Otherwise, it will turn on its wifi radio, wait some time, and then 
    // send the wifi packet
    void sendSensorState( int currentState);
    
    // No copy or assignment
    BigBuckSensingNode & operator=( const BigBuckSensingNode & );
    BigBuckSensingNode( const BigBuckSensingNode & );
};

#endif
