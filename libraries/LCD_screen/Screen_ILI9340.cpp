//
// Screen_ILI9340.cpp
// Library C++ code
// ----------------------------------
// Developed with embedXcode
// http://embedXcode.weebly.com
//
// Project LCD_screen Library Suite
//
// Created by Rei VILO, Feb 18, 2014
// http://embedXcode.weebly.com
//
//
// Copyright    © Rei VILO, 2013-2014
// License      Creative Commons
//              Attribution-NonCommercial-ShareAlike 3.0 Unported (CC BY-NC-SA 3.0)
//
//


// Library header
#include "Screen_ILI9340.h"
#include "SPI.h"

///
/// @name	ILI9340 constants
///
/// @{

#define ILINOP     0x00
#define ILISWRESET 0x01
#define ILIRDDID   0x04
#define ILIRDDST   0x09

#define ILISLPIN   0x10
#define ILISLPOUT  0x11
#define ILIPTLON   0x12
#define ILINORON   0x13

#define ILIRDMODE  0x0A
#define ILIRDMADCTL  0x0B
#define ILIRDPIXFMT  0x0C
#define ILIRDIMGFMT  0x0A
#define ILIRDSELFDIAG  0x0F

#define ILIINVOFF  0x20
#define ILIINVON   0x21
#define ILIGAMMASET 0x26
#define ILIDISPOFF 0x28
#define ILIDISPON  0x29

#define ILICASET   0x2A
#define ILIPASET   0x2B
#define ILIRAMWR   0x2C
#define ILIRAMRD   0x2E

#define ILIPTLAR   0x30
#define ILIGRAMMODE  0x36
#define DEFAULTILIGRAMMODE ILIMADCTL_BGR

#define ILIMADCTL_MY  0x80
#define ILIMADCTL_MX  0x40
#define ILIMADCTL_MV  0x20
#define ILIMADCTL_ML  0x10
#define ILIMADCTL_RGB 0x00
#define ILIMADCTL_BGR 0x08
#define ILIMADCTL_MH  0x04

#define ILIPIXFMT  0x3A

#define ILIFRMCTR1 0xB1
#define ILIFRMCTR2 0xB2
#define ILIFRMCTR3 0xB3
#define ILIINVCTR  0xB4
#define ILIDFUNCTR 0xB6

#define ILIPWCTR1  0xC0
#define ILIPWCTR2  0xC1
#define ILIPWCTR3  0xC2
#define ILIPWCTR4  0xC3
#define ILIPWCTR5  0xC4
#define ILIVMCTR1  0xC5
#define ILIVMCTR2  0xC7

#define ILIRDID1   0xDA
#define ILIRDID2   0xDB
#define ILIRDID3   0xDC
#define ILIRDID4   0xDD

#define ILIGMCTRP1 0xE0
#define ILIGMCTRN1 0xE1

#define LCD_HEIGHT      320  // Vertical
#define LCD_WIDTH       240  // Horizontal

#define TOUCH_TRIM  0x00 ///< No touch

/// @}



Screen_ILI9340::Screen_ILI9340(uint8_t pinScreenReset, uint8_t pinScreenCS, uint8_t pinScreenDataCommand, uint8_t pinSDcardCS)
{
    _pinScreenCS = pinScreenCS;
    _pinSDcardCS = pinSDcardCS;
    _pinScreenReset    = pinScreenReset;
    _pinScreenDataCommand = pinScreenDataCommand;
}

