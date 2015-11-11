//
// Screen_PICadillo.cpp
// Library C++ code
// ----------------------------------
// Developed with embedXcode
// http://embedXcode.weebly.com
//
// Project LCD_screen Library Suite
//
// Created by Rei VILO, mars 07, 2013 09:21
// embedXcode.weebly.com
//
//
// Copyright © Rei VILO, 2013-2014
// License CC = BY NC SA
//
// See Screen_PICadillo.h and ReadMe.txt for references
//


// Library header
#include "Screen_PICadillo.h"

#ifdef __32MX795F512L__

///
/// @name	HX8357B size
/// @{

#define HX8357B_WIDTH   320
#define HX8357B_HEIGHT  480

/// @}

///
/// @name	HX8357B constants
/// @{

#define HX8357B_MADCTL_MY  0x80
#define HX8357B_MADCTL_MX  0x40
#define HX8357B_MADCTL_MV  0x20
#define HX8357B_MADCTL_ML  0x10
#define HX8357B_MADCTL_RGB 0x08
#define HX8357B_MADCTL_MH  0x04

/// @}

///
/// @name	HX8357B commands
/// @{

#define HX8357B_NOP     0x00
#define HX8357B_SWRESET 0x01
#define HX8357B_RDDID   0x04
#define HX8357B_RDDST   0x09

#define HX8357B_SLPIN   0x10
#define HX8357B_SLPOUT  0x11
#define HX8357B_PTLON   0x12
#define HX8357B_NORON   0x13

#define HX8357B_INVOFF  0x20
#define HX8357B_INVON   0x21
#define HX8357B_GAMSET  0x26
#define HX8357B_DISPOFF 0x28
#define HX8357B_DISPON  0x29
#define HX8357B_CASET   0x2A
#define HX8357B_RASET   0x2B
#define HX8357B_RAMWR   0x2C
#define HX8357B_RGBSET  0x2d
#define HX8357B_RAMRD   0x2E

#define HX8357B_PTLAR   0x30
#define HX8357B_MADCTL  0x36
#define HX8357B_COLMOD  0x3A

#define HX8357B_SETPWCTR 0xB1
#define HX8357B_SETDISPL 0xB2
#define HX8357B_FRMCTR3  0xB3
#define HX8357B_SETCYC   0xB4
#define HX8357B_SETBGP   0xb5
#define HX8357B_SETVCOM  0xB6
#define HX8357B_ENEXTCOM 0xb9

#define HX8357B_SETSTBA  0xC0
#define HX8357B_SETID    0xC3
#define HX8357B_SETCHAR  0xcc

#define HX8357B_GETHID   0xd0

#define HX8357B_SETGAMMA 0xE0


/// @}

///
/// @name   Touch constants
///
/// @{

#define TOUCH_TRIM  0x10  ///< Touch threshold

// TOUCH_XP_XL PE4 Resistor touch screen terminal (Left)
// TOUCH_YP_YU PE5 Resistor touch screen terminal (Top)
// TOUCH_XN_XR PA3 Resistor touch screen terminal (Right)
// TOUCH_YN_YD PA2 Resistor touch screen terminal (Bottom)
#define TOUCH_XP_XL LCD_XL    // must be an analog pin, use "An" notation!
#define TOUCH_YP_YU LCD_YU    // must be an analog pin, use "An" notation!
#define TOUCH_XN_XR LCD_XR    // can be a digital pin
#define TOUCH_YN_YD LCD_YD    // can be a digital pin

/// @}

inline uint16_t absDiff(uint16_t a, uint16_t b) { return (a > b) ? a-b : b-a; }


// Class
Screen_PICadillo::Screen_PICadillo()
{
//#if defined(__32MX795F512L__)
//    //Screen_PICadillo myScreen(17, 31, 13, 39);
//    _pinScreenReset          = 17; // screen reset
//    _pinScreenDataCommand    = 31; // data or command
//    _pinScreenCS     = 13; // select
//    //    _pinSerialData     = PB_4;
//    //    _pinSerialClock    = PB_7;
//    _pinScreenBacklight      = NULL; // 39; // screen backlight - pin 39 for red PWM RGB LED
//#else
//#error Platform not supported
//#endif
//    
}

