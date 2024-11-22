#include <ArduinoBLE.h>

// Create a BLE service and characteristic
BLEService dataService("19B10000-E8F2-537E-4F6C-D104768A1214");
BLEStringCharacteristic dataCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLENotify, 20);

void setup() {
  Serial.begin(9600);
  
  // Initialize BLE
  if (!BLE.begin()) {
    Serial.println("BLE initialization failed!");
    while (1);
  }

  // Set device name
  BLE.setLocalName("Arduino Nano 33 IoT");
  
  // Add service and characteristic
  BLE.setAdvertisedService(dataService);
  dataService.addCharacteristic(dataCharacteristic);
  BLE.addService(dataService);

  // Start advertising
  BLE.advertise();
  Serial.println("Bluetooth device active, waiting for connections...");
}

void loop() {
  BLEDevice central = BLE.central();
  
  if (central) {
    Serial.print("Connected to central: ");
    Serial.println(central.address());

    while (central.connected()) {
      // Example: Send a string value
      String data = "Hello Flutter!";
      dataCharacteristic.writeValue(data);
      delay(1000);
    }
    
    Serial.print("Disconnected from central: ");
    Serial.println(central.address());
  }
}
