///
/// @file		Screen_ER8875_SD.h
/// @brief		Library header for screen East Rising 5" RA8875 with SD
/// @n
/// @n @b		Project LCD_screen Library Suite
/// @n @a		Developed with [embedXcode](http://embedXcode.weebly.com)
///
/// @author		Rei VILO
/// @author		http://embeddedcomputing.weebly.com
/// @date		Apr 13, 2014
/// @version	106
///
/// @copyright	© Rei VILO, 2013-2014
/// @copyright	All rights reserved
/// @n          http://embeddedcomputing.weebly.com/lcd_screen-library-suite
///
/// @n  Dual license:
/// *   For hobbyists and for personal usage: Attribution-NonCommercial-ShareAlike 3.0 Unported (CC BY-NC-SA 3.0)
/// *   For professionals or organisations or for commercial usage: All rights reserved
///
/// @n  For any enquiry about license, http://embeddedcomputing.weebly.com/contact
///
///
/// @see	ReadMe.txt for references
///

#if defined(__LM4F120H5QR__) || defined(__TM4C1230C3PM__) || defined(__TM4C129XNCZAD__)
                                                  // Stellaris only

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

#ifndef Screen_ER8875_SD_RELEASE
///
/// @brief	Library release number
///
#define Screen_ER8875_SD_RELEASE 107

// Libraries
#include "SPI.h"

// Other libraries
#include "Screen_ER8875.h"

// Release check
#if SCREEN_ER8875_RELEASE < 105
#error Required SCREEN_ER8875_RELEASE 105
#endif


// Objects
//
///
/// @brief      Class for 800x400 East Rising 5" RA8875 screen with SD-card
/// @details    Screen controllers
/// * LCD and touch: RA8875, SPI
/// * SD: separate SPI
//
class Screen_ER8875_SD : public Screen_ER8875
{
public:
//    ///
//    /// @brief	Constructor with default pins
//    ///
//    /// @note	Default pins for Arduino Uno / LaunchPad MSP430 / StellarPad LM4F
//    /// @n		? / 10 / 10 =  screen chip select for SPI(2) port
//    /// @n        ? / 9 /9 = SD chip select for SPI() port
//    ///
//    Screen_ER8875_SD();
    
    ///
    /// @brief	Constructor
    ///
    /// @param	pinScreenCS digital pin number for screen chip select
    /// @param	pinSDcardCS digital pin number for SD chip select
    /// @param  pinSDcardDetection digital pin number for card detection, optional, default=-1=none
    ///
    Screen_ER8875_SD(uint8_t pinScreenCS, uint8_t pinSDcardCS, int8_t pinSDcardDetection);
    
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
    /// @brief	Copy an area to an external support
	/// @param	x0 source top left coordinate, x-axis
	/// @param	y0 source top left coordinate, y-axis
    /// @param	dx width to be copied, x-axis
    /// @param	dy height to be copied, y-axis
    /// @param	address identifier, as SRAM address or file number
    /// @note   This feature requires a readable screen and a storage.
    ///
    /// @n @b	More: @ref Coordinates, @ref Screens, @ref Storage
    ///
    virtual void copyArea(uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy, uint32_t &address);
    
    ///
    /// @brief	Paste an area from an external support
	/// @param	x0 source top left coordinate, x-axis
	/// @param	y0 source top left coordinate, y-axis
    /// @param	dx target to be pasted, x-axis
    /// @param	dy target to be pasted, y-axis
    /// @param	address identifier, as SRAM address or file number
    /// @param	option false=default=original colours, true=halved colours
    /// @note   This feature requires a readable screen and a storage.
    ///
    /// @n @b	More: @ref Coordinates, @ref Screens, @ref Storage
    ///
    virtual void pasteArea(uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy, uint32_t &address, bool option=false);
    
private:
    uint8_t _pinSDChipSelect;
    int8_t _pinSDcardDetection;
    
    void _fastCopyAreaToSD(uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy, uint32_t &address);
    void _fastPasteAreaFromSD(uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy, uint32_t &address, bool option);

};

#endif

#endif
