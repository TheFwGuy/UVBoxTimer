//
// Screen_ILI9225B.cpp
// Library C++ code
// ----------------------------------
// Developed with embedXcode
// http://embedXcode.weebly.com
//
// Project LCD_screen Library Suite
//
// Created by Rei VILO, mars 07, 2013 11:13
// embedXcode.weebly.com
//
//
// Copyright © Rei VILO, 2013-2014
// License All rights reserved
//
// See TOUCH22_screen.h and ReadMe.txt for references
//


// Library header
#include "Screen_ILI9225B.h"

///
/// @name	ILI9225B constants
///
/// @{

#define ILIGRAMMODE     0x03
#define ILIGRAMADDRX	0x20
#define ILIGRAMADDRY 	0x21
#define ILIWRDATA   	0x22
#define ILIGRAMHEA		0x36
#define ILIGRAMHSA		0x37
#define ILIGRAMVEA		0x38
#define ILIGRAMVSA		0x39
#define LCD_HEIGHT      220  // Vertical
#define LCD_WIDTH       176  // Horizontal
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



const uint8_t  _commandLCD = 0x00;
const uint8_t  _dataLCD    = 0x01;
//const uint8_t  _dataRepeatLCD = 0x02;

//uint8_t _pinPushButton;

//#define LCD_SCLK_PIN BIT5
//#define LCD_SD_PIN BIT7
//#define LCD_SCE_PIN BIT2
//#define LCD_DC_PIN BIT4
//#define LCD_SELECT P1OUT &= ~LCD_SCE_PIN
//#define LCD_DESELECT P1OUT |= LCD_SCE_PIN
//#define LCD_CLOCK P1OUT |= LCD_SCLK_PIN; P1OUT &= ~LCD_SCLK_PIN


// Code
#if defined(ENERGIA) // LaunchPads specific
Screen_ILI9225B::Screen_ILI9225B(uint8_t version)
{
                                    // Pin number / LaunchPad MSP430 / LM4F120 TM4C123
    _pinScreenCS            = 2;    // 2  / P1_0 / PB_5;
    _pinTouchClock          = 7;    // 7  / P1_5 / PB_4;
    _pinTouchMOSI           = 15;   // 15 / P1_7 / PB_7;
    _pinTouchMISO           = 14;   // 14 / P1_6 / PB_6;
    _pinScreenDataCommand   = 6;    // 6  / P1_4 / PE_5;
    if (version == 1) {
        _pinTouchCS         = 9;    //  9 / P2_1 / PA_6;
    } else {
        _pinTouchCS         = 10;   // 10 / P2_2 / PA_7;
    }
    _pinReset               = 11;   // 11 / P2_3 / PA_2;
}

#elif defined(__AVR_ATmega328P__) || defined(__AVR_ATmega2560__) || defined(__AVR_ATmega644P__)
Screen_ILI9225B::Screen_ILI9225B(uint8_t version)
{
    uint8_t _pinScreenCS    = 2;
    uint8_t _pinReset       = 3;
    uint8_t _pinTouchCS     = 4;
    uint8_t _pinTouchClock  = 13;
    uint8_t _pinTouchMOSI   = 11;
    uint8_t _pinTouchMISO   = 12;
}

#elif defined(__MK20DX128__)
Screen_ILI9225B::Screen_ILI9225B(uint8_t version)
{
    uint8_t _pinScreenCS    = 2;
    uint8_t _pinReset       = 3;
    uint8_t _pinTouchCS     = 4;
    uint8_t _pinTouchClock  = 13;
    uint8_t _pinTouchMOSI   = 11;
    uint8_t _pinTouchMISO   = 12;
}

#else
#error Platform not supported
#endif


Screen_ILI9225B::Screen_ILI9225B(uint8_t pinScreenChipSelect,
                                 uint8_t pinSerialClock,
                                 uint8_t pinSerialDataMOSI,
                                 uint8_t pinSerialDataMISO,
                                 uint8_t pinScreenDataCommand,
                                 uint8_t pinTouchChipSelect,
                                 uint8_t pinScreenReset)
{
    _pinScreenCS = pinScreenChipSelect;
    _pinTouchClock      = pinSerialClock;
    _pinTouchMOSI   = pinSerialDataMOSI;
    _pinTouchMISO   = pinSerialDataMISO;
    _pinScreenDataCommand      = pinScreenDataCommand;
    _pinTouchCS  = pinTouchChipSelect;
    _pinReset            = pinScreenReset;
}

