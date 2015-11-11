///
/// @file       Screen_ILI9340.h
/// @brief      Library header
/// @details	ILI9225 screen library
/// @n
/// @n @b       Project LCD_screen Library Suite
/// @n @a       Developed with [embedXcode](http://embedXcode.weebly.com)
///
/// @author     Rei VILO
/// @author     http://embedXcode.weebly.com
/// @date       Feb 18, 2014
/// @version	101
///
/// @copyright	(c) Rei VILO, 2013-2014
/// @copyright	Attribution-NonCommercial-ShareAlike 3.0 Unported (CC BY-NC-SA 3.0)
///
/// @n  Single license: Attribution-NonCommercial-ShareAlike 3.0 Unported (CC BY-NC-SA 3.0)
///
/// @see
/// *		Rob's Color LCD Booster Pack (320x240 with microSD card socket) in Store
/// @n		https://www.tindie.com/products/RobG/color-lcd-booster-pack-320x240-with-microsd-card-socket/
/// *		Rob's (Universal) Color LCD graphics library (2) Posted 12 September 2013 - 01:41 PM
/// @n		http://forum.43oh.com/topic/4352-universal-color-lcd-graphics-library-2/#entry39090
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

#ifndef Screen_ILI9340_RELEASE
///
/// @brief	Library release number
///
#define Screen_ILI9340_RELEASE 101

#include "LCD_screen_font.h"

#if LCD_SCREEN_FONT_RELEASE < 120
#error Required LCD_SCREEN_FONT_RELEASE 120
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
/// @brief  Class for RobG's Rob's Color LCD Booster Pack (320x240 with microSD card socket)
/// @details    Screen controller
/// * LCD: ILI9340, SPI
/// * Touch: no touch
/// * SDcard: SPI
///
class Screen_ILI9340 : public LCD_screen_font {
public:
//    ///
//    /// @brief	Constructor with default pins
//    /// @param	version version of the BoosterPack, 1=sold by RobG or 2=default=sold by 43oh store
//    ///
//    /// @note	Default pins for LaunchPad MSP430 / StellarPad LM4F
//    /// @n		2 / P1_0 / PB_5 = Screen Chip Select
//    /// @n		6 / P1_4 / PE_5 = LCD Data/Command
//    ///
//    /// @n		11 / P2_3 / PA_2 = LCD Reset
//    ///
//    Screen_ILI9340();
    
    ///
    /// @brief	Constructor with specific pins
    /// @param	pinScreenReset Screen Reset pin number
    /// @param	pinScreenCS Screen Chip Select pin number
    /// @param	pinScreenDataCommand Screen Data Command pin number
    /// @param	pinSDcardCS SD-card Chip Select pin number
    ///
    Screen_ILI9340(uint8_t pinScreenReset = 11, uint8_t pinScreenCS = 2, uint8_t pinScreenDataCommand = 12, uint8_t pinSDcardCS = 13);
    
    ///
    /// @brief	Initialise
    /// @details	Open SPI connection
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

    ///
    /// @brief	Read pixel colour
    /// @param	x1 point coordinate, x-axis
    /// @param	y1 point coordinate, y-axis
    /// @return	16-bit colour, bits 15-11 red, bits 10-5 green, bits 4-0 blue
    /// @bug    Doesn't work. No MISO. SDA=SDI+SDO?
    ///
    uint16_t readPixel(uint16_t x1, uint16_t y1);
    
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
    
    // Energy
    void _setIntensity(uint8_t intensity); // compulsory
    void _setBacklight(bool flag); // compulsory
    
	// Touch
    void _getRawTouch(uint16_t &x0, uint16_t &y0, uint16_t &z0) ; // compulsory
    
    // * Other functions specific to the screen
    // Communication, write
    void _writeData8(uint8_t data8);
    void _writeData16(uint16_t data16);
    void _writeRegister8(uint8_t command8, uint8_t data8);
    void _writeRegister16(uint8_t command8, uint16_t data16);
    void _writeCommand8(uint8_t command8);
    
    // Communication, read
    uint16_t _readData();
    void _setWindowRead(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
    
    // Variables
    uint8_t _pinScreenReset;
    uint8_t _pinScreenDataCommand;
    uint8_t _pinScreenCS;
    uint8_t _pinSDcardCS;
};

#endif
