///
/// @file		Screen_ER8875.h
/// @brief		Library header for East Rising 5" RA8875 screen
/// @n
/// @n @b		Project LCD_screen Library Suite
/// @n @a		Developed with [embedXcode](http://embedXcode.weebly.com)
///
/// @author		Rei VILO
/// @author		http://embeddedcomputing.weebly.com
/// @date		Jan 12, 2014
/// @version	105
///
/// @copyright	© Rei VILO, 2013-2014
/// @copyright	All rights reserved
/// @n          http://embeddedcomputing.weebly.com/lcd_screen-library-suite
///
/// @n  Dual license:
/// *   For hobbyists and for personal usage: Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0)
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

#ifndef SCREEN_ER8875_RELEASE
///
/// @brief	Library release number
///
#define SCREEN_ER8875_RELEASE 105

//#if defined(__LM4F120H5QR__)                                                    // Stellaris only

///
/// @brief      Fonts
/// @details    Usage
/// * 1 to use the screen built-in fonts
/// * 0 to use the fonts from header files
///
#define BUILT_IN_FONTS 1


///
/// @brief		SPI selector
/// @{
///
#define NORMAL_SPI   0 ///< Normal hardware SPI with digitalWrite, default
#define FAST_SPI     2 ///< Fast software SPI with sbi() and cbi()
/// @}

#define SPI_MODE FAST_SPI ///< Selected SPI mode

#if defined(__LM4F120H5QR__) || defined(__TM4C1230C3PM__) || defined(__TM4C129XNCZAD__)
                                                   // Stellaris only
#else
#define SPI_MODE NORMAL_SPI
#endif

#if (SPI_MODE == FAST_SPI)
#define portOutputRegister(x) (regtype)portBASERegister(x)
#define cbi(reg, mask) GPIOPinWrite(reg, mask, 0)
#define sbi(reg, mask) GPIOPinWrite(reg, mask, mask)

typedef volatile uint32_t regtype;
typedef uint8_t regsize;
#endif


// Other libraries
#if (BUILT_IN_FONTS == 1)

#include "LCD_screen_standard.h"
// Release check
#if LCD_SCREEN_STANDARD_RELEASE < 116
#error Required LCD_SCREEN_STANDARD_RELEASE 116
#endif

#else

#include "LCD_screen_font.h"
// Release check
#if LCD_SCREEN_FONT_RELEASE < 117
#error Required LCD_SCREEN_FONT_RELEASE 117
#endif

#endif


/////
///// @brief		Hardware SPI selector
///// @details      Comment the line to use software SPI, slower
///// @n			Uncomment the line to use hardware SPI, faster
/////
#include "SPI.h"

#if (BUILT_IN_FONTS == 1)

#ifdef MAX_FONT_SIZE
#undef MAX_FONT_SIZE
#define MAX_FONT_SIZE 4
#else
#define MAX_FONT_SIZE 4
#endif

#endif

