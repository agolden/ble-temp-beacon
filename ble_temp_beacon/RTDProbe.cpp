#include "RTDProbe.h"

// The value of the Rref resistor. Use 430.0 for PT100 and 4300.0 for PT1000
#define RREF      430.0
// The 'nominal' 0-degrees-C resistance of the sensor
// 100.0 for PT100, 1000.0 for PT1000
#define RNOMINAL  100.0

void RTDProbe::begin() {
  thermo.begin(MAX31865_3WIRE);
}


float RTDProbe::getTemp() {
  uint16_t rtd = thermo.readRTD();

  // Serial.print("RTD value: "); Serial.println(rtd);
  float ratio = rtd;
  ratio /= 32768;
  return thermo.temperature(RNOMINAL, RREF);
}

bool RTDProbe::isProbeError() {

  return thermo.readFault();
}

void RTDProbe::resetError() {
  thermo.clearFault();
}

ReadTemperatureResult RTDProbe::readTemperature() {
  ReadTemperatureResult result;
  result.temperature = getTemp();
  result.isError = isProbeError();
  if (result.isError) {
    resetError();
  }
  return result;
}