void Screen_PICadillo::begin()
{
    PMCONbits.ON = 0;
    asm volatile("nop");
    
    PMCONbits.PTWREN = 1;
    PMCONbits.PTRDEN = 1;
    PMCONbits.CSF = 0;
    
    PMAEN = 0x0001; // Enable PMA0 pin for RS pin and CS1 as CS
    
    PMMODEbits.MODE16 = 1;
    PMMODEbits.MODE = 0b10;
    PMMODEbits.WAITB = 0;
    PMMODEbits.WAITM = 0;
    PMMODEbits.WAITE = 0;
    
    //PMADDR = 0; // Set current address to 0
    PMADDR = 0x0000; // Set current address to 0, CS1 Active
    
    PMCONbits.ON = 1;
    
    _writeCommand16(HX8357B_SLPOUT);
    delay(150);
    _writeCommand16(HX8357B_ENEXTCOM);
    _writeData16(0xFF);
    _writeData16(0x83);
    _writeData16(0x57);
	delay(1);
    _writeCommand16(HX8357B_SETPWCTR);
    _writeData16(0x00);
    _writeData16(0x12);
    _writeData16(0x12);
    _writeData16(0x12);
    _writeData16(0xC3);
    _writeData16(0x44);
	delay(1);
    _writeCommand16(HX8357B_SETCYC);
    _writeData16(0x02);
    _writeData16(0x40);
    _writeData16(0x00);
    _writeData16(0x2A);
    _writeData16(0x2A);
    _writeData16(0x20);
    _writeData16(0x91);
	delay(1);
    _writeCommand16(HX8357B_SETVCOM);
    _writeData16(0x38);
	delay(1);
    _writeCommand16(HX8357B_MADCTL_MY);
    _writeData16(0x68);
    _writeCommand16(0xE3);
    _writeData16(0x2F);
    _writeData16(0x1F);
    _writeCommand16(0xB5);
    _writeData16(0x01);
    _writeData16(0x01);
    _writeData16(0x67);
    _writeCommand16(HX8357B_SETSTBA);
    _writeData16(0x70);
    _writeData16(0x70);
    _writeData16(0x01);
    _writeData16(0x3C);
    _writeData16(0xC8);
    _writeData16(0x08);
	delay(1);
    _writeCommand16(0xC2);
    _writeData16(0x00);
    _writeData16(0x08);
    _writeData16(0x04);
	delay(1);
    _writeCommand16(HX8357B_SETCHAR);
    _writeData16(0x09);
	delay(1);
    _writeCommand16(HX8357B_SETGAMMA);
    _writeData16(0x01);
    _writeData16(0x02);
    _writeData16(0x03);
    _writeData16(0x05);
    _writeData16(0x0E);
    _writeData16(0x22);
    _writeData16(0x32);
    _writeData16(0x3B);
    _writeData16(0x5C);
    _writeData16(0x54);
    _writeData16(0x4C);
    _writeData16(0x41);
    _writeData16(0x3D);
    _writeData16(0x37);
    _writeData16(0x31);
    _writeData16(0x21);
    _writeData16(0x01);
    _writeData16(0x02);
    _writeData16(0x03);
    _writeData16(0x05);
    _writeData16(0x0E);
    _writeData16(0x22);
    _writeData16(0x32);
    _writeData16(0x3B);
    _writeData16(0x5C);
    _writeData16(0x54);
    _writeData16(0x4C);
    _writeData16(0x41);
    _writeData16(0x3D);
    _writeData16(0x37);
    _writeData16(0x31);
    _writeData16(0x21);
    _writeData16(0x00);
    _writeData16(0x01);
	delay(1);
    _writeCommand16(HX8357B_COLMOD);
    _writeData16(0x55);                                                         // 16 bits per pixel
    
    _writeCommand16(HX8357B_MADCTL);
    _writeData16(0x00);
    _writeCommand16(0x35);
    _writeData16(0x00);
	delay(10);
    _writeCommand16(0x29);
	delay(10);
    _writeCommand16(HX8357B_RAMWR);
    _writeCommand16(0x29);
    
    setBacklight(true);
    setOrientation(0);
    
    _setIntensity(255);
    _screenWidth  = HX8357B_WIDTH;
    _screenHeigth = HX8357B_HEIGHT;
    _screenDiagonal = 35;
    
    // Touch calibration
    _touchXmin = 803;
    _touchXmax = 126;
    _touchYmin = 856;
    _touchYmax = 94;

    // Required by screen orientation
    _swap(_touchXmin, _touchXmax);
    _swap(_touchYmin, _touchYmax);
    
    _penSolid  = false;
    _fontSolid = true;
    _flagRead  = true;
    _fsmArea   = 0x01;;
    _touchTrim = TOUCH_TRIM;
    
    clear();
}

