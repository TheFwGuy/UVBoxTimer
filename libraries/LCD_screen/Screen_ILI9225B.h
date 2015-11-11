///
/// @file       Screen_ILI9225B.h
/// @brief      Library header
/// @details	ILI9225 screen library
/// @n
/// @n @b       Project LCD_screen Library Suite
/// @n @a       Developed with [embedXcode](http://embedXcode.weebly.com)
///
/// @author     Rei VILO
/// @author     http://embedXcode.weebly.com
/// @date       Apr 03, 2014
/// @version	110
///
/// @copyright	(c) Rei VILO, 2013-2014
/// @copyright	Attribution-NonCommercial-ShareAlike 3.0 Unported (CC BY-NC-SA 3.0)
///
/// @n  Unique license: Attribution-NonCommercial-ShareAlike 3.0 Unported (CC BY-NC-SA 3.0)
/// *   For hobbyists and for personal usage
/// *   For professionals or organisations or for commercial usage
///
/// @see
/// *		Rob's 2.2" LCD+Touch Panel BoosterPack in Store
/// @n		http://www.43oh.com/2013/01/robs-2-2-lcdtouch-panel-boosterpack-in-store/
/// *		Mar 19, 2012 / Nov 04, 2012 - RobG - ILI9225B (2.2" with touch)
/// @n		http://forum.43oh.com/topic/1956-universal-color-lcd-graphics-library/#entry23618
/// *		Jan 20, 2012 - timotet - Port to Stellaris
/// @n		http://forum.43oh.com/topic/2574-22-color-lcd-booster-pack-with-touch/?p=28092
/// *		(Universal) Color LCD graphics library by RobG 22 March 2012 - 03:09 AM
/// @n		http://forum.43oh.com/topic/1956-universal-color-lcd-graphics-library/
/// *		Color LCD Booster Pack by RobG 02 February 2012 - 10:01 PM
///	@n		http://forum.43oh.com/topic/1758-color-lcd-booster-pack/
/// *       TSC2046 4-Wire Touch Screen Controller with low voltage digital I/O
/// @n      http://www.ti.com/product/tsc2046
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

#ifndef SCREEN_ILI9225B_RELEASE
///
/// @brief	Library release number
///
#define SCREEN_ILI9225B_RELEASE 110

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


// Other libraries
#ifdef HARDWARE_SPI
#include "SPI.h"
#endif

///
/// @brief      Class for RobG's 2.2" LCD+Touch Panel BoosterPack
/// @details    Screen controllers
/// * LCD: ILI9225B, SPI
/// * touch: XPT2046, SPI
///
class Screen_ILI9225B : public LCD_screen_font {
public:
    
    ///
    /// @brief	Constructor with default pins
    /// @param	version version of the BoosterPack, 1=sold by RobG or 2=default=sold by 43oh store
    ///
    /// @note	Default pins for LaunchPad MSP430 / StellarPad LM4F
    /// @n      Pin number / LaunchPad MSP430 / StellarPad LM4F / TM4C129
    /// @n	*	 2 / P1_0 / PB_5 / PE_4 = Screen Chip Select
    /// @n	*	 7 / P1_5 / PB_4 / PD_3 = Serial Clock
    /// @n	*	15 / P1_7 / PB_7 / PD_1 = Serial Data MOSI
    /// @n	*	14 / P1_6 / PB_6 / PD_0 = Serial Data MISO
    /// @n	*	 6 / P1_4 / PE_5 / PD_3 = LCD Data/Command
    ///
    /// @n		Touch Chip Select
    /// @n      Pin number / LaunchPad MSP430 / StellarPad LM4F
    /// @n	*	10 / P2_2 / PA_7 / PB_3 = Touch Chip Select for version 2=sold directly by RobG
    /// @n	*	 9 / P2_1 / PA_6 / PB_2 = Touch Chip Select for version 1=default=sold by 43oh store
    ///
    /// @n	*	11 / P2_3 / PA_2 / PP_2 = LCD Reset
    /// @n
    ///
    Screen_ILI9225B(uint8_t version = 1);
    
    ///
    /// @brief	Constructor with specific pins
    /// @param	pinScreenChipSelect Chip Select pin number
    /// @param	pinSerialClock Serial Clock pin number
    /// @param	pinSerialDataMOSI Serial Data MOSI pin number
    /// @param	pinSerialDataMISO Serial Data MISO pin number
    /// @param	pinScreenDataCommand Data Command pin number
    /// @param	pinTouchChipSelect Touch Select pin number
    /// @param	pinScreenReset Reset pin number, default=NULL=none
    ///
    Screen_ILI9225B(uint8_t pinScreenChipSelect,
                 uint8_t pinSerialClock, uint8_t pinSerialDataMOSI, uint8_t pinSerialDataMISO,
                 uint8_t pinScreenDataCommand,
                 uint8_t pinTouchChipSelect, uint8_t pinScreenReset=NULL);
    
    ///
    /// @brief	Initialise
    /// @details	Open SPI connection
    /// @note   	Maximum speed
    /// *	MSP430G2553 = SPI_CLOCK_DIV2
    /// *	LM4F120H5QR = SPI_CLOCK_DIV4 due to touch driver
    ///
    void begin();
    
    ///
    /// @brief	Request information about the screen
    /// @return	string with hardware version
    ///
    String WhoAmI();
    
//    ///
//    /// @brief	Switch backlight
//    /// @param	flag default=true backlight on, false off
//    ///
//    void setBacklight(boolean flag=true);

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
    void _write(bool dataCommand, uint8_t value8);
    void _writeData16(uint16_t data16);
    void _writeRegister(uint8_t command8, uint16_t data16);
    void _writeCommand(uint8_t command8);
    
    /// @todo add void _writeCommand(uint8_t command8);
    
    // Energy
    void _setIntensity(uint8_t intensity); // compulsory
    void _setBacklight(bool flag); // compulsory
    
    // Touch
    void _getOneTouch(uint8_t command8, uint8_t &a, uint8_t &b);
    
    // Variables
    uint8_t _pinReset;
    uint8_t _pinTouchMOSI;
    uint8_t _pinTouchMISO;
    uint8_t _pinScreenBacklight;
    uint8_t _pinScreenCS;
    uint8_t _pinTouchCS;
    uint8_t _pinScreenDataCommand;
    uint8_t _pinTouchClock;
};

#endif
