//
// Screen_ST7735.cpp
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
// See Screen_ST7735.h and ReadMe.txt for references
//


// Library header
#include "Screen_ST7735.h"

///
/// @name	ST7735 size
/// @{

#define ST7735_WIDTH  128
#define ST7735_HEIGHT 160

/// @}

///
/// @name	ST7735 constants
/// @{

#define ST7735_MADCTL_MY  0x80
#define ST7735_MADCTL_MX  0x40
#define ST7735_MADCTL_MV  0x20
#define ST7735_MADCTL_ML  0x10
#define ST7735_MADCTL_RGB 0x08
#define ST7735_MADCTL_MH  0x04

/// @}

///
/// @name	ST7735 commands
/// @{

#define ST7735_NOP     0x00
#define ST7735_SWRESET 0x01
#define ST7735_RDDID   0x04
#define ST7735_RDDST   0x09

#define ST7735_SLPIN   0x10
#define ST7735_SLPOUT  0x11
#define ST7735_PTLON   0x12
#define ST7735_NORON   0x13

#define ST7735_INVOFF  0x20
#define ST7735_INVON   0x21
#define ST7735_DISPOFF 0x28
#define ST7735_DISPON  0x29
#define ST7735_CASET   0x2A
#define ST7735_RASET   0x2B
#define ST7735_RAMWR   0x2C
#define ST7735_RAMRD   0x2E

#define ST7735_PTLAR   0x30
#define ST7735_COLMOD  0x3A
#define ST7735_MADCTL  0x36

#define ST7735_FRMCTR1 0xB1
#define ST7735_FRMCTR2 0xB2
#define ST7735_FRMCTR3 0xB3
#define ST7735_INVCTR  0xB4
#define ST7735_DISSET5 0xB6

#define ST7735_PWCTR1  0xC0
#define ST7735_PWCTR2  0xC1
#define ST7735_PWCTR3  0xC2
#define ST7735_PWCTR4  0xC3
#define ST7735_PWCTR5  0xC4
#define ST7735_VMCTR1  0xC5

#define ST7735_RDID1   0xDA
#define ST7735_RDID2   0xDB
#define ST7735_RDID3   0xDC
#define ST7735_RDID4   0xDD

#define ST7735_PWCTR6  0xFC

#define ST7735_GMCTRP1 0xE0
#define ST7735_GMCTRN1 0xE1

/// @}


// Class
Screen_ST7735::Screen_ST7735(uint8_t tab) {
#if defined(__MSP430G2553__)
    //Screen_ST7735 myScreen(NULL, P1_4, P1_2, NULL, ST7735R_RED_TAB);
    _pinScreenReset          = NULL; // screen reset
    _pinScreenDataCommand    = P1_4; // data or command
    _pinScreenCS     = P1_2; // select
    _pinScreenMOSI  = P1_7;
    _pinScreenClock = P1_5;
    _pinScreenBacklight      = NULL; // screen backlight
    _tab = tab;
#elif defined(__LM4F120H5QR__)
    //Screen_ST7735 myScreen(NULL, PE_5, PB_1, NULL, ST7735R_RED_TAB);
    _pinScreenReset          = NULL; // screen reset
    _pinScreenDataCommand    = PE_5; // data or command
    _pinScreenCS     = PB_1; // select
    _pinScreenMOSI  = PB_4;
    _pinScreenClock = PB_7;
    _pinScreenBacklight      = NULL; // screen backlight
    _tab            = tab;
#elif defined(__AVR_ATmega328P__) || defined(__AVR_ATmega2560__) || defined(__AVR_ATmega644P__)
    _pinScreenReset          = A0; // screen reset
    _pinScreenDataCommand    = 8; // data or command
    _pinScreenCS     = 7; // select
    _pinScreenMOSI  = 13;
    _pinScreenClock = 11;
    _pinScreenBacklight      = 9; // screen backlight
    _tab            = tab;
#else
//#error Platform not supported
#endif
    
}

Screen_ST7735::Screen_ST7735(uint8_t resetPin, uint8_t dataCommandPin, uint8_t chipSelectPin, uint8_t backlightPin, uint8_t tab)
{
    _pinScreenReset = resetPin; // screen reset
    _pinScreenDataCommand = dataCommandPin; // data or command
    _pinScreenCS = chipSelectPin; // select
    _pinScreenBacklight = backlightPin; // screen backlight
    _tab = tab;
};