String Screen_PICadillo::WhoAmI()
{
    return "PICadillo 35T";
}

void Screen_PICadillo::invert(boolean flag)
{
    _writeCommand16(flag ? HX8357B_INVON : HX8357B_INVOFF);
}

void Screen_PICadillo::_setBacklight(bool flag)
{
    if (_pinScreenBacklight != NULL) digitalWrite(_pinScreenBacklight, flag);
}

void Screen_PICadillo::_setIntensity(uint8_t intensity)
{
    analogWrite(PIN_BACKLIGHT, intensity);
}

void Screen_PICadillo::_orientCoordinates(uint16_t &x1, uint16_t &y1)
{
    ;
}

void Screen_PICadillo::_setOrientation(uint8_t orientation)
{
//#define HX8357B_MADCTL_MY  0x80
//#define HX8357B_MADCTL_MX  0x40
//#define HX8357B_MADCTL_MV  0x20
//#define HX8357B_MADCTL_ML  0x10
//#define HX8357B_MADCTL_RGB 0x08
//#define HX8357B_MADCTL_MH  0x04
    //    _orientation = orientation % 4;
    //
    _writeCommand16(HX8357B_MADCTL);
    switch (_orientation)
    {
        case 2: // initially 0
            // 0x80 + 0x40 + 0x08
            _writeData16(HX8357B_MADCTL_MX | HX8357B_MADCTL_MY); // | HX8357B_MADCTL_RGB);
            break;
        case 3: // initially 1
            // 0x80 + 0x20 + 0x08
            _writeData16(HX8357B_MADCTL_MY | HX8357B_MADCTL_MV); // | HX8357B_MADCTL_RGB);
            break;
        case 0: // initially 2
            // 0x08
            _writeData16(0); //HX8357B_MADCTL_RGB);
            break;
        case 1: // initially 3
            // 0x40 + 0x20 + 0x08
            _writeData16(HX8357B_MADCTL_MX | HX8357B_MADCTL_MV); // | HX8357B_MADCTL_RGB);
            break;
    }
}


void Screen_PICadillo::_fastFill(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t colour)
{
    if (x1 > x2) _swap(x1, x2);
    if (y1 > y2) _swap(y1, y2);
    
    _setWindow(x1, y1, x2, y2);
    
//    digitalWrite(_pinScreenDataCommand, HIGH);                                        // HIGH = data
//    digitalWrite(_pinScreenCS, LOW);                                          // LOW = select
//    uint8_t hi = highByte(colour);
//    uint8_t lo = lowByte(colour);
    
    //#if defined(HARDWARE_SPI)
    for (uint32_t t=(uint32_t)(y2-y1+1)*(x2-x1+1); t>0; t--)
    {
        _writeData16(colour);
    }
    //#else
    //    for (uint32_t t=(uint32_t)(y2-y1+1)*(x2-x1+1); t>0; t--) {
    //        shiftOut(_pinSerialData, _pinSerialClock, MSBFIRST, hi);
    //        shiftOut(_pinSerialData, _pinSerialClock, MSBFIRST, lo);
    //    }
    //#endif
    
//    digitalWrite(_pinScreenCS, HIGH);
}

