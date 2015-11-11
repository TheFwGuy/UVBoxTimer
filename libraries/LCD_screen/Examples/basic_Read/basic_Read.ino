///
/// @file	HY28A_basic_main.ino
/// @brief	Main sketch
///
/// @details	Examples for HY28A
/// @n @a	Developed with [embedXcode](http://embedXcode.weebly.com)
///
/// @author	Rei VILO
/// @author	embedXcode.weebly.com
/// @date	janv. 25, 2013 16:16
/// @version	107
///
/// @copyright	© Rei VILO, 2013
/// @copyright	CC = BY NC SA
///
/// @see	ReadMe.txt for references
/// @n
///


// Core library for code-sense
//#if defined(WIRING) // Wiring specific
//#include "Wiring.h"
//#elif defined(MAPLE_IDE) // Maple specific
//#include "WProgram.h"
//#elif defined(MPIDE) // chipKIT specific
//#include "WProgram.h"
//#elif defined(ENERGIA) // LaunchPad, FraunchPad and StellarPad specific
#include "Energia.h"
//#elif defined(CORE_TEENSY) // Teensy specific
//#include "WProgram.h"
//#elif defined(ARDUINO) && (ARDUINO >= 100) // Arduino 1.0 and 1.5 specific
//#include "Arduino.h"
//#elif defined(ARDUINO) && (ARDUINO < 100) // Arduino 23 specific
//#include "WProgram.h"
//#else // error
//#error Platform not defined
//#endif

#define WITH_READ
//#define WITH_TOUCH
//#define WITH_CLOCK
//#define WITH_FONTS
//#define WITH_GAUGE

// Include application, user and local libraries
#include "SPI.h"
#include "HY28A_screen.h"

#ifdef WITH_CLOCK
#include "LCD_clock.h"
#endif

#ifdef WITH_GAUGE
#include "LCD_gauge.h"
#endif


// Define variables and constants
LCD_HY28A myScreen;
uint16_t x, y, z;

#ifdef WITH_CLOCK
gClock myClock;
uint8_t h, m, s;
#endif

#ifdef WITH_GAUGE
gGauge myGauge;
#endif

