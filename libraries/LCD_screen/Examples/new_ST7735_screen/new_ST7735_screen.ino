///
/// @mainpage	new_ST7735_screen
///
/// @details	<#details#>
/// @n
/// @n
/// @n @a	Developed with [embedXcode](http://embedXcode.weebly.com)
///
/// @author	Rei VILO
/// @author	embedXcode.weebly.com
/// @date	mars 07, 2013 09:18
/// @version	<#version#>
///
/// @copyright	© Rei VILO, 2013
/// @copyright	CC = BY NC SA
///
/// @see	ReadMe.txt for references
///


///
/// @file	new_ST7735_screen.ino
/// @brief	Main sketch
///
/// @details	<#details#>
/// @n @a	Developed with [embedXcode](http://embedXcode.weebly.com)
///
/// @author	Rei VILO
/// @author	embedXcode.weebly.com
/// @date	mars 07, 2013 09:18
/// @version	<#version#>
///
/// @copyright	© Rei VILO, 2013
/// @copyright	CC = BY NC SA
///
/// @see	ReadMe.txt for references
/// @n
///


// Core library - IDE-based
#if defined(WIRING) // Wiring specific
#include "Wiring.h"
#elif defined(MAPLE_IDE) // Maple specific
#include "WProgram.h"
#elif defined(MPIDE) // chipKIT specific
#include "WProgram.h"
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

// Options
//#define GAUGE

// Include application, user and local libraries
#if defined(HARDWARE_SPI)
#include "SPI.h"
#endif

#include "LCD_screen.h"
#include "ST7735_screen.h"
//#include "HY28A_screen.h"


//#define WITH_GAUGE
#ifdef WITH_GAUGE
#include "LCD_gauge.h"
#endif

LCD_ST7735 myScreen(ST7735R_RED_TAB);
//LCD_HY28A myScreen;

int16_t x, y;
int16_t dx, dy;

#ifdef WITH_GAUGE
gGauge myGauge;
#endif

// Setup
void setup() {
    Serial.begin(9600);
    Serial.println("\n\n\n***");
    
    myScreen.begin();
}


// Loop
void loop() {
    myScreen.setPenSolid(false);
    myScreen.rectangle(0, 0, myScreen.screenSizeX()-1, myScreen.screenSizeY()-1, whiteColour);
    myScreen.gText(10, 10, "hola!");
    delay(1000);
    
    //    uint32_t t0, t1;
    //    myScreen.setFontSolid(true);
    //
    //    myScreen.setPenSolid(false);
    //    t0 = millis();
    //    myScreen.circle(myScreen.screenSizeX()/2, myScreen.screenSizeY()/2, myScreen.screenSizeX()/2-2, yellowColour);
    //    t1 = millis();
    //    myScreen.gText(10, 10, String(t1-t0) + " ms", redColour);
    //
    //    myScreen.setPenSolid(true);
    //    t0 = millis();
    //    myScreen.circle(myScreen.screenSizeX()/2, myScreen.screenSizeY()/2, myScreen.screenSizeX()/2-2, whiteColour);
    //    t1 = millis();
    //    myScreen.gText(10, 20, String(t1-t0) + " ms", blueColour);
    
    myScreen.clear();
    myScreen.gText(10, 20, "clear");
    delay(1000);
    
    myScreen.gText(10, 30, "small");
    myScreen.gText(60, 30, "BIG", redColour, yellowColour, 3, 2);
    
    myScreen.setFont(0);
    myScreen.setFontSolid(true);
    myScreen.gText(10, 110, "Font g0", whiteColour, blueColour);
    myScreen.setFontSolid(false);
    myScreen.gText(80, 110, "Font g0", whiteColour, blueColour);
    myScreen.setFont(1);
    myScreen.setFontSolid(true);
    myScreen.gText(10, 130, "Font g1", violetColour, greenColour);
    delay(1000);
    
    myScreen.gText(10, 40, "setBacklight off");
    delay(500);
    myScreen.setBacklight(LOW);
    delay(500);
    myScreen.setBacklight(HIGH);
    myScreen.gText(10, 50, "setBacklight on");
    delay(1000);
    
    
    myScreen.setPenSolid(false);
    myScreen.rectangle(10, 10, 60, 60, blueColour);
    myScreen.gText(10, 60, "rectangle");
    delay(1000);
    
    myScreen.setPenSolid(true);
    myScreen.rectangle(20, 20, 70, 70, redColour);
    myScreen.gText(10, 70, "solidRectangle");
    delay(1000);
    
    myScreen.setPenSolid(false);
    myScreen.circle(80, 80, 20, yellowColour);
    myScreen.gText(10, 80, "circle");
    delay(1000);
    
    myScreen.setPenSolid(true);
    myScreen.circle(90, 90, 20, greenColour);
    myScreen.gText(10, 90, "solidCircle");
    delay(1000);
    
    myScreen.line(0, 0, myScreen.screenSizeX()-1, myScreen.screenSizeY()-1, cyanColour);
    myScreen.gText(10, 100, "line");
    delay(1000);
    
    myScreen.invert(true);
    delay(1000);
    myScreen.invert(false);
    delay(1000);
    
    //    for (uint8_t i=0; i<127; i++)
    //        myScreen.point(random(myScreen.maxX()), random(myScreen.maxY()), random(0xffff));
    //    myScreen.text(10, 110, "point");
    //    delay(1000);
    
    for (uint8_t i=0; i<4; i++) {
        myScreen.clear();
        myScreen.setOrientation(i);
        myScreen.setPenSolid(false);
        myScreen.setFont(0);
        myScreen.rectangle(0, 0, myScreen.screenSizeX()-1, myScreen.screenSizeY()-1, grayColour);
        myScreen.gText(10, 10, "setOrientation (" + String("0123").substring(i, i+1) + ")");
        myScreen.rectangle(10, 20, 50, 60, greenColour);
        myScreen.circle(70, 80, 10, blueColour);
        myScreen.line(30, 40, 70, 80, yellowColour);
        
        myScreen.setFont(0);
        myScreen.gText(10, 50, "font g0", whiteColour, blueColour);
        myScreen.setFont(1);
        myScreen.gText(10, 70, "font g1", violetColour, greenColour);
        
        delay(1000);
    }
    
#ifdef WITH_GAUGE
    
    myScreen.setOrientation(1);
    myScreen.clear(grayColour);
    
    myGauge.dDefine(&myScreen, 0, 0, myScreen.maxX(), myScreen.maxY(), 0, 100);
    for (uint8_t i=0; i<100; i += 2) {
        myGauge.draw(i, "m3");
        delay(100);
    }
    myGauge.draw(50, "m3");
    delay(1000);
    
#endif
    
    
    myScreen.setOrientation(0);
    myScreen.clear();
    myScreen.gText(10, 40, "bye!", redColour, yellowColour, 3, 3);
    delay(1000);
    
    myScreen.gText(10, 60, "off");
    delay(1000);
    
    myScreen.setBacklight(false);
    myScreen.setDisplay(false);
    
    while(true);
}

