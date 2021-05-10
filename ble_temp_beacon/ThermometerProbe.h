/*
  ThermometerProbe.h - Abstract class for interacting with a temperature probe
*/
#ifndef ThermometerProbe_h
#define ThermometerProbe_h

#include "Arduino.h"
#include "ReadTemperatureResult.h"

class ThermometerProbe {
  public:
    
    // Starts the probe; should be called once before one or more 'getTemp' readings
    virtual void begin();

    virtual ReadTemperatureResult readTemperature() = 0;
    
    // Reads the temperature, in Celsius, from the probe
    virtual float getTemp() = 0;  // Needs to be implemented by each subclass

    // Indicates whether or not the probe is in an error state (e.g., as a result of the most recent reading)
    virtual bool isProbeError() = 0;

    // Resets the probe  necessary 
    virtual void resetError();
    
    virtual ~ThermometerProbe() {}            // Destructor
};

#endif
