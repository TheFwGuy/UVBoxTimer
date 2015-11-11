///
/// @file       Screen_K70.h
/// @brief      Library header
/// @details	SSD2119 screen library
/// @n
/// @n @b       Project LCD_screen Library Suite
/// @n @a       Developed with [embedXcode](http://embedXcode.weebly.com)
///
/// @author     Rei VILO
/// @author     http://embedXcode.weebly.com
/// @date       Feb 01, 2014
/// @version	102
///
/// @copyright	© Rei VILO, 2013
/// @copyright	Attribution-NonCommercial-ShareAlike 3.0 Unported (CC BY-NC-SA 3.0)
///
/// @n  Unique license: Attribution-NonCommercial-ShareAlike 3.0 Unported (CC BY-NC-SA 3.0)
/// *   For hobbyists and for personal usage
/// *   For professionals or organisations or for commercial usage
///
/// @see
/// *		7.0" inch TFT LCD module BoosterPack for LaunchPad Stellaris
/// @n      http://www.kentecdisplay.com/uploads/soft/Products_spec/EB-LM4F120-L35_UserGuide_03.pdf
/// *       Help from Stellaristi forum for SPI and touch
/// @n      http://forum.stellarisiti.com/topic/666-solved-writing-and-reading-the-values-of-an-8-bit-port-with-energia/?p=3391
/// @n      http://forum.stellarisiti.com/topic/666-solved-writing-and-reading-the-values-of-an-8-bit-port-with-energia/?p=3395
/// @n      http://forum.stellarisiti.com/topic/682-solved-4-wire-resistive-touch-screen-reading-with-energia/?p=3502
/// *       Low level routines based on Kentec320x240x16_ssd2119_8bit library
/// @n      Display driver for the Kentec Display K50DWN2/K70DWN2/K90DWN2 TFT display with an SSD1963 controller.
/// @n      This version assumes an 8080-8bit interface between the micro and display.
///
/// Copyright (c) 2008-2009 Texas Instruments Incorporated. All rights reserved.
/// Software License Agreement
///
/// Texas Instruments (TI) is supplying this software for use solely and
/// exclusively on TI's microcontroller products. The software is owned by
/// TI and/or its suppliers, and is protected under applicable copyright
/// laws. You may not combine this software with "viral" open-source
/// software in order to form a larger program.
///
/// THIS SOFTWARE IS PROVIDED "AS IS" AND WITH ALL FAULTS.
/// NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT
/// NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
/// A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. TI SHALL NOT, UNDER ANY
/// CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
/// DAMAGES, FOR ANY REASON WHATSOEVER.
///
/// This is part of revision 5450 of the DK-LM3S9B96 Firmware Package.
///
/// *       Documentation from Getting Started with the Stellaris EK-LM4F120XL LaunchPad Workshop
/// @n      http://processors.wiki.ti.com/index.php/Getting_Started_with_the_Stellaris_EK-LM4F120XL_LaunchPad_Workshop
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

#if defined(__LM4F120H5QR__) || defined(__TM4C123GH6PM__)
//#if defined(__LM4F120H5QR__) || defined(__TM4C1230C3PM__) || defined(__TM4C129XNCZAD__)
#else
#error LaunchPad Stellaris or Tiva C Series required.
#endif

#ifndef SCREEN_K70_RELEASE
///
/// @brief	Library release number
///
#define SCREEN_K70_RELEASE 102

#include "LCD_screen_font.h"

#if LCD_SCREEN_FONT_RELEASE < 117
#error Required LCD_SCREEN_FONT_RELEASE 117
#endif

///
/// @brief      Class for 3.5" 480x320 screen
/// @details    Screen controllers
/// *   LCD: SSD1963, 8-bit 8080 parallel
/// *   touch: direct, no controller
/// @warning    Support for LaunchPad Stellaris only
///
class Screen_K70 : public LCD_screen_font {
public:
    
    ///
    /// @brief	Constructor with default pins
    ///
    /// @note	Default pins for Stellaris LaunchPad
    ///
    Screen_K70();
    
    ///
    /// @brief	Initialise
    /// @details	Open connections
    ///
    void begin();
    
    ///
    /// @brief	Request information about the screen
    /// @return	string with hardware version
    ///
    String WhoAmI();
    
    
//    ///
//    /// @brief	Read pixel colour
//    /// @param	x1 point coordinate, x-axis
//    /// @param	y1 point coordinate, y-axis
//    /// @return	16-bit colour, bits 15-11 red, bits 10-5 green, bits 4-0 blue
//    ///
//    uint16_t readPixel(uint16_t x1, uint16_t y1);
//    
//    ///
//    /// @brief	Copy a source area to a target area
//	/// @param	x1 source top left coordinate, x-axis
//	/// @param	y1 source top left coordinate, y-axis
//	/// @param	x2 target top left coordinate, x-axis
//	/// @param	y2 target top left coordinate, y-axis
//    /// @param	dx width to be copied, x-axis
//    /// @param	dy height to be copied, y-axis
//    /// @note   This feature requires a readable screen.
//    /// @warning   The function doesn't manage the overlapping of the source and target areas.
//    /// If such a case, use copyArea() pasteArea() instead.
//    ///
//    /// @n @b	More: @ref Coordinates, @ref Screens
//    ///
//    virtual void copyPasteArea(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t dx, uint16_t dy);

    /// @name	Fast area features
    /// @details	Over-ridden K70 specific functions
    /// @{
    
    ///
    /// @brief      Feed the area with colour
    /// @details	Maximum dx*dy pixels, no check
    /// @param      colour 16-bit colour
    /// @note       Pixels are displayed from left to right, top to bottom
    /// @warning    Specific over-ridden procedure for K70
    ///
    /// @n @b       More: @ref Areas, @ref Colours
    ///
    void feedArea(uint16_t colour);
    
    /// @}
    
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
    void _writeRegister8(uint8_t command8, uint8_t data8);
    void _writeCommand8(uint8_t command8);
    void _writeData8(uint8_t data8);
    void _writeData888(uint8_t dataReg8, uint8_t dataGreen8, uint8_t dataBlue8);
    
//    // Communication, read
//    void _setWindowRead(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
//    uint16_t _readData16();

    // Energy
    void _setIntensity(uint8_t intensity); // compulsory
    void _setBacklight(bool flag); // compulsory
    
    // Touch
    void _getOneTouch(uint8_t command8, uint8_t &a, uint8_t &b);
};

#endif
