#include <MahonyAHRS.h>
#include <BleCombo.h>
#include <MPU9250_WE.h>
#include <Wire.h>

MPU6500_WE myMPU6500 = MPU6500_WE(0x68);// Make sure to keep module still while starting so it can callibrate properly.

Mahony filter; // This is used to get the orientation of the module by utilizing both gyroscope, accelerometer, and magnonmeter if available (MPU9250) both accel and gyro are enough.

int softwareID = 1; 
  // 0 = Default
  // 1 = AutoDesk Fusion 360

// Declare a function pointer type
typedef void (*FunctionPointer)();

// Create a variable to store the selected method
FunctionPointer selectedMethod = nullptr;

float deadZone = 0.35; // 
xyzFloat gyr;
float gx, gy, gz;
xyzFloat acc;
xyzFloat mag;
bool isMoving = false;
bool isPressing = false;
unsigned long lastTime;
float gyroScale = 0.244f;
float roll, pitch, heading;
float dH, dP, dR; // change in variable
float lH, lP, lR; // last variable

float sens = 3; // mouse senesitivity
void DetermineOrientation(){
  if(abs(gyr.x) > deadZone || abs(gyr.y) > deadZone || abs(gyr.z) > deadZone){ // if the module has moved enough to report a value
    roll = (filter.getRoll() * 57.2958279); // .getx() is returned in raidans, so we multiply by 180/π to get degrees
    pitch = (filter.getPitch() * 57.2958279);
    heading = (filter.getYaw() * 57.2958279);
  
    Serial.print(heading);
    Serial.print(",");
    Serial.print(pitch);
    Serial.print(",");
    Serial.println(roll);

    // I am having an issue with the roll axis, if the value gets too high the heading and pitch go to very high values as well. 
    // I can explain in more detail via a call 

    //dH = heading - lH;
    //dP = pitch - lP;
    //dR = roll - lR;

    //Serial.print(dH);
    //Serial.print(",");
    //Serial.print(dP);
    //Serial.print(",");
    //Serial.println(dR);

    //Mouse.move(-dH * sens, -dP * sens); dont need as we will use API to move 

    //lH = heading;
    //lP = pitch;
    //lR = roll;

    isMoving = true;
  }
  else{
    isMoving = false;
  }
}

void FusionMacro(){ // If we want macros for Fusion360 we will use this. 
  DetermineOrientation();
  if(isMoving){
    if(!isPressing){
      //Keyboard.press(KEY_LEFT_SHIFT);
      //Mouse.press(MOUSE_MIDDLE); 
      isPressing = true; 
      lastTime = millis();   
    }
  }
  else{
    if(millis() - lastTime > 300){
      //Keyboard.release(KEY_LEFT_SHIFT);
      //Mouse.release(MOUSE_MIDDLE);
      isPressing = false;
    }

  }
}

void setup() {
  Serial.begin(9600);
  Wire.begin();
  Keyboard.begin();
  Mouse.begin();
  if(!myMPU6500.init()){
    Serial.println("MPU6500 did not respond");
  }
  else{
    Serial.println("MPU6500 is connected");
  }

  switch (softwareID) {
    case 1: // AutoDesk Fusion 360
      selectedMethod = FusionMacro;
      break;
    default:
      selectedMethod = DetermineOrientation;
      break;
  }
  
  myMPU6500.autoOffsets(); // Must keep the module still while it offsets in order for it to calibrate properly. 
  myMPU6500.setGyrRange(MPU6500_GYRO_RANGE_2000); 
}

void loop() {
  if(Keyboard.isConnected()) {
    gyr = myMPU6500.getGyrValues();
    acc = myMPU6500.getCorrectedAccRawValues();
    gx = gyr.x * gyroScale;
    gy = gyr.y * gyroScale;
    gz = gyr.z * gyroScale;
    filter.updateIMU(gx, gy, gz, acc.x, acc.y, acc.z);
    
    selectedMethod();
  }


}