void Screen_PICadillo::_setPoint(uint16_t x1, uint16_t y1, uint16_t colour)
{
    if( (x1 < 0) or (x1 >= screenSizeX()) or (y1 < 0) or (y1 >= screenSizeY()) ) return;
    
    _setWindow(x1, y1, x1+1, y1+1);
    _writeData16(colour);
}


// Utilities
void Screen_PICadillo::_setWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
    _writeCommand16(HX8357B_CASET);
    _writeData16(highByte(x0));
    _writeData16(lowByte(x0));
    _writeData16(highByte(x1));
    _writeData16(lowByte(x1));
    
    _writeCommand16(HX8357B_RASET);
    _writeData16(highByte(y0));
    _writeData16(lowByte(y0));
    _writeData16(highByte(y1));
    _writeData16(lowByte(y1));

    _writeCommand16(HX8357B_RAMWR);
}

void Screen_PICadillo::_closeWindow()
{
    _setWindow(0, 0, screenSizeX()-1, screenSizeY()-1);
}

void Screen_PICadillo::_writeCommand16(uint16_t command16)
{
    while (PMMODEbits.BUSY == 1);
    PMADDR = 0x0000;
    PMDIN = command16;
}

void Screen_PICadillo::_writeData16(uint16_t data16)
{
    while (PMMODEbits.BUSY == 1);
    PMADDR = 0x0001;
    PMDIN = data16;
}

void Screen_PICadillo::_writeData88(uint8_t dataHigh8, uint8_t dataLow8)
{
    while (PMMODEbits.BUSY == 1);
    PMADDR = 0x0001;
    PMDIN = word(dataHigh8, dataLow8);
}

// Read
void Screen_PICadillo::_setWindowRead(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
    _writeCommand16(HX8357B_CASET);
    _writeData16(highByte(x0));
    _writeData16(lowByte(x0));
    _writeData16(highByte(x1));
    _writeData16(lowByte(x1));
    
    _writeCommand16(HX8357B_RASET);
    _writeData16(highByte(y0));
    _writeData16(lowByte(y0));
    _writeData16(highByte(y1));
    _writeData16(lowByte(y1));
    
    _writeCommand16(HX8357B_RAMRD);
}

uint16_t Screen_PICadillo::readPixel(uint16_t x1, uint16_t y1)
{
	_setWindowRead(x1, y1, x1 +1, y1 +1);
	return _readData16();
}

void Screen_PICadillo::copyPasteArea(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t dx, uint16_t dy)
{
    uint16_t buffer[dx];
    
    _closeWindow();
    
    for (uint16_t y=0; y<dy; y++)
    {
        for (uint16_t x=0; x<dx; x++)
        {
            _setWindowRead(x1+x, y1+y, x1+x, y1+y);
            buffer[x] = _readData16();
        }
        
        _setWindow(x2, y2+y, x2+dx, y2+y);
        
        for (uint16_t x=0; x<dx; x++)
        {
            _writeData16(buffer[x]);
        }
    }
    _closeWindow();
}

uint16_t Screen_PICadillo::_readData16()
{
    uint16_t dummy;
    PMADDR = 0x0001;
    dummy = PMDIN;
    
    for (uint8_t i=5; i>0; i--)
    {
        while (PMMODEbits.BUSY == 1);
        PMADDR = 0x0001;
        dummy = PMDIN;
    }
    while (PMMODEbits.BUSY == 1);                                               // 24-bit readings
    uint16_t val1 = PMDIN;
    while (PMMODEbits.BUSY == 1);
    uint16_t val2 = PMDIN;
    while (PMMODEbits.BUSY == 1);
    uint16_t val3 = PMDIN;

    return myColours.calculate(highByte(val1), lowByte(val1), highByte(val2));
}

