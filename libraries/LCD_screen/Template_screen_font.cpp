//
// Template_screen.cpp
// Library C++ code
// ----------------------------------
// Developed with embedXcode
// http://embedXcode.weebly.com
//
// Project new_Template_screen
//
// Created by Rei VILO, mars 07, 2013 09:21
// embedXcode.weebly.com
//
//
// Copyright © Rei VILO, 2013-2014
// License CC = BY NC SA
//
// See Template_screen.h and ReadMe.txt for references
//


// Library header
#include "Template_screen_font.h"

///
/// @name	Template size
/// @{

#define Template_WIDTH  128
#define Template_HEIGHT 160

/// @}


///
/// @name	Template constants
/// @{

#define Template_CONSTANT  0x00

/// @}

///
/// @name	Template commands
/// @{

#define Template_NOP     0x00

/// @}


// Class
LCD_font_Template::LCD_font_Template() {
#if defined(__MSP430F5529__)
    //LCD_font_Template myScreen(NULL, P1_4, P1_2, NULL, TemplateR_RED_option);
    _pinReset          = NULL; // screen reset
    _pinDataCommand    = NULL; // data or command
    _pinChipSelect     = NULL; // select
    _pinSerialData  = NULL;
    _pinSerialClock = NULL;
    _pinScreenBacklight      = NULL; // screen backlight
#elif defined(ENERGIA)
    //LCD_font_Template myScreen(NULL, P1_4, P1_2, NULL, TemplateR_RED_option); = MSP430G2553
    //LCD_font_Template myScreen(NULL, PE_5, PB_1, NULL, TemplateR_RED_option); = Stellaris LM4F120
    // pins number = MSP430G2553 / Stellaris LM4F
    _pinReset = NULL; // screen reset
    _pinDataCommand = 6; // P1_4 / PE_5; // data or command
    _pinChipSelect  = 4; // P1_2 / PB_1; // select
    _pinSerialData  = 7; // P1_7 / PB_4;
    _pinSerialClock = 15; // P1_5 / PB_7;
    _pinScreenBacklight = NULL; // screen backlight
#elif defined(__AVR_ATmega328P__) || defined(__AVR_ATmega2560__) || defined(__AVR_ATmega644P__)
    _pinReset          = A0; // screen reset
    _pinDataCommand    = 8; // data or command
    _pinChipSelect     = 7; // select
    _pinSerialData  = 13;
    _pinSerialClock = 11;
    _pinScreenBacklight      = 9; // screen backlight
#elif defined(__MK20DX128__)
    _pinReset          = NULL; // screen reset
    _pinDataCommand    = NULL; // data or command
    _pinChipSelect     = NULL; // select
    _pinSerialData  = NULL;
    _pinSerialClock = NULL;
    _pinScreenBacklight      = NULL; // screen backlight
#else
#error Platform not supported
#endif
    
}

LCD_font_Template::LCD_font_Template(uint8_t resetPin, uint8_t dataCommandPin, uint8_t chipSelectPin, uint8_t backlightPin)
{
    _pinReset = resetPin; // screen reset
    _pinDataCommand = dataCommandPin; // data or command
    _pinChipSelect = chipSelectPin; // select
    _pinScreenBacklight = backlightPin; // screen backlight
};


