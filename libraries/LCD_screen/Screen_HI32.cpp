//
// Screen_HI32.cpp
// Library C++ code
// ----------------------------------
// Developed with embedXcode
// http://embedXcode.weebly.com
//
// Project LCD_screen Library Suite
//
// Created by Rei VILO, May 29, 2013
// embedXcode.weebly.com
//
//
// Copyright © Rei VILO, 2013-2014
// License All rights reserved
//
// See LCD_document.h and ReadMe.txt for references
//


// Library header
#include "Screen_HI32.h"

///
/// @name	ILI9225B constants
///
/// @{

#define ILI_SetAddressMode      0x36
#define ILI_SetColumnAddress    0x2a
#define ILI_SetPageAddress      0x2b
#define ILI_WriteMemoryStart    0x2c
#define ILI_WriteMemoryContinue 0x3c
#define ILI_ReadMemoryStart     0x2e
#define ILI_ReadMemoryContinue  0x3e

#define HI32_WIDTH       480 // Vertical
#define HI32_HEIGHT      320 // Horizontal
#define LCD_OFFSET_HEIGHT 0
#define LCD_OFFSET_WIDTH  0

/// @}

///
/// @name   Touch constants
///
/// @{

#define TOUCH_X     0xd0
#define TOUCH_Y     0x90
#define TOUCH_Z1    0xb0
#define TOUCH_Z2    0xc0
#define TOUCH_TRIM  0x10 ///< Touch threshold

/// @}


// Code
Screen_HI32::Screen_HI32() {
#if defined(__AVR_ATmega2560__) || defined(__AVR_ATmega644P__)
    //    _pinScreenBacklight      = 9; // screen backlight
    //    _pinScreenBacklight = 44;
    
    //    myGLCD(CTE32_480X320,38,39,40,41);
    //    UTFT(byte model, int RS, int WR,int CS, int RST, int SER=0);
    //    Screen_HI32::UTFT(byte model, int RS, int WR,int CS, int RST, int SER)
    _pinScreenRS = 38;
    _pinScreenWR = 39;
    _pinScreenCS = 40;
    _pinScreenReset = 41;
    
    //    UTouch      myTouch(6,5,4,3,2);
    //    UTouch(byte tclk, byte tcs, byte tdin, byte dout INPUT, byte irq);
    //    UTouch::UTouch(byte tclk, byte tcs, byte din, byte dout INPUT, byte irq)
    // irq = 2
    _pinTouchCS = 5;
    _pinTouchMOSI  = 4;
    _pinTouchMISO  = 3;
    _pinTouchClock     = 6;
    
    DDRA = 0xFF;
    DDRC = 0xFF;
    
    _portRS	 = portOutputRegister(digitalPinToPort(_pinScreenRS));
    _bitRS	 = digitalPinToBitMask(_pinScreenRS);
    _portWR	 = portOutputRegister(digitalPinToPort(_pinScreenWR));
    _bitWR	 = digitalPinToBitMask(_pinScreenWR);
    _portCS	 = portOutputRegister(digitalPinToPort(_pinScreenCS));
    _bitCS   = digitalPinToBitMask(_pinScreenCS);
    _portRST = portOutputRegister(digitalPinToPort(_pinScreenReset));
    _bitRST	 = digitalPinToBitMask(_pinScreenReset);
    
#else
    //#error Platform not supported
#endif
    
}

