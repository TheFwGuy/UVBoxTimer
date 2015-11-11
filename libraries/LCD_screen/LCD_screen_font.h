///
/// @file       LCD_screen_font.h
/// @brief      Class library header
/// @details	Generic LCD with font class library
/// @n
/// @n @b       Project LCD_screen Library Suite
/// @n @a       Developed with [embedXcode](http://embedXcode.weebly.com)
///
/// @author     Rei VILO
/// @author     http://embedXcode.weebly.com
/// @date		May 13, 2014
/// @version	225
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

#ifndef LCD_SCREEN_FONT_RELEASE
///
/// @brief	Library release number
///
#define LCD_SCREEN_FONT_RELEASE 225

// Other libraries
#include "LCD_screen_virtual.h"

#if (LCD_SCREEN_VIRTUAL_RELEASE < 225)
#error Required LCD_SCREEN_VIRTUAL_RELEASE 225
#endif

///
/// @brief      Biggest font size
/// @details    Based on the MCU, by default=0
///
#if defined(__LM4F120H5QR__) || defined(__TM4C1230C3PM__) || defined(__TM4C129XNCZAD__) || defined(__32MX320F128H__) || defined(__AVR_ATmega2560__)
#define MAX_FONT_SIZE 4
#elif defined(__MSP430F5529__)
#define MAX_FONT_SIZE 2
#else
#define MAX_FONT_SIZE 1
#endif

//#undef MAX_FONT_SIZE
//#define MAX_FONT_SIZE 1

#if (MAX_FONT_SIZE > 0)
#include "Terminal6e.h"
#if (MAX_FONT_SIZE > 1)
#include "Terminal8e.h"
#if (MAX_FONT_SIZE > 2)
#include "Terminal12e.h"
#if (MAX_FONT_SIZE > 3)
#include "Terminal16e.h"
#endif
#endif
#endif
#endif

///
/// @brief	Generic class for LCD
///
class LCD_screen_font : public LCD_screen_virtual {
public:
    ///
    /// @brief	Constructor
    ///
    LCD_screen_font();
    
    /// @name	General
    /// @{
    
	///
	/// @brief	Initialisation
	/// @warning    Definition for this method is compulsory.
	///
	virtual void begin() =0; // compulsory
    
	///
	/// @brief	Request information about the screen
	/// @return	string with hardware version
	/// @warning    Definition for this method is compulsory.
	///
	virtual String WhoAmI() =0; // compulsory
    
	///
	/// @brief	Clear the screen
	///
    void clear(uint16_t colour = myColours.black);
    
	///
	/// @brief	Set orientation
	/// @param	orientation orientation, 0 = portrait, 1 = right rotated landscape, 2 = reverse portrait, 3 = left rotated landscape
	///
	virtual void setOrientation(uint8_t orientation);
    
    ///
    /// @brief	Get orientation
    /// @return	orientation orientation, 0 = portrait, 1 = right rotated landscape, 2 = reverse portrait, 3 = left rotated landscape
    ///
    uint8_t getOrientation();
    
	///
	/// @brief	Show information
	/// @param	x0  left coordinate, x-axis, default=0
	/// @param	y0  top coordinate, y-axis, default=0
	/// @details	Display information: screen, size, fonts, touch
	///
	virtual void showInformation(uint16_t &x0, uint16_t &y0);
    
	///
    /// @brief	Screen size, x-axis
    /// @return	horizontal size of the screen, in pixels
    /// @note	240 means 240 pixels and thus 0..239 coordinates (decimal)
    ///
    virtual uint16_t screenSizeX();
    
    ///
    /// @brief	Screen size, y-axis
    /// @return	vertical size of the screen, in pixels
    /// @note	240 means 240 pixels and thus 0..239 coordinates (decimal)
    ///
    virtual uint16_t screenSizeY();
    
	///
    /// @brief	Screen size, diagonal
    /// @return	diagonal size of the screen, in inches x 10
    /// @note	32 means 3.2"
    ///
    virtual uint16_t screenDiagonal();

    /// @}
    
    /// @name	Graphics
    /// @{
    
    ///
    /// @brief	Draw circle
    /// @param	x0 center, point coordinate, x-axis
    /// @param	y0 center, point coordinate, y-axis
    /// @param	radius radius
    /// @param	colour 16-bit colour
    ///
    virtual void circle(uint16_t x0, uint16_t y0, uint16_t radius, uint16_t colour);
    
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
    virtual void arc(uint16_t x0, uint16_t y0, uint16_t radius, uint16_t start, uint16_t end, uint16_t colour);
    
