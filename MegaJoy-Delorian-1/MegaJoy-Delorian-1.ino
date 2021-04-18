#include <Key.h>
#include <Keypad.h>

// Configuration file for use with AlanChatham's UnoJoy project: https://github.com/AlanChatham/UnoJoy
// With gratitude to Afterimage Flight (https://www.youtube.com/watch?v=5hawoJ8wwHk) for guidance/walkthrough on configuring this file.

// Megajoy.h includes the UnoJoy library with customizations for use with an Arduino Mega. Theoretically we shouldn't need to make any changes to megajoy.h.
#include "MegaJoy.h"

// Define Pins - This provides a naming convention for pins being used for various functions, making it easier to track the pins later on by using their variables instead.

// These are standard switches and buttons with one end connected to Ground and the other to a digital pin on the mega.
// These are the "Normal" inputs that don't actually need much modification in the MegaJoy code. I've named them here just to keep track of them, but it's unlikely I'll need to call these variables.
byte EmergencySW1 = 22;
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
byte ButtonMtxH1 = 41;
byte ButtonMtxH2 = 40;
byte ButtonMtxH3 = 39;
byte ButtonMtxH4 = 38;
byte ButtonMtxH5 = 37;
byte ButtonMtxH6 = 36;

byte ButtonMtxV1 = 53;
byte ButtonMtxV2 = 52;
byte ButtonMtxV3 = 51;
byte ButtonMtxV4 = 50;
byte ButtonMtxV5 = 49;
byte ButtonMtxV6 = 48;
byte ButtonMtxV7 = 47;
byte ButtonMtxV8 = 46;
byte ButtonMtxV9 = 45;
byte ButtonMtxV10 = 44;
byte ButtonMtxV11 = 43;
byte ButtonMtxV12 = 42;

int kbpress = 0;

// Now let's define the button matrix using Arduino's keypad.h library.
// My matrix has a bunch of empty nodes (not perfectly efficeint, I know).
// These are reperesented by *, which basically becomes a junk data warning.
// If * is expressed anywehre, then something has gone wrong.
const byte rows = 6; //rows
const byte cols = 12; //columns
char keys[rows][cols] = {
  {'*', '*', 'a', '*', '*', '*', '*', 'o', 'p', '*', 'q', 'r'},
  {'1', '2', 'b', '*', '*', '*', '*', '*', 's', 't', 'u', '*'},
  {'3', '4', 'c', 'g', 'h', 'i', 'j', '*', '*', 'v', '9', '0'},
  {'5', '6', 'd', '*', '*', '*', '*', '*', 'w', 'x', 'y', '*'},
  {'7', '8', 'e', 'k', 'l', 'm', 'n', '*', '*', '*', '*', '*'},
  {'*', '*', 'f', '*', '*', '*', '*', '*', 'z', '+', '-', '*'}
};

// Since I defined the button matrix rows and columns as variables above, I can use the variables here to define the matrix
byte rowPins[rows] = {ButtonMtxH1, ButtonMtxH2, ButtonMtxH3, ButtonMtxH4, ButtonMtxH5, ButtonMtxH6};
byte colPins[cols] = {ButtonMtxV1, ButtonMtxV2, ButtonMtxV3, ButtonMtxV4, ButtonMtxV5, ButtonMtxV6, ButtonMtxV7, ButtonMtxV8, ButtonMtxV9, ButtonMtxV10, ButtonMtxV11, ButtonMtxV12};
Keypad kpd = Keypad( makeKeymap(keys), rowPins, colPins, rows, cols );

void setup() {
  setupPins();
  setupMegaJoy();
}

void setupPins(void) {
  // Defining pins here will determine what pins are used as "normal" buttons. These buttons and switches will activate when shorted to ground.
  // You can define any pins (except the serial pins) here by adding them to the funtion. You can create separate blocks by copying the existing section and defining a new range.
  for (int i = 22; i <= 30; i++) {
    pinMode(i, INPUT);
    digitalWrite(i, HIGH);
  }
}

