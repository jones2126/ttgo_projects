#include <Arduino.h>

/*
 * ref: https://esp32io.com/tutorials/esp32-potentiometer
 * This program reads three inputs and prints out the results.  The inputs are associated with 
 * 2 potentiometers and a toggle switch.  The toggle switch has been fitted with 2 resistors in order
 * to provide 3 modes/states (down, middle, up)
 */
//Sensor Pin definitions
#define RC_POT_X 36
#define RC_POT_Y 37
#define RC_mode 38
/*
float floatMap(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
*/

// the setup routine runs once when you press reset:

int check_mode(int mode_value){
  Serial.println(mode_value);
  int mode = 0;
  if (mode_value > 3000) {
    mode = 2; //middle
    }
    else if (mode_value < 2000) {
      mode = 1;  // up
      }
      else {
        mode = 3;
        }  // down
  return mode;
}

void setup() {
  Serial.begin(115200);    // initialize serial communication at 115200 bits per second:
  pinMode(RC_POT_X, INPUT);
  pinMode(RC_POT_Y, INPUT);
  pinMode(RC_mode, INPUT);  
  
}
void loop() {
  // read the input on analog pin pins:
  int steering_val = analogRead(RC_POT_X);
  int throttle_val = analogRead(RC_POT_Y);
  int mode_switch_value = analogRead(RC_mode);
  String switch_mode;
  switch_mode = check_mode(mode_switch_value);
  //float voltage = floatMap(steering_val, 0, 4095, 0, 5);   // Rescale to potentiometer's voltage (from 0V to 5V):
  Serial.print("pot 1: ");    // print out the value you read:
  Serial.print(steering_val);
  Serial.print(" pot 2: ");
  Serial.print(throttle_val);
  Serial.print(", switch: ");
  Serial.print(mode_switch_value);
  Serial.print(", mode: ");
  Serial.println(switch_mode);
  delay(3000);
}