// Touch
void Screen_PICadillo::_getRawTouch(uint16_t &x0, uint16_t &y0, uint16_t &z0)
{
    uint16_t a, b, c, d;
    
    // Read x
    // xp = +Vref
    // xn = ground
    // yp = measure
    // yn = open
    pinMode(TOUCH_YP_YU, INPUT);
    pinMode(TOUCH_YN_YD, INPUT);
    digitalWrite(TOUCH_YP_YU, LOW);
    digitalWrite(TOUCH_YN_YD, LOW);
    
    pinMode(TOUCH_XP_XL, OUTPUT);
    pinMode(TOUCH_XN_XR, OUTPUT);
    digitalWrite(TOUCH_XP_XL, HIGH);
    digitalWrite(TOUCH_XN_XR, LOW);
    
    do
    {
        delay(1);
        a  = analogRead(TOUCH_YP_YU);
        delay(1);
        b  = analogRead(TOUCH_YP_YU);
    }
    while (absDiff(a, b) > 8);
    x0  = 1024 -a;
    
    // Read y
    // xp = measure
    // xn = open
    // yp = +Vref
    // yn = ground
    pinMode(TOUCH_XP_XL, INPUT);
    pinMode(TOUCH_XN_XR, INPUT);
    digitalWrite(TOUCH_XP_XL, LOW);
    digitalWrite(TOUCH_XN_XR, LOW);
    
    pinMode(TOUCH_YP_YU, OUTPUT);
    pinMode(TOUCH_YN_YD, OUTPUT);
    digitalWrite(TOUCH_YP_YU, HIGH);
    digitalWrite(TOUCH_YN_YD, LOW);
    
    do
    {
        delay(1);
        a  = analogRead(TOUCH_XP_XL);
        delay(1);
        b  = analogRead(TOUCH_XP_XL);
    }
    while (absDiff(a, b) > 8);
    y0  = 1024 - a;
    
/*
    // Read z
    // xp = ground
    // xn = measure
    // yp = measure
    // yn = +Vref
    pinMode(TOUCH_XP_XL, OUTPUT);
    digitalWrite(TOUCH_XP_XL, LOW);
    pinMode(TOUCH_YN_YD, OUTPUT);
    digitalWrite(TOUCH_YN_YD, HIGH);
    
    digitalWrite(TOUCH_XN_XR, LOW);
    pinMode(TOUCH_XN_XR, INPUT);
    digitalWrite(TOUCH_YP_YU, LOW);
    pinMode(TOUCH_YP_YU, INPUT);
    delay(10);

    do
    {
        delay(1);
        a = analogRead(TOUCH_XN_XR);
        delay(1);
        c = analogRead(TOUCH_YP_YU);
        delay(1);
        b = analogRead(TOUCH_XN_XR);
        delay(1);
        d = analogRead(TOUCH_YP_YU);
    }
    while ((absDiff(a, b) >8) or (absDiff(c, d) >8));
    
        z0 = max(abs(a - b), abs(c - d));
    if ((c < 1000) and (d < 1000)) z0 = TOUCH_TRIM; // x0 > 0) z0 = (a - c); // (4095 - (c-a));
    else z0 = 0;
*/
//    if ((1024-x0 > 100) and (1024-y0 > 100)) z0 = TOUCH_TRIM +1; // x0 > 0) z0 = (a - c); // (4095 - (c-a));
    if ((x0 > 100) and (y0 > 100)) z0 = TOUCH_TRIM +1; // x0 > 0) z0 = (a - c); // (4095 - (c-a));
    else z0 = 0;

//    if ((x0 < 1000) and (y0 < 1000)) z0 = TOUCH_TRIM +1; // x0 > 0) z0 = (a - c); // (4095 - (c-a));
//    else z0 = 0;

//    Serial.println(formatString("Touch xyz=(%i, %i, %i) abcd=(%i, %i, %i, %i)", x0, y0, z0, a, b, c, d));
    
    //    delay(10);
    //    if (z0>0) {
    //        Serial.print("xyz0\t");
    //        Serial.print(x0, DEC);
    //        Serial.print("\t");
    //        Serial.print(y0, DEC);
    //        Serial.print("\t");
    //        Serial.print(z0, DEC);
    //        Serial.println();
    //    }
}

#endif // end __32MX795F512L__

