///
/// @file       Screen_PicasoSGC.h
/// @brief      Library header
/// @details	102
/// @n
/// @n @b		Project LCD_screen Library Suite
/// @n @a       Developed with [embedXcode](http://embedXcode.weebly.com)
///
/// @author     Rei VILO
/// @author     http://embedXcode.weebly.com
/// @date       Dec 10, 2013
/// @version	104
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
/// @see        ReadMe.txt for references
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

#ifndef SCREEN_PICASO_SGC_RELEASE
///
/// @brief	Library release number
///
#define SCREEN_PICASO_SGC_RELEASE 104

///
/// @brief      Fonts
/// @details    Usage
/// * 1 to use the header fonts
/// * 0 to use the screen built-in fonts
///
#define BUILT_IN_FONTS 1

// Other libraries
#ifdef BUILT_IN_FONTS

#include "LCD_screen_standard.h"
#if LCD_SCREEN_STANDARD_RELEASE < 117
#error Required LCD_SCREEN_STANDARD_RELEASE 117
#endif

#else

#include "LCD_screen_font.h"
#if LCD_SCREEN_FONT_RELEASE < 117
#error Required LCD_SCREEN_FONT_RELEASE 117
#endif

#endif

// Objects
///
/// @brief	Screen_PicasoSGC
/// @details	Screen controller
/// * LCD, touch, SD: Picaso, Serial
///
#ifdef BUILT_IN_FONTS
class Screen_PicasoSGC : public LCD_screen_standard
#else
    class Screen_PicasoSGC : public LCD_screen_font
#endif
{
public:
	///
	/// @brief	Constructor
	///
	/// @param	resetPin digital pin number for screen reset
	/// @param	proxyPort Serial port name, hardware of software
	///
	Screen_PicasoSGC(int8_t resetPin, Stream * proxyPort);

	///
	/// @brief	Initialisation
	///
	void begin();

	///
	/// @brief	Set new connection speed
	/// @param	speed default=9600, 19200, 38400 (Arduino limit), 57600, 115200 (chipKIT limit)
	/// @return	0x06=success, 0x15=failure
	///
	void setSpeed(uint32_t speed); // Set new Baud-Rate - 51hex

	///
	/// @brief	Clear the screen
	/// @param	colour 16-bit colour, default=myColours.black
	///
	void clear(uint16_t colour=myColours.black);

	///
	/// @brief	Request information about the screen
	/// @return	string with hardware version
	///
	String WhoAmI();

    ///
    /// @brief	Copy a source area to a target area
	/// @param	x1 source top left coordinate, x-axis
	/// @param	y1 source top left coordinate, y-axis
	/// @param	x2 target top left coordinate, x-axis
	/// @param	y2 target top left coordinate, y-axis
    /// @param	dx width to be copied, x-axis
    /// @param	dy height to be copied, y-axis
    ///
    void copyPasteArea(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t dx, uint16_t dy);

    ///
	/// @brief	Invert screen
	/// @param	flag true to invert, false for normal screen
	///
	void invert(boolean flag);
    
	///
	/// @brief	Switch backlight on or off
	/// @param	flag true=on, false=off
	///
	void setBacklight(boolean flag);
    
	///
	/// @brief	Switch display on or off
	/// @param	flag true=on, false=off
	///
	void setScreen(boolean flag);
    
//	///
//	/// @brief	Set orientation
//	/// @param	orientation orientation, 0=portrait, 1=right rotated landscape, 2=reverse portrait, 3=left rotated landscape
//	///
//	void setOrientation(uint8_t orientation);
//    
    ///
    /// @name	Graphic functions
    /// @{
    
	///
	/// @brief	Draw line, rectangle coordinates
	/// @param	x1 top left coordinate, x-axis
	/// @param	y1 top left coordinate, y-axis
	/// @param	x2 bottom right coordinate, x-axis
	/// @param	y2 bottom right coordinate, y-axis
	/// @param	colour 16-bit colour
	///
	void line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t colour);

	/// @brief	Draw rectangle, rectangle coordinates
	/// @param	x1 top left coordinate, x-axis
	/// @param	y1 top left coordinate, y-axis
	/// @param	x2 bottom right coordinate, x-axis
	/// @param	y2 bottom right coordinate, y-axis
	/// @param	colour 16-bit colour
	///
	void rectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t colour);

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
    void triangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, uint16_t colour);

	///
	/// @brief	Draw circle
	/// @param	x0 center, point coordinate, x-axis
	/// @param	y0 center, point coordinate, y-axis
	/// @param	radius radius
	/// @param	colour 16-bit colour
	///
	void circle(uint16_t x0, uint16_t y0, uint16_t radius, uint16_t colour);

    ///
    /// @brief	Read pixel colour
    /// @param	x1 point coordinate, x-axis
    /// @param	y1 point coordinate, y-axis
    /// @return	16-bit colour, bits 15-11 red, bits 10-5 green, bits 4-0 blue
    ///
    uint16_t readPixel(uint16_t x1, uint16_t y1);
    
    /// @}
    
    ///
    /// @name	Text functions
    /// @{
#ifdef BUILT_IN_FONTS
    
	///
	/// @brief	Select font
	/// @param	font default = 0 = small, 1 = large
	///
	void setFontSize(uint8_t font = 0);

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

//	///
//	/// @brief	Set transparent or opaque text
//	/// @param	flag default = 1 = opaque = solid, false = transparent
//	///
//    void setFontSolid(bool flag = true);
//
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
#endif
    /// @}
    
	///
	/// @name Touch functions
	/// @{
    
    ///
    /// @brief	Poll touch
    /// @param	x x coordinate
    /// @param	y y coordinate
    /// @param	z z coordinate=pressure
    /// @return	true if pressed
    ///
	bool getTouch(uint16_t &x, uint16_t &y, uint16_t &z);

    /// @}
    
private:
	uint8_t _a;
	uint16_t _b;
    bool _fontSolid;

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
	void _writeCommand8(uint8_t command, uint8_t data);
	void _writeCommand88(uint8_t command, uint8_t data0, uint8_t data1);
	void _writeCommand16(uint8_t command, uint16_t data);

    // Other variables
	uint8_t nacAck();
	void _print16(uint16_t ui);
	void _printXY(uint16_t x, uint16_t y);
	uint16_t _read16();

	// Variables
	int8_t _resetPin; // screen reset
	Stream * _port;
	uint8_t _touch_buffer[4];
	uint8_t _font, _fontX, _fontY;
};

#endif
