///
/// @file	LCD22_Touch_main.ino
/// @brief	Main sketch
///
/// @details	Example for LCD 2.2" with touch
/// @n @a	Developed with [embedXcode](http://embedXcode.weebly.com)
///
/// @author	Rei VILO
/// @author	embedXcode.weebly.com
/// @date	Feb 05, 2013
/// @version	109
///
/// @copyright	© Rei VILO, 2013
/// @copyright	CC = BY NC SA
///
/// @see
/// *		Rob's 2.2" LCD+Touch Panel BoosterPack in Store
/// @n		http://www.43oh.com/2013/01/robs-2-2-lcdtouch-panel-boosterpack-in-store/
/// *		Mar 19, 2012 / Nov 04, 2012 - RobG - ILI9225B (2.2" with touch)
/// @n		http://forum.43oh.com/topic/1956-universal-color-lcd-graphics-library/#entry23618
/// *		Jan 20, 2012 - timotet - Port to Stellaris
/// @n		http://forum.43oh.com/topic/2574-22-color-lcd-booster-pack-with-touch/?p=28092
/// *		(Universal) Color LCD graphics library by RobG 22 March 2012 - 03:09 AM
/// @n		http://forum.43oh.com/topic/1956-universal-color-lcd-graphics-library/
/// *		Color LCD Booster Pack by RobG 02 February 2012 - 10:01 PM
///	@n		http://forum.43oh.com/topic/1758-color-lcd-booster-pack/
/// *		Serial_LCD Library Suite
/// @n		http://embeddedcomputing.weebly.com/serial-lcd.html
/// * 		Fonts generated with MikroElektronika GLCD Font Creator 1.2.0.0
/// @n		http://www.mikroe.com
///


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

// Variables

// myScreen(1)=sold by RobG or myScreen(2)=myScreen=default=sold by 43oh store;
LCD_ILI9225B myScreen(1);

//LCD22_Touch myScreen(P1_0,    // Screen Chip Select
//            P1_5,    // Serial Clock
//            P1_7,    // Serial Data MOSI
//            P1_6,    // Serial Data MISO
//            P1_4,    // Data/Command
//            P2_1,	 // Touch Chip Select
//            P2_3);   // Reset



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
    myScreen.clear();
    
    myScreen.setPenSolid(true);
    myScreen.rectangle(0, 0, myScreen.screenSizeX()-1, myScreen.screenSizeY()-1, whiteColour);
    
    String s = utf2iso("¡Hola!");
    myScreen.setFontSize(2);
    myScreen.gText(0, 0, s);
    
#ifdef HARDWARE_SPI
    myScreen.gText(50, 0, "Hardware SPI");
#else
    myScreen.gText(50, 0, "Software SPI");
#endif

    if (false) {
        s = utf2iso("Now áéíóúñÑ¿");
        
        myScreen.setFontSize(2);
        //    myScreen.gText(0, 20, utf2iso(s));
        myScreen.gText(0, 20, s, redColour);
        
        myScreen.setFontSize(0);
        myScreen.gText(0, 40, s, greenColour);
        
        myScreen.setFontSize(1);
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
    
    myScreen.setFontSize(2);
    myScreen.gText(40,  80, "MSP430", redColour);
    myScreen.gText(40, 100, " LM4F ", redColour);
    
    myScreen.setFontSize(0);
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
            
            myScreen.setFontSize(1);
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


