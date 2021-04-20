// Configuration file for use with AlanChatham's UnoJoy project: https://github.com/AlanChatham/UnoJoy
// With gratitude to Afterimage Flight (https://www.youtube.com/watch?v=5hawoJ8wwHk) for guidance/walkthrough on configuring this file.

// My button box uses a matrix keypad to handle all the inputs. The Adafruit Keypad library then converts these into usable information that we can hook into MegaJoy.
#include <Adafruit_Keypad.h>
#include <Adafruit_Keypad_Ringbuffer.h>

// Megajoy.h includes the UnoJoy library with customizations for use with an Arduino Mega. Theoretically we shouldn't need to make any changes to megajoy.h.
#include "MegaJoy.h"

int kbpress = 0;

// Now let's define the button matrix using Arduino's keypad.h library.
// My matrix has a bunch of empty nodes (not perfectly efficeint, I know).
// These are reperesented by a space, which basically becomes a junk data warning.
// If space is expressed anywehre, then something has gone wrong.
const byte ROWS = 12; //rows
const byte COLS = 6; //columns

// This is an older version of the key matrix from before when I realized my diode configuration made this not work.
// If you're getting no response from your key matrix, consider rotating it 90 degrees. This will swap which pins are being used for transmission and reception in the matrix scan.
// char keys[rows][cols] = {
//   {'*', '*', 'a', '*', '*', '*', '*', 'o', 'p', '*', 'q', 'r'},
//   {'1', '2', 'b', '*', '*', '*', '*', '*', 's', 't', 'u', '*'},
//   {'3', '4', 'c', 'g', 'h', 'i', 'j', '*', '*', 'v', '9', '0'},
//   {'5', '6', 'd', '*', '*', '*', '*', '*', 'w', 'x', 'y', '*'},
//   {'7', '8', 'e', 'k', 'l', 'm', 'n', '*', '*', '*', '*', '*'},
//   {'*', '*', 'f', '*', '*', '*', '*', '*', 'z', '+', '-', '*'}
// };

char keys[ROWS][COLS] = {
 {'&','1','3','5','7','&'},
 {'&','2','4','6','8','&'},
 {'a','b','c','d','e','f'},
 {'&','&','g','&','k','&'},
 {' ',' ','h',' ','l',' '},
 {' ',' ','i',' ','m',' '},
 {' ',' ','j',' ','n',' '},
 {'o',' ',' ',' ',' ',' '},
 {'p','s',' ','w',' ','z'},
 {' ','t','v','x',' ','+'},
 {'q','u','9','y',' ','-'},
 {'r',' ','0',' ',' ',' '}
};

// These are the pins used for the columns and rows. This is where you're plugging in the actual wiring of your key matrix.
byte rowPins[ROWS] = {53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43, 42};
byte colPins[COLS] = {41, 40, 39, 38, 37, 36};

// This defines the customKeypad variable using the datatype Adafruit_Keypad
Adafruit_Keypad customKeypad = Adafruit_Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS);

void setup() {
  //  The serial connection is useful for testing the output of your key matrix, but it will interfere with MegaJoy. Uncomment this when testing the key matrix, then comment it out for the actual Megajoy configuration.
  //  Serial.begin(9600);
  setupPins();
  setupMegaJoy();
  customKeypad.begin();
}

void setupPins(void) {
  // Defining pins here will determine what pins are used as "normal" buttons. These buttons and switches will activate when shorted to ground.
  // You can define any pins (except the serial pins) here by adding them to the funtion. You can create separate blocks by copying the existing section and defining a new range if you need to skip around other pins.
  // Mine handles all pins between 22 and 30 as normal switches that aren't part of a key matrix.
  for (int i = 22; i <= 30; i++) {
    pinMode(i, INPUT);
    digitalWrite(i, HIGH);
  }
}

void loop() {
  // This loop runs constantly. Effectivly this polls all your buttons for new information about their status. We shoudln't need to make any changes here.

  //char key = kpd.getKey();
  //if (key != NO_KEY){
  //  Serial.println(key);}

customKeypad.tick();

  while(customKeypad.available()){
    keypadEvent e = customKeypad.read();
//    This line will cause the matrix keypad to output to the Serial connection. This is great for testing, but interferes with Megajoy.
//    Uncomment this line to see what your key matrix is actually putting out. Comment it before working on the Megajoy portion of the code.
//    Serial.print((char)e.bit.KEY);
    if(e.bit.EVENT == KEY_JUST_PRESSED) Serial.println(" pressed");
    else if(e.bit.EVENT == KEY_JUST_RELEASED) Serial.println(" released");
  }

  delay(10);

  // Since I'm documenting this for others, I figure I should add a little in here about Functions in C.
  // This next line defines a variable. It does this in 3 parts. The first part is the variable's type. Normally this would be something like Byte or Char.
  // In this case it's megaJoyControllerData_t. This type is defined in the Megajoy library.
  // The second part is the variable (controllerData).
  // The third part is where controllerData gets it's contents from. getControllerData is a function (defined below).
  megaJoyControllerData_t controllerData = getControllerData();
  setControllerData(controllerData);
}

// Here's the primary function for running MegaJoy. Any data that MegaJoy needs to interpret as a button press should be included in here somewhere.
// This function (called in the main loop) will determine which joystick buttons have been pressed.
// This is a function declaration, so the first part is the type of data the function is expected to return (megaJoyControllerData_t)
// The second part is the function's name (getControllerData)
// The third part is what variables are we passing in to the function. This is presently Void, meaning the function isn't waiting for data from elsewhere in the program.
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
  
    if (customKeypad.isPressed(keyring[j])) {
        controllerData.buttonArray[(kbpress - 2) / 8] |= 1 << ((kbpress - 2) % 8);
    } else {
        controllerData.buttonArray[(kbpress - 2) / 8] &= 0 << ((kbpress - 2) % 8);
    }
  }

  // This section should match what you've set in the setupPins function (Make sure to add 1 to the upper range, since this is less than, rather than less than or equal.
  // This handles your "normal" buttons and switches that aren't part of a matrix. These pins, when shorted to ground, will report back a button press of the equivalant button (ex. pin 27 gets you button 27).
  for (int i = 22; i < 31; i++) {
    bool isPressed = !digitalRead(i);
    // This next line is in here to reverse the output of keys 25 through 29. I'm using 3-pin illuminated automotive keys. These report back the reverse of a typical key (because closing the circuit sends 5V).
    // You can modify this line to deal with any keys you want to reverse, or comment it out if you don't have any.
    if (i > 24 && i < 29){isPressed = !isPressed;}

    // This line here converts your digital pin (from setupPins) 
    controllerData.buttonArray[(i - 2) / 8] |= (isPressed) << ((i - 2) % 8);
  }

  // Set the analog inputs
  // Since analogRead(pin) returns a 10 bit value, we need to perform a bit shift operation to lose the 2 least significant bits and get an 8 bit number that we can use
  // I've commented out most axes because my button box only has one analog dial. This isn't strictly necessary, but I figure it's best not to leave a bunch of random Analog Read actions in the middle of my setup if I'm not really using any.
  // Which analog pin you use will determine which axis gets reported back to windows for your joystick. The first two are your standard joystick X and Y values.
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
