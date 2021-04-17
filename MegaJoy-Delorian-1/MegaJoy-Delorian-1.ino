#include <Key.h>
#include <Keypad.h>

// Configuration file for use with AlanChatham's UnoJoy project: https://github.com/AlanChatham/UnoJoy
// With gratitude to Afterimage Flight (https://www.youtube.com/watch?v=5hawoJ8wwHk) for guidance/walkthrough on configuring this file.

// Megajoy.h includes the UnoJoy library with customizations for use with an Arduino Mega. Theoretically we shouldn't need to make any changes to megajoy.h.
#include "MegaJoy.h"

// Define Pins - This provides a naming convention for pins being used for various functions, making it easier to track the pins later on by using their variables instead.

// These are standard switches and buttons with one end connected to Ground and the other to a digital pin on the mega. 
// These are the "Normal" inputs that don't actually need much modification in the MegaJoy code. I've named them here just to keep track of them, but it's unlikely I'll need to call these variables.
int EmergencySW1 = 22;
int EmergencySW2 = 23;
int BRButton = 24;
int RedToggle1 = 25;
int RedToggle2 = 26;
int RedToggle3 = 27;
int RedToggle4 = 28;
int BigSwitch1 = 29;
int BigSwitch2 = 30;

// These pins are used for the button matrix. In my box it's a huge 6x12 matrix handling almost 40 inputs, however this code can be easily modified to handle a smaller matrix by adjusting the relavant parts of the code. 
// The vertical portion of this matrix (V1 through V12)is where I have my diodes to prevent ghosting.
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

// Now let's define the button matrix using Arduino's keypad.h library.
// My matrix has a bunch of empty nodes (not perfectly efficeint, I know).
// These are reperesented by *, which basically becomes a junk data warning.
// If * is expressed anywehre, then something has gone wrong.
const byte rows = 6; //rows
const byte cols = 12; //columns
char keys[rows][cols] = {
  {'*','*','a','*','*','*','*','o','p','*','q','r'},
  {'1','2','b','*','*','*','*','*','s','t','u','*'},
  {'3','4','c','g','h','i','j','*','*','v','9','0'},
  {'5','6','d','*','*','*','*','*','w','x','y','*'},
  {'7','8','e','k','l','m','n','*','*','*','*','*'},
  {'*','*','f','*','*','*','*','*','z','+','-','*'}
};

// Since I defined the button matrix rows and columns as variables above, I can use the variables here to define the matrix
byte rowPins[rows] = {ButtonMtx-H1, ButtonMtx-H2, ButtonMtx-H3, ButtonMtx-H4, ButtonMtx-H5, ButtonMtx-H6};
byte colPins[cols] = {ButtonMtx-V1, ButtonMtx-V2, ButtonMtx-V3, ButtonMtx-V4, ButtonMtx-V5, ButtonMtx-V6, ButtonMtx-V7, ButtonMtx-V8, ButtonMtx-V9, ButtonMtx-V10, ButtonMtx-V11, ButtonMtx-V12}; 
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, rows, cols );

void setup(){
  setupPins();
  setupMegaJoy();
}

void loop(){
  // This loop runs constantly. Effectivly this polls all your buttons for new information about their status. We shoudln't need to make any changes here.
  megaJoyControllerData_t controllerData = getControllerData();
  setControllerData(controllerData);
}

void setupPins(void){
  // Defining pins here will determine what pins are used as "normal" buttons. These buttons and switches will activate when shorted to ground.
  // You can define any pins (except the serial pins) here by adding them to the funtion. You can create separate blocks by copying the existing section and defining a new range.
  for (int i = 22; i <= 30; i++){
    pinMode(i, INPUT);
    digitalWrite(i, HIGH);
  }
}

// Here's the primary function for running MegaJoy. Any data that MegaJoy needs to interpret as a button press should be included in here somewhere.
megaJoyControllerData_t getControllerData(void){
  
  // Start by blanking out the variables so it's not full of junk data.
  megaJoyControllerData_t controllerData = getBlankDataForMegaController();

  // This function can be used to re-assign button presses from the Arduino sample button matrix code for use in MegaJoy. We will likely need this function to make our button matrix function.
  char key = getKey();
  if (key == ‘1’){
    controllerData.crossOn = 1;
  }
  setControllerData(controllerData);
  }

  // This section should match what you've set in the setupPins function (Make sure to add 1 to the upper range, since this is less than, rather than less than or equal. 
  // This handles your "normal" buttons and switches that aren't part of a matrix.
  for (int i = 22; i < 31; i++){
    controllerData.buttonArray[(i - 2) / 8] |= (!digitalRead(i)) << ((i - 2) % 8);
  }

  // Set the analog inputs
  // Since analogRead(pin) returns a 10 bit value, we need to perform a bit shift operation to lose the 2 least significant bits and get an 8 bit number that we can use
  // I've commented out most axes because my button box only has one analog dial.
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
  
  // Because this is running in a loop, this end of the function will return the current status of all buttons and axes back to the loop.
  return controllerData;
}
