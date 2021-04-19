#include <Key.h>
#include <Keypad.h>

// Configuration file for use with AlanChatham's UnoJoy project: https://github.com/AlanChatham/UnoJoy
// With gratitude to Afterimage Flight (https://www.youtube.com/watch?v=5hawoJ8wwHk) for guidance/walkthrough on configuring this file.

// Megajoy.h includes the UnoJoy library with customizations for use with an Arduino Mega. Theoretically we shouldn't need to make any changes to megajoy.h.
#include "MegaJoy.h"

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

//char keys[rows][cols] = {
// {'&','1','3','5','7','&'},
// {'&','2','4','6','8','&'},
// {'a','b','c','d','e','f'},
// {'&','&','g','&','k','&'},
// {' ',' ','h',' ','l',' '},
// {' ',' ','i',' ','m',' '},
// {' ',' ','j',' ','n',' '},
// {'o',' ',' ',' ',' ',' '},
// {'p','s',' ','w',' ','z'},
// {' ','t','v','x',' ','+'},
// {'q','u','9','y',' ','-'},
// {'r',' ','0',' ',' ',' '}
//};

// Since I defined the button matrix rows and columns as variables above, I can use the variables here to define the matrix
byte rowPins[rows] = {41, 40, 39, 38, 37, 36};
byte colPins[cols] = {53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43, 42};
Keypad kpd = Keypad( makeKeymap(keys), rowPins, colPins, rows, cols );

void setup() {
  setupPins();
  setupMegaJoy();
  Serial.begin(9600);
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

  char key = kpd.getKey();
  if (key != NO_KEY){
    Serial.println(key);}
    
  megaJoyControllerData_t controllerData = getControllerData();
  setControllerData(controllerData);
}

// Here's the primary function for running MegaJoy. Any data that MegaJoy needs to interpret as a button press should be included in here somewhere.
megaJoyControllerData_t getControllerData(void){

  // Start by blanking out the variables so it's not full of junk data.
  megaJoyControllerData_t controllerData = getBlankDataForMegaController();

  String keyring = "1234567890abcdefghijklmnopqrstuvwxyz+-";
  for (int j = 0; j < keyring.length(); j++) {
    if (j <= 20) { 
      kbpress = j + 1; 
    } else {
      kbpress = j + 11; 
    }
  
    if (kpd.isPressed(keyring[j])) {
        controllerData.buttonArray[(kbpress - 2) / 8] |= 1 << ((kbpress - 2) % 8);
    } else {
        controllerData.buttonArray[(kbpress - 2) / 8] &= 0 << ((kbpress - 2) % 8);
    }
  }

  // This section should match what you've set in the setupPins function (Make sure to add 1 to the upper range, since this is less than, rather than less than or equal.
  // This handles your "normal" buttons and switches that aren't part of a matrix.
  for (int i = 22; i < 31; i++) {
    bool isPressed = !digitalRead(i);
    if (i > 24 && i < 29){isPressed = !isPressed;}
    controllerData.buttonArray[(i - 2) / 8] |= (isPressed) << ((i - 2) % 8);
  }

  // Set the analog inputs
  // Since analogRead(pin) returns a 10 bit value, we need to perform a bit shift operation to lose the 2 least significant bits and get an 8 bit number that we can use
  // I've commented out most axes because my button box only has one analog dial.
  //  controllerData.analogAxisArray[0] = analogRead(A0);
  //  controllerData.analogAxisArray[1] = analogRead(A1);
  controllerData.analogAxisArray[2] = analogRead(A2);
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
