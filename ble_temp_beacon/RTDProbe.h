/*
  RTDProbe.h - Class for interacting with a RTD-type temperature probe
*/
#ifndef RTDProbe_h
#define RTDProbe_h

#include "ThermometerProbe.h"

// For reading RTD probes
#include <Adafruit_MAX31865.h>

class RTDProbe : public ThermometerProbe {
  public:
    Adafruit_MAX31865 thermo = Adafruit_MAX31865(7, 8, 9, 10);
    virtual float getTemp();
    virtual void begin();
    virtual void resetError();
    virtual bool isProbeError();
    virtual ReadTemperatureResult readTemperature();
};

#endif
