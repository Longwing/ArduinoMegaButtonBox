#include <FastLED.h>

// Megajoy.h includes the UnoJoy library with customizations for use with an Arduino Mega. Theoretically we shouldn't need to make any changes to megajoy.h.
#include "MegaJoy.h"

#define DEBUG false
#define REAL_CODE true

void setup() {
  setupMegaJoy();
  setupPins();
}

void setupPins(void) {
  // Defining pins here will determine what pins are used as "normal" buttons. These buttons and switches will activate when shorted to ground.
  // You can define any pins (except the serial pins) here by adding them to the funtion. You can create separate blocks by copying the existing section and defining a new range if you need to skip around other pins.
  // Mine handles all pins between 22 and 30 as normal switches that aren't part of a key matrix.
  for (int i = 11; i <= 54; i++) {
    pinMode(i, INPUT);
    digitalWrite(i, HIGH);
  }

  pinMode(A12, INPUT);
  digitalWrite(A12, HIGH);
  pinMode(A13, INPUT);
  digitalWrite(A13, HIGH);
  pinMode(A14, INPUT);
  digitalWrite(A14, HIGH);
  pinMode(A15, INPUT);
  digitalWrite(A15, HIGH);

}

void loop() {
  // This loop runs constantly. Effectivly this polls all your buttons for new information about their status. We shoudln't need to make any changes here.

#if DEBUG && !REAL_CODE
  delay(10);
#endif

  // Since I'm documenting this for others, I figure I should add a little in here about Functions in C.
  // This next line defines a variable. It does this in 3 parts. The first part is the variable's type. Normally this would be something like Byte or Char.
  // In this case it's megaJoyControllerData_t. This type is defined in the Megajoy library.
  // The second part is the variable (controllerData).
  // The third part is where controllerData gets it's contents from. getControllerData is a function (defined below).
#if REAL_CODE
  megaJoyControllerData_t controllerData = getControllerData();
  setControllerData(controllerData);
#endif
}

// Here's the primary function for running MegaJoy. Any data that MegaJoy needs to interpret as a button press should be included in here somewhere.
// This function (called in the main loop) will determine which joystick buttons have been pressed.
// This is a function declaration, so the first part is the type of data the function is expected to return (megaJoyControllerData_t)
// The second part is the function's name (getControllerData)
// The third part is what variables are we passing in to the function. This is presently Void, meaning the function isn't waiting for data from elsewhere in the program.
megaJoyControllerData_t getControllerData(void) {

  // Start by blanking out the variables so it's not full of junk data.
  megaJoyControllerData_t controllerData = getBlankDataForMegaController();

  // This section should match what you've set in the setupPins function (Make sure to add 1 to the upper range, since this is less than, rather than less than or equal.
  // This handles your "normal" buttons and switches that aren't part of a matrix. These pins, when shorted to ground, will report back a button press of the equivalant button (ex. pin 27 gets you button 27).
  for (int i = 11; i < 54; i++) {
    bool isPressed = !digitalRead(i);
    // This next line is in here to reverse the output of keys 25 through 29. I'm using 3-pin illuminated automotive keys. These report back the reverse of a typical key (because closing the circuit sends 5V).
    // You can modify this line to deal with any keys you want to reverse, or comment it out if you don't have any.
    if (i > 29 && i < 42) {
      isPressed = !isPressed;
    }
    // This line here converts your digital pin (from setupPins) into a button press
    controllerData.buttonArray[(i - 11) / 8] |= (isPressed) << ((i - 11) % 8);
  }

  controllerData.dpad0UpOn = !digitalRead(A12);
  controllerData.dpad0LeftOn = !digitalRead(A13);
  controllerData.dpad0RightOn = !digitalRead(A14);
  controllerData.dpad0DownOn = !digitalRead(A15);


  //controllerData.buttonArray[(56 - 2) / 8] |= (isPressed) << ((56 - 2) % 8);
  //isPressed = !digitalRead(A14);
  //controllerData.buttonArray[(57 - 2) / 8] |= (isPressed) << ((57 - 2) % 8);
  //isPressed = !digitalRead(A15);
  //controllerData.buttonArray[(58 - 2) / 8] |= (isPressed) << ((58 - 2) % 8);

  // Set the analog inputs
  // Since analogRead(pin) returns a 10 bit value, we need to perform a bit shift operation to lose the 2 least significant bits and get an 8 bit number that we can use
  // I've commented out most axes because my button box only has one analog dial. This isn't strictly necessary, but I figure it's best not to leave a bunch of random Analog Read actions in the middle of my setup if I'm not really using any.
  // Which analog pin you use will determine which axis  gets reported back to windows for your joystick. The first two are your standard joystick X and Y values.
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
