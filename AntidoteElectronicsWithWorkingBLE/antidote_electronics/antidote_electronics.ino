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

  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while(1);
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

    while (central.connected()) {
      digitalWrite(LED_BUILTIN, HIGH);

      // Read accelerometer values
      float x, y, z;
      IMU.readAcceleration(x, y, z);

      // Map accelerometer values to mouse movement
      int moveX = map(x, -9.8, 9.8, -127, 127);
      int moveY = map(y, -9.8, 9.8, -127, 127);

      // Create a buffer for the mouse report
      uint8_t inputBuffer[] = {0, (uint8_t)moveX, (uint8_t)moveY, 0, 0, 0};

      // Send the mouse report via BLE
      mouseCharacteristic.writeValue(inputBuffer, sizeof(inputBuffer));

      delay(50);
    }

    Serial.print("Disconnected from central: ");
    Serial.println(central.address());
  }
}