void Screen_ILI9340::begin()
{
    
    pinMode(_pinScreenCS, OUTPUT);
    pinMode(_pinSDcardCS, OUTPUT);
    pinMode(_pinScreenReset, OUTPUT);
    pinMode(_pinScreenDataCommand, OUTPUT);
    pinMode(_pinScreenReset, OUTPUT);
    
    digitalWrite(_pinScreenDataCommand, HIGH);
    
#if defined(__LM4F120H5QR__) || defined(__TM4C1230C3PM__) || defined(__TM4C129XNCZAD__)
    SPI.setModule(2);
    SPI.begin();
    SPI.setClockDivider(SPI_CLOCK_DIV2);

#else
    SPI.begin();
    SPI.setClockDivider(SPI_CLOCK_DIV2);

#endif
    
//#endif
    SPI.setBitOrder(MSBFIRST);
    SPI.setDataMode(SPI_MODE0);
    
    delay(1);
    
    digitalWrite(_pinScreenReset, LOW);
    digitalWrite(_pinScreenCS, HIGH);
    digitalWrite(_pinSDcardCS, HIGH);

    delay(10);
    digitalWrite(_pinScreenReset, HIGH);
    delay(10);

    /// ILI9340
    _writeCommand8(0xEF);
    _writeData8(0x03);
    _writeData8(0x80);
    _writeData8(0x02);
    
    _writeCommand8(0xCF);
    _writeData8(0x00);
    _writeData8(0XC1);
    _writeData8(0X30);
    
    _writeCommand8(0xED);
    _writeData8(0x64);
    _writeData8(0x03);
    _writeData8(0X12);
    _writeData8(0X81);
    
    _writeCommand8(0xE8);
    _writeData8(0x85);
    _writeData8(0x00);
    _writeData8(0x78);
    
    _writeCommand8(0xCB);
    _writeData8(0x39);
    _writeData8(0x2C);
    _writeData8(0x00);
    _writeData8(0x34);
    _writeData8(0x02);
    
    _writeCommand8(0xF7);
    _writeData8(0x20);
    
    _writeCommand8(0xEA);
    _writeData8(0x00);
    _writeData8(0x00);
    
    _writeCommand8(ILIPWCTR1);    //Power control
    _writeData8(0x23);   //VRH[5:0]
    
    _writeCommand8(ILIPWCTR2);    //Power control
    _writeData8(0x10);   //SAP[2:0];BT[3:0]
    
    _writeCommand8(ILIVMCTR1);    //VCM control
    _writeData8(0x3e); //�Աȶȵ���
    _writeData8(0x28);
    
    _writeCommand8(ILIVMCTR2);    //VCM control2
    _writeData8(0x86);  //--
    
    _writeCommand8(ILIGRAMMODE);    // Memory Access Control
    _writeData8(DEFAULTILIGRAMMODE);
    
    _writeCommand8(ILIPIXFMT);
    _writeData8(0x55);
    
    _writeCommand8(ILIFRMCTR1);
    _writeData8(0x00);
    _writeData8(0x18);
    
    _writeCommand8(ILIDFUNCTR);    // Display Function Control
    _writeData8(0x08);
    _writeData8(0x82);
    _writeData8(0x27);
    
    _writeCommand8(0xF2);    // 3Gamma Function Disable
    _writeData8(0x00);
    
    _writeCommand8(ILIGAMMASET);    //Gamma curve selected
    _writeData8(0x01);
    
    _writeCommand8(ILIGMCTRP1);    //Set Gamma
    _writeData8(0x0F);
    _writeData8(0x31);
    _writeData8(0x2B);
    _writeData8(0x0C);
    _writeData8(0x0E);
    _writeData8(0x08);
    _writeData8(0x4E);
    _writeData8(0xF1);
    _writeData8(0x37);
    _writeData8(0x07);
    _writeData8(0x10);
    _writeData8(0x03);
    _writeData8(0x0E);
    _writeData8(0x09);
    _writeData8(0x00);
    
    _writeCommand8(ILIGMCTRN1);    //Set Gamma
    _writeData8(0x00);
    _writeData8(0x0E);
    _writeData8(0x14);
    _writeData8(0x03);
    _writeData8(0x11);
    _writeData8(0x07);
    _writeData8(0x31);
    _writeData8(0xC1);
    _writeData8(0x48);
    _writeData8(0x08);
    _writeData8(0x0F);
    _writeData8(0x0C); 
    _writeData8(0x31); 
    _writeData8(0x36); 
    _writeData8(0x0F); 
    
    _writeCommand8(ILISLPOUT);    //Exit Sleep 
    delay(120); 		
    _writeCommand8(ILIDISPON);    //Display on 

    _writeCommand8(ILIINVOFF);
    
    _writeRegister8(0x55, 0x01);
    
    _penSolid = false;
    
    _screenWidth = LCD_WIDTH;
    _screenHeigth = LCD_HEIGHT;
    _screenDiagonal = 22;
    
    _touchTrim = TOUCH_TRIM; // no touch
    _penSolid  = false;
    _fontSolid = true;
    _flagRead = false;                               // bug on pixel read
    _fsmArea = 0x01;
    
    setOrientation(0);
    clear();
}


