///
/// @file	LCD_screen_109.ino
/// @brief	Main sketch
///
/// @details	<#details#>
/// @n @a	Developed with [embedXcode](http://embedXcode.weebly.com)
///
/// @author	Rei VILO
/// @author	embedXcode.weebly.com
/// @date	juin 13, 2013 14:42
/// @version	<#version#>
///
/// @copyright 	(c) Rei VILO, 2013
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

// Include application, user and local libraries
#define HY28A // HI32 ILI9225B HY28A ST7735 PicasoSPE PicasoSGC

#if defined(ILI9225B)
#include "ILI9225B_screen.h"
LCD_ILI9225B myScreen;

#elif defined(HY28A)
#include "HY28A_SRAM_screen.h"
LCD_HY28A_SRAM myScreen(PE_5, NULL, PC_4, PC_5, PC_6, PC_7);
//LCD_HY28A(uint8_t pinScreenChipSelect = PE_5, uint8_t pinScreenReset = PB_5,
//          uint8_t pinTouchChipSelect = PA_6, uint8_t pinTouchSerialClock = PA_7,
//          uint8_t pinTouchSerialDataMOSI = PA_3, uint8_t pinTouchSerialDataMISO = PA_2);

#elif defined(HI32)
#include "HI32_screen.h"
LCD_HI32 myScreen;

#elif defined(ST7735)
#include "ST7735_screen.h"
LCD_ST7735 myScreen(ST7735R_RED_TAB);

#elif defined(PicasoSPE)
#include "PicasoSPE_screen.h"
#define mySerial Serial1
LCD_PicasoSPE myScreen(20, &mySerial);

#elif defined(PicasoSGC)
#include "PicasoSGC_screen.h"
#define mySerial Serial1
LCD_PicasoSGC myScreen(20, &mySerial);

#else
#error Unkwon screen
#endif


#include "LCD_GUI.h"

// Define variables and constants
uint16_t x, y, z;
uint32_t l;
button b0;
button b1;
button b2;
button b3;
button b4;

uint16_t result=0;
uint16_t ui=255;
uint16_t option=0;
textBox myTextBox;

// menu declaration and size
item myMenuItems[] = {
    {     0x0000, "Menu 0"        }  ,
    {     0x1000, "Item 1"        }  ,
    {     0x1100, "Item 11"       }  ,
    {     0x1200, "Item 12"       }  ,
    {     0x2000, "Item 2"        }  ,
    {     0x2100, "Item 21"       }  ,
    {     0x2110, "Item 211"      }  ,
    {     0x2120, "Item 212"      }  ,
    {     0x2121, "Item 2121"     }  ,
    {     0x2122, "Item 2122"     }  ,
    {     0x2200, "Item 22"       }  ,
    {     0x2300, "Item 23"       }  ,
    {     0x3000, "Item 3"        }
};

uint8_t nItems = sizeof(myMenuItems) / sizeof(myMenuItems[0]);
uint32_t sizeRAW;

// Add setup code
void setup() {
    Serial.begin(9600);
    delay(10);
    Serial.println("\n\n\n***");
    
    myScreen.begin();
    myScreen.setOrientation(1);
    myScreen.clear();
    
    
    Serial.println(myScreen.WhoAmI());
    Serial.print("touch ");
    Serial.println(myScreen.isTouch(), DEC);
    Serial.print("readable ");
    Serial.println(myScreen.isReadable(), DEC);
    
    myScreen.setFontSize(myScreen.fontMax());
    myScreen.gText((myScreen.screenSizeX()-6*myScreen.fontSizeX())/2,
                   (myScreen.screenSizeY()-myScreen.fontSizeY())/2, "hello!", redColour);
    myScreen.setPenSolid(false);
    myScreen.dRectangle(0, 0, myScreen.screenSizeX(), myScreen.screenSizeY(), redColour);
    delay(1000);
    myScreen.clear();
    
    //    myScreen.initSD();
    myScreen.setPenSolid(false);
    myScreen.dRectangle(0, 0, myScreen.screenSizeX(), myScreen.screenSizeY(), grayColour);
    
    myScreen.setPenSolid(true);
    myScreen.setFontSolid(true);
    myScreen.setFontSize(1);
    myScreen.gText( 0, 210, myScreen.WhoAmI());
    
    //    myScreen.setTouch(true);
    
    myScreen.setFontSize(1);
    myScreen.gText(0,  0, "         1         2         3         4");
    myScreen.gText(0, 20, "1234567890123456789012345678901234567890");
    myScreen.gText(0, 60, i32toa(myScreen.fontSizeX(), 1, 0, 8));
    
    myScreen.setFontSize(0);
    myScreen.gText(0,  80, "         1         2         3         4         5");
    myScreen.gText(0, 100, "12345678901234567890123456789012345678901234567890123");
    myScreen.gText(0, 120, i32toa(myScreen.fontSizeX(), 1, 0, 8));
    
    dLabel(&myScreen, 20, 30, 220, 25, "label left",   redColour,   yellowColour, 1, 0, 9);
    dLabel(&myScreen, 20, 60, 220, 25, "label center", greenColour, violetColour, 0, 0, 9);
    dLabel(&myScreen, 20, 90, 220, 25, "label right",  blueColour,  cyanColour,   2, 0, 9);
    
    b0.dStringDefine(&myScreen,  10, 160, 60, 40, "Dialog", whiteColour, grayColour);
    b1.dStringDefine(&myScreen,  70, 160, 60, 40, "Menu",   whiteColour, blueColour);
    b2.dStringDefine(&myScreen, 130, 160, 60, 40, "Slider", whiteColour, greenColour);
    b4.dStringDefine(&myScreen, 190, 160, 60, 40, "Text",   whiteColour, yellowColour);
    b3.dStringDefine(&myScreen, 250, 160, 60, 40, "Stop",   whiteColour, redColour);
    b0.enable(true);
    b1.enable(true);
    b2.enable(true);
    b3.enable(true);
    b4.enable(true);
    b0.draw();
    b1.draw();
    b2.draw();
    b3.draw();
    b4.draw();
    
    myScreen.setFontSize(1);
    dLabel(&myScreen,  10, 120, 60, 40, i32toa(option), grayColour,  blackColour, 0, 2, 2);
    dLabel(&myScreen,  70, 120, 60, 40, htoa(result),   blueColour,  blackColour, 0, 2, 2);
    dLabel(&myScreen, 130, 120, 60, 40, i32toa(ui),     greenColour, blackColour, 0, 2, 2);
    
    myScreen.setPenSolid(false);
    myScreen.dRectangle(0, 0, myScreen.screenSizeX(), myScreen.screenSizeY(), darkGrayColour);
}

