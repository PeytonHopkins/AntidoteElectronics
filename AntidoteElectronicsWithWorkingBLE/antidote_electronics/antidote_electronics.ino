#include <ArduinoBLE.h>
#include <Arduino_BMI270_BMM150.h>

BLEService mouseService("1812");  // Bluetooth HID mouse service UUID
BLECharacteristic mouseCharacteristic("2A4D", BLERead | BLEWrite | BLENotify, 6);  // Report characteristic UUID

void setup() {
  if (!BLE.begin()) {
    while (1);
  }

  if (!IMU.begin()) {
    while (1);
  }

  BLE.setLocalName("Antidote Electronics");

  mouseService.addCharacteristic(mouseCharacteristic);
  BLE.addService(mouseService);

  BLE.advertise();

  Serial.begin(9600);
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

      // Create a buffer for the mouse report
      uint8_t mouseReport[6] = {0, 0, (int)(x * 10), (int)(y * 10), 0, 0};

      // Send the mouse report via BLE
      mouseCharacteristic.writeValue(mouseReport, sizeof(mouseReport));

      delay(50);
    }

    digitalWrite(LED_BUILTIN, LOW);

    Serial.print("Disconnected from central: ");
    Serial.println(central.address());
  }
}