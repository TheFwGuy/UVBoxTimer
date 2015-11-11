///
/// @file       Screen_HY28A.h
/// @brief      Library header
/// @details	HY28A screen library
/// @n
/// @n @b		Project LCD_screen Library Suite
/// @n @a       Developed with [embedXcode](http://embedXcode.weebly.com)
///
/// @author     Rei VILO
/// @author     http://embedXcode.weebly.com
/// @date       Dec 10, 2013
/// @version	113
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
/// @see
/// *       HY28A screen
/// @n      http://www.hotmcu.com/28-touch-screen-tft-lcd-with-spi-interface-p-42.html
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

#ifndef SCREEN_HY28A_RELEASE
///
/// @brief	Library release number
///
#define SCREEN_HY28A_RELEASE 113

// Other libraries
#include "SPI.h"
#include "LCD_screen_font.h"

#if LCD_SCREEN_FONT_RELEASE < 117
#error Required LCD_SCREEN_FONT_RELEASE 117
#endif

///
/// @brief		SPI selector
/// @{
///
#define NORMAL_SPI   0 ///< normal hardware SPI, default
#define SOFTWARE_SPI 1 ///< software SPI with shiftOut() and shiftIn()
#define FAST_SPI     2 ///< fast software SPI with sbi(), cbi() and pulseLow()
/// @}

// Select SPI mode, for LM4F and TM4C only, otherwise NORMAL_SPI
#if defined(__LM4F120H5QR__) || defined(__TM4C1230C3PM__) || defined(__TM4C129XNCZAD__)
 // StellarPad LM4F specific
#define SPI_MODE NORMAL_SPI                                                     // StellarPad LM4F specific
#else
#define SPI_MODE NORMAL_SPI                                                     // otherwise NORMAL_SPI
#endif

#if (SPI_MODE == FAST_SPI)
#define portOutputRegister(x) (regtype)portBASERegister(x)
#define cbi(reg, mask) GPIOPinWrite(reg, mask, 0)
#define sbi(reg, mask) GPIOPinWrite(reg, mask, mask)
#define pulse_high(reg, bitmask) { sbi(reg, bitmask); cbi(reg, bitmask); }
#define pulse_low(reg, bitmask) { cbi(reg, bitmask); sbi(reg, bitmask); }

typedef volatile uint32_t regtype;
typedef uint8_t regsize;
#endif

// Objects
//
///
/// @brief	Class for HY28A screen
/// @details    HY28A controllers
/// * LCD: ILI9320, SPI
/// * touch: ADS7843 or XPT2046, SPI
///
class Screen_HY28A : public LCD_screen_font {
public:
    ///
    /// @brief	Constructor
    /// @param	pinScreenChipSelect digital pin number for screen select
    /// @param	pinScreenReset digital pin number for screen reset
    /// @param	pinTouchChipSelect digital pin number for touch select
    /// @param	pinTouchSerialClock digital pin number for screen SPI serial
    /// @param	pinTouchSerialDataMOSI digital pin number for screen SPI MOSI
    /// @param	pinTouchSerialDataMISO digital pin number for screen SPI MISO
    /// @note   Two distinct SPI buses are required:
    /// *	Hardware SPI for screen
    /// *	Software emulated SPI for touch
    ///
    /// @note	Default pins for LaunchPad MSP430 / StellarPad LM4F
    /// @n		2 = P1_0 / PB_5 = Screen Reset
    /// @n		6 = P1_4 / PE_5 = Screen Chip Select
    /// @n		9 = P2_1 / PA_6 = Touch Chip Select
    /// @n		10 = P2_2 / PA_7 = Touch Serial Clock
    /// @n		12 = P2_4 / PA_3 = Touch Serial Data MOSI
    /// @n		11 = P2_3 / PA_2 = Touch Serial Data MISO
    /// @n
    /// @n      Additional pins for fast SPI
    /// @n		7 = P1_5 / PB_4 = Screen Serial Clock *
    /// @n		15 = P1_7 / PB_7 = Screen Serial Data MOSI *
    /// @n		14 = P1_6 / PB_6 = Screen Serial Data MISO *
    ///
    /// August 12 Roberto Romano changed:
    /// @n	New define conditional to share or not SPI for touch


#if defined(ENERGIA) // LaunchPad MSP430 and LM4F specific
// comment to leave original use of software SPI 4 Touch Panel
#define SPI_IS_SHAREDBYTP 1