void Screen_ST7735::begin()
{
#if defined(HARDWARE_SPI)
    
#if defined(__LM4F120H5QR__)
    SPI.setModule(2);
#endif
    
    SPI.begin();
    SPI.setClockDivider(SPI_CLOCK_DIV2);
    SPI.setBitOrder(MSBFIRST);
    SPI.setDataMode(SPI_MODE0);
    
#else
    //    _pinScreenClock = SPI_CLOCK_PIN;    // Serial Clock
    //    _pinScreenMOSI  = SPI_DATA_PIN;    // Serial Data
    
    pinMode(_pinScreenClock, OUTPUT);
    pinMode(_pinScreenMOSI, OUTPUT);
#endif
    
    if (_pinScreenReset!=NULL) pinMode(_pinScreenReset, OUTPUT);
    if (_pinScreenBacklight!=NULL) pinMode(_pinScreenBacklight, OUTPUT);
    pinMode(_pinScreenDataCommand, OUTPUT);
    pinMode(_pinScreenCS, OUTPUT);
    
    if (_pinScreenBacklight!=NULL) digitalWrite(_pinScreenBacklight, HIGH);
    
    
    // ST7735 reset
    if (_pinScreenReset!=NULL) digitalWrite(_pinScreenReset, 1);
    delay(100);
    if (_pinScreenReset!=NULL) digitalWrite(_pinScreenReset, 0);
    delay(50);
    if (_pinScreenReset!=NULL) digitalWrite(_pinScreenReset, 1);
    delay(120);
    
    // ST7735 reset
    _writeCommand(ST7735_SWRESET);
    delay(150);
    _writeCommand(ST7735_SLPOUT);
    delay(200);
    
    if (_tab==ST7735B) {
        // ST7735B initialisation
        _writeRegister(ST7735_COLMOD, 0x05);
        delay(10);
        _writeRegister(ST7735_FRMCTR1, 0x00);
        _writeData88(0x06, 0x03);
        delay(10);
        _writeRegister(ST7735_MADCTL, 0x08);
        _writeCommand(ST7735_DISSET5);
        _writeData88(0x15, 0x02);
        _writeRegister(ST7735_INVCTR, 0x00);
        _writeCommand(ST7735_PWCTR1);
        _writeData88(0x02, 0x70);
        delay(10);
        _writeRegister(ST7735_PWCTR2, 0x05);
        _writeCommand(ST7735_PWCTR3);
        _writeData88(0x01, 0x02);
        _writeCommand(ST7735_VMCTR1);
        _writeData88(0x3C, 0x38);
        delay(10);
        _writeCommand(ST7735_PWCTR6);
        _writeData88(0x11, 0x15);
        _writeCommand(ST7735_GMCTRP1);
        _writeData8888(0x09, 0x16, 0x09, 0x20);
        _writeData8888(0x21, 0x1B, 0x13, 0x19);
        _writeData8888(0x17, 0x15, 0x1E, 0x2B);
        _writeData8888(0x04, 0x05, 0x02, 0x0E);
        _writeCommand(ST7735_GMCTRN1);
        _writeData8888(0x0B, 0x14, 0x08, 0x1E);
        _writeData8888(0x22, 0x1D, 0x18, 0x1E);
        _writeData8888(0x1B, 0x1A, 0x24, 0x2B);
        _writeData8888(0x06, 0x06, 0x02, 0x0F);
        delay(10);
        _writeCommand(ST7735_CASET);
        _writeData8888(0x00, 0x02, 0x00, 0x81);
        _writeCommand(ST7735_RASET);
        _writeData8888(0x00, 0x02, 0x00, 0x81);
    } else {
        // ST7735R initialisation- part 1=red or green tab)
        // ST7735R display control setting
        _writeRegister(ST7735_FRMCTR1, 0x01);
        _writeData88(0x2c, 0x2d);
        _writeRegister(ST7735_FRMCTR2, 0x01);
        _writeData88(0x2c, 0x2d);
        _writeCommand(ST7735_FRMCTR3);
        _writeData88(0x01, 0x2c);
        _writeData88(0x2d, 0x01);
        _writeData88(0x2c, 0x2d);
        _writeRegister(ST7735_INVCTR, 0x00);
        
        _writeRegister(ST7735_PWCTR1, 0xa2);
        _writeData88(0x02, 0x84);
        _writeRegister(ST7735_PWCTR2, 0xc5);
        _writeCommand(ST7735_PWCTR3);
        _writeData88(0x0a, 0x00);
        _writeCommand(ST7735_PWCTR4);
        _writeData88(0x8a, 0x2a);
        _writeCommand(ST7735_PWCTR5);
        _writeData88(0x8a, 0xee);
        _writeRegister(ST7735_VMCTR1, 0x0e);
        _writeCommand(ST7735_INVOFF);
        _writeRegister(ST7735_MADCTL, 0xc8);
        
        // ST7735R colour mode 64K
        _writeRegister(ST7735_COLMOD, 0x05);
        if (_tab==ST7735R_GREEN_TAB) {
            // ST7735R initialisation- part 2=green tab only
            _writeCommand(ST7735_CASET);
            _writeData8888(0x00, 0x02, 0x00, 0x7f+0x02);
            _writeCommand(ST7735_RASET);
            _writeData8888(0x00, 0x01, 0x00, 0x9f+0x01);
        } else {
            // ST7735R initialisation- part 2=red tab only
            _writeCommand(ST7735_CASET);
            _writeData8888(0x00, 0x00, 0x00, 0x7F);
            _writeCommand(ST7735_RASET);
            _writeData8888(0x00, 0x00, 0x00, 0x9F);
        }
        
        // ST7735R initialisation- part 3=red or green tab)
        // ST7735R gamma
        _writeCommand(ST7735_GMCTRP1);
        _writeData8888(0x02, 0x1c, 0x07, 0x12);
        _writeData8888(0x37, 0x32, 0x29, 0x2d);
        _writeData8888(0x29, 0x25, 0x2B, 0x39);
        _writeData8888(0x00, 0x01, 0x03, 0x10);
        _writeCommand(ST7735_GMCTRN1);
        _writeData8888(0x03, 0x1d, 0x07, 0x06);
        _writeData8888(0x2E, 0x2C, 0x29, 0x2D);
        _writeData8888(0x2E, 0x2E, 0x37, 0x3F);
        _writeData8888(0x00, 0x00, 0x02, 0x10);
    }
    
    // ST7735B or ST7735R end setting
    _writeCommand(ST7735_NORON);
    delay(10);
    _writeCommand(ST7735_DISPON);
    delay(120);
    _writeCommand(ST7735_RAMWR);
    
    _setBacklight(true);
    setOrientation(0);
    
    _screenDiagonal = 18;
    _screenWidth  = ST7735_WIDTH;
    _screenHeigth = ST7735_HEIGHT;
    _penSolid  = false;
    _fontSolid = true;
    _flagRead  = false;
    _touchTrim = 0; // no touch
    clear();
}