// Add setup code
void setup() {
    
    Serial.begin(9600);
    Serial.println("\n\n\n");
    
    Serial.println("*** NEW LIBRARY");
    Serial.print("begin... ");
    myScreen.begin();
    Serial.println("done");
    
    //    Serial.println("line");
    //    myScreen.line(0, 0, 100, 100, greenColour);
    //    delay(1000);
    //
    //    Serial.println("clear");
    //    myScreen.clear();
    //    myScreen.rectangle(20, 40, 80, 80, grayColour);
    //    myScreen.gText(10, 50, utf2iso("¡hola!"), redColour, blackColour);
    //
    myScreen.setOrientation(1);
    Serial.print("clear... ");
    myScreen.clear(grayColour);
    Serial.println("done");
    
#ifdef WITH_CLOCK
    myClock.dDefine(&myScreen, 0, 0, myScreen.screenSizeX(), myScreen.screenSizeY());
    h = 10;
    m = 10;
    s = 55;
#endif
    
#ifdef WITH_FONTS
    String s = utf2iso("font ñÑ¿");
    
    myScreen.clear(grayColour);
    myScreen.setFontSolid(false);
    myScreen.setFont(0);
    myScreen.gText(20, 40, s, blueColour);
    myScreen.setFont(1);
    myScreen.gText(20, 60, s, blueColour);
    myScreen.setFont(2);
    myScreen.gText(20, 80, s, blueColour);
    myScreen.setFont(0);
    myScreen.gText(20, 100, s, blueColour, blackColour, 2, 2);
    
    myScreen.setFontSolid(true);
    myScreen.setFont(0);
    myScreen.gText(120, 40, "font0", blueColour);
    myScreen.setFont(1);
    myScreen.gText(120, 60, "font1", blueColour);
    myScreen.setFont(2);
    myScreen.gText(120, 80, "font2", blueColour);
    myScreen.setFont(0);
    myScreen.gText(120, 100, "font0x2", blueColour, blackColour, 2, 2);
#endif
    
#ifdef WITH_GAUGE
    myScreen.setOrientation(1);
    myScreen.clear(grayColour);
    
    myGauge.dDefine(&myScreen, 0, 0, myScreen.screenSizeX(), myScreen.screenSizeY(), -100, 100);
    for (int8_t i=-100; i<100; i += 10) {
        myGauge.draw(i, String(i) + "m3");
        //        delay(100);
    }
    myGauge.draw(50, " m3 ");
    delay(1000);
    
    while(true);
#endif
    
#ifdef WITH_TOUCH
    myScreen.calibrateTouch();
    
    myScreen.setPenSolid(false);
    myScreen.rectangle(0, 0, myScreen.screenSizeX()-1, myScreen.screenSizeY()-1, yellowColour);
    myScreen.setFontSolid(true);
    myScreen.gText(10, 90, utf2iso("¡A dibujar ñokis!"), yellowColour, blackColour, 2, 2);
#endif
    
    
#ifdef WITH_READ
    myScreen.calibrateTouch();

    for (uint8_t i=0; i<128; i++) {
        for (uint8_t j=0; j<128; j++) {
            myScreen.point(10+i, 10+j, myScreen.calculateColour(2*i, 2*j, 254-i-j));
        }
    }
    
    myScreen.setPenSolid(true);
    myScreen.dRectangle(10, 148, 32, 32, redColour);
    myScreen.dRectangle(52, 148, 32, 32, greenColour);
    myScreen.dRectangle(94, 148, 32, 32, blueColour);
    
    myScreen.setPenSolid(false);
    myScreen.dRectangle(9, 9, 128, 128, whiteColour);
#endif
    
    //
    //    for (uint8_t i=1; i<4; i++) {
    //        uint16_t j = 0;
    //
    //
    //        myScreen.clear();
    //        myScreen.setOrientation(i);
    //        myScreen.setPenSolid(false);
    //        myScreen.line(0, 0, 100, 100, redColour);
    //        myScreen.rectangle(0, 0, myScreen.screenSizeX()-1, myScreen.screenSizeY()-1, whiteColour);
    //        myScreen.setPenSolid(true);
    //        myScreen.rectangle(130, 40, 140, 50, violetColour);
    //
    //        myScreen.setFontSolid(false);
    //        myScreen.rectangle(20, 40, 80, 80, greenColour);
    //
    //        myScreen.setPenSolid(true);
    //        myScreen.rectangle(130, 40, 140, 50, violetColour);
    //        myScreen.line(       120,     41,      150,     47, blueColour);
    //
    //
    //        myScreen.gText(10, 60, "orientation " + String(i), redColour, blackColour);
    //
    //
    //        myScreen.setPenSolid(true);
    //        do {
    //            if (myScreen.getTouch(x, y, z)) {
    //                myScreen.rectangle(x-1, y-1, x+1, y+1, yellowColour);
    //
    //                myScreen.line(       133,     41,      135,     47, blueColour);
    //                myScreen.point(x, y, redColour);
    //                j++;
    //            }
    //        }   while (j<16);
    //        delay(2000);
    //    }
    //
    //
    //    Serial.println("text");
    //
    //    myScreen.calibrateTouch();
    //
    //    myScreen.setPenSolid(false);
    //    myScreen.rectangle(0, 0, myScreen.screenSizeX()-1, myScreen.screenSizeY()-1, yellowColour);
    //    myScreen.setFontSolid(true);
    //    myScreen.gText(10, 90, utf2iso("¡A dibujar ñokis!"), yellowColour, blackColour, 2, 2);
    //
}


// Add loop code
void loop() {
#ifdef WITH_CLOCK
    
    //    myClock.draw(h, m, s, String(s));
    myClock.draw(h, m, s);
    s++;
    if (s > 59) {
        m++;
        s = 0;
    }
    if (m > 59) {
        h++;
        m = 0;
    }
    if (h > 23) {
        h = 0;
    }
    delay(100);
#endif
    
#ifdef WITH_TOUCH
    if (myScreen.getTouch(x, y, z)) {
        
        if (x < 1) x = 1;
        if (y < 1) y = 1;
        if (x > myScreen.screenSizeX()) x = myScreen.screenSizeX();
        if (y > myScreen.screenSizeY()) y = myScreen.screenSizeY();
        
        myScreen.rectangle(x-1, y-1, x+1, y+1, yellowColour);
    } else {
        delay(100);
    }
#endif
    
#ifdef WITH_READ
    if (myScreen.getTouch(x, y, z)) {
        
//        if ((x > 10) & (y > 10) & (x < 138) & (y < 138)) {
            z = myScreen.readPixel(x, y);
        Serial.println(z, HEX);
            myScreen.setPenSolid(true);
            myScreen.dRectangle(148, 10, 64, 64, z);
//        }
    } else {
        delay(100);
    }
#endif
}