void LCD_font_Template::begin()
{
#if defined(HARDWARE_SPI)
    
// Interface initialisation
#if defined(__LM4F120H5QR__)
    SPI.setModule(2);
#endif
    
    SPI.begin();
    SPI.setClockDivider(SPI_CLOCK_DIV2);
    SPI.setBitOrder(MSBFIRST);
    SPI.setDataMode(SPI_MODE0);
    
#else
//    _pinSerialClock = SPI_CLOCK_PIN;    // Serial Clock
//    _pinSerialData  = SPI_DATA_PIN;    // Serial Data
    
    pinMode(_pinSerialClock, OUTPUT);
    pinMode(_pinSerialData, OUTPUT);
#endif
    
    if (_pinReset!=NULL) pinMode(_pinReset, OUTPUT);
    if (_pinScreenBacklight!=NULL) pinMode(_pinScreenBacklight, OUTPUT);
    pinMode(_pinDataCommand, OUTPUT);
    pinMode(_pinChipSelect, OUTPUT);
    
    if (_pinScreenBacklight!=NULL) digitalWrite(_pinScreenBacklight, HIGH);

    
    // Screen reset
    if (_pinReset!=NULL) digitalWrite(_pinReset, 1);
    delay(100);
    if (_pinReset!=NULL) digitalWrite(_pinReset, 0);
    delay(50);
    if (_pinReset!=NULL) digitalWrite(_pinReset, 1);
    delay(120);
    
    // Screen reset
//    _writeCommand(Template_SWRESET);
    delay(150);
//    _writeCommand(Template_SLPOUT);
    delay(200);
    
    // Screen initialisation
    
    // Standard
    setBacklight(true);
    setOrientation(0);
    
    _screenWidth = Template_WIDTH;
    _screenHeigth = Template_HEIGHT;
    _penSolid = false;
    _fontSolid = false;
    _touchTrim = 0; // no touch
    clear();
}

//String LCD_font_Template::WhoAmI()
//{
//    return "Template screen";
//}
//
//
//void LCD_font_Template::invert(boolean flag)
//{
////    _writeCommand(flag ? Template_INVON : Template_INVOFF);
//}
//
//void LCD_font_Template::setBacklight(boolean flag)
//{
//    if (_pinScreenBacklight!=NULL) digitalWrite(_pinScreenBacklight, flag);
//}
//
//void LCD_font_Template::setDisplay(boolean flag)
//{
//    if (_pinScreenBacklight!=NULL) setBacklight(flag);
//}
//
void LCD_font_Template::_setOrientation(uint8_t orientation)
{
    _orientation = orientation % 4;
    
//    _writeCommand(Template_MADCTL);
//    switch (_orientation) {
//        case 0:
//            _writeData(Template_MADCTL_MX | Template_MADCTL_MY | Template_MADCTL_RGB);
//            break;
//        case 1:
//            _writeData(Template_MADCTL_MY | Template_MADCTL_MV | Template_MADCTL_RGB);
//            break;
//        case 2:
//            _writeData(Template_MADCTL_RGB);
//            break;
//        case 3:
//            _writeData(Template_MADCTL_MX | Template_MADCTL_MV | Template_MADCTL_RGB);
//            break;
//    }
}


 void LCD_font_Template::_fastFill(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t colour)
{
    _setWindow(x1, y1, x2, y2);
    
    for(uint16_t t=(y2-y1+1)*(x2-x1+1); t>0; t--)     _writeData88(highByte(colour), lowByte(colour));
}

//void LCD_font_Template::_setPoint(uint16_t x1, uint16_t y1, uint16_t colour)
//{
//    if( (x1 < 0) || (x1 >= screenSizeX()) || (y1 < 0) || (y1 >= screenSizeY()) ) return;
//    
//    _setWindow(x1, y1, x1+1, y1+1);
//    _writeData16(colour);
//}


// Utilities
//void LCD_font_Template::_setWindow(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1)
//{
//    if (_option == TemplateR_GREEN_option) {
//        switch (_orientation)
//        {
//            case 0:
//            case 2:
//                x0 += 2;
//                y0 += 1;
//                x1 += 2;
//                y1 += 1;
//                break;
//            case 1:
//            case 3:
//                x0 += 1;
//                y0 += 2;
//                x1 += 1;
//                y1 += 2;
//                break;
//            default:
//                break;
//        }
//    }
//    
//    _writeRegister(0x2a, 0, x0, 0, x1, -1);
//    _writeRegister(0x2b, 0, y0, 0, y1, -1);
//    
//    _writeCommand(0x2C);
//}


void LCD_font_Template::_getRawTouch(uint16_t &x0, uint16_t &y0, uint16_t &z0)
{
    x0 = 0;
    y0 = 0;
    z0 = 0;
}

void LCD_font_Template::_setBacklight(bool state)
{
    ;
}

void LCD_font_Template::_setIntensity(uint8_t intensity)
{
    ;
}



