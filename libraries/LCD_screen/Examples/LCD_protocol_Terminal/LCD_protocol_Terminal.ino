///
/// @file		LCD_protocol.ino
/// @brief		Main sketch
///
/// @details	LCD_screen Test Protocol
/// @n @a		Developed with [embedXcode+](http://embedXcode.weebly.com)
///
/// @author		Rei Vilo
/// @author		http://embeddedcomputing.weebly.com
/// @date		03/04/2014 18:09
/// @version	122
///
/// @copyright	(c) Rei Vilo, 2010-2015
/// @copyright	All rights reserved
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
#elif defined(ENERGIA) // LaunchPad MSP430 G2 and F5529, Stellaris and Tiva, Experimeter Board FR5739 specific
#include "Energia.h"
#elif defined(MICRODUINO) // Microduino specific
#include "Arduino.h"
#elif defined(TEENSYDUINO) // Teensy specific
#include "Arduino.h"
#elif defined(ARDUINO) // Arduino 1.0 and 1.5 specific
#include "Arduino.h"
#else // error
#error Platform not defined
#endif

// Include application, user and local libraries
#include "SPI.h"
//#include "debug.h"

// test: PicasoSPE PicasoSGC
// ok: HX8353E W32 HI32 HY28A K35 ST7735 ILI9225B HY28A_SRAM ER8875
// nok:
// next:

// Define tests to perform
///
/// @brief      Tests
/// @details	Uncomment to select the test
/// @{


#define TEST_SPLASH 1                   ///< splash screen
#define TEST_COLOURS 1                  ///< ten colours
#define PROTOCOL_COLOURS 1              ///< 64x64x16 colours
#define PROTOCOL_FPS 1                  ///< fps frame per second
#define PROTOCOL_SQUARE 1               ///< 400, 300, 200 and 100 squares
#define PROTOCOL_FONT 1                 ///< all sizes for text
#define TEST_COPY_PASTE_COLOUR 1        ///< separate copy-paste for red, green, blue and white
#define TEST_COPY_PASTE_COLOUR_SD 1     ///< separate copy-paste for red, green, blue and white
#define PROTOCOL_TEXT 1                 ///< fonts with solid=true and false
#define PROTOCOL_COPY_PASTE 1           ///< 64x64 copy-paste
#define PROTOCOL_TOUCH 1                ///< touch test

/// @}

// Screen selection
#define ILI9340 // ER1963_SD // ER1963_SD ER1963 ER8875_SD ER8875 ER1963 HX8353E K35 K35_SPI K70 HI32 HI32b W32 ILI9340 ILI9225B HY28A HY28A_SRAM ST7735 PicasoSPE PicasoSGC PICadillo

#if defined(ILI9340)
#include "Screen_ILI9340.h"
Screen_ILI9340 myScreen;

#elif defined(ER1963_SD)
#include "Screen_ER1963_SD.h"
Screen_ER1963_SD myScreen;

#elif defined(ER1963)
#include "Screen_ER1963.h"
Screen_ER1963 myScreen;

#elif defined(ER8875_SD)
#include "Screen_ER8875_SD.h"
Screen_ER8875_SD myScreen(10, 9);

#elif defined(ER8875)
#include "Screen_ER8875.h"
Screen_ER8875 myScreen;

#elif defined(ILI9225B)
#include "Screen_ILI9225B.h"
Screen_ILI9225B myScreen;

#elif defined(HX8353E)
#include "Screen_HX8353E.h"
Screen_HX8353E myScreen;

#elif defined(W32)
#include "Screen_W32.h"
Screen_W32 myScreen;

#elif defined(K35)
#include "Screen_K35.h"
Screen_K35 myScreen;

#elif defined(K35_SPI)
#include "Screen_K35_SPI.h"
Screen_K35_SPI myScreen;

#elif defined(K70)
#include "Screen_K70.h"
Screen_K70 myScreen;

#elif defined(HY28A)
#include "Screen_HY28A.h"
Screen_HY28A myScreen;

#elif defined(HY28A_SRAM)
#include "Screen_HY28A_SRAM.h"
Screen_HY28A_SRAM myScreen;