String Screen_ILI9340::WhoAmI()
{
    return "LCD 2.2\" + SD BoosterPack";
}

void Screen_ILI9340::_setBacklight(bool state)
{
    ;
}

void Screen_ILI9340::_setIntensity(uint8_t intensity)
{
    ;
}


void Screen_ILI9340::_setOrientation(uint8_t orientation)
{    
    switch (_orientation) {
        case 0:
            //                             hvO
            _writeRegister8(ILIGRAMMODE, 0b01000000 | DEFAULTILIGRAMMODE);
            break;
        case 1:
            //                             hvO
            _writeRegister8(ILIGRAMMODE, 0b00100000 | DEFAULTILIGRAMMODE);
            break;
        case 2:
            //                             hvO
            _writeRegister8(ILIGRAMMODE, 0b10000000 | DEFAULTILIGRAMMODE);
            break;
        case 3:
            //                             hvO
            _writeRegister8(ILIGRAMMODE, 0b11100000 | DEFAULTILIGRAMMODE);
            break;
    }
}

// Utilities
void Screen_ILI9340::_writeCommand8(uint8_t command8)
{
    digitalWrite(_pinScreenDataCommand, LOW);
    digitalWrite(_pinScreenCS, LOW);
    
#ifdef HARDWARE_SPI
    SPI.transfer(command8);
#else
//    shiftOut(_pinTouchMOSI, _pinTouchClock, MSBFIRST, command8);
#endif
    
    digitalWrite(_pinScreenCS, HIGH);
    digitalWrite(_pinScreenDataCommand, HIGH);
}


void Screen_ILI9340::_writeData16(uint16_t data16)
{
    digitalWrite(_pinScreenDataCommand, HIGH);
    digitalWrite(_pinScreenCS, LOW);
    
#ifdef HARDWARE_SPI
    SPI.transfer(highByte(data16)); // write D8..D15
    SPI.transfer(lowByte(data16)); // write D0..D7
#else
//    shiftOut(_pinTouchMOSI, _pinTouchClock, MSBFIRST, highByte(data16));
//    shiftOut(_pinTouchMOSI, _pinTouchClock, MSBFIRST, lowByte(data16));
#endif
    
    digitalWrite(_pinScreenCS, HIGH);
    digitalWrite(_pinScreenDataCommand, LOW);
}

void Screen_ILI9340::_writeData8(uint8_t data8)
{
    digitalWrite(_pinScreenDataCommand, HIGH);
    digitalWrite(_pinScreenCS, LOW);
    
#ifdef HARDWARE_SPI
    SPI.transfer(data8);
#else
    //    shiftOut(_pinTouchMOSI, _pinTouchClock, MSBFIRST, data8);
#endif
    
    digitalWrite(_pinScreenCS, HIGH);
}

void Screen_ILI9340::_writeData88(uint8_t dataHigh8, uint8_t dataLow8)
{
    digitalWrite(_pinScreenDataCommand, HIGH);
    digitalWrite(_pinScreenCS, LOW);
    
#ifdef HARDWARE_SPI
    SPI.transfer(dataHigh8); // write D8..D15
    SPI.transfer(dataLow8); // write D0..D7
#else
//    shiftOut(_pinTouchMOSI, _pinTouchClock, MSBFIRST, dataHigh8);
//    shiftOut(_pinTouchMOSI, _pinTouchClock, MSBFIRST, dataLow8);
#endif
    
    digitalWrite(_pinScreenCS, HIGH);
}


void Screen_ILI9340::_writeRegister16(uint8_t command8, uint16_t data16)
{
    _writeCommand8(command8);
	_writeData88(highByte(data16), lowByte(data16));
}