void Screen_HI32::begin()
{
    pinMode(_pinTouchClock, OUTPUT);
    pinMode(_pinTouchMOSI, OUTPUT);
    pinMode(_pinTouchMISO, INPUT);
    pinMode(_pinTouchCS, OUTPUT);
    
    pinMode(_pinScreenRS, OUTPUT);
    pinMode(_pinScreenWR, OUTPUT);
    pinMode(_pinScreenCS, OUTPUT);
    pinMode(_pinScreenReset, OUTPUT);
    
    if (_pinScreenBacklight != NULL) pinMode(_pinScreenBacklight, OUTPUT);
    //    if (_pinScreenBacklight!=NULL) digitalWrite(_pinScreenBacklight, HIGH);
    
    // Screen reset
	sbi(_portRST, _bitRST);
	delay(5);
	cbi(_portRST, _bitRST);
	delay(15);
	sbi(_portRST, _bitRST);
	delay(15);
    
    cbi(_portCS, _bitCS);                                                       // CS LOW
    delay(100);
    
    // Screen initialisation
    _writeCommand16(0x11);
    delay(20);
    _writeCommand16(0xD0);
    _writeData16(0x07);
    _writeData16(0x42);
    _writeData16(0x18);
    
    _writeCommand16(0xD1);
    _writeData16(0x00);
    _writeData16(0x07);//07
    _writeData16(0x10);
    
    _writeCommand16(0xD2);
    _writeData16(0x01);
    _writeData16(0x02);
    
    _writeCommand16(0xC0);
    _writeData16(0x10);
    _writeData16(0x3B);
    _writeData16(0x00);
    _writeData16(0x02);
    _writeData16(0x11);
    
    _writeCommand16(0xC5);
    _writeData16(0x03);
    
    _writeCommand16(0xC8);
    _writeData16(0x00);
    _writeData16(0x32);
    _writeData16(0x36);
    _writeData16(0x45);
    _writeData16(0x06);
    _writeData16(0x16);
    _writeData16(0x37);
    _writeData16(0x75);
    _writeData16(0x77);
    _writeData16(0x54);
    _writeData16(0x0C);
    _writeData16(0x00);
    
    _writeCommand16(ILI_SetAddressMode);
    _writeData16(0x002E);
    
    _writeCommand16(0x3A);
    _writeData16(0x55);
    
    _writeCommand16(0x2A);
    _writeData16(0x00);
    _writeData16(0x00);
    _writeData16(0x01);
    _writeData16(0x3F);
    
    _writeCommand16(0x2B);
    _writeData16(0x00);
    _writeData16(0x00);
    _writeData16(0x01);
    _writeData16(0xE0);
    delay(120);
    _writeCommand16(0x29);
    
    sbi(_portCS, _bitCS);                                                       // CS HIGH
    
    // Standard
    //    setBacklight(true);
    setOrientation(0);
    
    _screenWidth = HI32_HEIGHT;
    _screenHeigth = HI32_WIDTH;
    _screenDiagonal = 32;
    
    _touchTrim = TOUCH_TRIM;
    _touchXmin = 1879;
    _touchXmax = 349;
    _touchYmin = 1823;
    _touchYmax = 194;
    
    _penSolid  = false;
    _fontSolid = true;
    _flagRead  = false;
    _fsmArea   = 0x01;
    _touchTrim = 10;
    clear();
}

String Screen_HI32::WhoAmI()
{
    return "HI32 screen";
}


void Screen_HI32::_setOrientation(uint8_t orientation)
{
    //_orientation = orientation % 4;
//    LCD_screen_font::setOrientation(orientation);
    
    //    Serial.print("Orientation ");
    //    Serial.print(orientation, DEC);
    //    Serial.print(" = ");
    //    Serial.print(screenSizeX(), DEC);
    //    Serial.print(" x ");
    //    Serial.print(screenSizeY(), DEC);
    //    Serial.println();
    
    //    b7=h  Page Address Order
    //    b6=v  Column Address Order
    //    b5=r  Page/Column Selection
    //    b4    Vertical Order
    //    b3=0  RGB Order
    //    b2=0  Display data latch data order
    //    b1    Horizontal Flip
    //    b0    Vertical Flip
    
    cbi(_portCS, _bitCS);                                                       // CS LOW
//    cbi(_portCS, _bitCS);
    
    _writeCommand16(ILI_SetAddressMode);
    switch (_orientation) {
        case 0:
            //             hvR
            _writeData16(0b00001010);
            break;
        case 1:
            //             hvR
            _writeData16(0b01101010);
            break;
        case 2:
            //             hvR
            _writeData16(0b11001010);
            break;
        case 3:
            //             hvR
            _writeData16(0b10101010);
            break;
    }
    sbi(_portCS, _bitCS);                                                       // CS HIGH
//    sbi(_portCS, _bitCS);
}

