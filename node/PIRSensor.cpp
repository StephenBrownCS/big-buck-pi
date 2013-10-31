#include <iostream>
#define BOARD RASPBERRY_PI
#include "gnublin.h"
#include "PIRSensor.h"

using namespace std;

const int PIR_INPUT = 7;

PIRSensor* PIRSensor::create(){
    return new PIRSensor();
}

PIRSensor::PIRSensor(){
    gpio = new gnublin_gpio();

    gpio->pinMode( PIR_INPUT, INPUT );

    cout<<"Waiting to settle"<<endl;
    while( gpio->digitalRead( PIR_INPUT) ){
       sleep(1);
    }
    cout<<"Ready"<<endl;
}

PIRSensor::~PIRSensor(){
    delete gpio;
}
    
bool PIRSensor::getCurrentState(){
    return gpio->digitalRead( PIR_INPUT );
}
