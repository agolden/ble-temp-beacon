#include "DSProbe.h"

DSProbe::DSProbe()
{
    ds = OneWire(10);
}

void DSProbe::begin() {
}

float DSProbe::getTemp() {

  byte i;
  byte present = 0;
  byte type_s = 0;
  byte data[12];
  byte addr[8];

  // Searches for a thermometer attached to this arduino
  // Let's assume that there's only one and reset the search each time
  ds.reset_search();
  ds.search(addr);
  delay(1000);
  
  /*if ( !ds.search(addr)) {
      Serial.print("No more addresses.\n");
      ds.reset_search();
      delay(1000);
      return;
  }*/
  
  ds.reset();
  ds.select(addr);
  ds.write(0x44, 1);

  delay(1000);

  present = ds.reset();
  ds.select(addr);    
  ds.write(0xBE);

  for ( i = 0; i < 9; i++) {           // we need 9 bytes
    data[i] = ds.read();
  }
  
  int16_t raw = (data[1] << 8) | data[0];
  if (type_s) {
    raw = raw << 3; // 9 bit resolution default
    if (data[7] == 0x10) {
      // "count remain" gives full 12 bit resolution
      raw = (raw & 0xFFF0) + 12 - data[6];
    }
  } else {
    byte cfg = (data[4] & 0x60);
    // at lower res, the low bits are undefined, so let's zero them
    if (cfg == 0x00) raw = raw & ~7;  // 9 bit resolution, 93.75 ms
    else if (cfg == 0x20) raw = raw & ~3; // 10 bit res, 187.5 ms
    else if (cfg == 0x40) raw = raw & ~1; // 11 bit res, 375 ms
    //// default is 12 bit resolution, 750 ms conversion time
  }
  
  return (float)raw / 16.0;
}

ReadTemperatureResult DSProbe::readTemperature() {
  ReadTemperatureResult result;
  return result;
}

bool DSProbe::isProbeError() {

  return false;
}

void DSProbe::resetError() {
  
}



