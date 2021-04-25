// The FastLED library manages the neopixels I have stashed under the main FTL lever.
#include <FastLED.h>

// Megajoy.h includes the UnoJoy library with customizations for use with an Arduino Mega. Theoretically we shouldn't need to make any changes to megajoy.h.
#include "MegaJoy.h"

#define DEBUG false
#define REAL_CODE true
#define LED_CODE true

#if LED_CODE
// Fire2012 by Mark Kriegsman, July 2012
// as part of "Five Elements" shown here: http://youtu.be/knWiGsmgycY
////
// This basic one-dimensional 'fire' simulation works roughly as follows:
// There's a underlying array of 'heat' cells, that model the temperature
// at each point along the line.  Every cycle through the simulation,
// four steps are performed:
//  1) All cells cool down a little bit, losing heat to the air
//  2) The heat from each cell drifts 'up' and diffuses a little
//  3) Sometimes randomly new 'sparks' of heat are added at the bottom
//  4) The heat from each cell is rendered as a color into the leds array
//     The heat-to-color mapping uses a black-body radiation approximation.
//
// Temperature is in arbitrary units from 0 (cold black) to 255 (white hot).
//
// This simulation scales it self a bit depending on NUM_LEDS; it should look
// "OK" on anywhere from 20 to 100 LEDs without too much tweaking.
//
// I recommend running this simulation at anywhere from 30-100 frames per second,
// meaning an interframe delay of about 10-35 milliseconds.
//
// Looks best on a high-density LED setup (60+ pixels/meter).
//
//
// There are two main parameters you can play with to control the look and
// feel of your fire: COOLING (used in step 1 above), and SPARKING (used
// in step 3 above).
//
// COOLING: How much does the air cool as it rises?
// Less cooling = taller flames.  More cooling = shorter flames.
// Default 50, suggested range 20-100
#define COOLING  75

// SPARKING: What chance (out of 255) is there that a new spark will be lit?
// Higher chance = more roaring fire.  Lower chance = more flickery fire.
// Default 120, suggested range 50-200.
#define SPARKING 200

#define LED_PIN     2
#define COLOR_ORDER GRB
#define CHIPSET     WS2811
#define NUM_LEDS    12

#define BRIGHTNESS  400
#define FRAMES_PER_SECOND 60

bool gReverseDirection = false;

CRGB leds[NUM_LEDS];
#endif

#if REAL_CODE
#define BUTTONSHIFT 11
#endif

void setup() {
  setupMegaJoy();
  setupPins();
#if LED_CODE
  // delay(3000); // sanity delay
  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness( BRIGHTNESS );
#endif
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
#if LED_CODE
  if (!digitalRead(12) == HIGH) {
    Fire2012(); // run simulation frame
    FastLED.show(); // display this frame
    FastLED.delay(1000 / FRAMES_PER_SECOND);
  }
  else if (!digitalRead(13) == HIGH) {
    Fire2012(); // run simulation frame
    FastLED.show(); // display this frame
    FastLED.delay(1000 / FRAMES_PER_SECOND);
  }
  else {
    FastLED.clear();
    // fadeToBlackBy(leds, NUM_LEDS, 256);
    FastLED.show(); // display this frame
    FastLED.delay(1000 / FRAMES_PER_SECOND);
  }
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
  // This is basically the range of digital pins you're using for buttons.
  for (int i = BUTTONSHIFT; i < 55; i++) {
    bool isPressed = !digitalRead(i);
    // This next line is in here to reverse the output of pins 30 through 41. I'm using 3-pin illuminated automotive keys. These report back the reverse of a typical key (because closing the circuit sends 5V).
    // You can modify this line to deal with any keys you want to reverse, or comment it out if you don't have any.
    if (i > 29 && i < 42) {
      isPressed = !isPressed;
    }
    // This line here converts your digital pin (from setupPins) into a button press
    // Note the -11 here, this shifts the output down by 11. The default is -2 to handle the serial pins on pin 0 and pin 1.
    // Since buttons start at pin 2, you subtract 2.
    // My version starts at pin 11, so I subtract 11. This makes the first pin I'm using (pin 11) into button 1 and increments from there.
    // I changed this so I could save the PWM pins 2 through 10 for use with an LED matrix.
    // What's happening here is it's taking your digital pin and flipping a single 0 to 1 in a 64 bit array.
    // It uses the positions of 0s and 1s in that array to designate 2 sets of 32 buttons (for 2 HID joysticks with 32 buttons each)
    controllerData.buttonArray[(i - 11) / 8] |= (isPressed) << ((i - 11) % 8);
  }

  // This part was tricky and took a lot of work to figure out. I wanted to borrow some of the analog pins to use as buttons.
  // However, how do I designate a button in the buttonArray when I'm not using the for loop above? This was the answer.
  // 54 - 11 puts the button press right at the end of the actual digital pins from the loop above.
  // using -11 isn't really necesarry here. I could just do the math, but this keeps it consistent with the loop above, in case I move the pins later it's clear that I need to shift all of these.
  if (!digitalRead(A12) == HIGH) {
    controllerData.buttonArray[(54 - 11) / 8] |= (!digitalRead(A12)) << ((54 - 11) % 8);
  }
  if (!digitalRead(A13) == HIGH) {
    controllerData.buttonArray[(55 - 11) / 8] |= (!digitalRead(A13)) << ((55 - 11) % 8);
  }
  if (!digitalRead(A14) == HIGH) {
    controllerData.buttonArray[(56 - 11) / 8] |= (!digitalRead(A14)) << ((56 - 11) % 8);
  }
  if (!digitalRead(A15) == HIGH) {
    controllerData.buttonArray[(57 - 11) / 8] |= (!digitalRead(A15)) << ((57 - 11) % 8);
  }

  // controllerData.dpad0UpOn = !digitalRead(A12);
  //controllerData.dpad0LeftOn = !digitalRead(A13);
  //controllerData.dpad0RightOn = !digitalRead(A14);
  // controllerData.dpad0DownOn = !digitalRead(A15);


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

#if LED_CODE
void Fire2012()
{
  // Array of temperature readings at each simulation cell
  static uint8_t heat[NUM_LEDS];

  // Step 1.  Cool down every cell a little
  for ( int i = 0; i < NUM_LEDS; i++) {
    heat[i] = qsub8( heat[i],  random8(0, ((COOLING * 10) / NUM_LEDS) + 2));
  }

  // Step 2.  Heat from each cell drifts 'up' and diffuses a little
  for ( int k = NUM_LEDS - 1; k >= 2; k--) {
    heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
  }

  // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
  if ( random8() < SPARKING ) {
    int y = random8(7);
    heat[y] = qadd8( heat[y], random8(160, 255) );
  }

  // Step 4.  Map from heat cells to LED colors
  for ( int j = 0; j < NUM_LEDS; j++) {
    CRGB color = HeatColor( heat[j]);
    int pixelnumber;
    if ( gReverseDirection ) {
      pixelnumber = (NUM_LEDS - 1) - j;
    } else {
      pixelnumber = j;
    }
    leds[pixelnumber] = color;
  }
}
#endif
