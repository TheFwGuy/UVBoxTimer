///
/// @file		Template_screen_font.h
/// @brief		Library header
/// @n	
/// @n @b		Project LCD_screen Library Suite
/// @n @a		Developed with [embedXcode](http://embedXcode.weebly.com)
/// 
/// @author		Rei VILO
/// @author		embedXcode.weebly.com
/// @date		mars 07, 2013 09:21
/// @version	101
/// 
/// @copyright	© Rei VILO, 2013-2014
/// @copyright	All rights reserved
/// @n          http://embeddedcomputing.weebly.com/lcd_screen-library-suite
///
/// @n  Dual license:
/// *   For hobbyists and for personal usage: Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0)
/// *   For professionals or organisations or for commercial usage: All rights reserved
///
/// @n  For any enquiry about license, http://embeddedcomputing.weebly.com/contact
///
///
/// @see	ReadMe.txt for references
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

#ifndef SCREEN_TEMPLATE_RELEASE
///
/// @brief	Library release number
///
#define SCREEN_TEMPLATE_RELEASE 100

// Other libraries
//#include "stdarg.h"
#include "LCD_screen_font.h"

#if LCD_SCREEN_FONT_RELEASE < 117
#error Required LCD_SCREEN_FONT_RELEASE 117
#endif

///
/// @brief		Hardware SPI selector
/// @details    Comment the line to use software SPI, slower
/// @n			Uncomment the line to use hardware SPI, faster
///
#define HARDWARE_SPI

#if defined(HARDWARE_SPI)
#include "SPI.h"
#endif


// Objects
//
///
/// @class	LCD_font_Template
/// @brief	Main and core class
///
class LCD_font_Template : public LCD_screen_font {
public:
    ///
    /// @brief	Constructor with default pins
    ///
    LCD_font_Template();
 
    ///
    /// @brief	Constructor
    ///
    /// @param	resetPin digital pin number for screen reset
    /// @param	dataCommandPin digital pin number for command / data
    /// @param	chipSelectPin digital pin number for SPI chip select
    /// @param	backlightPin digital pin number for backlight
    ///
    LCD_font_Template(uint8_t resetPin, uint8_t dataCommandPin, uint8_t chipSelectPin, uint8_t backlightPin);
    
    ///
    /// @brief	Initialisation
    ///
    void begin();
    
    ///
    /// @brief	Request information about the screen
    /// @return	string with hardware version
    ///
    String WhoAmI();
    
    ///
    /// @brief	Invert screen
    /// @param	flag true to invert, false for normal screen
    ///
    void invert(boolean flag);
    
//    ///
//    /// @brief	Switch backlight on or off
//    /// @param	flag true=on, false=off
//    ///
//    void setBacklight(boolean flag);
    
//    ///
//    /// @brief	Switch display on or off
//    /// @param	flag true=on, false=off
//    ///
//    void setBacklight(boolean flag);
    
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
    void _setIntensity(uint8_t intensity); // compulsory
    void _setBacklight(bool flag); // compulsory

    // Variables
    uint8_t _pinReset; // screen reset
    uint8_t _pinDataCommand; // data or command
    uint8_t _pinChipSelect; // select
    uint8_t _pinScreenBacklight; // screen backlight
    uint8_t _pinSerialData;
    uint8_t _pinSerialClock;
};

#endif
