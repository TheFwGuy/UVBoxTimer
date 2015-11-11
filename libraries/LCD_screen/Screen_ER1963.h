///
/// @file       Screen_ER1963.h
/// @brief      Library header
/// @details	SSD1963 screen library
/// @n
/// @n @b       Project LCD_screen Library Suite
/// @n @a       Developed with [embedXcode](http://embedXcode.weebly.com)
///
/// @author     Rei VILO
/// @author     http://embedXcode.weebly.com
/// @date		May 13, 2014
/// @version	107
///
/// @copyright	© Rei VILO, 2013-2014
/// @copyright	All rights reserved
///
/// @n  Dual license:
/// *   For hobbyists and for personal usage: Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0)
/// *   For professionals or organisations or for commercial usage: All rights reserved
///
/// @see
/// *       Help from Stellaristi forum for SPI and touch
/// @n      http://forum.stellarisiti.com/topic/666-solved-writing-and-reading-the-values-of-an-8-bit-port-with-energia/?p=3391
/// @n      http://forum.stellarisiti.com/topic/666-solved-writing-and-reading-the-values-of-an-8-bit-port-with-energia/?p=3395
/// @n      http://forum.stellarisiti.com/topic/682-solved-4-wire-resistive-touch-screen-reading-with-energia/?p=3502
/// @n      This version assumes an 8080-8bit interface between the MCU and display.
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

/*#if defined(__LM4F120H5QR__) || defined(__TM4C1230C3PM__) || defined(__TM4C129XNCZAD__)
#else
#error LaunchPad Stellaris or Tiva C Series required.
#endif
*/

#ifndef SCREEN_ER1963_RELEASE
///
/// @brief	Library release number
///
#define SCREEN_ER1963_RELEASE 104

#include "LCD_screen_font.h"

#define RESOLUTION480x272 1
//#define RESOLUTION800x480 1
#define LCDC_CS_ISGROUNDED 1

#ifdef RESOLUTION480x272
	#define ER1963_WIDTH       480 // Horizontal
	#define ER1963_HEIGHT      272 // Vertical
#else
	#define ER1963_WIDTH       800 // Horizontal
	#define ER1963_HEIGHT      480 // Vertical
#endif

#if LCD_SCREEN_FONT_RELEASE < 120
#error Required LCD_SCREEN_FONT_RELEASE 120
#endif

//#if (SPI_MODE == FAST_SPI)
//#define portOutputRegister(x) (regtype)portBASERegister(x)
//typedef volatile uint32_t regtype;
//typedef uint8_t regsize;
//
//#endif

///
/// @brief      Class for 800x400 East Rising 5" SSD1963 screen
/// @details    Screen controllers
/// *   LCD: SSD1963, 8-bit 8080 parallel
/// *   touch: ADS7843 or XPT2046, software SPI
/// *   SD-card: hardware SPI
/// @warning    Support for LaunchPad Stellaris only
///
class Screen_ER1963 : public LCD_screen_font {
public:
    
    ///
    /// @brief	Constructor with default pins
    /// @note	Default pins for Stellaris LaunchPad
    /// @param	pinScreenCS screen chip select default=10=PA_7 on Stellaris
    /// @param	pinTouchCS touch chip select default=39=PF_3 on Stellaris
    ///
    Screen_ER1963(uint8_t pinScreenCS = 10, uint8_t pinTouchCS = 39);
    
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
    
    ///
    /// @brief	Read pixel colour
    /// @param	x1 point coordinate, x-axis
    /// @param	y1 point coordinate, y-axis
    /// @return	16-bit colour, bits 15-11 red, bits 10-5 green, bits 4-0 blue
    ///
    uint16_t readPixel(uint16_t x1, uint16_t y1);
    
    ///
    /// @brief	Copy a source area to a target area
	/// @param	x1 source top left coordinate, x-axis
	/// @param	y1 source top left coordinate, y-axis
	/// @param	x2 target top left coordinate, x-axis
	/// @param	y2 target top left coordinate, y-axis
    /// @param	dx width to be copied, x-axis
    /// @param	dy height to be copied, y-axis
    /// @note   This feature requires a readable screen.
    /// @warning   The function doesn't manage the overlapping of the source and target areas.
    /// If such a case, use copyArea() pasteArea() instead.
    ///
    /// @n @b	More: @ref Coordinates, @ref Screens
    ///
    virtual void copyPasteArea(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t dx, uint16_t dy);
    
    /// @name       Fast area features
    /// @details	Over-ridden ER1963 specific functions
    /// @{
    
    ///
    /// @brief      Feed the area with colour
    /// @details	Maximum dx*dy pixels, no check
    /// @param      colour 16-bit colour
    /// @note       Pixels are displayed from left to right, top to bottom
    /// @warning    Specific over-ridden procedure for ER1963
    ///
    /// @n @b       More: @ref Areas, @ref Colours
    ///
    void feedArea(uint16_t colour);
    
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
    // Communication, write
    void _writeRegister8(uint8_t command8, uint8_t data8);
    void _writeCommand8(uint8_t command8);
    void _writeData8(uint8_t data8);
    void _writeData888(uint8_t dataReg8, uint8_t dataGreen8, uint8_t dataBlue8);
    void _writeData16(uint16_t data16);
    
    // Communication, read
    void _setWindowRead(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
    uint16_t _readData16();
    
    // Touch
    void _getOneTouch(uint8_t command8, uint8_t &a, uint8_t &b);
    
    // Energy
    void _setIntensity(uint8_t intensity); // compulsory
    void _setBacklight(bool flag); // compulsory
    
    uint8_t _pinScreenCS;
    uint8_t _pinTouchCS;
    
    uint8_t _pinTouchMOSI;
    uint8_t _pinTouchMISO;
    uint8_t _pinTouchClock;
    
    //#if (SPI_MODE == FAST_SPI)
    //    regtype LCD_CS_BASE;
    //    regsize LCD_CS_PIN;
    //#endif
};

#endif
