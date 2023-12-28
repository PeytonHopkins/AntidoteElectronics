#include <ArduinoBLE.h>
#include <Arduino_BMI270_BMM150.h>

BLEService mouseService("1812"); // Mouse service

BLECharacteristic mouseCharacteristic("2A32", BLERead | BLENotify, 2); // Mouse characteristic

void setup() {
  Serial.begin(9600);

  if (!BLE.begin()) {
    Serial.println("Failed to initialize BLE!");
    while (1);
  }

  BLE.setLocalName("Antidote Electronics"); // Set the local name for Bluetooth

  BLE.setAdvertisedService(mouseService); // Advertise the mouse service
  mouseService.addCharacteristic(mouseCharacteristic); // Add the mouse characteristic
  BLE.addService(mouseService); // Add the mouse service

  BLE.advertise(); // Start advertising the BLE device

  Serial.println("Bluetooth device active, waiting for connections...");
}

void loop() {
  BLEDevice central = BLE.central();

  if (central) {
    Serial.print("Connected to central: ");
    Serial.println(central.address());
  }
}
