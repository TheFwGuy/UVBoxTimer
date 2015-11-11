//
// Screen_W32.cpp
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
#include "Screen_W32.h"

///
/// @name	HX8352-A constants
///
/// @{

#define HX_SetAddressMode      0x16

#define W32_WIDTH       400 // Vertical
#define W32_HEIGHT      240 // Horizontal
#define LCD_OFFSET_HEIGHT 0
#define LCD_OFFSET_WIDTH  0

/// @}

///
/// @name   Touch constants
///
/// @{

#define TOUCH_INIT  0x80
#define TOUCH_Y     0xd0
#define TOUCH_X     0x90
#define TOUCH_Z1    0xb0
#define TOUCH_Z2    0xc0
#define TOUCH_TRIM  0x10 ///< Touch threshold

/// @}


// Code
Screen_W32::Screen_W32() {
#if defined(__AVR_ATmega2560__) || defined(__AVR_ATmega644P__)
    //    _pinScreenBacklight      = 9; // screen backlight
    //    _pinScreenBacklight = 44;
    
    //    myGLCD(CTE32_480X320,38,39,40,41);
    //    UTFT(byte model, int RS, int WR,int CS, int RST, int SER=0);
    //    Screen_W32::UTFT(byte model, int RS, int WR,int CS, int RST, int SER)
    _pinScreenRS = 38;
    _pinScreenWR = 39;
    _pinScreenCS = 40;
    _pinScreenReset = 41;
    
    //    UTouch      myTouch(6,5,4,3,2);
    //    UTouch(byte tclk, byte tcs, byte tdin, byte dout INPUT, byte irq);
    //    UTouch::UTouch(byte tclk, byte tcs, byte din, byte dout INPUT, byte irq)
    // irq = 2
    _pinTouchCS = 5;
    _pinScreenMOSI  = 4;
    _pinScreenMISO  = 3;
    _pinScreenClock     = 6;
    
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

void Screen_W32::begin()
{
    pinMode(_pinScreenClock, OUTPUT);
    pinMode(_pinScreenMOSI, OUTPUT);
    pinMode(_pinScreenMISO, INPUT);
    pinMode(_pinTouchCS, OUTPUT);

    uint8_t a, b;
    _getOneTouch(TOUCH_INIT, a, b);

    pinMode(_pinScreenRS, OUTPUT);
    pinMode(_pinScreenWR, OUTPUT);
    pinMode(_pinScreenCS, OUTPUT);
    pinMode(_pinScreenReset, OUTPUT);
    
    if (_pinScreenBacklight!=NULL) pinMode(_pinScreenBacklight, OUTPUT);
    //    if (_pinScreenBacklight!=NULL) digitalWrite(_pinScreenBacklight, HIGH);
    
    // Screen reset
	sbi(_portRST, _bitRST);
	delay(5);
	cbi(_portRST, _bitRST);
	delay(15);
	sbi(_portRST, _bitRST);
	delay(15);
    
	cbi(_portCS, _bitCS);
    delay(100);
    
    // Screen initialisation
    _writeCommand16(0x83);
	_writeData16(0x02);  //TESTM=1
    
	_writeCommand16(0x85);
	_writeData16(0x03);  //VDC_SEL=011
	_writeCommand16(0x8B);
	_writeData16(0x01);
	_writeCommand16(0x8C);
	_writeData16(0x93); //STBA[7]=1,STBA[5:4]=01,STBA[1:0]=11
    
	_writeCommand16(0x91);
	_writeData16(0x01); //DCDC_SYNC=1
    
	_writeCommand16(0x83);
	_writeData16(0x00); //TESTM=0
	//Gamma Setting
    
	_writeCommand16(0x3E);
	_writeData16(0xB0);
	_writeCommand16(0x3F);
	_writeData16(0x03);
	_writeCommand16(0x40);
	_writeData16(0x10);
	_writeCommand16(0x41);
	_writeData16(0x56);
	_writeCommand16(0x42);
	_writeData16(0x13);
	_writeCommand16(0x43);
	_writeData16(0x46);
	_writeCommand16(0x44);
	_writeData16(0x23);
	_writeCommand16(0x45);
	_writeData16(0x76);
	_writeCommand16(0x46);
	_writeData16(0x00);
	_writeCommand16(0x47);
	_writeData16(0x5E);
	_writeCommand16(0x48);
	_writeData16(0x4F);
	_writeCommand16(0x49);
	_writeData16(0x40);
	// Power on sequence
    
	_writeCommand16(0x17);
	_writeData16(0x91);
    
	_writeCommand16(0x2B);
	_writeData16(0xF9);
	delay(10);
    
	_writeCommand16(0x1B);
	_writeData16(0x14);
    
	_writeCommand16(0x1A);
	_writeData16(0x11);
    
	_writeCommand16(0x1C);
	_writeData16(0x06);
    
	_writeCommand16(0x1F);
	_writeData16(0x42);
	delay(20);
    
	_writeCommand16(0x19);
	_writeData16(0x0A);
    
	_writeCommand16(0x19);
	_writeData16(0x1A);
	delay(40);
    
    
	_writeCommand16(0x19);
	_writeData16(0x12);
	delay(40);
    
	_writeCommand16(0x1E);
	_writeData16(0x27);
	delay(100);
    
	_writeCommand16(0x24);
	_writeData16(0x60);
    
	_writeCommand16(0x3D);
	_writeData16(0x40);
    
	_writeCommand16(0x34);
	_writeData16(0x38);
    
	_writeCommand16(0x35);
	_writeData16(0x38);
    
	_writeCommand16(0x24);
	_writeData16(0x38);
	delay(40);
    
	_writeCommand16(0x24);
	_writeData16(0x3C);
    
	_writeCommand16(HX_SetAddressMode);
	_writeData16(0b00001100); // 0x1c = 0b00011100
    
	_writeCommand16(0x01);
	_writeData16(0x06);
    
	_writeCommand16(0x55);
	_writeData16(0x00);
    
	_writeCommand16(0x02);
	_writeData16(0x00);
	_writeCommand16(0x03);
	_writeData16(0x00);
	_writeCommand16(0x04);
	_writeData16(0x00);
	_writeCommand16(0x05);
	_writeData16(0xef);
    
	_writeCommand16(0x06);
	_writeData16(0x00);
	_writeCommand16(0x07);
	_writeData16(0x00);
	_writeCommand16(0x08);
	_writeData16(0x01);
	_writeCommand16(0x09);
	_writeData16(0x8f);
    
	_writeCommand16(0x22);
    
    sbi (_portCS, _bitCS);
    
    // Standard
    //    setBacklight(true);
    setOrientation(0);
    
    _screenWidth  = W32_HEIGHT;
    _screenHeigth = W32_WIDTH;
    _screenDiagonal = 32;

    _touchTrim = TOUCH_TRIM;
    _touchXmin = 1879;
    _touchXmax = 349;
    _touchYmin = 1823;
    _touchYmax = 194;
    
    _touchXmin =1610;
    _touchXmax =385;
    _touchYmin =1664;
    _touchYmax =240;
    
    _penSolid  = false;
    _fontSolid = true;
    _flagRead  = false;
    _touchTrim = 10;
    _fsmArea = 0x01;

    clear();
    
}

String Screen_W32::WhoAmI()
{
    return "W32 screen";
}

void Screen_W32::_orientCoordinates(uint16_t &x1, uint16_t &y1)
{
//	switch (_orientation) {
//        case 0:                                                                 // ok
//            break;
//        case 1:                                                                 // ok
//            break;
//        case 2:                                                                 // ok
//            break;
//        case 3:                                                                 // ok
//            break;
//	}
}

void Screen_W32::_setOrientation(uint8_t orientation)
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
    
    _writeCommand16(HX_SetAddressMode);
    switch (_orientation) {
        case 0:
            // default 1c= 00011100
            //             hvR
            _writeData16(0b00011100);                                           // ok
            break;
        case 1:
            //             hvR
            _writeData16(0b01111100);                                           // ok
            break;
        case 2:
            //             hvR
            _writeData16(0b11011100);                                           // ok
            break;
        case 3:
            //             hvR
            _writeData16(0b10111100);                                           // nok
            break;
    }
}