// Utilities
void Screen_HI32::_writeParallelLCD88(uint8_t dataHigh8, uint8_t dataLow8)
{
#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
    cbi(_portCS, _bitCS);                                                       // CS LOW
    PORTA = dataHigh8;
    PORTC = dataLow8;
    //    digitalWrite(_pinScreenWR, LOW);
    //    digitalWrite(_pinScreenWR, HIGH);
    pulse_low(_portWR, _bitWR);
    sbi(_portCS, _bitCS);                                                       // CS HIGH
#else
    //#error Platform not supported.
#endif
}

void Screen_HI32::_readParallelLCD88(uint8_t &dataHigh8, uint8_t &dataLow8)
{
#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
    cbi(_portCS, _bitCS);                                                       // CS LOW
    //    digitalWrite(_pinScreenWR, LOW);
    //    digitalWrite(_pinScreenWR, HIGH);
    pulse_low(_portRS, _bitRS);
    dataHigh8 = PORTA;
    dataLow8 = PORTC;
    sbi(_portCS, _bitCS);                                                       // CS HIGH
#else
    //#error Platform not supported.
#endif
}

void Screen_HI32::_writeCommand16(uint16_t command16)
{
    cbi(_portCS, _bitCS);                                                       // CS LOW
    cbi(_portRS, _bitRS); // RS : low
    _writeParallelLCD88(0x00, command16);
    sbi(_portCS, _bitCS);                                                       // CS HIGH
}

void Screen_HI32::_writeRegister16(uint8_t command8, uint16_t data16)
{
    cbi(_portCS, _bitCS);                                                       // CS LOW
    cbi(_portRS, _bitRS); // RS : low
    _writeParallelLCD88(0x00, command8);
    sbi(_portRS, _bitRS); // RS : high
    _writeParallelLCD88(highByte(data16), lowByte(data16));
    sbi(_portCS, _bitCS);                                                       // CS HIGH
}

void Screen_HI32::_writeData88(uint8_t dataHigh8, uint8_t dataLow8)
{
    cbi(_portCS, _bitCS);                                                       // CS LOW
    sbi(_portRS, _bitRS); // RS : high
    _writeParallelLCD88(dataHigh8, dataLow8);
    sbi(_portCS, _bitCS);                                                       // CS HIGH
}

void Screen_HI32::_writeData16(uint16_t data16)
{
    cbi(_portCS, _bitCS);                                                       // CS LOW
    sbi(_portRS, _bitRS); // RS : high
    _writeParallelLCD88(highByte(data16), lowByte(data16));
    sbi(_portCS, _bitCS);                                                       // CS HIGH
}


void Screen_HI32::_writeAddress16(uint16_t data16)
{
    cbi(_portCS, _bitCS);                                                       // CS LOW
    sbi(_portRS, _bitRS); // RS : high
    _writeParallelLCD88(0x00, highByte(data16));
    _writeParallelLCD88(0x00, lowByte(data16));
    sbi(_portCS, _bitCS);                                                       // CS HIGH
}


void Screen_HI32::_orientCoordinates(uint16_t &x1, uint16_t &y1)
{
	switch (_orientation) {
        case 0:  // ok
            break;
        case 1: // ok
            y1 = screenSizeY() - y1 -1;
            break;
        case 2: // ok
            x1 = screenSizeX() - x1 -1;
            y1 = screenSizeY() - y1 -1;
            break;
        case 3: // ok
            x1 = screenSizeX() - x1 -1;
            break;
	}
}

void Screen_HI32::_setPoint(uint16_t x1, uint16_t y1, uint16_t colour) // compulsory
{
    cbi(_portCS, _bitCS);                                                       // CS LOW
    
    _setWindow(x1, y1, x1, y1);
    _writeData16(colour);
    
    sbi(_portCS, _bitCS);                                                       // CS HIGH
}