#elif defined(HI32)
#include "Screen_HI32.h"
Screen_HI32 myScreen;

#elif defined(HI32b)
#include "Screen_HI32b.h"
Screen_HI32b myScreen;

#elif defined(ST7735)
#include "Screen_ST7735.h"
Screen_ST7735 myScreen(ST7735R_RED_TAB);

#elif defined(PICadillo)
#include "Screen_PICadillo.h"
Screen_PICadillo myScreen;

#elif defined(PicasoSPE)
#include "Screen_PicasoSPE.h"

#if defined(__LM4F120H5QR__)
//#include "SoftwareSerial.h"
//SoftwareSerial mySerial(PB_0, PB_1);
#define mySerial Serial1
Screen_PicasoSPE myScreen(PA_2, &mySerial);
#else
#define mySerial Serial1
Screen_PicasoSPE myScreen(4, &mySerial);
#endif

#elif defined(PicasoSGC)
#include "Screen_PicasoSGC.h"
#define mySerial Serial1
Screen_PicasoSGC myScreen(20, &mySerial);

#else
#error Unknown screen
#endif

#if (LCD_SCREEN_VIRTUAL_RELEASE < 123)
#error Required LCD_SCREEN_VIRTUAL_RELEASE 123
#endif

// Define variables and constants
uint16_t x, y, z;
uint8_t largeOrientation = 0;


///
/// @brief      protocolSquare
/// @details    measure time to draw a square with side=pixels
/// @param      pixels number of pixels of one side
///
void protocolSquare(uint16_t pixels)
{
    if ((pixels < myScreen.screenSizeX()) && (pixels < myScreen.screenSizeY()) && (pixels > 48))
    {
        
        myScreen.setOrientation(largeOrientation);
        myScreen.setPenSolid(false);
        
        uint16_t x100 = (myScreen.screenSizeX()-pixels)/2;
        uint16_t y100 = (myScreen.screenSizeY()-pixels)/2;
        
        myScreen.dRectangle(x100-1, y100-1, pixels+2, pixels+2, myColours.white);
        myScreen.setPenSolid(true);
        uint32_t chrono = millis();
        myScreen.dRectangle(x100, y100, pixels, pixels, myColours.grey);
        chrono = millis() - chrono;
        
        myScreen.setFontSize(0);
        myScreen.setPenSolid(false);
        myScreen.setFontSolid(false);
        myScreen.gText(x100 +2, y100 +2, "(" + i32toa(pixels) + ")", myColours.white);
        myScreen.gText(x100 +2, y100 +pixels -myScreen.fontSizeY() -2, i32toa(chrono) + " ms");
        
        Serial.print("Square(");
        Serial.print(pixels, DEC);
        Serial.print(")\t");
        Serial.println(chrono, DEC);
    }
}

void protocolFPS()
{
    uint32_t offr = 0;
    uint32_t offg = 0;
    uint32_t offb = 0;
    uint32_t ts = millis();
    uint32_t count = 0;
    
    myScreen.clear();
    
    uint16_t pixels = 128;
    if ((myScreen.screenSizeX() > 256) and (myScreen.screenSizeY() > 256))
    {
        pixels = 256;
    }
    
    do
    {
        myScreen.openArea((myScreen.screenSizeX() - pixels) /2, (myScreen.screenSizeY() - pixels) /2, pixels, pixels);
        
        for (int y = 0; y < pixels; y++)
        {
            for (int x = 0; x < pixels; x++)
            {
                uint8_t red = (x + offr);
                uint8_t green = (x + offg);
                uint8_t blue = (y + offb);
                myScreen.feedArea(myColours.calculate(red, green, blue));
            }
        }
        offr++;
        offg+=2;
        offb+=3;
        myScreen.closeArea();
        
        count++;
    }
    while (millis() - ts < 10000);
    
    float fps = count / 10.0 * pixels / 256.0 * pixels / 256.0;
    
    Serial.print("fps(256^2)\t");
    Serial.println(-fps, 1);
    Serial.print("sp1000f(256^2)\t");
    Serial.println(1000.0 / fps, 1);
}


