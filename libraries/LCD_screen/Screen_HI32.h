///
/// @file		screen_HI32.h
/// @brief		Library header
/// @details	ILI9225 screen library
/// @n
/// @n @b		Project LCD_screen Library Suite
/// @n @a		Developed with [embedXcode](http://embedXcode.weebly.com)
///
/// @author		Rei VILO
/// @author		embedXcode.weebly.com
/// @date		Dec 10, 2013
/// @version	102
///
/// @copyright 	(c) Rei VILO, 2010-2015
/// @copyright	All rights reserved
/// @n          http://embeddedcomputing.weebly.com/lcd_screen-library-suite
///
/// @n  Unique license: Attribution-NonCommercial-ShareAlike 3.0 Unported (CC BY-NC-SA 3.0)
/// *   For hobbyists and for personal usage
/// *   For professionals or organisations or for commercial usage
///
///
/// @see
/// *		3.2" inch 480x320 TFT LCD module with font
/// @n		http://www.ebay.com/itm/3-2-inch-480x320-TFT-LCD-module-w-Font-IC-ILI9481-arduino-DUE-MEGA-2-8-3-5-/121097673674
/// *       Shield for Arduino Mega2560
/// @n      http://www.ebay.com/itm/TFT-SD-Shield-for-Arduino-MEGA-2560-LCD-Module-SD-level-translation-2-8-3-2-DUE-/121057451964
/// *       Shield for Arduino Due
/// @n      http://www.ebay.com/itm/TFT-SD-Shield-for-Arduino-DUE-TFT-LCD-Module-SD-Card-Adapter-2-8-3-2-inch-Mega-/121057447668
/// *		Use of portions of code from Coldtears library
/// @n      http://code.google.com/p/cte-lcd-modules-arduino-library/downloads/list
/// *		Coldtears library is based on Henning Karlsen's UTFT library
/// @n      http://www.henningkarlsen.com/electronics/
/// * 		Fonts generated with MikroElektronika GLCD Font Creator 1.2.0.0
/// @n		http://www.mikroe.com
/// *		LCD_screen Library Suite
/// @n		http://embeddedcomputing.weebly.com/lcd_screen-library-suite.html
/// *		Serial_LCD Library Suite
/// @n		http://embeddedcomputing.weebly.com/serial-lcd.html
///


// Core library - IDE-based
#if defined(MPIDE) // chipKIT specific
#include "WProgram.h"
#elif defined(DIGISPARK) // Digispark specific
#include "Arduino.h"
#elif defined(ENERGIA) // LaunchPad, FraunchPad and StellarPad specific
#include "Energia.h"
#elif defined(MAPLE_IDE) // Maple specific
#include "WProgram.h"
#elif defined(CORE_TEENSY) // Teensy specific
#include "WProgram.h"
#elif defined(WIRING) // Wiring specific
#include "Wiring.h"
#elif defined(ARDUINO) // Arduino 1.0x and 1.5x specific
#include "Arduino.h"
#endif // end IDE

#ifndef SCREEN_HI32_RELEASE
///
/// @brief	Library release number
///
#define SCREEN_HI32_RELEASE 102

#include "LCD_screen_font.h"

#if LCD_SCREEN_FONT_RELEASE < 117
#error Required LCD_SCREEN_FONT_RELEASE 117
#endif

///
/// @brief  AVR defines
/// @{
#define cbi(reg, bitmask) *reg &= ~bitmask
#define sbi(reg, bitmask) *reg |= bitmask
#define pulse_high(reg, bitmask) sbi(reg, bitmask); cbi(reg, bitmask);
#define pulse_low(reg, bitmask) cbi(reg, bitmask); sbi(reg, bitmask);

#define cport(port, data) port &= data
#define sport(port, data) port |= data

#define swap(type, i, j) {type t = i; i = j; j = t;}

#define fontbyte(x) pgm_read_byte(&cfont.font[x])

#define regtype volatile uint8_t
#define regsize uint8_t
#define bitmapdatatype unsigned int*
/// @}

///
/// @brief  Class for 3.2" 480x320 screen
/// @details    Screen controllers
/// * LCD: ILI9481, 16-bit parallel
/// * touch: XPT2046, SPI
/// @warning    Support for Arduino Mega2560 and Arduino Due only
///
class Screen_HI32 : public LCD_screen_font {
public:
    
    ///
    /// @brief	Constructor with default pins
    ///
    /// @note	Default pins for Arduino Mega2560 and Due
    ///
    Screen_HI32();
    
    ///
    /// @brief      Initialise
    /// @details	Open connections
    ///
    void begin();
    
    ///
    /// @brief	Request information about the screen
    /// @return	string with hardware version
    ///
    String WhoAmI();
    
private:
	// * Virtual =0 compulsory functions
    // Orientation
    void _setOrientation(uint8_t orientation) ; // compulsory
    void _orientCoordinates(uint16_t &x1, uint16_t &y1) ; // compulsory
    
	// Position
    void _setWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) ; // compulsory
    void _closeWindow(); // compulsory
	void _setPoint(uint16_t x1, uint16_t y1, uint16_t colour) ; // compulsory
    void _fastFill(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t colour) ; // compulsory
    
    // Write and Read
    void _writeData88(uint8_t dataHigh8, uint8_t dataLow8) ; // compulsory;
    
	// Touch
    void _getRawTouch(uint16_t &x0, uint16_t &y0, uint16_t &z0) ; // compulsory

    // * Other functions specific to the screen
    // Communication, write
    void _writeRegister16(uint8_t command8, uint16_t data16);

    void _writeAddress16(uint16_t data16);
    void _writeCommand16(uint16_t command16);
    void _writeData16(uint16_t data16);
    void _writeColour88(uint8_t colourHigh8, uint8_t colourLow8);
    void _writeColour16(uint16_t data16);
    
    void _writeParallelLCD88(uint8_t dataHigh8, uint8_t dataLow8);
    void _readParallelLCD88(uint8_t &dataHigh8, uint8_t &dataLow8);

    // Touch
    void _getOneTouch(uint8_t command8, uint8_t &a, uint8_t &b);
    
    // AVR registers
    regtype *_portRS, *_portWR, *_portCS, *_portRST, *_portSDA, *_portSCL, *_portF_CS;
    regsize _bitRS, _bitWR, _bitCS, _bitRST, _bitSDA, _bitSCL, _bitF_CS;
    
    // Energy
    void _setIntensity(uint8_t intensity); // compulsory
    void _setBacklight(bool flag); // compulsory
    
    // Variables
    uint8_t _pinReset;
    uint8_t _pinTouchMOSI;
    uint8_t _pinTouchMISO;
    uint8_t _pinScreenBacklight;
//    uint8_t _pinScreenChipSelect;
    uint8_t _pinTouchCS;
    uint8_t _pinScreenDataCommand;
    uint8_t _pinTouchClock;
    
    uint8_t _pinScreenRS;
    uint8_t _pinScreenWR;
    uint8_t _pinScreenCS;
    uint8_t _pinScreenReset;
};

#endif
