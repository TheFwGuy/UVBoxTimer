///
/// @file		Screen_ER1963_SD.h
/// @brief		Library header for screen East Rising 5" SSD1963 with SD
/// @n
/// @n @b		Project LCD_screen Library Suite
/// @n @a		Developed with [embedXcode](http://embedXcode.weebly.com)
///
/// @author		Rei VILO
/// @author		http://embeddedcomputing.weebly.com
/// @date		May 13, 2014
/// @version	107
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

#ifndef Screen_ER1963_SD_RELEASE
///
/// @brief	Library release number
///
#define Screen_ER1963_SD_RELEASE 107

// Libraries
#include "SPI.h"

// Other libraries
#include "Screen_ER1963.h"

/// moved to header and conditional defined from ER1963 header
/// @name	ER1963 size
/// @{
#define ER1963_SD_WIDTH  ER1963_WIDTH
#define ER1963_SD_HEIGHT ER1963_HEIGHT

/// @}


// Choice of SD card library
#define SD_LIBRARY 1
#define FATFS_LIBRARY 2
#define FILE_LIBRARY SD_LIBRARY


#if (FILE_LIBRARY == FATFS_LIBRARY)
#include "fatfs.h"                                                              // with FatFs on SPI already available
#elif (FILE_LIBRARY == SD_LIBRARY)
#include "SD.h"
#else
#error File library not selected
#endif

// Release check
#if SCREEN_ER1963_RELEASE < 103
#error Required SCREEN_ER1963_RELEASE 103
#endif


// Objects
//
///
/// @brief      Class for 800x400 East Rising 5" SSD1963 screen with SD-card
/// @details    Screen controllers
/// *   LCD: SSD1963, 8-bit 8080 parallel
/// *   touch: ADS7843 or XPT2046, software SPI
/// *   SD-card: hardware SPI
//
class Screen_ER1963_SD : public Screen_ER1963
{
public:
    ///
    /// @brief	Constructor
    ///
    /// @param	pinScreenCS digital pin number for screen chip select
    /// @param  pinTouchCS  digital pin number for touch chip select
    /// @param	pinSDcardCS digital pin number for SD-card chip select
    /// @param  pinSDcardDetection digital pin number for card detection, optional, default=-1=none
    /// @note	Default pins for LaunchPad MSP430 / StellarPad LM4F
    /// *   10 = ? / PA_7 = screen chip select
    /// *   39 = ? / PF_3 = touch chip select
    /// *   30 = ? / PF_1 = SD chip select
    ///
    /*RS  = PN_1;	//18;    // RS signal connected to Arduino digital pin 30
    WR  = PP_4;	//12;    // /WR signal connected to Arduino digital pin 31
    RD  = PP_5;	//11;    // /RD signal connected to Arduino digital pin 32
    RES = PN_0;	//13;    // /RES signal connected to Arduino digital pin 33
    */
    // int CS  = 19;
    // /CS signal tied to GND
//    SDCS = PQ_2;
//    TCCS = PL_3;

#if  defined(__TM4C1294NCPDT__)
    Screen_ER1963_SD(uint8_t pinScreenCS = -1, uint8_t pinTouchCS = PL_3, uint8_t pinSDcardCS = PQ_2, int8_t pinSDcardDetection = -1);
#else
    Screen_ER1963_SD(uint8_t pinScreenCS = -1, uint8_t pinTouchCS = 8, uint8_t pinSDcardCS = 19, int8_t pinSDcardDetection = -1);
#endif

    ///
    /// @brief	Initialisation
    /// @note   begin requires SD-card available
    ///
    void begin();
    
    ///
    /// @brief	SD-card hardware detection
    /// @return	true is SD-card inserted, false otherwise
    ///
    bool detectSD();

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
    uint8_t _pinSDcardCS;
    int8_t  _pinSDcardDetection;
    
    void _fastCopyAreaToSD(uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy, uint32_t &address);
    void _fastPasteAreaFromSD(uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy, uint32_t &address, bool option);

};

#endif
