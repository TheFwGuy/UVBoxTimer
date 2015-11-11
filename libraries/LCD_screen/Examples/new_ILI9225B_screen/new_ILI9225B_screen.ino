/// 
/// @mainpage	new_LCD22_screen 
///
/// @details	<#details#>
/// @n 
/// @n 
/// @n @a	Developed with [embedXcode](http://embedXcode.weebly.com)
/// 
/// @author	Rei VILO
/// @author	embedXcode.weebly.com
/// @date	mars 07, 2013 11:06
/// @version	<#version#>
/// 
/// @copyright	© Rei VILO, 2013
/// @copyright	CC = BY NC SA
///
/// @see	ReadMe.txt for references
///


///
/// @file	new_LCD22_screen.ino 
/// @brief	Main sketch
///
/// @details	<#details#>
/// @n @a	Developed with [embedXcode](http://embedXcode.weebly.com)
/// 
/// @author	Rei VILO
/// @author	embedXcode.weebly.com
/// @date	mars 07, 2013 11:06
/// @version	<#version#>
/// 
/// @copyright	© Rei VILO, 2013
/// @copyright	CC = BY NC SA
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

// Core library
#if defined(ENERGIA) // LaunchPad specific
#include "Energia.h"
#else
#error Board not supported
#endif

// Include application, user and local libraries
#include "ILI9225B_screen.h"

// Following includes for Energia only
#include "SPI.h"

// myScreen(1)=sold by RobG or myScreen(2)=myScreen=default=sold by 43oh store;
LCD_ILI9225B myScreen(1);

// Variables
uint16_t x, y, z;
uint8_t fsm;

///
/// @brief	Setup code
///
void setup() {
    Serial.begin(9600);
    
#ifdef HARDWARE_SPI
    Serial.println("*** Hardware SPI");
#else
    Serial.println("*** Software SPI");
#endif
    
    myScreen.begin();
    myScreen.calibrateTouch();
    
    myScreen.setPenSolid(false);
    myScreen.rectangle(0, 0, myScreen.screenSizeX() -1, myScreen.screenSizeY()-1, grayColour);
    
    String s = utf2iso("¡Hola!");
    myScreen.setFont(2);
    myScreen.gText(0, 0, s, whiteColour);
    
#ifdef HARDWARE_SPI
    myScreen.gText(50, 0, "Hardware SPI");
#else
    myScreen.gText(50, 0, "Software SPI");
#endif

    if (false) {
        s = utf2iso("Now áéíóúñÑ¿");
        
        myScreen.setFont(2);
        //    myScreen.gText(0, 20, utf2iso(s));
        myScreen.gText(0, 20, s, redColour);
        
        myScreen.setFont(3);
        myScreen.gText(0, 40, s, greenColour);
        
        myScreen.setFont(4);
        myScreen.gText(0, 60, s, blueColour);
    }
    
    myScreen.point(100, 5, orangeColour);
    
    myScreen.line(20, 20, 80, 60, yellowColour);
    
    myScreen.setPenSolid(true);
    myScreen.rectangle(50, 50, 58, 58, blueColour);
    
    myScreen.setPenSolid(false);
    myScreen.rectangle(60, 60, 68, 68, greenColour);
    
    myScreen.setPenSolid(true);
    myScreen.circle(20, 70, 9, grayColour);
    
    myScreen.setPenSolid(false);
    myScreen.circle(20, 92, 9, magentaColour);
    
//    myScreen.setBacklight(false);
//    delay(1000);
//    myScreen.setBacklight(true);
    
    myScreen.setFont(2);
    myScreen.gText(40,  80, "MSP430", redColour);
    myScreen.gText(40, 100, " LM4F ", redColour);
    
    myScreen.setFont(0);
    myScreen.gText(0, 130, "         1         2", whiteColour);
    myScreen.gText(0, 140, "123456789012345678901", whiteColour);
    
    fsm = 2;
    Serial.print("FSM");
}

///
/// @brief	Loop code
///
void loop() {
    Serial.print(".");
    Serial.print(fsm, DEC);
    
    switch (fsm) {
        case 2:
            myScreen.setPenSolid(true);
            myScreen.rectangle(10, 190, 60, 210, myScreen.halveColour(redColour));
            myScreen.gText(20, 195, "QUIT", whiteColour);
            myScreen.setPenSolid(false);
            myScreen.rectangle(11, 191, 59, 209, redColour);
            
            myScreen.setPenSolid(true);
            myScreen.rectangle(70, 190, 130, 210, myScreen.halveColour(greenColour));
            myScreen.gText(73, 195, "Calibrate", whiteColour);
            myScreen.setPenSolid(false);
            myScreen.rectangle(71, 191, 129, 209, greenColour);
            
            myScreen.setFont(1);
            myScreen.gText(myScreen.screenSizeX()/2 -3*myScreen.fontSizeX(), myScreen.screenSizeY()/2 -myScreen.fontSizeY(), "Paint!", whiteColour);
            
            fsm = 1;
            break;
            
        case 1:
            if (myScreen.getTouch(x, y, z)) {
                
                if ((x>2) && (y>2) && (x<myScreen.screenSizeX()-2) && (y<myScreen.screenSizeY()-2) )
                    myScreen.rectangle(x-2, y-2, x+2, y+2, whiteColour);
                
                if ((x>10) && (x<60) && (y>190) && (y<210)) {
                    delay(1000);
                    
                    fsm = 0;
                }
                if ((x>70) && (x<130) && (y>190) && (y<210)) {
                    delay(1000);
                    myScreen.calibrateTouch();
                    
                    fsm = 2;
                }
            }
            break;
            
        case 0:
            myScreen.clear();
            while(true);
            break;
            
        default:
            delay(100);
            break;
    }
    
    //        Serial.print("loop\t");
    //        Serial.print(x, DEC);
    //        Serial.print("\t");
    //        Serial.print(y, DEC);
    //        Serial.print("\t");
    //        Serial.println(z, DEC);
    //
}


