#include <ArduinoBLE.h>

float value;

union {
 uint8_t b[16];
 float f[4];
} data;

void setup() {
  Serial.begin(9600);

  // initialize the Bluetooth® Low Energy hardware
  BLE.begin();
  // scan for peripherials
  BLE.scan();

}

void loop() {
  // check if a peripheral has been discovered
  BLEDevice peripheral = BLE.available();

  if (peripheral) {
    // discovered a peripheral, print out address, local name, and advertised service
    Serial.print("Found ");
    Serial.print(peripheral.address());
    Serial.print(" '");
    Serial.print(peripheral.localName());
    Serial.print("' ");
    Serial.print(peripheral.advertisedServiceUuid());
    Serial.println();

    if (peripheral.localName() != "Antidote") {
      return;
    }

    // stop scanning
    BLE.stopScan();

    controlLed(peripheral);

    // peripheral disconnected, start scanning again
    BLE.scan();
  }
}

void controlLed(BLEDevice peripheral) {
  // connect to the peripheral
  Serial.println("Connecting ...");

  if (peripheral.connect()) {
    Serial.println("Connected");
  } else {
    Serial.println("Failed to connect!");
    return;
  }

  // discover peripheral attributes
  Serial.println("Discovering attributes ...");
  if (peripheral.discoverAttributes()) {
    Serial.println("Attributes discovered");
  } else {
    Serial.println("Attribute discovery failed!");
    peripheral.disconnect();
    return;
  }

  // retrieve the characteristic
  BLECharacteristic gyroCharacteristic = peripheral.characteristic("19b10001-e8f2-537e-4f6c-d104768a1214");

  if (!gyroCharacteristic) {
    Serial.println("Peripheral does not have correct characteristic!");
    peripheral.disconnect();
    return;
  }

  while (peripheral.connected()) {
    byte arr [16];

    gyroCharacteristic.readValue(arr, 16);

    for(int i = 0; i < 16; i++){
      data.b[i] = arr[i];
    }

    for(int i = 0; i < 4; i++){
      if(i == 3){
        Serial.println(data.f[i]);
      }
      else{
        Serial.print(data.f[i]);
        Serial.print('\t');
      }
    }

  }

  Serial.println("Peripheral disconnected");
}
