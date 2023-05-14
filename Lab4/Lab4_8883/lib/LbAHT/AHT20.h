// AHT20.h
// AHT20 library for Arduino

#ifndef AHT20_h
#define AHT20_h

#include <Arduino.h>
#include <Wire.h> 

class AHT20{
    private:
        bool startSensor();
    public:
        void begin();
        bool getSensors(float *h, float *t);        
        bool getTemperature(float *t);
        bool getHumidity(float *h);
};

#endif