    ///
    /// @brief	Draw line, rectangle coordinates
    /// @param	x1 top left coordinate, x-axis
    /// @param	y1 top left coordinate, y-axis
    /// @param	x2 bottom right coordinate, x-axis
    /// @param	y2 bottom right coordinate, y-axis
    /// @param	colour 16-bit colour
    ///
    virtual void line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t colour);
    
    ///
    /// @brief	Draw line, vector coordinates
    /// @param	x0 point coordinate, x-axis
    /// @param	y0 point coordinate, y-axis
    /// @param	dx length, x-axis
    /// @param	dy height, y-axis
    /// @param	colour 16-bit colour
    ///
    /// @n @b	More: @ref Coordinates, @ref Colours
    ///
    virtual void dLine(uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy, uint16_t colour);
    
    ///
    /// @brief	Set pen opaque
    /// @param	flag default = true = opaque = solid, false = wire frame
    ///
    virtual void setPenSolid(bool flag = true);
    
    ///
    /// @brief	Draw triangle, rectangle coordinates
	/// @param	x1 first point coordinate, x-axis
	/// @param	y1 first point coordinate, y-axis
	/// @param	x2 second point coordinate, x-axis
	/// @param	y2 second point coordinate, y-axis
	/// @param	x3 third point coordinate, x-axis
	/// @param	y3 third point coordinate, y-axis
	/// @param	colour 16-bit colour
    /// @note   If two points are equal, draw a line
	///
    /// @n @b	More: @ref Coordinates, @ref Colours
    ///
    virtual void triangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, uint16_t colour);
    
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
    virtual void rectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t colour);
    
    ///
    /// @brief	Draw rectangle, vector coordinates
    /// @param	x0 point coordinate, x-axis
    /// @param	y0 point coordinate, y-axis
    /// @param	dx length, x-axis
    /// @param	dy height, y-axis
    /// @param	colour 16-bit colour
    ///
    /// @n @b	More: @ref Coordinates, @ref Colours
    ///
    virtual void dRectangle(uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy, uint16_t colour);
    
    ///
    /// @brief	Draw pixel
    /// @param	x1 point coordinate, x-axis
    /// @param	y1 point coordinate, y-axis
    /// @param	colour 16-bit colour
    ///
    /// @n @b	More: @ref Coordinates, @ref Colours
    ///
    virtual void point(uint16_t x1, uint16_t y1, uint16_t colour);
    
    ///
    /// @brief	Read pixel colour
    /// @param	x1 point coordinate, x-axis
    /// @param	y1 point coordinate, y-axis
    /// @return	16-bit colour, bits 15-11 red, bits 10-5 green, bits 4-0 blue
    ///
    //    uint16_t readPixel(uint16_t x1, uint16_t y1);
    
    /// @}
    
    /// @name	Fast area features
    /// @details	Specific to each controller
    /// @{
    
    ///
    /// @brief	Is area available?
    /// @return	true if area available, false otherwise
    ///
    /// @n @b	More: @ref Area
    ///
    virtual bool isArea();
    
    ///
    /// @brief	Open an area for fast sequential writing, vector coordinates
	/// @param	x0 source top left coordinate, x-axis
	/// @param	y0 source top left coordinate, y-axis
    /// @param	dx target to be pasted, x-axis
    /// @param	dy target to be pasted, y-axis
    ///
    /// @n @b	More: @ref Area, @ref Coordinates
    ///
    virtual void openArea(uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy);
    
    ///
    /// @brief	Feed the area with colour
    /// @details	Maximum dx*dy pixels
    /// @warning    No check!
    /// @param	colour 16-bit colour
    /// @note   Pixels are displayed from left to right, top to bottom
    ///
    /// @n @b	More: @ref Area, @ref Colours
    ///
    virtual void feedArea(uint16_t colour);
    
    ///
    /// @brief	Close the area previously opened
    ///
    /// @n @b	More: @ref Area
    ///
    virtual void closeArea();
    
    /// @}
    
    /// @name	Text
    /// @{
    
	///
	/// @brief	Select font size
	/// @param	size default = 0 = small, 1 = large
	/// @warning    Definition for this method is compulsory.
	///
	virtual void setFontSize(uint8_t size);
    
	///
	/// @brief	Set transparent or opaque text
	/// @param	flag default = 1 = opaque = solid, false = transparent
	/// @warning    Definition for this method is compulsory.
	///
	virtual void setFontSolid(bool flag = true);

	///
	/// @brief	Font size, x-axis
	/// @return	horizontal size of current font, in pixels
	///
	virtual uint8_t fontSizeX();
    
	///
	/// @brief	Font size, y-axis
	/// @return	vertical size of current font, in pixels
	///
	virtual uint8_t fontSizeY();
    
    ///
	/// @brief	Number of fonts
	/// @return	number of fonts available
    /// @note   First font is numbered 0, second 1, ...
    /// @n      The latest font is numbered fontMax()-1
	///
	virtual uint8_t fontMax();
    
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
    /// @n @b	More: @ref Colours
    ///
	virtual void gText(uint16_t x0, uint16_t y0,
                       String s,
                       uint16_t textColour = myColours.white, uint16_t backColour = myColours.black,
                       uint8_t ix = 1, uint8_t iy = 1);
    
	/// @}
    
    /// @name	Energy features
    /// @{
    
    ///
    /// @brief	Does the screen provide energy features?
    /// @return	true is energy featured, false otherwise
    ///
    /// @n @b	More: @ref Energy
    ///
    bool isEnergy();
    
    ///
    /// @brief	Is screen backlight variable?
    /// @return	true is backlight variable, false otherwise
    ///
    /// @n @b	More: @ref Screens
    ///
    bool isIntensity();
    
    ///
    /// @brief		Turn display backlight on or off
    /// @param      state true=high=on, false=low=off
    /// @note       Feature specific to the SSD1963
    ///
    void setBacklight(bool state);
    
    ///
    /// @brief		Set backlight intensity
    /// @param      intensity 0..255
    /// @note       Feature specific to the SSD1963
    ///
    void setIntensity(uint8_t intensity);

    ///
    /// @brief      Set delay for power saving
    /// @param      seconds idle period in seconds before power saving, default=6 s
    /// @note       Set seconds=0 to disable
    /// @note       Feature specific to the SSD1963
    ///
    void setEnergy(uint32_t seconds=6);
    
    ///
    /// @brief      Update power saving
    /// @param      flag true=update or start new period or false=check period
    /// @note       Feature specific to the SSD1963
    ///
    void updateEnergy(bool state);
    
	/// @}
    
    /// @name	Advanced features
    /// @{
    
    ///
    /// @brief	Is screen readable?
    /// @return	true is screen readable, false otherwise
    ///
    /// @n @b	More: @ref Screens
    ///
    bool isReadable();
    
    ///
    /// @brief	Does the screen feature an external storage?
    /// @return	true is storage available, false otherwise
    ///
    /// @n @b	More: @ref Storage
    ///
    bool isStorage();
    
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
    /// @n @b	More: @ref Coordinates, @ref Screens, @ref Storage
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
    
    
    /// @name	Touch
    /// @note   Those functions are optional, and thus defined as empty and not compulsory.
    /// @{
    
    ///
    /// @brief	Is touch available?
    /// @return	true is touch available, false otherwise
    ///
    bool isTouch();
    
    ///
    /// @brief	Poll touch
    /// @param	x x coordinate
    /// @param	y y coordinate
    /// @param	z z coordinate=pressure
    /// @return	true if pressed
    ///
    bool getTouch(uint16_t &x, uint16_t &y, uint16_t &z);
    
    ///
    /// @brief	Calibrate the touch
    ///
    void calibrateTouch();
    
    /// @}
    
    