//void Screen_W32::_writeBeginParallel()
//{
//    cbi(_portCS, _bitCS);                                                       // CS LOW
//}
//
//void Screen_W32::_writeEndParallel()
//{
//    sbi(_portCS, _bitCS);                                                       // CS HIGH
//}

// Utilities
void Screen_W32::_writeParallelLCD88(uint8_t dataHigh8, uint8_t dataLow8)
{
#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)

    PORTA = dataHigh8;
    PORTC = dataLow8;
    //    digitalWrite(_pinScreenWR, LOW);
    //    digitalWrite(_pinScreenWR, HIGH);
    pulse_low(_portWR, _bitWR);

//    sbi(_portCS, _bitCS);                                                       // CS : high
#else
    //#error Platform not supported.
#endif
}

void Screen_W32::_readParallelLCD88(uint8_t &dataHigh8, uint8_t &dataLow8)
{
#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
//    cbi(_portCS, _bitCS);                                                       // CS : low

    //    digitalWrite(_pinScreenWR, LOW);
    //    digitalWrite(_pinScreenWR, HIGH);
    pulse_low(_portRS, _bitRS);
    dataHigh8 = PORTA;
    dataLow8 = PORTC;

//    sbi(_portCS, _bitCS);                                                       // CS : high
#else
    //#error Platform not supported.
#endif
}