///
/// @brief      testColours
/// @details    Test basic colours
/// @param      orientation default=1
///
void testColours(uint8_t orientation = 1)
{
    myScreen.setOrientation(orientation);
    myScreen.clear();
    uint16_t number = 10;
    uint16_t size = myScreen.screenSizeX() / number;
    myScreen.setFontSize(myScreen.fontMax());
    myScreen.gText(0, 0, "Colours");
    myScreen.setPenSolid();
    uint16_t tx = (size - myScreen.fontSizeX()) /2;
    uint16_t ty = (myScreen.screenSizeY() + size + myScreen.fontSizeY()) /2;
    
    myScreen.setFontSize(1);
    for (uint16_t i=0; i<number; i++)
    {
        myScreen.dRectangle(i*size, (myScreen.screenSizeY() - size) /2, size, size, myColours.code(i));
        myScreen.gText(i*size + tx, ty, String(i));
    }
}

///
/// @brief      protocolCopyPaste
/// @details    measure time to copy-paste a 64x64 area
/// @param      orientation default=1
///
void protocolCopyPaste(uint8_t orientation = 1)
{
    uint32_t chrono;
    uint16_t y;
    
    myScreen.clear();
    myScreen.setOrientation(orientation);
    
    myScreen.setPenSolid();
    myScreen.dRectangle(0, 0, 72, 72, myColours.grey);
    myScreen.setPenSolid();
    myScreen.dRectangle(myScreen.screenSizeX()/2, 0, 72, 72, myColours.grey);
    myScreen.setPenSolid();
    myScreen.dRectangle(0, myScreen.screenSizeY()/2, 72, 72, myColours.grey);
    myScreen.setPenSolid();
    myScreen.dRectangle(myScreen.screenSizeX()/2, myScreen.screenSizeY()/2, 72, 72, myColours.grey);
    
    // Image
    chrono = millis();
    
    if (myScreen.isArea())
    {
        
        myScreen.openArea(4, 4, 64, 64);
        for (uint8_t j=0; j<64; j++)
        {
            for (uint8_t i=0; i<64; i++)
            {
                myScreen.feedArea(myColours.calculate(4*i, 4*j, 254-2*i-2*j));
            }
        }
        myScreen.closeArea();
        
    } else {
        
        for (uint8_t i=0; i<64; i++)
        {
            for (uint8_t j=0; j<64; j++)
            {
                myScreen.point(i+4, j+4, myColours.calculate(4*i, 4*j, 254-2*i-2*j));
            }
        }
    }
    
    myScreen.setPenSolid(false);
    myScreen.dRectangle(1+4, 1+4, 62, 62, myColours.black);
    myScreen.line(0+4,   0+4, 63+4, 63+4, myColours.white);
    myScreen.line(32+4,  0+4, 63+4, 63+4, myColours.white);
    myScreen.line(0+4,  32+4, 63+4, 63+4, myColours.white);
    myScreen.dRectangle(0+4, 0+4, 64, 64, myColours.white);
    chrono = millis() - chrono;
    
    myScreen.setFontSize(0);
    y = 66+4;
    myScreen.gText(0, y +4, "0: Original");
    y += myScreen.fontSizeY();
    myScreen.setFontSize(1);
    myScreen.gText(0, y +4, i32toa(chrono) + " ms");
    
    Serial.print("Original(=");
    Serial.print(orientation, DEC);
    Serial.print(")\t");
    Serial.println(chrono, DEC);
    
    // Method 1
    if (myScreen.isReadable())
    {
        chrono = millis();
        for (uint16_t i=0; i<64; i++)
        {
            for (uint16_t j=0; j<64; j++)
            {
                myScreen.point(myScreen.screenSizeX()/2+i+4, j+4, myScreen.readPixel(i+4, j+4));
            }
        }
        chrono = millis() - chrono;
        myScreen.setFontSize(0);
        y = 66+4;
        myScreen.gText(myScreen.screenSizeX()/2, y +4, "1. point readPixel");
        y += myScreen.fontSizeY();
        myScreen.setFontSize(1);
        myScreen.gText(myScreen.screenSizeX()/2, y +4, i32toa(chrono) + " ms");
        
        Serial.print("point(readPixel())\t");
        Serial.println(chrono, DEC);
    }
    else
    {
        Serial.println("point(readPixel())\tNot readable");
    }
    
    // Method 2
    if (myScreen.isReadable())
    {
        chrono = millis();
        myScreen.copyPasteArea(0+4, 0+4, 0+4, myScreen.screenSizeY()/2+4, 64, 64);
        chrono = millis() - chrono;
        
        myScreen.setFontSize(0);
        y = myScreen.screenSizeY()/2 +66+4;
        myScreen.gText(0, y +4, "2. copyPaste", myColours.white);
        y += myScreen.fontSizeY();
        myScreen.setFontSize(1);
        myScreen.gText(0, y +4, i32toa(chrono) + " ms", myColours.white);
        
        Serial.print("copyPaste()\t");
        Serial.println(chrono, DEC);
    }
    else
    {
        Serial.println("copyPaste()\tNot readable");
    }
    
    // Method 3
    if (myScreen.isStorage())
    {
        uint32_t address;
        
        chrono = millis();
        address = 100;
        myScreen.copyArea(0+4, 0+4, 64, 64, address);
        address = 100;
        myScreen.pasteArea(myScreen.screenSizeX()/2 +4, myScreen.screenSizeY()/2+4, 64, 64, address);
        chrono = millis() - chrono;
        
        myScreen.setFontSize(0);
        y = myScreen.screenSizeY()/2 +66+4;
        myScreen.gText(myScreen.screenSizeX()/2, y +4, "3. copy-paste SD/SRAM");
        y += myScreen.fontSizeY();
        myScreen.setFontSize(1);
        myScreen.gText(myScreen.screenSizeX()/2, y +4, i32toa(chrono) + " ms");
        
        Serial.print("copyArea()+pasteArea() SD/SRAM\t");
        Serial.println(chrono, DEC);
    }
    else
    {
        Serial.println("copyArea()+pasteArea() SD/SRAM\tNo storage");
    }
}