void Screen_ILI9225B::begin()
{
    
#if defined(__MSP430G2553__) // LaunchPad MSP430G2553 specific
    pinMode((_pinTouchCS==P2_1) ? P2_2 : P2_1, OUTPUT);
    digitalWrite((_pinTouchCS==P2_1) ? P2_2 : P2_1, HIGH);
    
#elif defined(__MSP430F5529__) // LaunchPad MSP430G2553 specific
    pinMode((_pinTouchCS==P4_2) ? P4_1 : P4_2, OUTPUT);
    digitalWrite((_pinTouchCS==P4_2) ? P4_1 : P4_2, HIGH);
    
#elif defined(__LM4F120H5QR__)  || defined(__TM4C123GH6PM__) // StellarPad LM4F specific
    pinMode((_pinTouchCS==PA_6) ? PA_7 : PA_6, OUTPUT);
    digitalWrite((_pinTouchCS==PA_6) ? PA_7 : PA_6, HIGH);
    
#endif
    
    pinMode(_pinScreenCS, OUTPUT);
    pinMode(_pinTouchCS, OUTPUT);
    pinMode(_pinScreenDataCommand, OUTPUT);
    
#ifdef HARDWARE_SPI
    
#if defined(__MSP430G2553__) || defined(__MSP430F5529__)
    SPI.begin();
    SPI.setClockDivider(SPI_CLOCK_DIV2);
    SPI.setBitOrder(MSBFIRST);
    SPI.setDataMode(SPI_MODE0);
    
#elif defined(__LM4F120H5QR__)  || defined(__TM4C123GH6PM__)
    SPI.setModule(2);
    SPI.begin();
    // for Touch on LM4F120H5QR DIV4 = 4 MHz
    // for Screen on LM4F120H5QR DIV2 = 8 MHz
    SPI.setClockDivider(SPI_CLOCK_DIV2);
    
#elif defined(__TM4C129XNCZAD__)
    SPI.setModule(2);
    SPI.begin();
    // for Touch on LM4F120H5QR DIV4 = 4 MHz
    // for Screen on LM4F120H5QR DIV2 = 8 MHz
    SPI.setClockDivider(SPI_CLOCK_DIV2);

#endif
    
#else
    pinMode(_pinTouchMOSI, OUTPUT);
    pinMode(_pinTouchMISO, INPUT);
    pinMode(_pinTouchClock, OUTPUT);
    
#endif
    
    delay(1);
    
    if (_pinReset != NULL) {
        pinMode(_pinReset, OUTPUT);
        digitalWrite(_pinReset, LOW);
        delay(100);
        digitalWrite(_pinReset, HIGH);
    }
    
	_writeCommand(0x28);
	delay(20);
	_writeRegister(0x01, 0x011c); // set SS and NL bit
	_writeRegister(0x02, 0x0100); // set 1 line inversion
	_writeRegister(0x03, 0x1030); // set GRAM write direction and BGR=1.//1030
	_writeRegister(0x08, 0x0808); // set BP and FP
	_writeRegister(0x0C, 0x0000); // RGB interface setting 0x0110 for RGB 18Bit and 0111for RGB16Bit
	_writeRegister(0x0F, 0x0b01); // Set frame rate//0b01
	delay(5);
	_writeRegister(0x10, 0x0a00); // Set SAP,DSTB,STB//0800
	_writeRegister(0x11, 0x1038); // Set APON,PON,AON,VCI1EN,VC
	delay(5);
	_writeRegister(0x12, 0x1121); // Internal reference voltage= Vci;
	_writeRegister(0x13, 0x0063); // Set GVDD
	_writeRegister(0x14, 0x4b44); // Set VCOMH/VCOML voltage//3944
	_writeRegister(ILIGRAMADDRX, 0x0000);
	_writeRegister(ILIGRAMADDRY, 0x0000);
    
    //	setGRAMILI9225B();
    _writeRegister(0x30, 0x0000);
    _writeRegister(0x31, 0x00db);
    _writeRegister(0x32, 0x0000);
    _writeRegister(0x33, 0x0000);
    _writeRegister(0x34, 0x00db);
    _writeRegister(0x35, 0x0000);
    _writeRegister(0x36, 0x00af);
    _writeRegister(0x37, 0x0000);
    _writeRegister(0x38, 0x00db);
    _writeRegister(0x39, 0x0000);
    
    //	gammaAdjustmentILI9225B();
    //	 _writeRegister(0x50, 0x0003);
    //   _writeRegister(0x51, 0x0900);
    //   _writeRegister(0x52, 0x0d05);
    //   _writeRegister(0x53, 0x0900);
    //   _writeRegister(0x54, 0x0407);
    //   _writeRegister(0x55, 0x0502);
    //   _writeRegister(0x56, 0x0000);
    //   _writeRegister(0x57, 0x0005);
    //   _writeRegister(0x58, 0x1700);
    //   _writeRegister(0x59, 0x001f);
    
	delay(5);
	_writeRegister(0x07, 0x1017);
    
    _penSolid = false;
    _flagRead = false;
    
    _screenWidth = LCD_WIDTH;
    _screenHeigth = LCD_HEIGHT;
    _screenDiagonal = 22;
    
    _touchTrim = TOUCH_TRIM;
    _touchXmin = 311;
    _touchXmax = 1632;
    _touchYmin = 195;
    _touchYmax = 1859;
    
    setOrientation(0);
    clear();
}