void Screen_ILI9340::_writeRegister8(uint8_t command8, uint8_t data8)
{
    _writeCommand8(command8);
	_writeData8(data8);
}

void Screen_ILI9340::_setPoint(uint16_t x1, uint16_t y1, uint16_t colour)
{
    _setWindow(x1, y1, x1, y1);
    _writeData88(highByte(colour), lowByte(colour));
}

void Screen_ILI9340::_orientCoordinates(uint16_t &x1, uint16_t &y1)
{
    ;
}

void Screen_ILI9340::_setWindow(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    _orientCoordinates(x1, y1);
	_orientCoordinates(x2, y2);
    
    if (x1>x2) _swap(x1, x2);
    if (y1>y2) _swap(y1, y2);
    
    _writeCommand8(ILICASET);
    _writeData16(x1);
    _writeData16(x2);

    _writeCommand8(ILIPASET);
    _writeData16(y1);
    _writeData16(y2);

    _writeCommand8(ILIRAMWR);
}

void Screen_ILI9340::_setWindowRead(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    _orientCoordinates(x1, y1);
	_orientCoordinates(x2, y2);
    
    if (x1>x2) _swap(x1, x2);
    if (y1>y2) _swap(y1, y2);
    
    _writeCommand8(ILICASET);
    _writeData16(x1);
    _writeData16(x2);
    
    _writeCommand8(ILIPASET);
    _writeData16(y1);
    _writeData16(y2);
    
    _writeCommand8(ILIRAMRD);
}

void Screen_ILI9340::_fastFill(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t colour)
{
    if (x1 > x2) _swap(x1, x2);
    if (y1 > y2) _swap(y1, y2);
    
    _setWindow(x1, y1, x2, y2);
    uint8_t hi = highByte(colour);
    uint8_t lo = lowByte(colour);
    
    for (uint32_t t=(uint32_t)(y2-y1+1)*(x2-x1+1); t>0; t--) {
        _writeData88(hi, lo);
    }
}

// Touch
void Screen_ILI9340::_getRawTouch(uint16_t &x0, uint16_t &y0, uint16_t &z0)
{
    ;
}

uint16_t Screen_ILI9340::_readData()
{
	uint16_t data;
//    uint8_t _pinScreenMISO = 15;
//    uint8_t _pinScreenClock = 7;
//    pinMode(_pinScreenMISO, INPUT);
//    pinMode(_pinScreenClock, OUTPUT);
//    
    digitalWrite(_pinScreenDataCommand, HIGH);
    digitalWrite(_pinScreenCS, LOW);
//
//    data = shiftIn(_pinScreenMISO, _pinScreenClock, MSBFIRST);
//	data <<= 8;
//    data = shiftIn(_pinScreenMISO, _pinScreenClock, MSBFIRST);

#ifdef HARDWARE_SPI
//    SPI.setClockDivider(SPI_CLOCK_DIV8);
    
	data   = SPI.transfer(0x00);                      // read D8..D15
	data <<= 8;
	data  |= SPI.transfer(0x00);                      // read D0..D7

//    SPI.setClockDivider(SPI_CLOCK_DIV2);
#else
    //    data = shiftIn(_pinScreenMISO, _pinScreenClock, MSBFIRST);
#endif
    digitalWrite(_pinScreenCS, HIGH);
    
//    Serial.print("1:");
//    Serial.println(data, BIN);
//    // Convert B5G5R5 into R5G6B5
//    // Colour         BBBBBxGGGGGRRRRR                     BBBBBxGGGGGRRRRR                     BBBBBxGGGGGRRRRR
//    data = ((data & 0b1111100000000000) >>11) + ((data & 0b0000001111100000)<<1) + ((data & 0b0000000000011111)<<11);
//    Serial.print("2:");
//    Serial.println(data, BIN);
    
	return data;
}

uint16_t Screen_ILI9340::readPixel(uint16_t x1, uint16_t y1)
{
	_setWindowRead(x1, y1, x1+1, y1+1);
	
	uint16_t w = _readData();   // An empty read
	return _readData();
}

void Screen_ILI9340::_closeWindow()
{
    _setWindow(0, 0, screenSizeX()-1, screenSizeY()-1);
}