///
/// @brief      protocolText
/// @details    measure time to draw text in 3 fonts, 4 orientations, 10x
///
void protocolText()
{
    uint32_t chrono1, chrono2;
    uint16_t colour;
    uint8_t k = 0;
    
    // Serial.print("fast gText... ");
    myScreen.clear(myColours.grey);
    myScreen.setFontSolid(true);
    chrono1 = millis();
    for (uint8_t j=0; j<10; j++)
    {
        for (uint8_t i=0; i<4; i++)
        {
            if      (k==1) colour = myColours.red;
            else if (k==2) colour = myColours.yellow;
            else if (k==3) colour = myColours.green;
            else if (k==4) colour = myColours.cyan;
            else if (k==5) colour = myColours.blue;
            else           colour = myColours.violet;
            k++;
            k %= 7;
            
            myScreen.setPenSolid(false);
            myScreen.dRectangle(0, 0, myScreen.screenSizeX(), myScreen.screenSizeY(), colour);
            
            myScreen.setOrientation(i);
            myScreen.setFontSize(0);
            myScreen.gText(4, 4, "font 0 on " + String(i), colour);
            myScreen.setFontSize(1);
            myScreen.gText(4, 14, "font 1 on " + String(i), colour);
            myScreen.setFontSize(2);
            myScreen.gText(4, 34, "font 2 on " + String(i), colour);
        }
    }
    chrono1 = millis()-chrono1;
    Serial.print("10xFontSolid(true)\t");
    Serial.println(chrono1, DEC);
    delay(1000);
    
    // Serial.print("slow gText... ");
    myScreen.clear(myColours.grey);
    delay(1000);
    
    myScreen.setFontSolid(false);
    chrono2 = millis();
    
    for (uint8_t j=0; j<10; j++)
    {
        for (uint8_t i=0; i<4; i++)
        {
            if      (k==1) colour = myColours.red;
            else if (k==2) colour = myColours.yellow;
            else if (k==3) colour = myColours.green;
            else if (k==4) colour = myColours.cyan;
            else if (k==5) colour = myColours.blue;
            else           colour = myColours.violet;
            k++;
            k %= 7;
            
            myScreen.setPenSolid(false);
            myScreen.dRectangle(0, 0, myScreen.screenSizeX(), myScreen.screenSizeY(), colour);
            
            myScreen.setOrientation(i);
            myScreen.setFontSize(0);
            myScreen.gText(4, 4, "font 0 on " + String(i), colour);
            myScreen.setFontSize(1);
            myScreen.gText(4, 14, "font 1 on " + String(i), colour);
            myScreen.setFontSize(2);
            myScreen.gText(4, 34, "font 2 on " + String(i), colour);
        }
    }
    
    chrono2 = millis()-chrono2;
    Serial.print("10xFontSolid(false)\t");
    Serial.println(chrono2, DEC);
    
    Serial.print("Ratio%\t");
    Serial.println((uint32_t)((uint64_t)(chrono1*100)/chrono2), DEC);
}

