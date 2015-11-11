///
/// @file       LCD_screen_Paint.ino
/// @brief		Main sketch
/// @details	Example of paint for LCD_screen
/// @n
/// @n
/// @n @a		Developed with [embedXcode](http://embedXcode.weebly.com)
///
/// @author		Rei VILO
/// @author		Rei VILO
/// @date		15/07/13 20:29
/// @version	<#version#>
///
/// @copyright	(c) Rei VILO, 2013
/// @copyright	CC = BY NC SA
///
/// @see		ReadMe.txt for references
///


// Include application, user and local libraries
// Include application, user and local libraries
#define K35 // K35 HI32 ILI9225B HY28A ST7735 PicasoSPE PicasoSGC

#if defined(ILI9225B)
#include "ILI9225B_screen.h"
LCD_ILI9225B myScreen;

#elif defined(K35)
#include "K35_screen.h"
LCD_K35 myScreen;

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

// Following includes for Energia only
#include "SPI.h"


#define WITH_PAINT
// WITH_COPY_PASTE WITH_TOUCH WITH_PAINT
// WITH_READ WITH_COPY_PASTE WITH_CLOCK WITH_YPR WITH_GAUGE WITH_HISTOGRAM WITH_MEMORY WITH_MULTIPLE WITH_TABLE WITH_TOUCH WITH_FONT

// Define variables and constants
uint16_t x, y, z;
uint32_t chrono, chrono1, chrono2;
uint32_t address;


#ifdef WITH_COPY_PASTE
void testCopyPasteSpeed(uint8_t k)
{
    myScreen.clear();
    myScreen.setOrientation(k);
    
    Serial.print("Orientation: ");
    Serial.println(k, DEC);
    
    // Image
    chrono = millis();
    for (uint8_t i=0; i<64; i++) {
        for (uint8_t j=0; j<64; j++) {
            myScreen.point(i, j, myScreen.calculateColour(4*i, 4*j, 254-2*i-2*j));
        }
    }
    
    myScreen.setPenSolid(false);
    myScreen.rectangle(1, 1, 62, 62, blackColour);
    myScreen.line(0,   0, 63, 63, whiteColour);
    myScreen.line(32,  0, 63, 63, whiteColour);
    myScreen.line(0,  32, 63, 63, whiteColour);
    myScreen.dRectangle(0, 0, 64, 64, whiteColour);
    
    myScreen.setFontSize(0);
    myScreen.gText(0, 66, "0: Original");
    myScreen.setFontSize(1);
    myScreen.gText(0, 76, i32toa(millis()-chrono) + " ms");
    
    Serial.print("0: Original");
    Serial.print("\t");
    Serial.print(millis()-chrono, DEC);
    Serial.println(" ms");
    
    // Method 1
    chrono = millis();
    for (uint16_t i=0; i<64; i++) {
        for (uint16_t j=0; j<64; j++) {
            myScreen.point(myScreen.screenSizeX()/2+i, j, myScreen.readPixel(i, j));
        }
    }
    myScreen.setFontSize(0);
    myScreen.gText(myScreen.screenSizeX()/2, 66, "1. point readPixel");
    myScreen.setFontSize(1);
    myScreen.gText(myScreen.screenSizeX()/2, 76, i32toa(millis()-chrono) + " ms");
    
    Serial.print("1. point readPixel");
    Serial.print("\t");
    Serial.print(millis()-chrono, DEC);
    Serial.println(" ms");
    
    // Method 2
    chrono = millis();
    myScreen.copyPaste(0, 0, 0, myScreen.screenSizeY()/2, 64, 64);
    
    myScreen.setFontSize(0);
    myScreen.gText(0, myScreen.screenSizeY()/2 +66, "2. copyPaste");
    myScreen.setFontSize(1);
    myScreen.gText(0, myScreen.screenSizeY()/2 +76, i32toa(millis()-chrono) + " ms");
    
    Serial.print("2. copyPaste");
    Serial.print("\t");
    Serial.print(millis()-chrono, DEC);
    Serial.println(" ms");
    
    // Method 3
#if (USE_SPI_SRAM==1)
    chrono = millis();
    
    address = 100;
    myScreen.copyArea(0, 0, 64, 64, address);
    address = 100;
    myScreen.pasteArea(myScreen.screenSizeX()/2, myScreen.screenSizeY()/2, 64, 64, address);
    
    myScreen.setFontSize(0);
    myScreen.gText(myScreen.screenSizeX()/2, myScreen.screenSizeY()/2 +66, "3. copy-paste SRAM");
    myScreen.setFontSize(1);
    myScreen.gText(myScreen.screenSizeX()/2, myScreen.screenSizeY()/2 +76, i32toa(millis()-chrono) + " ms");
    
    Serial.print("3. copy-paste SRAM");
    Serial.print("\t");
    Serial.print(millis()-chrono, DEC);
    Serial.println(" ms");
    
#endif
    Serial.println("---");
    
    delay(1000);
}