void Screen_HI32::_setWindow(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    _orientCoordinates(x1, y1);
    _orientCoordinates(x2, y2);
    
    if (x1 > x2) _swap(x1, x2);
    if (y1 > y2) _swap(y1, y2);
    
    cbi(_portCS, _bitCS);                                                       // CS LOW

    _writeCommand16(ILI_SetColumnAddress);
    _writeAddress16(x1);
    _writeAddress16(x2);
    _writeCommand16(ILI_SetPageAddress);
    _writeAddress16(y1);
    _writeAddress16(y2);
    _writeCommand16(ILI_WriteMemoryStart);
    
    sbi(_portCS, _bitCS);                                                       // CS HIGH
}

void Screen_HI32::_fastFill(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t colour)
{
    if (x1 > x2) _swap(x1, x2);
    if (y1 > y2) _swap(y1, y2);
    
    uint8_t hi = highByte(colour);
    uint8_t lo = lowByte(colour);
    
    cbi(_portCS, _bitCS);                                                       // CS LOW
    _setWindow(x1, y1, x2, y2);
    
    sbi(_portRS, _bitRS);
    for (uint32_t t=(uint32_t)(y2-y1+1)*(x2-x1+1); t>0; t--) {
        _writeParallelLCD88(hi, lo);
    }
    sbi(_portCS, _bitCS);                                                       // CS HIGH
}

// Touch
void Screen_HI32::_getOneTouch(uint8_t command8, uint8_t &a, uint8_t &b)
{
    digitalWrite(_pinTouchCS, LOW);
    
    shiftOut(_pinTouchMOSI, _pinTouchClock, MSBFIRST, command8);
    a = shiftIn(_pinTouchMISO, _pinTouchClock, MSBFIRST);
    b = shiftIn(_pinTouchMISO, _pinTouchClock, MSBFIRST);
    
    digitalWrite(_pinTouchCS, HIGH);
}

void Screen_HI32::_getRawTouch(uint16_t &x0, uint16_t &y0, uint16_t &z0)
{
    uint16_t z1, z2;
    uint8_t a, aa, b, c, cc, d;
    
    delay(10);
    _getOneTouch(TOUCH_Z1, a, b); // 8-bit
    _getOneTouch(TOUCH_Z2, c, d);
    //    _getOneTouch(TOUCH_Z1 + 0x08 + 0x01, a, b); // 8-bit
    //    _getOneTouch(TOUCH_Z2 + 0x08 + 0x01, c, d);
    
    z0 = a + (0x7f - c);
    
    //    Serial.println(z0, DEC);
    
    if (z0 > _touchTrim) {
        do {
            //            _getOneTouch(TOUCH_X + 0x00 + 0x01, a, b);  // 12-bit
            //            _getOneTouch(TOUCH_Y + 0x00 + 0x01, c, d);
            _getOneTouch(TOUCH_X + 0x01, a, b);  // 8-bit
            _getOneTouch(TOUCH_Y + 0x01, c, d);
            delay(10);
            //            _getOneTouch(TOUCH_X + 0x00 + 0x01, aa, b); // 12-bit
            //            _getOneTouch(TOUCH_Y + 0x00 + 0x01, cc, d);
            _getOneTouch(TOUCH_X + 0x01, aa, b); // 12-bit
            _getOneTouch(TOUCH_Y + 0x01, cc, d);
        } while ( (a != aa) || (c != cc) );
        
        x0 = a <<4 | b >>4;
        y0 = c <<4 | d >>4;
    }
    
    delay(10);
    //    Serial.print("_getRawTouch\t");
    //    Serial.print(x0, DEC);
    //    Serial.print("\t");
    //    Serial.print(y0, DEC);
    //    Serial.print("\t");
    //    Serial.println(z0, DEC);
}

void Screen_HI32::_closeWindow()
{
    _setWindow(0, 0, screenSizeX()-1, screenSizeY()-1);
}

void Screen_HI32::_setBacklight(bool state)
{
    ;
}

void Screen_HI32::_setIntensity(uint8_t intensity)
{
    ;
}



