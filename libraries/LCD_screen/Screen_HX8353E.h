///
/// @file		Screen_HX8353E.h
/// @brief		Library header
/// @details	HX8353E screen library
/// @n
/// @n @b		Project LCD_screen Library Suite
/// @n @a		Developed with [embedXcode](http://embedXcode.weebly.com)
///
/// @author		Rei VILO
/// @author		embedXcode.weebly.com
/// @date		Jan 04, 2014
/// @version	103
///
/// @copyright	© Rei VILO, 2013-2014
/// @copyright	All rights reserved
///
/// @n  Dual license:
/// *   For hobbyists and for personal usage: Attribution-NonCommercial-ShareAlike 3.0 Unported (CC BY-NC-SA 3.0)
/// *   For professionals or organisations or for commercial usage: All rights reserved
///
/// @n  For any enquiry about license, http://embeddedcomputing.weebly.com/contact
///
///
/// @see
/// * 		Fonts generated with MikroElektronika GLCD Font Creator 1.2.0.0
/// @n		http://www.mikroe.com
/// *		LCD_screen Library Suite
/// @n		http://embeddedcomputing.weebly.com/lcd_screen-library-suite.html
/// *		Serial_LCD Library Suite
/// @n		http://embeddedcomputing.weebly.com/serial-lcd.html
///


// Core library - IDE-based
#if defined(ENERGIA) // LaunchPad, FraunchPad and StellarPad specific
#include "Energia.h"
#else
//#error Board not supported
#endif // end IDE

#ifndef SCREEN_HX8353E_RELEASE
///
/// @brief	Library release number
///
#define SCREEN_HX8353E_RELEASE 103


// Other libraries
#include "LCD_screen_font.h"

#if LCD_SCREEN_FONT_RELEASE < 117
#error Required LCD_SCREEN_FONT_RELEASE 117
#endif

/////
///// @brief    Hardware SPI selector
///// @details  Comment the line to use software SPI, slower
///// @n		Uncomment the line to use hardware SPI, faster
/////
//#define HARDWARE_SPI
//
//#if defined(HARDWARE_SPI)
#include "SPI.h"
//#endif


// Objects
//
///
/// @brief      Class for Educational BoosterPack MKII
/// @details    Screen controller
/// * LCD: HX8353E, 4-wire 8-bit SPI with R/S line
/// * touch: no touch
///
class Screen_HX8353E : public LCD_screen_font {
public:
    ///
    /// @brief	Constructor with default pins
    ///
    /// @note	Default pins for LaunchPad MSP430F5529 / LaunchPad Stellaris LM4F
    /// @n		17 / NULL / NULL = LCD Reset
    /// @n		31 / P?_? / P?_? = LCD Data/Command
    /// @n		13 / P?_? / P?_? = LCD Chip Select
    /// @n		39 / P?_? / P?_? = LCD PWM Backlight
    ///
    Screen_HX8353E();
    
    ///
    /// @brief	Constructor
    ///
    /// @param	resetPin digital pin number for screen reset
    /// @param	dataCommandPin digital pin number for command / data
    /// @param	chipSelectPin digital pin number for SPI chip select
    /// @param	backlightPin PWM pin number for backlight
    ///
    Screen_HX8353E(uint8_t resetPin, uint8_t dataCommandPin, uint8_t chipSelectPin, uint8_t backlightPin);
    
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
    void _writeRegister(uint8_t command8, uint8_t data8);
    void _writeCommand(uint8_t command8);
    void _writeData(uint8_t data8);
    void _writeData16(uint16_t data16);
    void _writeData8888(uint8_t dataHigh8, uint8_t dataLow8, uint8_t data8_3, uint8_t data8_4);

    // Energy
    void _setIntensity(uint8_t intensity); // compulsory
    void _setBacklight(bool flag); // compulsory

    // Variables
    uint8_t _pinScreenReset;                                                          // screen reset
    uint8_t _pinScreenDataCommand;                                                    // data or command
    uint8_t _pinScreenCS;                                                     // select
    uint8_t _pinScreenBacklight;                                                      // screen backlight
    //    uint8_t _pinSerialData;
    //    uint8_t _pinSerialClock;
};

#endif
