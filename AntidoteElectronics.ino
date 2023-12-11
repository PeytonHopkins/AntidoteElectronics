#include <HIDMouse.h>
#include <ArduinoBLE.h>
#include <Arduino_BMI270_BMM150.h>

HIDMouse bleMouse;

void setup() {
  Serial.begin(9600);
  while (!Serial);

  if(!IMU.begin()){
    Serial.println("Failed to initialize IMU");
    while(1);
  }
  else{
    Serial.println("Initialized IMU!");
  }  

  bleMouse.setDeviceName("Antidote Electronics");
  bleMouse.begin();    
  BLE.advertise();

}

float x,y,z;

void loop() {
  while(bleMouse.isConnected()){
    if(IMU.gyroscopeAvailable()){
      IMU.readGyroscope(x,y,z);

      Serial.print("X:");
      Serial.print(x);
      Serial.print("\t");
      Serial.print("Y:");
      Serial.print(y);
      Serial.print("\t");
      Serial.print("Z:");
      Serial.print(z);
      Serial.print("\t");
      Serial.println();

      bleMouse.move(z,0,0);
      delay(50);
    }  
  }


}