void loop() {
  // This loop runs constantly. Effectivly this polls all your buttons for new information about their status. We shoudln't need to make any changes here.
  //char key = kpd.getKey();
  megaJoyControllerData_t controllerData = getControllerData();
  // controlLEDs ();
  setControllerData(controllerData);
}

// Here's the primary function for running MegaJoy. Any data that MegaJoy needs to interpret as a button press should be included in here somewhere.
megaJoyControllerData_t getControllerData(void) {

  // Start by blanking out the variables so it's not full of junk data.
  megaJoyControllerData_t controllerData = getBlankDataForMegaController();

  // This code is from the keypad example library. If I understand it correctly, then it will match up to valid keys and execute code based on the key pressed.
  char key = kpd.getKey();
  if (key) // Check for a valid key.
  {
    switch (key)
    {
      case NO_KEY: // If no keyboard key is pressed, then use the normal digital read instead.
        break;  
      case '1':
        kbpress = 1;
        break;
      case '2':
        kbpress = 2;
        break;
      case '3':
        kbpress = 3;
        break;
      case '4':
        kbpress = 4;
        break;
      case '5':
        kbpress = 5;
        break;
      case '6':
        kbpress = 6;
        break;
      case '7':
        kbpress = 7;
        break;
      case '8':
        kbpress = 8;
        break;
      case '9':
        kbpress = 9;
        break;
      case '0':
        kbpress = 10;
        break;
      case 'a':
        kbpress = 11;
        break;
      case 'b':
        kbpress = 12;
        break;
      case 'c':
        kbpress = 13;
        break;
      case 'd':
        kbpress = 14;
        break;
      case 'e':
        kbpress = 15;
        break;
      case 'f':
        kbpress = 16;
        break;
      case 'g':
        kbpress = 17;
        break;
      case 'h':
        kbpress = 18;
        break;
      case 'i':
        kbpress = 19;
        break;
      case 'j':
        kbpress = 20;
        break;
      case 'k':
        kbpress = 31;
        break;
      case 'l':
        kbpress = 32;
        break;
      case 'm':
        kbpress = 33;
        break;
      case 'n':
        kbpress = 34;
        break;
      case 'o':
        kbpress = 35;
        break;
      case 'p':
        kbpress = 36;
        break;
      case 'q':
        kbpress = 37;
        break;
      case 'r':
        kbpress = 38;
        break;
      case 's':
        kbpress = 39;
        break;
      case 't':
        kbpress = 40;
        break;
      case 'u':
        kbpress = 41;
        break;
      case 'v':
        kbpress = 42;
        break;
      case 'w':
        kbpress = 43;
        break;
      case 'x':
        kbpress = 44;
        break;
      case 'y':
        kbpress = 45;
        break;
      case 'z':
        kbpress = 46;
        break;
      case '+':
        kbpress = 47;
        break;
      case '-':
        kbpress = 48;
        break;
      default:  // If no keyboard key is pressed, then use the normal digital read instead.
        break;
    }
    // Will this work correctly? No idea. Should this take the i variables from above and turn them into the right bitecode?
    controllerData.buttonArray[(kbpress - 2) / 8] |= 1 << ((kbpress - 2) % 8); 
  }

  // /PSEUDO
char keys = ['01234567890abcdefghijklmnopqrstuvwxyz+-']
for (int j = 0; i < keys.length(); j++) {
  if (j <= 20) 
  { 
    kbpress = j + 1; 
    }
  else 
  {
    kbpress = j + 11; 
    }
  
  switch(kpd.getState(keys[j]))
  {
    case PRESS:
    case HOLD:
      controllerData.buttonArray[(kbpress - 2) / 8] &= 0 << ((kbpress - 2) % 8);
      break;
    default:
      controllerData.buttonArray[(kbpress - 2) / 8] |= 1 << ((kbpress - 2) % 8);
  }
}

  // PSEUDO

  
  // This section should match what you've set in the setupPins function (Make sure to add 1 to the upper range, since this is less than, rather than less than or equal.
  // This handles your "normal" buttons and switches that aren't part of a matrix.
  for (int i = 22; i < 31; i++) {
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

// TODO: pass in the keystroke in the parens, not void
// megaJoyControllerData_t controlLEDs (controllerData) {

  
//  return controllerData;
//}