void testcopyPasteColour(uint8_t maxRed, uint8_t maxGreen, uint8_t maxBlue)
{
    uint8_t iRed, iGreen, iBlue;
    uint16_t value;
    myScreen.clear();
    myScreen.setOrientation(1);
    
    myScreen.setPenSolid(false);
    myScreen.dRectangle(31,  11, 258, 66, whiteColour);
    
    for (uint8_t i=0; i<0xff; i++) {
        iRed = map(i, 0, 0xff, 0, maxRed);
        iGreen = map(i, 0, 0xff, 0, maxGreen);
        iBlue = map(i, 0, 0xff, 0, maxBlue);
        
        if (i%4 == 0) {
            Serial.print(i, DEC);
            Serial.print(":\t");
            Serial.print(btoa(myScreen.calculateColour(iRed, iGreen, iBlue), 16));
            Serial.print("\t:\t");
            myScreen.dLine(32+i,  12, 1, 32, myScreen.calculateColour(iRed, iGreen, iBlue));
            value = myScreen.readPixel(32+i,  12);
            myScreen.dLine(32+i,  12+32, 1, 32, value);
            Serial.println(btoa(value, 16));
        }
    }
}


// Add setup code
void setup() {
    
    Serial.begin(9600);
    delay(100);
    Serial.println("WITH_COPY_PASTE");
    
    myScreen.begin();
    myScreen.setFontSize(1);
    Serial.println(myScreen.WhoAmI());
    
    for (uint8_t k=0; k<4; k++) testCopyPasteSpeed(k);
    
    if (false) {
        testcopyPasteColour(0xff, 0, 0);
        testcopyPasteColour(0, 0xff, 0);
        testcopyPasteColour(0, 0, 0xff);
        
        testcopyPasteColour(0xff, 0xff, 0xff);
    }
}

// Add loop code
void loop() {
}
#endif


#ifdef WITH_TOUCH
// Add setup code
void setup() {
    
    Serial.begin(9600);
    delay(100);
    Serial.println("WITH_TOUCH");
    
    myScreen.begin();
    myScreen.setFontSize(1);
    Serial.println(myScreen.WhoAmI());
    
    myScreen.calibrateTouch();
    
    myScreen.setPenSolid(false);
    myScreen.rectangle(0, 0, myScreen.screenSizeX()-1, myScreen.screenSizeY()-1, yellowColour);
    myScreen.setFontSolid(true);
    myScreen.setFontSize(myScreen.fontMax());
    myScreen.gText((myScreen.screenSizeX()-14*myScreen.fontSizeX())/2, (myScreen.screenSizeY()-myScreen.fontSizeY())/2,
                   "Touch to draw!", yellowColour, blackColour);
}

// Add loop code
void loop() {
    if (myScreen.getTouch(x, y, z)) {
        
        Serial.print("xy\t");
        Serial.print(x, DEC);
        Serial.print("\t");
        Serial.println(y, DEC);
        
        
        if (x < 1) x = 1;
        if (y < 1) y = 1;
        if (x > myScreen.screenSizeX()) x = myScreen.screenSizeX();
        if (y > myScreen.screenSizeY()) y = myScreen.screenSizeY();
        
        myScreen.rectangle(x-1, y-1, x+1, y+1, yellowColour);
    } else {
        delay(100);
    }
    
}
#endif

#ifdef WITH_PAINT
// Add setup code
uint16_t colour;
uint16_t bottomY, leftX, stepX, stepY;

