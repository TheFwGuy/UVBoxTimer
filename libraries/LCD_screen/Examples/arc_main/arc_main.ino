///
/// @file	arc_main.ino
/// @brief	Main sketch
///
/// @details	Example of arc
/// @n @a	Developed with [embedXcode](http://embedXcode.weebly.com)
///
/// @author	Rei VILO
/// @author	embedXcode.weebly.com
/// @date	mars 08, 2013 15:07
/// @version	104
///
/// @copyright	© Rei VILO, 2013
/// @copyright	All rights reserved
///
/// @see	ReadMe.txt for references
/// @n
///


// Core library for code-sense
#if defined(WIRING) // Wiring specific
#include "Wiring.h"
#elif defined(MAPLE_IDE) // Maple specific
#include "WProgram.h"
#elif defined(MPIDE) // chipKIT specific
#include "WProgram.h"
#elif defined(DIGISPARK) // Digispark specific
#include "Arduino.h"
#elif defined(ENERGIA) // LaunchPad, FraunchPad and StellarPad specific
#include "Energia.h"
#elif defined(CORE_TEENSY) // Teensy specific
#include "WProgram.h"
#elif defined(ARDUINO) && (ARDUINO >= 100) // Arduino 1.0 and 1.5 specific
#include "Arduino.h"
#elif defined(ARDUINO) && (ARDUINO < 100) // Arduino 23 specific
#include "WProgram.h"
#else // error
#error Platform not defined
#endif

// Include application, user and local libraries
#include "LCD_screen.h"
#include "SPI.h"

// Define variables and constants
//#include "TOUCH22_screen.h"
//LCD_TOUCH22 myScreen;

#include "HY28A_screen.h"
LCD_HY28A   myScreen;

//// Code


// Add setup code
void setup() {
    pinMode(PUSH2, INPUT_PULLUP);
    Serial.begin(9600);
    
#if true
    myScreen.begin();
    myScreen.setOrientation(0);
    myScreen.clear();
    myScreen.gText(10, 10, "arc");
    
    myScreen.setPenSolid(false);
    myScreen.circle(myScreen.screenSizeX()/2, myScreen.screenSizeY()/2, 100, blueColour);
    
    myScreen.setPenSolid(true);
    
    uint32_t chrono = millis();
    
    myScreen.arc(myScreen.screenSizeX()/2, myScreen.screenSizeY()/2, 90, 30, 330, redColour);
    
    myScreen.arc(myScreen.screenSizeX()/2, myScreen.screenSizeY()/2, 50, 300, 0, yellowColour);
    myScreen.arc(myScreen.screenSizeX()/2, myScreen.screenSizeY()/2, 40, 0, 60, greenColour);
    
    myScreen.arc(myScreen.screenSizeX()/2, myScreen.screenSizeY()/2, 80, 93, 133, blueColour);
    
    myScreen.gText(100, 10, String(millis() - chrono) + " ms");
    
#else
    myScreen.begin();
    myScreen.setOrientation(0);
    myScreen.clear();
    myScreen.gText(10, 10, "triangle");
    
    myScreen.setPenSolid(true);
    _penSolid = true;
    myScreen.triangle(20, 20, 110, 50, 120, 200, whiteColour);
    
    myScreen.setPenSolid(false);
    _penSolid = false;
    myScreen.triangle(20, 20, 110, 50, 120, 200, blueColour);
    
#endif
    
    Serial.end();
    
    while (digitalRead(PUSH2));
    
    myScreen.clear();
}



// Add loop code
void loop() {
}


