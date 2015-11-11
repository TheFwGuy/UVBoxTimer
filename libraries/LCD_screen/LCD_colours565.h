///
/// @file		LCD_colours565.h
/// @brief		Class library header
/// @details	Library for 5-6-5 coded colours
/// @n
/// @n @b		Project LCD_screen Library Suite
/// @n @a		Developed with [embedXcode+](http://embedXcode.weebly.com)
///
/// @author		Rei Vilo
/// @author		http://embeddedcomputing.weebly.com
///
/// @date		May 15, 2014
/// @version	103
///
/// @copyright	(c) Rei Vilo, 2013-2014
/// @copyright	All rights reserved
///
/// @see		ReadMe.txt for references
///


// Core library - IDE-based
#if defined(MPIDE) // chipKIT specific
#include "WProgram.h"
#elif defined(DIGISPARK) // Digispark specific
#include "Arduino.h"
#elif defined(ENERGIA) // LaunchPad MSP430, Stellaris and Tiva, Experimeter Board FR5739 specific
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

#ifndef LCD_COLOURS_RELEASE
///
/// @brief	Library release number
///
#define LCD_COLOURS_RELEASE 103


///
/// @class  LCD_colours565
/// @brief  for 5-6-5 coded colours = 64K colours
///
class LCD_colours565 {
    
public:
    ///
    /// @brief	Constructor
    ///
    LCD_colours565();
    
    /// @name	Colours constants
    // Colours                  Red  Green Blue
    //                          4321054321043210
    /// @{
    
    static const uint16_t black    = 0b0000000000000000; ///<	black
    static const uint16_t white    = 0b1111111111111111; ///<	white
    static const uint16_t red      = 0b1111100000000000; ///<	red
    static const uint16_t green    = 0b0000011111100000; ///<	green
    static const uint16_t blue     = 0b0000000000011111; ///<	blue
    static const uint16_t yellow   = 0b1111111111100000; ///<	yellow
    static const uint16_t cyan     = 0b0000011111111111; ///<	cyan
    static const uint16_t orange   = 0b1111101111100000; ///<	orange
    static const uint16_t magenta  = 0b1111100000001111; ///<	magenta
    static const uint16_t violet   = 0b1111100000011111; ///<	violet
    static const uint16_t grey     = 0b0111101111101111; ///<	grey
    static const uint16_t darkGrey = 0b0011100111100111; ///<	dark grey
    static const uint16_t brown    = 0b0111100000000000; ///<	dark gray
    
    /// @}
    
	/// @name	Colours utilities
	/// @{
    
    ///
    /// @brief	Calculate 16-bit colour from 8-bit Red-Green-Blue components
    /// @param	red red component, 0x00..0xff
    /// @param	green green component, 0x00..0xff
    /// @param	blue blue component, 0x00..0xff
    /// @return	16-bit colour
    ///
    /// @n @b	More: @ref Colours
    ///
    uint16_t calculate(uint8_t red, uint8_t green, uint8_t blue);
    
    ///
    /// @brief	Calculate 8-bit Red-Green-Blue components from 16-bit colour
    /// @param	rgb 16-bit colour
    /// @param	red red component, 0x00..0xff
    /// @param	green green component, 0x00..0xff
    /// @param	blue blue component, 0x00..0xff
    ///
    /// @n @b	More: @ref Colours
    ///
    void split(uint16_t rgb, uint8_t &red, uint8_t &green, uint8_t &blue);
    
    ///
    /// @brief	Half 16-bit colour
    /// @param	rgb 16-bit colour
    /// @return	halved 16-bit colour
    ///
    /// @n @b	More: @ref Colours
    ///
    uint16_t halve(uint16_t rgb);
    
    ///
    /// @brief	Average two 16-bit colours
    /// @param	rgb1 first 16-bit colour
    /// @param	rgb2 second 16-bit colour
    /// @return	averaged 16-bit colour
    ///
    /// @n @b	More: @ref Colours
    ///
    uint16_t average(uint16_t rgb1, uint16_t rgb2);
    
	///
    /// @brief	Reverse 16-bit colour
    /// @param	rgb 16-bit colour
    /// @return	reversed 16-bit colour
    ///
    /// @n @b	More: @ref Colours
    ///
    uint16_t reverse(uint16_t rgb);
    
	///
    /// @brief	Return 16-bit colour from international colour code
    /// @param	value09 0..9 for normal colours, 10..19 for dark colours
    /// @return	rgb 16-bit colour
    ///
    /// @n @b	More: @ref Colours
    ///
    uint16_t code(uint8_t value09);
    
    /// @}
    
};

///
/// @brief      Instantiated object
/// @details	myColours
///
extern LCD_colours565 myColours;

#endif
