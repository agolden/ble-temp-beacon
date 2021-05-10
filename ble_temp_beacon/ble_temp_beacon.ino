// For reading temperature probe
// #include "RTDProbe.h"
#include "DSProbe.h"

// For broadcasting sensor state via Bluetooth
#include <ArduinoBLE.h>

// For displaying temperature readings on the 7-segment display
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"

typedef enum {
  RESET_PIN = 6
} DigitalPin;

Adafruit_7segment matrix = Adafruit_7segment();

BLEService kettleService("256B");
BLEFloatCharacteristic tempCharacteristic("bd5b8257-9668-4525-95d3-04e19c7b96f2",  // Standard 16-bit Temperature characteristic
    BLERead | BLENotify | BLEBroadcast); // Remote clients can read and get updates

long previousMillis = 0;
int waitingConnectCount = 0;
int faultCount = 0;

// RTDProbe rtdProbe;
DSProbe dsProbe;

// We've decided to use the ds18b20 probe, for now
// ThermometerProbe *probe = &rtdProbe;
ThermometerProbe *probe = &dsProbe;

void setup() {
  Serial.begin(115200);
  
  // Prepare to reset
  digitalWrite(RESET_PIN, HIGH);
  delay(200);
  pinMode(RESET_PIN, OUTPUT);
  // To reset: digitalWrite(RESET_PIN, LOW);

  // Start bluetooth
  if (!BLE.begin())
  {
    // Reset the device if starting bluetooth failed
    Serial.println("starting BLE failed!");
    digitalWrite(RESET_PIN, LOW);
  }

  // Service setup
  BLE.setLocalName("SmartKettle");
  BLE.setAdvertisedService(kettleService);
  kettleService.addCharacteristic(tempCharacteristic);
  BLE.addService(kettleService);

  // Set and broadcast the initial value
  tempCharacteristic.setValue(0);
  tempCharacteristic.broadcast();
  BLE.advertise();

  probe->begin();
  matrix.begin(0x70);
  matrix.setBrightness(3);
  matrix.blinkRate(3);
}

bool faultCheck() {
  // If there's a fault, try up to five times before resetting the device
  if (probe->isProbeError()) {
    faultCount++;
    if (faultCount > 5) {
      digitalWrite(RESET_PIN, LOW);
    } else {
      delay(1000);
    }
    return true;
  } else {
    faultCount = 0;
    return false;
  }
}

void loop() {

  // wait for a BLE central
  BLEDevice central = BLE.central();

  if (central) {
    waitingConnectCount = 0;
    while (central.connected()) {
      long currentMillis = millis();
      // if 1000ms have passed, check the battery level:
      if (currentMillis - previousMillis >= 1000) {
        previousMillis = currentMillis;

        // Get the temp
        float temp = probe->getTemp();

        // If there's a fault, clear the error, wait a second, and try again
        if (faultCheck()) {
          probe->resetError();
          //thermo.clearFault();
          delay(1000);
          matrix.clear();
          matrix.writeDisplay();
          return;
        }

        // If we reach here, there's no fault
        Serial.println(temp);
        float testme = 175.2384;
    
        float TempF = 0;
        TempF = (temp*1.8)+32;
        matrix.print(TempF);
        matrix.writeDisplay();

        tempCharacteristic.writeValue(temp);
      }
    }
  } else {
    matrix.clear();
    matrix.writeDisplay();
    long currentMillis = millis();
    if (currentMillis - previousMillis >= 1000) {
      previousMillis = currentMillis;

      waitingConnectCount++;
      Serial.println(waitingConnectCount);

      // Reset after 15 seconds without a connection
      if (waitingConnectCount > 15) {
        digitalWrite(RESET_PIN, LOW);
      }
    }
  }
}