String Screen_ST7735::WhoAmI()
{
    return "Universal Colour LCD BoosterPack";
}

void Screen_ST7735::invert(boolean flag)
{
    _writeCommand(flag ? ST7735_INVON : ST7735_INVOFF);
}

void Screen_ST7735::_setBacklight(bool flag)
{
    if (_pinScreenBacklight!=NULL) digitalWrite(_pinScreenBacklight, flag);
}

void Screen_ST7735::_setIntensity(uint8_t intensity)
{
    ;
}

//void Screen_ST7735::setDisplay(boolean flag)
//{
//    if (_pinScreenBacklight!=NULL) setBacklight(flag);
//}

void Screen_ST7735::_setOrientation(uint8_t orientation)
{
//    _orientation = orientation % 4;
    
    _writeCommand(ST7735_MADCTL);
    switch (_orientation) {
        case 0:
            _writeData(ST7735_MADCTL_MX | ST7735_MADCTL_MY | ST7735_MADCTL_RGB);
            break;
        case 1:
            _writeData(ST7735_MADCTL_MY | ST7735_MADCTL_MV | ST7735_MADCTL_RGB);
            break;
        case 2:
            _writeData(ST7735_MADCTL_RGB);
            break;
        case 3:
            _writeData(ST7735_MADCTL_MX | ST7735_MADCTL_MV | ST7735_MADCTL_RGB);
            break;
    }
}

void Screen_ST7735::_orientCoordinates(uint16_t &x1, uint16_t &y1)
{
    ;
}

