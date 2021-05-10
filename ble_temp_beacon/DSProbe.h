/*
  DSProbe.h - Class for interacting with a ds18b20 temperature probe
*/
#ifndef DSProbe_h
#define DSProbe_h

#include "ThermometerProbe.h"

// For reading ds18b20 probes
#include <OneWire.h>


class DSProbe : public ThermometerProbe {

  public:
    DSProbe();
    virtual void begin();
    virtual float getTemp();
    virtual void resetError();
    virtual bool isProbeError();
    virtual ReadTemperatureResult readTemperature();
  private:
    OneWire ds;
     
};


#endif
