#include <ArduinoBLE.h>
#include "config.h"

BLEService customService(BLUETOOTH_SERVICE_IDENTIFIER); // Custom BLE service
BLEUnsignedIntCharacteristic customCharacteristic(BLUETOOTH_SERVICE_CHARACTERISTIC_IDENTIFIER, BLUETOOTH_SERVICE_CHARACTERISTIC_CONTENT); // Custom characteristic

void setup() {
  Serial.begin(DEBUG_SERIAL_CONNECTION_SPEED);
  while (!Serial);

  if (!BLE.begin()) {
    Serial.println("Starting BLE failed!");
    while (1);
  }

  BLE.setLocalName(BLUETOOTH_DEVICE_LOCAL_NAME); // Name the device
  BLE.setAdvertisedService(customService); // Advertise the custom service

  customService.addCharacteristic(customCharacteristic); // Add the characteristic
  BLE.addService(customService); // Add the service

  customCharacteristic.writeValue(INIT_BLUETOOTH_SERVICE_CHARACTERISTIC_VALUE); // Initial value

  BLE.advertise();
  Serial.println("Bluetooth device active, waiting for connections...");
}

void loop() {
  BLEDevice central = BLE.central(); // Wait for a BLE central to connect

  if (central) {
    Serial.println("Connected to central");

    while (central.connected()) {
      if (customCharacteristic.written()) {
        unsigned int value = customCharacteristic.value();
        Serial.print("Received new value: ");
        Serial.println(value);
      }
    }

    Serial.println("Disconnected from central");
  }
}