void Screen_ST7735::_fastFill(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t colour)
{
    if (x1 > x2) _swap(x1, x2);
    if (y1 > y2) _swap(y1, y2);
    
    _setWindow(x1, y1, x2, y2);
    
    digitalWrite(_pinScreenDataCommand, HIGH); // HIGH = data
    digitalWrite(_pinScreenCS, LOW); // LOW = select
    uint8_t hi = highByte(colour);
    uint8_t lo = lowByte(colour);
    
#if defined(HARDWARE_SPI)
    for (uint32_t t=(uint32_t)(y2-y1+1)*(x2-x1+1); t>0; t--) {
        SPI.transfer(hi); // write D8..D15
        SPI.transfer(lo); // write D0..D7
    }
#else
    for (uint32_t t=(uint32_t)(y2-y1+1)*(x2-x1+1); t>0; t--) {
        shiftOut(_pinScreenMOSI, _pinScreenClock, MSBFIRST, hi);
        shiftOut(_pinScreenMOSI, _pinScreenClock, MSBFIRST, lo);
    }
#endif
    
    digitalWrite(_pinScreenCS, HIGH);
}

void Screen_ST7735::_setPoint(uint16_t x1, uint16_t y1, uint16_t colour)
{
    if( (x1 < 0) || (x1 >= screenSizeX()) || (y1 < 0) || (y1 >= screenSizeY()) ) return;
    
    _setWindow(x1, y1, x1+1, y1+1);
    _writeData16(colour);
}


// Utilities
void Screen_ST7735::_setWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
    if (_tab == ST7735R_GREEN_TAB) {
        switch (_orientation)
        {
            case 0:
            case 2:
                x0 += 2;
                y0 += 1;
                x1 += 2;
                y1 += 1;
                break;
            case 1:
            case 3:
                x0 += 1;
                y0 += 2;
                x1 += 1;
                y1 += 2;
                break;
            default:
                break;
        }
    }
    
    _writeCommand(0x2a);
    _writeData16(x0);
    _writeData16(x1);
    _writeCommand(0x2b);
    _writeData16(y0);
    _writeData16(y1);
    _writeCommand(0x2C);
}

void Screen_ST7735::_closeWindow()
{
    ;
}

void Screen_ST7735::_writeRegister(uint8_t command8, uint8_t data8)
{
    _writeCommand(command8);
    _writeData(data8);
}

void Screen_ST7735::_writeCommand(uint8_t command8)
{
    digitalWrite(_pinScreenDataCommand, LOW);
    digitalWrite(_pinScreenCS, LOW);
    
#if defined(HARDWARE_SPI)
    SPI.transfer(command8);
#else
    shiftOut(_pinScreenMOSI, _pinScreenClock, MSBFIRST, command8);
#endif
    
    digitalWrite(_pinScreenCS, HIGH);
}

void Screen_ST7735::_writeData(uint8_t data8)
{
    digitalWrite(_pinScreenDataCommand, HIGH);
    digitalWrite(_pinScreenCS, LOW);
    
#if defined(HARDWARE_SPI)
    SPI.transfer(data8);
#else
    shiftOut(_pinScreenMOSI, _pinScreenClock, MSBFIRST, data8);
#endif
    
    digitalWrite(_pinScreenCS, HIGH);
}

void Screen_ST7735::_writeData16(uint16_t data16)
{
    digitalWrite(_pinScreenDataCommand, HIGH);
    digitalWrite(_pinScreenCS, LOW);
    
#if defined(HARDWARE_SPI)
    SPI.transfer(highByte(data16));
    SPI.transfer(lowByte(data16));
#else
    shiftOut(_pinScreenMOSI, _pinScreenClock, MSBFIRST, highByte(data16));
    shiftOut(_pinScreenMOSI, _pinScreenClock, MSBFIRST, lowByte(data16));
#endif
    
    digitalWrite(_pinScreenCS, HIGH);
}

void Screen_ST7735::_writeData88(uint8_t dataHigh8, uint8_t dataLow8)
{
    digitalWrite(_pinScreenDataCommand, HIGH);
    digitalWrite(_pinScreenCS, LOW);
    
#if defined(HARDWARE_SPI)
    SPI.transfer(dataHigh8);
    SPI.transfer(dataLow8);
#else
    shiftOut(_pinScreenMOSI, _pinScreenClock, MSBFIRST, dataHigh8);
    shiftOut(_pinScreenMOSI, _pinScreenClock, MSBFIRST, dataLow8);
#endif
    
    digitalWrite(_pinScreenCS, HIGH);
}

void Screen_ST7735::_writeData8888(uint8_t dataHigh8, uint8_t dataLow8, uint8_t data8_3, uint8_t data8_4)
{
    _writeData(dataHigh8);
    _writeData(dataLow8);
    _writeData(data8_3);
    _writeData(data8_4);
}

void Screen_ST7735::_getRawTouch(uint16_t &x0, uint16_t &y0, uint16_t &z0)
{
    x0 = 0;
    y0 = 0;
    z0 = 0;
}