void protocolFont()
{
    uint32_t chrono;
    myScreen.clear();
    
    uint16_t x = 0;
    uint16_t y = 0;
    
    for (uint8_t k=0; k<myScreen.fontMax(); k++)
    {
        myScreen.setFontSize(k);
        y += myScreen.fontSizeY();
        x += myScreen.fontSizeX() *6;
    }
    
    x = (myScreen.screenSizeX() - x) /2;
    y = (myScreen.screenSizeY() - y) /2;
    
    chrono = millis();
    myScreen.setPenSolid(false);
    myScreen.setFontSolid(true);
    for (uint8_t i=0; i<myScreen.fontMax(); i++)
    {
        myScreen.setFontSize(i);
        myScreen.gText(x, y, "Hello!", myColours.yellow, myColours.black);
        myScreen.dRectangle(x, y, myScreen.fontSizeX()*6, myScreen.fontSizeY(), myColours.red);
        myScreen.dRectangle(x, y, myScreen.fontSizeX(), myScreen.fontSizeY(), myColours.red);
        
        y += myScreen.fontSizeY();
        x += myScreen.fontSizeX() *6;
    }
    chrono = millis() - chrono;
    Serial.print("Font Number\t");
    Serial.println(myScreen.fontMax(), DEC);
    Serial.print("Font Average\t");
    Serial.println(chrono / myScreen.fontMax(), DEC);
}

void protocolTouch()
{
    uint16_t counter = 32;
    uint16_t x, y, z;
    Serial.print("Touch\t");
    myScreen.clear();
    
    if (myScreen.isTouch())
    {
        //        myScreen.calibrateTouch();
        myScreen.setPenSolid();
        
        myScreen.setFontSize(1);
        myScreen.gText((myScreen.screenSizeX()-5*myScreen.fontSizeX())/2, (myScreen.screenSizeY()-myScreen.fontSizeY())/2,
                       "Draw!", myColours.yellow);
        
        
        while (counter)
        {
            if (myScreen.getTouch(x, y, z))
            {
                x = checkRange(x, 1, myScreen.screenSizeX()-2);
                y = checkRange(y, 1, myScreen.screenSizeY()-2);
                myScreen.dRectangle(x-1, y-1, 3, 3, myColours.yellow);
                counter--;
            }
        }
    } else {
        Serial.println("No touch");
    }
}