String Screen_ILI9225B::WhoAmI()
{
    return "LCD 2.2 Touch BoosterPack";
}

void Screen_ILI9225B::_setBacklight(bool state)
{
    _writeCommand((state) ? 0x11: 0x10);
}

void Screen_ILI9225B::_setIntensity(uint8_t intensity)
{
    ;
}


void Screen_ILI9225B::_setOrientation(uint8_t orientation)
{
    //_orientation = orientation % 4;
//    LCD_screen_font::setOrientation(orientation);
    
    switch (_orientation) {
        case 0:
            //                                       hvO
            _writeRegister(ILIGRAMMODE, 0x1000 + 0b00110000);
            break;
        case 1:
            //                                       hvO
            _writeRegister(ILIGRAMMODE, 0x1000 + 0b00101000);
            break;
        case 2:
            //                                       hvO
            _writeRegister(ILIGRAMMODE, 0x1000 + 0b00000000);
            break;
        case 3:
            //                                       hvO
            _writeRegister(ILIGRAMMODE, 0x1000 + 0b00011000);
            break;
    }
}

// Utilities
void Screen_ILI9225B::_writeCommand(uint8_t command8)
{
    digitalWrite(_pinScreenDataCommand, LOW);
    digitalWrite(_pinScreenCS, LOW);
    
#ifdef HARDWARE_SPI
    SPI.transfer(command8);
#else
    shiftOut(_pinTouchMOSI, _pinTouchClock, MSBFIRST, command8);
#endif
    
    digitalWrite(_pinScreenCS, HIGH);
}

//void Screen_ILI9225B::_write(bool dataCommand, uint8_t value8)
//{
//    digitalWrite(_pinScreenDataCommand, dataCommand);
//    digitalWrite(_pinScreenCS, LOW);
//    
//#ifdef HARDWARE_SPI
//    SPI.transfer(value8);
//#else
//    shiftOut(_pinTouchMOSI, _pinTouchClock, MSBFIRST, value8);
//#endif
//    
//    digitalWrite(_pinScreenCS, HIGH);
//}

void Screen_ILI9225B::_writeData16(uint16_t data16)
{
    digitalWrite(_pinScreenDataCommand, HIGH);
    digitalWrite(_pinScreenCS, LOW);
    
#ifdef HARDWARE_SPI
    SPI.transfer(highByte(data16)); // write D8..D15
    SPI.transfer(lowByte(data16)); // write D0..D7
#else
    shiftOut(_pinTouchMOSI, _pinTouchClock, MSBFIRST, highByte(data16));
    shiftOut(_pinTouchMOSI, _pinTouchClock, MSBFIRST, lowByte(data16));
#endif
    
    digitalWrite(_pinScreenCS, HIGH);
}

void Screen_ILI9225B::_writeData88(uint8_t dataHigh8, uint8_t dataLow8)
{
    digitalWrite(_pinScreenDataCommand, HIGH);
    digitalWrite(_pinScreenCS, LOW);
    
#ifdef HARDWARE_SPI
    SPI.transfer(dataHigh8); // write D8..D15
    SPI.transfer(dataLow8); // write D0..D7
#else
    shiftOut(_pinTouchMOSI, _pinTouchClock, MSBFIRST, dataHigh8);
    shiftOut(_pinTouchMOSI, _pinTouchClock, MSBFIRST, dataLow8);
#endif
    
    digitalWrite(_pinScreenCS, HIGH);
}


void Screen_ILI9225B::_writeRegister(uint8_t command8, uint16_t data16)
{
    _writeCommand(command8);
	_writeData88(highByte(data16), lowByte(data16));
}

void Screen_ILI9225B::_setPoint(uint16_t x1, uint16_t y1, uint16_t colour)
{
    _setWindow(x1, y1, x1, y1);
    _writeData88(highByte(colour), lowByte(colour));
}