void drawPalette()
{
    String s;
    
    myScreen.setOrientation(1);

    //    myScreen.calibrateTouch();
    
    leftX   = 56;
    stepX   = (myScreen.screenSizeX()-leftX)/12;
    stepY   = stepX;
    if (stepY < 16) stepY = 22;
    bottomY = myScreen.screenSizeY()-stepY;
    
    myScreen.setPenSolid(false);
    myScreen.rectangle(0, 0, myScreen.screenSizeX()-1, bottomY-1, grayColour);
    
    myScreen.setFontSize(2);
    myScreen.gText((myScreen.screenSizeX()-14*myScreen.fontSizeX())/2, bottomY/2-myScreen.fontSizeY(),
                   "Touch to draw!", grayColour, blackColour);
    myScreen.setFontSize(1);
    s = myScreen.WhoAmI();
    myScreen.gText((myScreen.screenSizeX()-s.length()*myScreen.fontSizeX())/2, bottomY/2,
                   s, grayColour, blackColour);
    s = "LCD_Screen Library Suite";
    myScreen.gText((myScreen.screenSizeX()-s.length()*myScreen.fontSizeX())/2, bottomY/2+myScreen.fontSizeY(),
                   s, grayColour, blackColour);
    
    myScreen.setFontSize(0);
    myScreen.gText((leftX-6*myScreen.fontSizeX())/2, bottomY+(stepY-myScreen.fontSizeY())/2, "Colour", grayColour, blackColour);
    
    myScreen.setPenSolid();
    myScreen.dRectangle(leftX + stepX * 0 +2, bottomY +2, stepX -4, stepY -4, whiteColour);
    myScreen.dRectangle(leftX + stepX * 1 +2, bottomY +2, stepX -4, stepY -4, grayColour);
    myScreen.dRectangle(leftX + stepX * 2 +2, bottomY +2, stepX -4, stepY -4, darkGrayColour);
    myScreen.dRectangle(leftX + stepX * 3 +2, bottomY +2, stepX -4, stepY -4, blackColour);
    myScreen.dRectangle(leftX + stepX * 4 +2, bottomY +2, stepX -4, stepY -4, violetColour);
    myScreen.dRectangle(leftX + stepX * 5 +2, bottomY +2, stepX -4, stepY -4, magentaColour);
    myScreen.dRectangle(leftX + stepX * 6 +2, bottomY +2, stepX -4, stepY -4, blueColour);
    myScreen.dRectangle(leftX + stepX * 7 +2, bottomY +2, stepX -4, stepY -4, cyanColour);
    myScreen.dRectangle(leftX + stepX * 8 +2, bottomY +2, stepX -4, stepY -4, greenColour);
    myScreen.dRectangle(leftX + stepX * 9 +2, bottomY +2, stepX -4, stepY -4, yellowColour);
    myScreen.dRectangle(leftX + stepX *10 +2, bottomY +2, stepX -4, stepY -4, orangeColour);
    myScreen.dRectangle(leftX + stepX *11 +2, bottomY +2, stepX -4, stepY -4, redColour);
    
    myScreen.setPenSolid(false);
    myScreen.dRectangle(leftX + stepX * 3 +2, bottomY +2, stepX -4, stepY -4, grayColour);
    
    colour = yellowColour;
    myScreen.dRectangle(0 +2, bottomY +2, leftX -4, stepY -4, colour);
    myScreen.dRectangle(0 +3, bottomY +3, leftX -6, stepY -6, colour);
}


void setup() {
    Serial.begin(9600);
    delay(100);
    //    Serial.println("WITH_PAINT example for LCD_screen");
    
    myScreen.begin();
    
    drawPalette();
    
}

// Add loop code
void loop() {
    if (myScreen.getTouch(x, y, z)) {
        Serial.print("xyz = \t");
        Serial.print(x, DEC);
        Serial.print("\t");
        Serial.print(y, DEC);
        Serial.print("\t");
        Serial.println(z, DEC);
        
        if (y > bottomY) {
            if (x < leftX) {
                drawPalette();
            } else {
                if (myScreen.isReadable()) {
                    colour = myScreen.readPixel(leftX+((x-leftX)/stepX)*stepX+2, bottomY+stepY/2);
                } else {
                    switch ((x-leftX)/stepX) {
                        case 0:
                            colour = whiteColour;
                            break;
                        case 1:
                            colour = grayColour;
                            break;
                        case 2:
                            colour = darkGrayColour;
                            break;
                        case 3:
                            colour = blackColour;
                            break;
                        case 4:
                            colour = violetColour;
                            break;
                        case 5:
                            colour = magentaColour;
                            break;
                        case 6:
                            colour = blueColour;
                            break;
                        case 7:
                            colour = cyanColour;
                            break;
                        case 8:
                            colour = greenColour;
                            break;
                        case 9:
                            colour = yellowColour;
                            break;
                        case 10:
                            colour = orangeColour;
                            break;
                        case 11:
                            colour = redColour;
                            break;
                            
                        default:
                            break;
                    }
                }
                myScreen.setPenSolid(false);
                myScreen.dRectangle(0 +2, bottomY +2, leftX -4, stepY -4, colour);
                myScreen.dRectangle(0 +3, bottomY +3, leftX -6, stepY -6, colour);
            }
        } else {
            if (x < 1) x = 1;
            if (y < 1) y = 1;
            if (x > myScreen.screenSizeX()) x = myScreen.screenSizeX();
            if (y > bottomY) y = bottomY;
            myScreen.setPenSolid(true);
            myScreen.rectangle(x-1, y-1, x+1, y+1, colour);
        }
    } else {
        delay(50);
    }
}
#endif



