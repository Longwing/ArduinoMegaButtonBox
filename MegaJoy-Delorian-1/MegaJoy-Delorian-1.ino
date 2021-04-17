
#include "MegaJoy.h"

// Define Pins - This provides a naming convention for pins being used for various functions, making it easier to track the pins later on by using their variables instead.

int EmergencySW1 = 22;
int EmergencySW2 = 23;
int BRButton = 24;
int RedToggle1 = 25;
int RedToggle2 = 26;
int RedToggle3 = 27;
int RedToggle4 = 28;
int BigSwitch1 = 29;
int BigSwitch2 = 30;

int ButtonMtx-H1 = 41;
int ButtonMtx-H2 = 40;
int ButtonMtx-H3 = 39;
int ButtonMtx-H4 = 38;
int ButtonMtx-H5 = 37;
int ButtonMtx-H6 = 36;

int ButtonMtx-V1 = 53;
int ButtonMtx-V2 = 52;
int ButtonMtx-V3 = 51;
int ButtonMtx-V4 = 50;
int ButtonMtx-V5 = 49;
int ButtonMtx-V6 = 48;
int ButtonMtx-V7 = 47;
int ButtonMtx-V8 = 46;
int ButtonMtx-V9 = 45;
int ButtonMtx-V10 = 44;
int ButtonMtx-V11 = 43;
int ButtonMtx-V12 = 42;

void setup(){
  setupPins();
  setupMegaJoy();
}

void loop(){
  // Always be getting fresh data
  megaJoyControllerData_t controllerData = getControllerData();
  setControllerData(controllerData);
}

void setupPins(void){
  // Set all the digital pins as inputs
  // with the pull-up enabled, except for the 
  // two serial line pins
  // DG - I've set it to only use pins 22 through 54. This leaves the unused PWM pins free for the future LED matrix.
  for (int i = 22; i <= 54; i++){
    pinMode(i, INPUT);
    digitalWrite(i, HIGH);
  }
}

megaJoyControllerData_t getControllerData(void){
  
  // Set up a place for our controller data
  //  Use the getBlankDataForController() function, since
  //  just declaring a fresh dataForController_t tends
  //  to get you one filled with junk from other, random
  //  values that were in those memory locations before
  megaJoyControllerData_t controllerData = getBlankDataForMegaController();
  // Since our buttons are all held high and
  //  pulled low when pressed, we use the "!"
  //  operator to invert the readings from the pins
  for (int i = 22; i < 54; i++){
    controllerData.buttonArray[(i - 2) / 8] |= (!digitalRead(i)) << ((i - 2) % 8);
  }


void loop(void){

  megaJoyControllerData_t controllerData = getBlankDataForController();
  // Get your key, then assign proper controller presses

  char key = getKey();

  if (key == ‘1’){

    controllerData.crossOn = 1;

  }
  setControllerData(controllerData);

}
  
  // Set the analog sticks
  //  Since analogRead(pin) returns a 10 bit value,
  //  we need to perform a bit shift operation to
  //  lose the 2 least significant bits and get an
  //  8 bit number that we can use 
  controllerData.analogAxisArray[0] = analogRead(A0);
  //  controllerData.analogAxisArray[1] = analogRead(A1);
  //  controllerData.analogAxisArray[2] = analogRead(A2); 
  //  controllerData.analogAxisArray[3] = analogRead(A3); 
  //  controllerData.analogAxisArray[4] = analogRead(A4); 
  //  controllerData.analogAxisArray[5] = analogRead(A5); 
  //  controllerData.analogAxisArray[6] = analogRead(A6); 
  //  controllerData.analogAxisArray[7] = analogRead(A7); 
  //  controllerData.analogAxisArray[8] = analogRead(A8); 
  //  controllerData.analogAxisArray[9] = analogRead(A9); 
  //  controllerData.analogAxisArray[10] = analogRead(A10); 
  //  controllerData.analogAxisArray[11] = analogRead(A11); 
  
  // And return the data!
  return controllerData;
}
