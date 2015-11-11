///
/// @file		screen_ST7735.h
/// @brief		Library header
/// @details	ST7735 screen library
/// @n	
/// @n @b		Project LCD_screen Library Suite
/// @n @a		Developed with [embedXcode](http://embedXcode.weebly.com)
/// 
/// @author		Rei VILO
/// @author		embedXcode.weebly.com
/// @date		mars 07, 2013 09:21
/// @version	105
/// 
/// @copyright	© Rei VILO, 2013-2014
/// @copyright	Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0)
///
/// @n  Unique license: Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0)
/// *   For hobbyists and for personal usage
/// *   For professionals or organisations or for commercial usage
///
/// @see
/// *		(Universal) Color LCD graphics library by RobG 22 March 2012 - 03:09 AM
/// @n		http://forum.43oh.com/topic/1956-universal-color-lcd-graphics-library/
/// *		Color LCD Booster Pack by RobG 02 February 2012 - 10:01 PM
///	@n		http://forum.43oh.com/topic/1758-color-lcd-booster-pack/
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

#ifndef SCREEN_ST7735_RELEASE
///
/// @brief	Library release number
///
#define SCREEN_ST7735_RELEASE 105


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

///
/// @name	ST7735 types
/// @{

#define ST7735R_RED_TAB   0x00 ///< ST7735R screen with red tab
#define ST7735R_GREEN_TAB 0x01 ///< ST7735R screen with green tab
#define ST7735B			  0x02 ///< ST7735B screen

/// @}


// Objects
//
///
/// @brief	Class for RobG's Color LCD Booster Pack
/// @details    Screen controllers
/// * LCD: ST7735, SPI
/// * touch: no touch
///
class Screen_ST7735 : public LCD_screen_font {
public:
    ///
    /// @brief	Constructor with default pins
    /// @param  tab tab colour
    /// @n		LCD Tab Colour
    /// @n	*	ST7735R_RED_TAB = red = default
    /// @n	*	ST7735R_GREEN_TAB = green
    /// @n  *   Other colours may exist but are undocumented.
    ///
    /// @note	Default pins for Arduino Uno / LaunchPad MSP430 / StellarPad LM4F
    /// @n		A0 / NULL / NULL = LCD Reset
    /// @n		 8 / P1_4 / PE_5 = LCD Data/Command
    /// @n		 7 / P1_2 / PB_1 = LCD Chip Select
    /// @n		 9 / PB_4 = LCD Backlight
    ///
    Screen_ST7735(uint8_t tab = ST7735R_RED_TAB);
 
    ///
    /// @brief	Constructor
    ///
    /// @param	resetPin digital pin number for screen reset
    /// @param	dataCommandPin digital pin number for command / data
    /// @param	chipSelectPin digital pin number for SPI chip select
    /// @param	backlightPin digital pin number for backlight
    /// @param  tab colour, RED_TAB or GREEN_TAB
    ///
    Screen_ST7735(uint8_t resetPin, uint8_t dataCommandPin, uint8_t chipSelectPin, uint8_t backlightPin, uint8_t tab);
    
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
//    
//    ///
//    /// @brief	Switch display on or off
//    /// @param	flag true=on, false=off
//    ///
//    void setBacklight(boolean flag);
//    
//    ///
//    /// @brief	Set orientation
//    /// @param	orientation orientation, 0=portrait, 1=right rotated landscape, 2=reverse portrait, 3=left rotated landscape
//    ///
//    void setOrientation(uint8_t orientation);

private:
    uint8_t _tab;

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
    
    // Energy
    void _setIntensity(uint8_t intensity); // compulsory
    void _setBacklight(bool flag); // compulsory
    
	// Touch
    void _getRawTouch(uint16_t &x0, uint16_t &y0, uint16_t &z0) ; // compulsory
    
    // * Other functions specific to the screen
    // Communication, write
    void _writeRegister(uint8_t command8, uint8_t data8);
    void _writeCommand(uint8_t command8);
    void _writeData(uint8_t data8);
    /// @bug raw read requires fix << 1
    void _writeData16(uint16_t data16);
    void _writeData8888(uint8_t dataHigh8, uint8_t dataLow8, uint8_t data8_3, uint8_t data8_4);
    
    // Variables
    uint8_t _pinScreenReset; // screen reset
    uint8_t _pinScreenDataCommand; // data or command
    uint8_t _pinScreenCS; // select
    uint8_t _pinScreenBacklight; // screen backlight
    uint8_t _pinScreenMOSI;
    uint8_t _pinScreenClock;
};

#endif