    Screen_HY28A(uint8_t pinScreenChipSelect = 2, uint8_t pinScreenReset = 11,
                 uint8_t pinTouchChipSelect = 12
	#ifdef SPI_IS_SHAREDBYTP
		);
	#else
		 , uint8_t pinTouchSerialClock = -1,
                 uint8_t pinTouchSerialDataMOSI = -1, uint8_t pinTouchSerialDataMISO = -1);
	#endif // SPI SHARE
    
#elif defined(__AVR_ATmega328P__) || defined(__AVR_ATmega2560__) || defined(__AVR_ATmega644P__)
    Screen_HY28A(uint8_t pinScreenChipSelect = 2, uint8_t pinScreenReset = 3,
                 uint8_t pinTouchChipSelect = 4, uint8_t pinTouchSerialClock = 5,
                 uint8_t pinTouchSerialDataMOSI = 6, uint8_t pinTouchSerialDataMISO = 7);
    
#elif defined(CORE_TEENSY)
    Screen_HY28A(uint8_t pinScreenChipSelect = 2, uint8_t pinScreenReset = 3,
                 uint8_t pinTouchChipSelect = 4, uint8_t pinTouchSerialClock = 5,
                 uint8_t pinTouchSerialDataMOSI = 6, uint8_t pinTouchSerialDataMISO = 7);
#else
    //#error
#endif
    
    ///
    /// @brief	Initialisation
    ///
    void begin();
    
    ///
    /// @brief	Request information about the screen
    /// @return	string with hardware version
    ///
    String WhoAmI();
    
    /// @name	Specific features
    /// @{
    
    ///
    /// @brief	Invert screen
    /// @param	flag true to invert, false for normal screen
    ///
    void invert(boolean flag);
    
    ///
    /// @brief	Copy-paste area
    /// @param	x1 left-top source area coordinate, x axis
    /// @param	y1 left-top source area coordinate, y axis
    /// @param	x2 left-top target area coordinate, x axis
    /// @param	y2 left-top target area coordinate, x axis
    /// @param	dx area size, x axis
    /// @param	dy area size, x axis
    ///
    void copyPasteArea(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t dx, uint16_t dy);

    ///
    /// @brief	Read pixel colour
    /// @param	x1 point coordinate, x-axis
    /// @param	y1 point coordinate, y-axis
    /// @return	16-bit colour, bits 15-11 red, bits 10-5 green, bits 4-0 blue
    ///
    uint16_t readPixel(uint16_t x1, uint16_t y1);
    
    /// @}
    
protected:
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
    void _setCursor(uint16_t x0, uint16_t y0);
    
    // Communication, write
    void _writeCommand(uint8_t command8);
    void _writeData16(uint16_t data16);
    void _writeRegister(uint8_t command8, uint16_t data16);
    
    // Communication, read
    uint16_t _readData();
    
    // Touch
    void _getOneTouch(uint8_t command, uint8_t &a, uint8_t &b);
    
    // SPI hardware and fast software
    void _writeSPI(uint8_t data8);
    uint8_t _readSPI();
    
#if (SPI_MODE == FAST_SPI)
    regtype _portScreenCS, _portScreenMOSI, _portScreenMISO, _portScreenSCK, _portScreenRST;
    regsize _bitScreenCS, _bitScreenMOSI, _bitScreenMISO, _bitScreenSCK, _bitScreenRST;
#endif
    
    // Energy
    void _setIntensity(uint8_t intensity); // compulsory
    void _setBacklight(bool flag); // compulsory

    // Variables
    uint8_t _pinScreenChipSelect;
    uint8_t _pinScreenSerialClock;
    uint8_t _pinScreenSerialDataMOSI;
    uint8_t _pinScreenSerialDataMISO;
    uint8_t _pinScreenReset;
    uint8_t _pinTouchChipSelect;
#ifndef SPI_IS_SHAREDBYTP
    uint8_t _pinTouchSerialClock;
    uint8_t _pinTouchSerialDataMOSI;
    uint8_t _pinTouchSerialDataMISO;
#endif // SPI SHARE
};

#endif
