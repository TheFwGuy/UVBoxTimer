///
/// @file       Screen_HY28A_SRAM.h
/// @brief      Library header
/// @details	HY28A with SRAM screen library
/// @n
/// @n @b       Project LCD_screen Library Suite
/// @n @a       Developed with [embedXcode](http://embedXcode.weebly.com)
///
/// @author     Rei VILO
/// @author     http://embedXcode.weebly.com
/// @date       Dec 10, 2013
/// @version	113
///
/// @copyright 	(c) Rei VILO, 2010-2015
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
/// @see
/// *       HY28A screen
/// @n      http://www.hotmcu.com/28-touch-screen-tft-lcd-with-spi-interface-p-42.html
/// *       Improved SPI library by reaper7
/// @n      http://forum.stellarisiti.com/topic/675-solved
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

#ifndef HY28A_SRAM_RELEASE
///
/// @brief	Library release number
///
#define SCREEN_HY28A_SRAM_RELEASE 113

#if defined(__LM4F120H5QR__)
// Other libraries
#include "SPI.h"
#include "Screen_HY28A.h"
#include "FastDigitalWrite.h"

#if LCD_SCREEN_FONT_RELEASE < 117
#error Required LCD_SCREEN_FONT_RELEASE 117
#endif

///
/// @brief		SPI selector
/// @{
///
#define NORMAL_SPI   0 ///< normal hardware SPI, default
#define SOFTWARE_SPI 1 ///< software SPI with shiftOut() and shiftIn()
#define SPI_MODE NORMAL_SPI ///< select SPI mode
/// @}

#define FAST_DIGITALWRITE 1 ///< with sbi() and cbi()

///
/// @brief      Use SPI SRAM
/// @details    0=no SPI SRAM, 1=SPI SRAM
///
#define USE_SPI_SRAM 1

// Objects
//
///
/// @brief	Class for HY28A screen
/// @details    HY28A controllers
/// * LCD: ILI9320, SPI
/// * touch: ADS7843 or XPT2046, SPI
/// * SRAM memory: 2x 23LC1024
///
class Screen_HY28A_SRAM : public Screen_HY28A {
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
    /// @n		P1_0 / PB_5 = Screen Reset or RESET pin
    /// @n		P1_4 / PE_5 = Screen Chip Select
    /// @n		P2_1 / PA_6 = Touch Chip Select
    /// @n		P2_2 / PA_7 = Touch Serial Clock
    /// @n		P2_4 / PA_3 = Touch Serial Data MOSI
    /// @n		P2_3 / PA_2 = Touch Serial Data MISO
    /// @n
    /// @n      Additional pins for fast SPI
    /// @n		P1_5 / PB_4 = Screen Serial Clock *
    /// @n		P1_7 / PB_7 = Screen Serial Data MOSI *
    /// @n		P1_6 / PB_6 = Screen Serial Data MISO *
    ///
#if defined(__MSP430G2553__) // LaunchPad MSP430G2553 specific
    Screen_HY28A_SRAM(uint8_t pinScreenChipSelect = P1_4, uint8_t pinScreenReset = P1_0,
                      uint8_t pinTouchChipSelect = P2_1, uint8_t pinTouchSerialClock = P2_2,
                      uint8_t pinTouchSerialDataMOSI = P2_4, uint8_t pinTouchSerialDataMISO = P2_3);
    
#elif defined(__LM4F120H5QR__) // StellarPad LM4F specific
    Screen_HY28A_SRAM(uint8_t pinScreenChipSelect = PE_5, uint8_t pinScreenReset = PB_5,
                      uint8_t pinTouchChipSelect = PA_6, uint8_t pinTouchSerialClock = PA_7,
                      uint8_t pinTouchSerialDataMOSI = PA_3, uint8_t pinTouchSerialDataMISO = PA_2);
    
#elif defined(__AVR_ATmega328P__) || defined(__AVR_ATmega2560__) || defined(__AVR_ATmega644P__)
    Screen_HY28A_SRAM(uint8_t pinScreenChipSelect = 2, uint8_t pinScreenReset = 3,
                      uint8_t pinTouchChipSelect = 4, uint8_t pinTouchSerialClock = 5,
                      uint8_t pinTouchSerialDataMOSI = 6, uint8_t pinTouchSerialDataMISO = 7);
#else
#error Board not supported
#endif                                                                          // defined boards
    
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
    /// @brief	Copy an area to an external storage, vector coordinates
	/// @param	x0 source top left coordinate, x-axis
	/// @param	y0 source top left coordinate, y-axis
    /// @param	dx width to be copied, x-axis
    /// @param	dy height to be copied, y-axis
    /// @param	address identifier, as SRAM address or file number
    /// @note   This feature requires a readable screen and a storage.
    ///
    /// @n @b	More: @ref Coordinates, @ref Storage
    ///
    void copyArea(uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy, uint32_t &address);
    
    ///
    /// @brief	Paste an area from an external storage, vector coordinates
	/// @param	x0 source top left coordinate, x-axis
	/// @param	y0 source top left coordinate, y-axis
    /// @param	dx target to be pasted, x-axis
    /// @param	dy target to be pasted, y-axis
    /// @param	address identifier, as SRAM address or file number
    /// @param  option false=default=normal colours, true=halved colours
    /// @note   This feature requires a readable screen and a storage.
    ///
    /// @n @b	More: @ref Coordinates, @ref Storage
    ///
    void pasteArea(uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy, uint32_t &address, bool option=false);
    
private:
//    // SPI hardware and fast software
//    void _writeBeginSPI();
//    void _writeEndSPI();
//    void _writeSPI(uint8_t data8);
//    uint8_t _readSPI();

    // SRAM
#if (USE_SPI_SRAM == 1)
    void _beginSRAM();
    //    void _writeSRAM(uint32_t &address, uint8_t dataHigh8, uint8_t dataLow8);
    //    void _readSRAM(uint32_t &address, uint8_t &dataHigh8, uint8_t &dataLow8);
    void _fastCopyAreaToSRAM(uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy, uint32_t &address);
    void _fastPasteAreaFromSRAM(uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy, uint32_t &address, bool option=false);
#endif                                                                          // end USE_SPI_SRAM
    
    // Variables
//    uint8_t _pinScreenChipSelectNumber;
//    uint8_t _pinScreenSerialClock;
//    uint8_t _pinScreenSerialDataMOSI;
//    uint8_t _pinScreenSerialDataMISO;
//    uint8_t _pinScreenReset;
//    uint8_t _pinTouchChipSelectNumber;
//    uint8_t _pinTouchSerialClock;
//    uint8_t _pinTouchSerialDataMOSI;
//    uint8_t _pinTouchSerialDataMISO;
//    
//    fastDigitalOutputPin _pinTouchChipSelect;
//    fastDigitalOutputPin _pinScreenChipSelectFast;

    fastDigitalOutputPin _fastPinSRAMChipSelectMSB;
    fastDigitalOutputPin _fastPinSRAMChipSelectLSB;
};

#endif                                                                          // end HY28A_SRAM_RELEASE
#endif                                                                          // end __LM4F120H5QR__