// Objects
//
///
/// @brief      Class for 800x400 East Rising 5" RA8875 screen
/// @details    Screen controllers
/// * LCD and touch: RA8875, SPI
///
#if (BUILT_IN_FONTS == 1)
class Screen_ER8875 : public LCD_screen_standard
#else
class Screen_ER8875 : public LCD_screen_font
#endif
{
public:
//    ///
//    /// @brief	Constructor with default pins
//    ///
//    /// @note	Default pins for Arduino Uno / LaunchPad MSP430 / StellarPad LM4F
//    /// @n		? / 10 / 10 =  Chip Select
//    ///
//    Screen_ER8875();
    
    ///
    /// @brief	Constructor
    ///
    /// @param	pinScreenCS digital pin number for screen chip select
    ///
    Screen_ER8875(uint8_t pinScreenCS = 10);
    
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
	/// @brief	Clear the screen
	///
    void clear(uint16_t colour = myColours.black);
    
    
    /// @name       Graphics
    /// @details    The RA8875 includes the primitives for basic graphics
    ///
    /// @{
    
    ///
    /// @brief	Draw circle
    /// @param	x0 center, point coordinate, x-axis
    /// @param	y0 center, point coordinate, y-axis
    /// @param	radius radius
    /// @param	colour 16-bit colour
    ///
    void circle(uint16_t x0, uint16_t y0, uint16_t radius, uint16_t colour);
    
    ///
    /// @brief	Draw line, rectangle coordinates
    /// @param	x1 top left coordinate, x-axis
    /// @param	y1 top left coordinate, y-axis
    /// @param	x2 bottom right coordinate, x-axis
    /// @param	y2 bottom right coordinate, y-axis
    /// @param	colour 16-bit colour
    ///
    void line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t colour);
    
    ///
    /// @brief	Draw triangle, rectangle coordinates
	/// @param	x1 first point coordinate, x-axis
	/// @param	y1 first point coordinate, y-axis
	/// @param	x2 second point coordinate, x-axis
	/// @param	y2 second point coordinate, y-axis
	/// @param	x3 third point coordinate, x-axis
	/// @param	y3 third point coordinate, y-axis
	/// @param	colour 16-bit colour
	///
    /// @n @b	More: @ref Coordinates, @ref Colours
    ///
    void triangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, uint16_t colour);
    
    ///
    /// @brief	Draw rectangle, rectangle coordinates
    /// @param	x1 top left coordinate, x-axis
    /// @param	y1 top left coordinate, y-axis
    /// @param	x2 bottom right coordinate, x-axis
    /// @param	y2 bottom right coordinate, y-axis
    /// @param	colour 16-bit colour
    ///
    /// @n @b	More: @ref Coordinates, @ref Colours
    ///
    void rectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t colour);
    
    ///
    /// @brief	Draw arc
    /// @param	x0 center, point coordinate, x-axis
    /// @param	y0 center, point coordinate, y-axis
    /// @param	radius radius
    /// @param	start starting angle, in degrees
    /// @param	end ending angle, in degrees
    /// @param	colour 16-bit colour
    /// @note   if ending angle < starting angle, then starting angle..360 and 0..starting angle arcs are drawn
    ///
    //void arc(uint16_t x0, uint16_t y0, uint16_t radius, uint16_t start, uint16_t end, uint16_t colour);
    
    ///
    /// @brief	Read pixel colour
    /// @param	x1 point coordinate, x-axis
    /// @param	y1 point coordinate, y-axis
    /// @return	16-bit colour, bits 15-11 red, bits 10-5 green, bits 4-0 blue
    ///
    //    uint16_t readPixel(uint16_t x1, uint16_t y1);
    
    /// @}
    
#if (BUILT_IN_FONTS == 1)
    ///
    /// @name	Text
    /// @details    Text commands
    /// @{
    
    ///
    /// @brief	Set font size
    /// @param	font default=0=small, 1=larger, up to fontMax()-1
    ///
    void setFontSize(uint8_t font = 0);
    
    ///
    /// @brief	Set transparent or opaque text
    /// @param	flag default = 1 = opaque = solid, false = transparent
    /// @warning    Definition for this method is compulsory.
    ///
    void setFontSolid(bool flag = true);
    
    ///
    /// @brief	Number of fonts
    /// @return	number of fonts available
    /// @note   First font is numbered 0, second 1, ...
    /// @n      The latest font is numbered fontMax()-1
    ///
    uint8_t fontMax();
    
    ///
    /// @brief	Font size, x-axis
    /// @return	horizontal size of current font, in pixels
    ///
    uint8_t fontSizeX();
    
    ///
    /// @brief	Font size, y-axis
    /// @return	vertical size of current font, in pixels
    ///
    uint8_t fontSizeY();
    
    ///
    /// @brief	Draw ASCII Text (pixel coordinates) with selection of size
    /// @param	x0 point coordinate, x-axis
    /// @param	y0 point coordinate, y-axis
    /// @param	s text string
    /// @param	textColour 16-bit colour, default = white
    /// @param	backColour 16-bit colour, default = black
    /// @param	ix x-axis font size multiplier, default = 1
    /// @param	iy y-axis font size multiplier, default = 1
    ///
    void gText(uint16_t x0, uint16_t y0,
               String s,
               uint16_t textColour = myColours.white, uint16_t backColour = myColours.black,
               uint8_t ix = 1, uint8_t iy = 1);
    
    /// @}