void Screen_W32::_writeCommand16(uint16_t command16)
{
    cbi(_portCS, _bitCS);                                                       // CS : low
    cbi(_portRS, _bitRS); // RS : low
    _writeParallelLCD88(0x00, command16);
    sbi(_portCS, _bitCS);                                                       // CS : high
}

void Screen_W32::_writeRegister(uint8_t command8, uint16_t data16)
{
    cbi(_portCS, _bitCS);                                                       // CS : low
    cbi(_portRS, _bitRS); // RS : low
    _writeParallelLCD88(0x00, command8);
    sbi(_portRS, _bitRS); // RS : high
    _writeParallelLCD88(highByte(data16), lowByte(data16));
    sbi(_portCS, _bitCS);                                                       // CS : high
}

void Screen_W32::_writeData88(uint8_t dataHigh8, uint8_t dataLow8)
{
    cbi(_portCS, _bitCS);                                                       // CS : low
    sbi(_portRS, _bitRS); // RS : high
    _writeParallelLCD88(dataHigh8, dataLow8);
    sbi(_portCS, _bitCS);                                                       // CS : high
}

void Screen_W32::_writeData16(uint16_t data16)
{
    cbi(_portCS, _bitCS);                                                       // CS : low
    sbi(_portRS, _bitRS); // RS : high
    _writeParallelLCD88(highByte(data16), lowByte(data16));
    sbi(_portCS, _bitCS);                                                       // CS : high
}


void Screen_W32::_writeAddress16(uint16_t data16)
{
    cbi(_portCS, _bitCS);                                                       // CS : low
    sbi(_portRS, _bitRS); // RS : high
    _writeParallelLCD88(0x00, highByte(data16));
    _writeParallelLCD88(0x00, lowByte(data16));
    sbi(_portCS, _bitCS);                                                       // CS : high
}


void Screen_W32::_setPoint(uint16_t x1, uint16_t y1, uint16_t colour) // compulsory
{
    _setWindow(x1, y1, x1, y1);
    _writeData16(colour);
}

void Screen_W32::_setWindow(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    _orientCoordinates(x1, y1);
    _orientCoordinates(x2, y2);
    
    if (x1 > x2) _swap(x1, x2);
    if (y1 > y2) _swap(y1, y2);
    
    _writeRegister(0x02, highByte(x1));
	_writeRegister(0x03, lowByte(x1));
    _writeRegister(0x04, highByte(x2));
	_writeRegister(0x05, lowByte(x2));
    _writeRegister(0x06, highByte(y1));
	_writeRegister(0x07, lowByte(y1));
    _writeRegister(0x08, highByte(y2));
	_writeRegister(0x09, lowByte(y2));
	_writeCommand16(0x22);
}

void Screen_W32::_closeWindow()
{
    _setWindow(0, 0, screenSizeX()-1, screenSizeY()-1);
}

void Screen_W32::_fastFill(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t colour)
{
    if (x1 > x2) _swap(x1, x2);
    if (y1 > y2) _swap(y1, y2);
    
    uint8_t hi = highByte(colour);
    uint8_t lo = lowByte(colour);
    
    _setWindow(x1, y1, x2, y2);
    
    cbi(_portCS, _bitCS);                                                       // CS : low
    sbi(_portRS, _bitRS);
    for (uint32_t t=(uint32_t)(y2-y1+1)*(x2-x1+1); t>0; t--) {
        _writeParallelLCD88(hi, lo);
    }
    sbi(_portCS, _bitCS);                                                       // CS : high
}

// Touch
void Screen_W32::_getOneTouch(uint8_t command8, uint8_t &a, uint8_t &b)
{
    digitalWrite(_pinTouchCS, LOW);
    
    shiftOut(_pinScreenMOSI, _pinScreenClock, MSBFIRST, command8);
    a = shiftIn(_pinScreenMISO, _pinScreenClock, MSBFIRST);
    b = shiftIn(_pinScreenMISO, _pinScreenClock, MSBFIRST);
    
    digitalWrite(_pinTouchCS, HIGH);
}

void Screen_W32::_getRawTouch(uint16_t &x0, uint16_t &y0, uint16_t &z0)
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
        
        x0 = a << 4 | b >> 4;
        y0 = c << 4 | d >> 4;
    }
    
    delay(10);
    //    Serial.print("_getRawTouch\t");
    //    Serial.print(x0, DEC);
    //    Serial.print("\t");
    //    Serial.print(y0, DEC);
    //    Serial.print("\t");
    //    Serial.println(z0, DEC);
}

void Screen_W32::_setBacklight(bool state)
{
    ;
}

void Screen_W32::_setIntensity(uint8_t intensity)
{
    ;
}




