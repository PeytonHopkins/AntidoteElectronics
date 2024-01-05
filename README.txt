Arduino Nano 33 BLE Sense REV2 should be plugged into the computer.

ESP32 & MPU6050 Module should be in the trackball.

Right now, the programs assume the Arduino that is plugged into the computer is assigned to the COM4 port. If it is not, the scripts will not work. Eventually, we will have it detect automatically.
If it is not assigned to the COM4 Port, let me (Peyton) know and I will change the script for the Unity Demo.

The Unity demo should be ran only after both devices are both plugged in and connected via bluetooth. 

You can determine if they are connected via bluetooth through the Arduino Serial Monitor for the BLE Sense. 
If it is displaying 4 numbers ex:(-1.00, 0.98, 0.00, -0.86), it means that the two Arduinos are communicating.
If it is displaying MAC addresses ex:(58:cc:1b:31:90:b8) it means that the BLE Sense is still searching for the ESP32, give it a little time to connect.

Once the ESP32 is plugged in, keep it very still for 30-45 seconds. It has to calibrate the gyro and adjust the orientation.
You will know it is ready to go once the values are [1.00, 0.00, 0.00, 0.00] (If the last 3 numbers have a small variation of 0.01-0.03 it is to be expected). 
If you have not rotated the ESP32 since starting it, and these numbers are staying at a different value, reset the ESP32 by pushing the button on it.

The UnityDemo will only work if there is no other device reading the COM4 port. So, turn off the Arduino IDE before running the UnityDemo.
You can scroll using the mouse whell to zoom in and out in the UnityDemo.
You can exit the demo by pressing Escape.