#endif

    /// @name	Advanced features
    /// @{
    
    ///
    /// @brief	Read pixel colour
    /// @param	x1 point coordinate, x-axis
    /// @param	y1 point coordinate, y-axis
    /// @return	16-bit colour, bits 15-11 red, bits 10-5 green, bits 4-0 blue
    /// @note   This feature requires a readable screen.
    ///
    /// @n @b	More: @ref Coordinates, @ref Colours, @ref Screens
    ///
    virtual uint16_t readPixel(uint16_t x1, uint16_t y1);
    
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
    
    /// @}
    
    
    /// @name	Fast area features
    /// @details	Over-ridden RA8875 specific functions
    /// @{
    
    ///
    /// @brief	Open an area for fast sequential writing
	/// @param	x0 source top left coordinate, x-axis
	/// @param	y0 source top left coordinate, y-axis
    /// @param	dx target to be pasted, x-axis
    /// @param	dy target to be pasted, y-axis
    /// @warning    Specific over-ridden procedure for K70
    ///
    /// @n @b	More: @ref Coordinates
    ///
    void openArea(uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy);
    
    ///
    /// @brief	Feed the area with colour
    /// @details	Maximum dx*dy pixels, no check
    /// @param	colour 16-bit colour
    /// @note   Pixels are displayed from left to right, top to bottom
    /// @warning    Specific over-ridden procedure for K70
    ///
    /// @n @b	More: @ref Colours
    ///
    void feedArea(uint16_t colour);
    
    ///
    /// @brief	Close the area previously opened
    /// @warning    Specific over-ridden procedure for K70
    ///
    void closeArea();
    
    /// @}
    
protected:
	// * Virtual =0 compulsory functions
    // Orientation
    void _setOrientation(uint8_t orientation); // compulsory
    void _orientCoordinates(uint16_t &x1, uint16_t &y1); // compulsory
    
	// Position
    void _setWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1); // compulsory *
	void _setPoint(uint16_t x1, uint16_t y1, uint16_t colour); // compulsory *
    void _fastFill(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t colour); // compulsory *
    
    // Write and Read
    void _writeData88(uint8_t dataHigh8, uint8_t dataLow8); // compulsory; *
    
	// Touch
    void _getRawTouch(uint16_t &x0, uint16_t &y0, uint16_t &z0); // compulsory
    
    // * Other functions specific to the screen
    // Communication, write
    void _writeRegister8(uint8_t command8, uint8_t data8);
    void _writeRegister16(uint8_t command8, uint16_t data16);
    void _writeCommand8(uint8_t command8);
    void _writeData8(uint8_t data8);
    void _writeData16(uint16_t data16);
    
    // Communication, read
    uint8_t _readRegister8(uint8_t register8);
    uint8_t _readData8();
    void _readData88(uint8_t &dataHigh8, uint8_t &dataLow8);
    uint16_t _readData16();

    // Position
    void _setWindowRead(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);

    // Set colours
    void _setDrawingCoordinates(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1); // compulsory *
    void _setFrontColour(uint16_t colour);
    void _setBackColour(uint16_t colour);
    
    // Energy
    void _setIntensity(uint8_t intensity); // compulsory
    void _setBacklight(bool flag); // compulsory

    // Variables
    uint8_t _pinScreenCS; // select
    
#if (SPI_MODE == FAST_SPI)
    regtype _portScreenCS;
    regsize _bitScreenCS;
#endif

};

#endif

//#endif