protected:
//	uint8_t      _fontX, _fontY, _fontSize;
//	uint8_t      _orientation;
//	bool         _penSolid, _fontSolid, _flagRead, _flagStorage;
//	uint16_t     _screenWidth, _screenHeigth;
//	uint8_t      _touchTrim;
//	uint16_t     _touchXmin, _touchXmax, _touchYmin, _touchYmax;
    
	// * Virtual =0 compulsory functions
    // Orientation
    virtual void _setOrientation(uint8_t orientation) =0; // compulsory
    virtual void _orientCoordinates(uint16_t &x1, uint16_t &y1) =0; // compulsory
    
	// Position
    virtual void _setWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) =0; // compulsory
    virtual void _closeWindow() =0; // compulsory
	virtual void _setPoint(uint16_t x1, uint16_t y1, uint16_t colour) =0; // compulsory
    virtual void _fastFill(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t colour) =0; // compulsory
    
    // Write and Read
    virtual void _writeData88(uint8_t dataHigh8, uint8_t dataLow8) =0; // compulsory;
    
	// Touch
    virtual void _getRawTouch(uint16_t &x0, uint16_t &y0, uint16_t &z0) =0; // compulsory
    
    // Other functions
	// required by calibrateTouch
	void         _displayTarget(uint16_t x0, uint16_t y0, uint16_t colour);

    // required by triangle()
    void         _triangleArea(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, uint16_t colour);

    // required by gText()
    uint8_t _getCharacter(uint8_t c, uint8_t i);
    
    // Energy
    uint32_t _msEnergy;
    uint32_t _chronoEnergy;
    bool     _stateEnergy;
    
    // required by setBacklight() and setBacklightPWM
    bool     _flagIntensity;
    virtual void _setIntensity(uint8_t intensity) =0; // compulsory
    virtual void _setBacklight(bool flag) =0; // compulsory
    
	// Utilities
	void         _swap(int16_t &a, int16_t &b);
	void         _swap(uint16_t &a, uint16_t &b);
	void         _swap(uint8_t &a, uint8_t &b);
	uint16_t     _check(uint16_t x0, uint16_t xmin, uint16_t xmax);
};

#endif
