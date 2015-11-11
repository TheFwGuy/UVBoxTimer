///
/// @mainpage	LCD_protocol100
///
/// @details	Example
/// @n
/// @n
/// @n @a		Developed with [embedXcode+](http://embedXcode.weebly.com)
///
/// @author		Rei VILO
/// @author		http://embeddedcomputing.weebly.com
///
/// @date		05/10/13 16:55
/// @version	<#version#>
///
/// @copyright	(c) Rei VILO, 2013
/// @copyright	CC = BY SA NC
///
/// @see		ReadMe.txt for references
///


///
/// @file		LCD_protocol100.ino
/// @brief		Main sketch
///
/// @details	<#details#>
/// @n @a		Developed with [embedXcode+](http://embedXcode.weebly.com)
///
/// @author		Rei VILO
/// @author		http://embeddedcomputing.weebly.com
/// @date		05/10/13 16:55
/// @version	<#version#>
///
/// @copyright	(c) Rei VILO, 2013
/// @copyright	CC = BY SA NC
///
/// @see		ReadMe.txt for references
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
#elif defined(ENERGIA) // LaunchPad MSP430, Stellaris and Tiva, Experimeter Board FR5739 specific
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
#include "Arduino.h"

// Include application, user and local libraries
// Screen selection
#define K35 // K35 HI32 W32 ILI9225B HY28A ST7735 PicasoSPE PicasoSGC

#if defined(ILI9225B)
#include "screen_ILI9225B.h"
Screen_ILI9225B myScreen;

#elif defined(W32)
#include "screen_W32.h"
Screen_W32 myScreen;

#elif defined(K35)
#include "screen_K35.h"
Screen_K35 myScreen;

#elif defined(HY28A)
#include "screen_HY28A.h"
Screen_HY28A myScreen;

#elif defined(HI32)
#include "screen_HI32.h"
Screen_HI32 myScreen;

#elif defined(ST7735)
#include "screen_ST7735.h"
Screen_ST7735 myScreen(ST7735R_RED_TAB);

#elif defined(PicasoSPE)
#include "screen_PicasoSPE.h"
#define mySerial Serial1
Screen_PicasoSPE myScreen(20, &mySerial);

#elif defined(PicasoSGC)
#include "screen_PicasoSGC.h"
#define mySerial Serial1
Screen_PicasoSGC myScreen(20, &mySerial);

#else
#error Unknown screen
#endif

// Define variables and constants
//uint32_t chrono;
uint32_t colour;


void protocol(uint16_t pixels)
{
    if ((pixels < myScreen.screenSizeX()) && (pixels < myScreen.screenSizeY()) && (pixels > 48)) {
        
        myScreen.setOrientation(0);
        myScreen.setPenSolid(false);
        
        uint16_t x100 = (myScreen.screenSizeX()-pixels)/2;
        uint16_t y100 = (myScreen.screenSizeY()-pixels)/2;
        
        myScreen.dRectangle(x100-1, y100-1, pixels+2, pixels+2, whiteColour);
        myScreen.setPenSolid(true);
        uint32_t chrono = millis();
        myScreen.dRectangle(x100, y100, pixels, pixels, grayColour);
        chrono = millis() - chrono;
        
        myScreen.setFontSize(myScreen.fontMax());
        myScreen.gText(x100 +2, y100 +2, "(" + i32toa(pixels) + ")");
        myScreen.gText(x100 +2, y100 +pixels -myScreen.fontSizeY() -2, i32toa(chrono) + " ms");
        
    }
}


// Add setup code
void setup()
{
    Serial.begin(9600);
    Serial.println("*** LCD_protocol");
    
#if defined(PicasoSPE)
    mySerial.begin(9600);
#endif;
    
    myScreen.begin();
    Serial.println(myScreen.WhoAmI());

#if defined(PicasoSPE)
    myScreen.setSpeed(38400);
    mySerial.begin(38400);
#endif;

    myScreen.setFontSize(myScreen.fontMax());
    myScreen.clear(darkGrayColour);
    
    for (uint8_t i=0; i<4; i++) {
        myScreen.setOrientation(i);
        switch (i) {
            case 0:
                colour = redColour;
                break;
            case 1:
                colour = greenColour;
                break;
            case 2:
                colour = blueColour;
                break;
            case 3:
                colour = yellowColour;
                break;
            default:
                break;
        }
        myScreen.setFontSolid(true);
        myScreen.gText(10, 10 + myScreen.fontSizeY(),     "solid font " + i32toa(i), colour);
        myScreen.setFontSolid(false);
        myScreen.gText(10, 10 + 2*myScreen.fontSizeY()+2, "not solid  " + i32toa(i), colour);
        myScreen.dRectangle(0, 0, myScreen.screenSizeX(), myScreen.screenSizeY(), colour);
    }
    myScreen.dRectangle(0, 0, myScreen.screenSizeX(), myScreen.screenSizeY(), whiteColour);
    
    protocol(300);
    protocol(200);
    protocol(100);
    protocol(50);
}

// Add loop code
void loop() 
{
    
}