void protocolColours()
{
    myScreen.setOrientation(largeOrientation);
    uint32_t chrono1, chrono2;
    uint16_t x, y;
    uint16_t r, g, b;
    
    
    if ((myScreen.screenSizeY() > 256) && (myScreen.screenSizeX() > 512))
    {
        
        myScreen.clear();
        x = (myScreen.screenSizeX() - 512) /3;
        y = (myScreen.screenSizeY() - 256) /2;
        
        Serial.print("Points(256)\t");
        chrono1 = millis();
        for (r=0; r<32; r++)
        {
            for (g=0; g<64; g++)
            {
                for (b=0; b<32; b++)
                {
                    myScreen.point(x+(8*r+g%8)*1, y+(8*b+g/8)*1, (r << 11) | (g << 6) | b);
                }
            }
        }
        chrono1 = millis() - chrono1;
        Serial.println(chrono1, DEC);
        
        x *= 2;
        x += 256;
        
        if (myScreen.isArea())
        {
            
            Serial.print("feedArea\t");
            chrono2 = millis();
            myScreen.openArea(x, y, 256, 256);
            for (uint16_t j=0; j<256; j++)
            {
                for (uint16_t i=0; i<256; i++)
                {
                    g = ((i & 0b00001111) /8) +8*((j & 0b00001111) %8);
                    r = i/8;
                    b = j/8;
                    myScreen.feedArea((r << 11) | (g << 5) | b);
                    
                }
            }
            myScreen.closeArea();
            chrono2 = millis() - chrono2;
            
            Serial.println(chrono2, DEC);
            Serial.print("Ratio%\t");
            Serial.println((uint32_t)((uint64_t)(chrono1*100)/chrono2), DEC);
            
        } else {
            
            Serial.println("feedArea\t");
            Serial.println("Ratio%\t");
        }
        delay(2000);
    }
    
    if ((myScreen.screenSizeY() > 128) && (myScreen.screenSizeX() > 256))
    {
        
        myScreen.clear();
        x = (myScreen.screenSizeX() - 256) /3;
        y = (myScreen.screenSizeY() - 128) /2;
        
        Serial.print("Points(128)\t");
        chrono1 = millis();
        for (g=0; g<64; g++)
        {
            for (r=0; r<32; r++)
            {
                for (b=0; b<32; b++)
                {
                    myScreen.point(x + (4*r+((g/4)%4)), y + (4*b+((g/4)/4)), (r << 11) | (g << 5) | b);
                }
            }
        }
        chrono1 = millis() - chrono1;
        Serial.println(chrono1, DEC);
        
        x *= 2;
        x += 128;
        
        uint8_t r, g, b;
        
        if (myScreen.isArea())
        {
            
            Serial.print("feedArea\t");
            chrono2 = millis();
            myScreen.openArea(x, y, 128, 128);
            for (uint16_t j=0; j<128; j++)
            {
                for (uint16_t i=0; i<128; i++)
                {
                    g = ((i & 0b00000111) /4) +4*((j & 0b00000111) %4);
                    r = i/4;
                    b = j/4;
                    myScreen.feedArea((r << 11) | (g << 5) | b);
                    
                }
            }
            myScreen.closeArea();
            chrono2 = millis() - chrono2;
            
            Serial.println(chrono2, DEC);
            Serial.print("Ratio%\t");
            Serial.println((uint32_t)((uint64_t)(chrono1*100)/chrono2), DEC);
            
        } else {
            
            Serial.println("feedArea\t");
            Serial.println("Ratio%\t");
        }
    }
}


void testCopyPasteColour(uint16_t &x, uint16_t &y, uint8_t maxRed, uint8_t maxGreen, uint8_t maxBlue)
{
    if (y+72 > myScreen.screenSizeY()) y = 0;
    
    uint8_t iRed, iGreen, iBlue;
    uint16_t value;
    
    myScreen.setFontSize(0);
    uint16_t tx = x + myScreen.fontSizeY()*5;
    
    if (myScreen.screenSizeY() > tx + 256 +2)
    {
        myScreen.gText(x, y+16, "from");
        myScreen.gText(x, y+48, "to");
        
        myScreen.setPenSolid(false);
        myScreen.dRectangle(tx-1,  y+11, 256 +2, 64 +2, myColours.white);
        
        for (uint8_t i=0; i<0xff; i++)
        {
            iRed   = map(i, 0, 0xff, 0, maxRed);
            iGreen = map(i, 0, 0xff, 0, maxGreen);
            iBlue  = map(i, 0, 0xff, 0, maxBlue);
            
            if (i%2 == 0)
            {
                myScreen.dLine(tx+i,  y+12, 1, 32, myColours.calculate(iRed, iGreen, iBlue));
                value = myScreen.readPixel(tx+i,  y+12);
                myScreen.dLine(tx+i,  y+12 +32, 1, 32, value);
            }
        }
        y += 64 +8;
        
    } else if (myScreen.screenSizeY() > tx + 128 +2)
    {
        myScreen.gText(x, y+8,  "from");
        myScreen.gText(x, y+24, "to");
        
        myScreen.setPenSolid(false);
        myScreen.dRectangle(tx-1,  y+3, 128 +2, 32 +2, myColours.white);
        
        for (uint8_t i=0; i<0x7f; i++)
        {
            iRed   = map(i, 0, 0x7f, 0, maxRed);
            iGreen = map(i, 0, 0x7f, 0, maxGreen);
            iBlue  = map(i, 0, 0x7f, 0, maxBlue);
            
            if (i%2 == 0)
            {
                myScreen.dLine(tx+i,  y+4, 1, 16, myColours.calculate(iRed, iGreen, iBlue));
                value = myScreen.readPixel(tx+i,  y+4);
                myScreen.dLine(tx+i,  y+4 +16, 1, 16, value);
            }
        }
        y += 32 +8;
    }
}


