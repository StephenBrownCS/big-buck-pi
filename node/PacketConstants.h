/*
    Common constants used by packet users
*/


#ifndef PACKET_CONSTANTS_H_
#define PACKET_CONSTANTS_H_

// Types of BigBuckPackets (first byte)
extern const char PKT_LETTER_REGISTRATION;
extern const char PKT_LETTER_ACK;
extern const char PKT_LETTER_HEARTBEAT;
extern const char PKT_LETTER_MASTER;

// Placeholders for default args to pass into packet constructors
extern const int DEFAULT_NODE_ID;
extern const int NO_SEQUENCE;
extern const int NO_PAYLOAD;
extern const char* EMPTY_PAYLOAD;

#endif