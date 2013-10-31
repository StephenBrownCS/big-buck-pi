#ifndef PIR_SENSOR_H_
#define PIR_SENSOR_H_

#include "Sensor.h"

// Forward Reference
class gnublin_gpio;

class PIRSensor : public Sensor{
public:
    static PIRSensor* create();
    bool getCurrentState();
    ~PIRSensor();
private:
    PIRSensor();
    gnublin_gpio* gpio;
    
    // No copy or assignment
    PIRSensor( const PIRSensor & );
    PIRSensor & operator=(const PIRSensor & );
};

#endif