void testCopyPasteColourSD(uint16_t &x, uint16_t &y, uint8_t maxRed, uint8_t maxGreen, uint8_t maxBlue)
{
    if (myScreen.isStorage())
    {
        uint32_t address;
        
        if (y+72 > myScreen.screenSizeY()) y = 0;
        
        uint8_t iRed, iGreen, iBlue;
        
        myScreen.setFontSize(0);
        myScreen.gText(x+8, y+16, "from");
        myScreen.gText(x+8, y+50, " to");
        
        myScreen.setPenSolid(false);
        myScreen.dRectangle(x+64-1,  y+11, 258, 66, myColours.white);
        
        for (uint8_t i=0; i<0xff; i++)
        {
            iRed   = map(i, 0, 0xff, 0, maxRed);
            iGreen = map(i, 0, 0xff, 0, maxGreen);
            iBlue  = map(i, 0, 0xff, 0, maxBlue);
            
            if (i%2 == 0)
            {
                myScreen.dLine(x+64+i,  y+12, 1, 32, myColours.calculate(iRed, iGreen, iBlue));
            }
        }
        
        address = 1000;
        myScreen.copyArea(x+64,  y+12,     258, 32, address);
        address = 1000;
        myScreen.pasteArea(x+64,  y+12+32, 258, 32, address);
        
        y += 72;
    } else {
        Serial.println("No storage");
    }
}


