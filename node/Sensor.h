#ifndef SENSOR_H_
#define SENSOR_H_

class Sensor{
public:
    virtual bool getCurrentState() = 0;
};

#endif
