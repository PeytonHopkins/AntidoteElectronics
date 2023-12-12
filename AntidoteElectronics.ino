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

      //applying dead zones 
      float sensitivity = 0.5;
      if (abs(z) > 0.1){
        bleMouse.move(z,0,0);
      } 

      delay(50); //will need to test the delay (potentially need to decrease)
    }  
  } else {
    Serial.println("Device not connected. Attempting to reconnect...");

    if (attemptReconnect()){
      Serial.println("Reconnection successful!");
    } else {
      Serial.println("Reconnection failed. Retrying... ");
    }
  }
}

bool attemptReconnect() {

  BLE.advertise(); //Restart Advertising (basic scenario)

  bool reconnected = true;

  return reconnected;
}