uint8_t c;
bool b=true;
uint32_t address = 0;

// Add loop code
void loop() {
    
    if (myScreen.getTouch(x, y, z)) {
        myScreen.setFontSolid(true);
        myScreen.setFontSize(0);
        myScreen.gText( 250, 225, "o", yellowColour);
        myScreen.dRectangle(x, y, 3, 3, violetColour);
        
        if (b0.check()) {
            option = dialog(&myScreen, "Options!", 2, whiteColour, myScreen.halveColour(grayColour), grayColour, \
                            "First option text",  "First",  whiteColour, yellowColour, myScreen.halveColour(yellowColour), \
                            "Second option text", "Second", whiteColour, greenColour,  myScreen.halveColour(greenColour), \
                            "Third option text",  "Third",  whiteColour, redColour,    myScreen.halveColour(redColour));
            
            dLabel(&myScreen, 10, 120, 60, 40, i32toa(option), grayColour, blackColour, 0, 2, 2);
            Serial.print("dialog >\t");
            Serial.println(option, HEX);
        }
        // menu
        else if (b1.check()) {
            result = menu(&myScreen, myMenuItems, nItems, whiteColour, blueColour, myScreen.halveColour(blueColour));
            dLabel(&myScreen, 70, 120, 60, 40, htoa(result), blueColour, blackColour, 0, 2, 2);
            Serial.print("menu >\t");
            Serial.println(result, HEX);
        }
        // slider
        else if (b2.check()) {
            if (slider(&myScreen, ui)) {
                dLabel(&myScreen, 130, 120, 60, 40, i32toa(ui), greenColour, blackColour, 0, 2, 2);
                Serial.print("slider >\t");
                Serial.println(i32toa(ui));
            }
        }
        // text
        else if (b4.check()) {
            uint32_t address = 0;
            uint16_t x0 = (myScreen.screenSizeX() - 200) /2;
            uint16_t y0 = (myScreen.screenSizeY() - 200) /2;
#if (USE_SPI_SRAM == 1)
            address = 0;
            if (myScreen.isStorage()) myScreen.copyArea(x0, y0, 200, 200, address);
#endif
            myScreen.setPenSolid(false);
            myScreen.dRectangle(x0, y0, 200, 200, grayColour);
            myTextBox.dDefine(&myScreen, x0+1, y0+1, 200-2, 200-2, whiteColour, darkGrayColour, 1);
            myTextBox.clear();
            
            myTextBox.print("Smart Text Box");
            delay(500);
            myTextBox.scroll();
            delay(500);
            myTextBox.scroll();
            delay(500);
            
            myTextBox.print("Lorem ipsum dolor sit amet, consectetur adipiscing elit.");
            delay(500);
            myTextBox.scroll();
            delay(500);
            myTextBox.scroll();
            delay(500);
            
            myTextBox.print("with automatic line formatting...");
            delay(500);
            myTextBox.scroll();
            delay(500);
            myTextBox.scroll();
            delay(500);

            myTextBox.print("Sed non risus. Suspendisse lectus tortor, dignissim sit amet, adipiscing nec, ultricies sed, dolor.");
            delay(500);
            myTextBox.scroll();
            delay(500);
            myTextBox.scroll();
            delay(500);
            myTextBox.print("...and automatic scrolling.");
            delay(500);
            myTextBox.scroll();
            delay(500);

#if (USE_SPI_SRAM == 1)
            address = 0;
            if (myScreen.isStorage()) myScreen.pasteArea(x0, y0, 200, 200, address);
#endif
        }
        // quit
        else if (b3.check()) {
            option = dialog(&myScreen, "Quit?", 3, whiteColour, myScreen.halveColour(grayColour), grayColour, \
                            "Are you sure to quit?", "Yes", whiteColour, greenColour, myScreen.halveColour(greenColour), \
                            "Please confirm.","No",whiteColour, redColour, myScreen.halveColour(redColour));
            Serial.print("Quit? >\t");
            Serial.println(option, DEC);
            if ( 1 == option ) {
                //                myScreen.off();
                myScreen.clear();
                Serial.println("END");
                while(true);
            }
        }
    }
    
    myScreen.setFontSize(0);
    myScreen.setFontSolid(true);
    myScreen.gText(  64, 225,  i32toa(x, 1, 0, 6), greenColour);
    myScreen.gText( 128, 225,  i32toa(y, 1, 0, 6), redColour);
    myScreen.gText( 250, 225, ttoa(millis()-l, 6), yellowColour);
    l=millis();
    
}