void Screen_ILI9225B::_orientCoordinates(uint16_t &x1, uint16_t &y1)
{
	switch (_orientation) {
        case 0:  // ok
            break;
        case 1: // ok
            y1 = screenSizeY() - y1 -1;
            _swap(x1, y1);
            break;
        case 2: // ok
            x1 = screenSizeX() - x1 -1;
            y1 = screenSizeY() - y1 -1;
            break;
        case 3: // ok
            x1 = screenSizeX() - x1 -1;
            _swap(x1, y1);
            break;
	}
}

void Screen_ILI9225B::_setWindow(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    _orientCoordinates(x1, y1);
    _orientCoordinates(x2, y2);
    
    // unsorted coordinates required for initial position
	_writeRegister(ILIGRAMADDRX, (uint8_t)x1 + LCD_OFFSET_WIDTH);
	_writeRegister(ILIGRAMADDRY, (uint8_t)y1 + LCD_OFFSET_HEIGHT);
    
    if (x1 > x2) _swap(x1, x2);
    if (y1 > y2) _swap(y1, y2);
    
    _writeRegister(ILIGRAMHEA, (uint8_t)x2 + LCD_OFFSET_WIDTH);
	_writeRegister(ILIGRAMHSA, (uint8_t)x1 + LCD_OFFSET_WIDTH);
    
	_writeRegister(ILIGRAMVEA, (uint8_t)y2 + LCD_OFFSET_HEIGHT);
	_writeRegister(ILIGRAMVSA, (uint8_t)y1 + LCD_OFFSET_HEIGHT);
    
	_writeCommand(ILIWRDATA);
}

void Screen_ILI9225B::_fastFill(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t colour)
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

void Screen_ILI9225B::_closeWindow()
{
    _setWindow(0, 0, screenSizeX()-1, screenSizeY()-1);
}

// Touch
void Screen_ILI9225B::_getOneTouch(uint8_t command8, uint8_t &a, uint8_t &b)
{
    // for Touch on LM4F120H5QR DIV4 = 4 MHz
#if defined(HARDWARE_SPI)
#if (F_CPU > 16000000ul)
    SPI.setClockDivider(SPI_CLOCK_DIV8);
#elif defined(__MSP430F5529__)
    SPI.setClockDivider(SPI_CLOCK_DIV4);
#endif
#endif
    
    digitalWrite(_pinTouchCS, LOW);
    
#ifdef HARDWARE_SPI
    SPI.transfer(command8);
    a = SPI.transfer(0x00);
    b = SPI.transfer(0x00);
#else
    shiftOut(_pinTouchMOSI, _pinTouchClock, MSBFIRST, command8);
    a = shiftIn(_pinTouchMISO, _pinTouchClock, MSBFIRST);
    b = shiftIn(_pinTouchMISO, _pinTouchClock, MSBFIRST);
#endif
    
    digitalWrite(_pinTouchCS, HIGH);
    
    // for Screen on LM4F120H5QR DIV2 = 8 MHz
#if defined(HARDWARE_SPI) && (F_CPU > 16000000ul) // && defined(__LM4F120H5QR__)
    SPI.setClockDivider(SPI_CLOCK_DIV2);
#endif
}

void Screen_ILI9225B::_getRawTouch(uint16_t &x0, uint16_t &y0, uint16_t &z0)
{
    uint16_t z1, z2;
    uint8_t a, aa, b, c, cc, d;
    
    // Need for cleaning screen SPI communication for LM4F
#if (defined(__LM4F120H5QR__) || defined(__TM4C123GH6PM__) || defined(__TM4C129XNCZAD__)) && defined(HARDWARE_SPI)
    _setPoint(0, 0, 0);
#endif
    
    delay(10);
    _getOneTouch(TOUCH_Z1 + 0x08 + 0x01, a, b); // 8-bit
    _getOneTouch(TOUCH_Z2 + 0x08 + 0x01, c, d);
    
    z0 = a + (0x7f - c);
    
    if (z0 > _touchTrim) {
        do {
            _getOneTouch(TOUCH_X + 0x00 + 0x01, a, b);  // 12-bit
            _getOneTouch(TOUCH_Y + 0x00 + 0x01, c, d);
            delay(10);
            _getOneTouch(TOUCH_X + 0x00 + 0x01, aa, b); // 12-bit
            _getOneTouch(TOUCH_Y + 0x00 + 0x01, cc, d);
        } while ( (a != aa) || (c != cc) );
        
        x0 = a << 4 | b >> 4;
        y0 = c << 4 | d >> 4;
    }
}