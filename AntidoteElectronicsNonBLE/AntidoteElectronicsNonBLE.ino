#include <Arduino_BMI270_BMM150.h>
#include <PluggableUSBHID.h>
#include <USBMouse.h>

USBMouse Mouse;
float sensitivity = 10;
float deadZone = .5;
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

}

float x,y,z;

void loop() {
    if(IMU.gyroscopeAvailable()){
      IMU.readGyroscope(x,y,z);

      if(abs(x) > deadZone || abs(y) > deadZone || abs(z) > deadZone){
        Mouse.press(4);
        Mouse.move(z/sensitivity,x/sensitivity);
      }


      delay(10); //will need to test the delay (potentially need to decrease)
    } 
}


