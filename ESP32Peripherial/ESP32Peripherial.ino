#include <ArduinoBLE.h>
#include <FastIMU.h>
#include "Madgwick.h"
#include <Wire.h>


MPU6050 IMU;
calData calib = {0}; // Calibration data.
AccelData IMUAccel;    //Sensor data
GyroData IMUGyro;
Quaternion quat;

Madgwick filter;

BLEService gyroService("19B10000-E8F2-537E-4F6C-D104768A1214"); // Bluetooth® Low Energy LED Service

// Bluetooth® Low Energy Characteristic - custom 128-bit UUID, readable by central
BLECharacteristic gyroCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite, 16);

#define IMU_ADDRESS 0x68

union {
  float quat[4];
  uint8_t b[16];
} data;


void setup() {

  Wire.begin();
  Wire.setClock(400000); 
  Serial.begin(115200);

  IMU.init(calib, IMU_ADDRESS);
  delay(7000);
  IMU.calibrateAccelGyro(&calib);
  delay(5000);
  IMU.init(calib, IMU_ADDRESS);

  filter.begin(0.2f);

  // begin initialization
  if (!BLE.begin()) {
    Serial.println("starting Bluetooth® Low Energy module failed!");
    while (1);
  }

  // set advertised local name and service UUID:
  BLE.setLocalName("Antidote");
  BLE.setAdvertisedService(gyroService);

  // add the characteristic to the service
  gyroService.addCharacteristic(gyroCharacteristic);

  // add service
  BLE.addService(gyroService);
  // start advertising
  BLE.advertise();

}

void loop() {
  // listen for Bluetooth® Low Energy peripherals to connect:
  BLEDevice central = BLE.central();

  // if a central is connected to peripheral:
  if (central) {
    Serial.print("Connected to central: ");
    // print the central's MAC address:
    Serial.println(central.address());

    // while the central is still connected to peripheral:
    while (central.connected()) {
      IMU.update();

      IMU.getAccel(&IMUAccel);
      IMU.getGyro(&IMUGyro);

      filter.updateIMU(IMUGyro.gyroX, IMUGyro.gyroY, IMUGyro.gyroZ, IMUAccel.accelX, IMUAccel.accelY, IMUAccel.accelZ);

      IMU.getQuat(&quat);

      data.quat[0] = filter.getQuatW();
      data.quat[1] = filter.getQuatX();
      data.quat[2] = filter.getQuatY();
      data.quat[3] = filter.getQuatZ();

      gyroCharacteristic.writeValue(data.b, 16);
      delay(50);
    }

    // when the central disconnects, print it out:
    Serial.print(F("Disconnected from central: "));
    Serial.println(central.address());
  }
}