// Add setup code
void setup()
{
#if defined(ER1963) || defined(ER1963_SD)                                       // backlight on pin 40
    pinMode(40, OUTPUT);
    digitalWrite(40, HIGH);
#endif
    
    // Start
    Serial.begin(9600);
    delay(100);
    Serial.println();
    Serial.println("*** LCD_protocol");
    Serial.println("(All times in ms)");
    Serial.print("LCD_screen release\t");
    Serial.println(LCD_SCREEN_VIRTUAL_RELEASE, DEC);
    
#if defined(PicasoSPE)
    mySerial.begin(9600);
#endif
    
    Serial.print("begin...");
    myScreen.begin();
    Serial.println(" done");
    
    largeOrientation = 0;
    myScreen.setOrientation(largeOrientation);
    if (myScreen.screenSizeX() < myScreen.screenSizeY())
    {
        largeOrientation = 1;
        myScreen.setOrientation(largeOrientation);
    }
    
    Serial.println(myScreen.WhoAmI());
    Serial.print(myScreen.screenSizeX(), DEC);
    Serial.print("x");
    Serial.println(myScreen.screenSizeY(), DEC);
    
#if defined(PicasoSPE)
    myScreen.setSpeed((uint32_t) 38400); // 38400 ok, 57600 ok, 115200 nok, 256000 nok
    mySerial.begin((uint32_t) 38400);
    Serial.println((uint32_t) 38400, DEC);
#endif
    
    Serial.println("---");
    
#if (TEST_SPLASH == 1)
    myScreen.clear(myColours.red);
    
    myScreen.setFontSolid(false);
    myScreen.setFontSize(myScreen.fontMax());
    myScreen.gText((myScreen.screenSizeX()-11*myScreen.fontSizeX())/2,
                   (myScreen.screenSizeY()-myScreen.fontSizeY())/2,
                   "Protocol100", myColours.white);
    delay(2000);
#else
    myScreen.clear();
#endif
    
#if (TEST_COLOURS == 1)
    testColours(largeOrientation);
    delay(2000);
#endif
    
    /*
     myScreen.setPenSolid();
     myScreen.dRectangle(0,   100, 100, 100, myColours.black);
     myScreen.dRectangle(100, 100, 100, 100, myColours.red);
     myScreen.dRectangle(200, 100, 100, 100, myColours.yellow);
     myScreen.dRectangle(300, 100, 100, 100, myColours.green);
     myScreen.dRectangle(400, 100, 100, 100, myColours.cyan);
     myScreen.dRectangle(500, 100, 100, 100, myColours.blue);
     myScreen.dRectangle(600, 100, 100, 100, myColours.violet);
     myScreen.dRectangle(700, 100, 100, 100, myColours.white);
     
     
     debugln("black  %4x %4x", myColours.black,  myScreen.readPixel(0,   100));
     debugln("red    %4x %4x", myColours.red,    myScreen.readPixel(100, 100));
     debugln("yellow %4x %4x", myColours.yellow, myScreen.readPixel(200, 100));
     debugln("green  %4x %4x", myColours.green,  myScreen.readPixel(300, 100));
     debugln("cyan   %4x %4x", myColours.cyan,   myScreen.readPixel(400, 100));
     debugln("blue   %4x %4x", myColours.blue,   myScreen.readPixel(500, 100));
     debugln("violet %4x %4x", myColours.violet, myScreen.readPixel(600, 100));
     debugln("white  %4x %4x", myColours.white,  myScreen.readPixel(700, 100));
     
     Serial.println("*** END");
     while (1);
     */
    
#if (PROTOCOL_COLOURS == 1)
    protocolColours();
    delay(2000);
#endif
    
#if (PROTOCOL_FPS == 1)
    protocolFPS();
    delay(2000);
#endif
    
#if (PROTOCOL_SQUARE == 1)
    myScreen.clear(myColours.darkGrey);
    
    protocolSquare(400);
    protocolSquare(300);
    protocolSquare(200);
    protocolSquare(100);
    protocolSquare(50);
    delay(2000);
#endif
    
#if (PROTOCOL_FONT == 1)
    protocolFont();
    delay(2000);
#endif
    
#if (TEST_COPY_PASTE_COLOUR == 1)
    myScreen.clear();
    myScreen.setOrientation(largeOrientation);
    
    x = 0;
    y = 0;
    testCopyPasteColour(x, y, 0xff, 0, 0);
    testCopyPasteColour(x, y, 0, 0xff, 0);
    testCopyPasteColour(x, y, 0, 0, 0xff);
    testCopyPasteColour(x, y, 0xff, 0xff, 0xff);
    delay(2000);
#endif
    
#if (TEST_COPY_PASTE_COLOUR_SD == 1)
    myScreen.clear();
    myScreen.setOrientation(largeOrientation);
    
    x = 0;
    y = 0;
    testCopyPasteColourSD(x, y, 0xff, 0, 0);
    testCopyPasteColourSD(x, y, 0, 0xff, 0);
    testCopyPasteColourSD(x, y, 0, 0, 0xff);
    testCopyPasteColourSD(x, y, 0xff, 0xff, 0xff);
    delay(2000);
#endif
    
    
#if (PROTOCOL_TEXT == 1)
    protocolText();
    delay(2000);
#endif
    
#if (PROTOCOL_COPY_PASTE == 1)
    protocolCopyPaste(largeOrientation);
    delay(2000);
#endif
    
#if (PROTOCOL_TOUCH == 1)
    myScreen.setOrientation(largeOrientation);
    protocolTouch();
#endif
    
    // End
    myScreen.clear();
    myScreen.setOrientation(largeOrientation);
    myScreen.setFontSize(myScreen.fontMax());
    myScreen.gText((myScreen.screenSizeX()-10*myScreen.fontSizeX())/2,
                   (myScreen.screenSizeY()-myScreen.fontSizeY())/2,
                   "Thank you!", myColours.white);
    delay(2000);
    
    myScreen.setOrientation(largeOrientation);
    myScreen.clear();
    
    Serial.println("---");
    Serial.println();
}


void loop()
{
    ;
}


