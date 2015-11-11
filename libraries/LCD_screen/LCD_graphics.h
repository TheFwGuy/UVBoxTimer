///
/// @file       LCD_graphics.h
/// @brief      Library header
/// @details	Graphics library for LCD_screen
/// @n	
/// @n @b       Project LCD_screen Library Suite
/// @n @a       Developed with [embedXcode](http://embedXcode.weebly.com)
/// 
/// @author     Rei VILO
/// @author     http://embedXcode.weebly.com
/// @date       Jun 04, 2013
/// @version	420
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

#ifndef LCD_GRAPHICS_RELEASE
///
/// @brief	Library release number
///
#define LCD_GRAPHICS_RELEASE 420

// Other libraries
#include "LCD_screen_virtual.h"

#if LCD_SCREEN_VIRTUAL_RELEASE < 114
#error Required LCD_SCREEN_VIRTUAL_RELEASE 114
#endif

///
/// @brief	Meta-class for graphics based on a dial
/// @details	mtgDial is a meta-class for dial-based graphics
///
class mtgDial {
public:
    ///
    /// @brief	Constructor
    ///
    mtgDial();
    
protected:
    void drawDial(LCD_screen_virtual * lcd,
                  uint16_t x0, uint16_t y0, uint16_t radius,
                  uint16_t backColour, uint16_t frontColour,
                  uint16_t gridColour, uint16_t valueColour);
    
    LCD_screen_virtual  * _pscreen;
    uint16_t    _x0, _y0, _radius;
    uint16_t    _backColour, _frontColour;
    uint16_t    _gridColour, _valueColour;
};

///
/// @class	gClock
/// @brief	Display an analog clock
///
class gClock: public mtgDial {
public:
    ///
    /// @brief	Constructor
    ///
    gClock();
    
    ///
    /// @brief	Define the clock, vector coordinates
    /// @param	lcd &LCD to which the graphic belongs
    /// @param	x0 point coordinate, x-axis
    /// @param	y0 point coordinate, y-axis
    /// @param	dx length, x-axis
    /// @param	dy height, y-axis
    /// @param	backColour 16-bit background colour, default=black, optional
    /// @param	frontColour 16-bit colour for dial, default=green, optional
    /// @param	hourColour 16-bit colour for hours, default=red, optional
    /// @param	minuteColour 16-bit colour for minutes, default=blue, optional
    /// @param	secondColour 16-bit colour for seconds, default=yellow, optional
    ///
    /// @n @b	More: @ref Coordinates, @ref Colours
    ///
    void dDefine(LCD_screen_virtual * lcd,
                 uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy,
                 uint16_t backColour=myColours.black, uint16_t frontColour=myColours.green,
                 uint16_t hourColour=myColours.red, uint16_t minuteColour=myColours.blue, uint16_t secondColour=myColours.yellow);
    
    ///
    /// @brief	Define the clock, rectangle coordinates
    /// @param	lcd &LCD to which the graphic belongs
    /// @param	x0 center, point coordinate, x-axis
    /// @param	y0 center, point coordinate, y-axis
    /// @param	radius radius
    /// @param	backColour 16-bit background colour, default=black, optional
    /// @param	frontColour 16-bit colour for dial, default=green, optional
    /// @param	hourColour 16-bit colour for hours, default=red, optional
    /// @param	minuteColour 16-bit colour for minutes, default=blue, optional
    /// @param	secondColour 16-bit colour for seconds, default=yellow, optional
    ///
    /// @n @b	More: @ref Coordinates, @ref Colours
    ///
    void define(LCD_screen_virtual * lcd,
                uint16_t x0, uint16_t y0, uint16_t radius,
                uint16_t backColour=myColours.black, uint16_t frontColour=myColours.green,
                uint16_t hourColour=myColours.red, uint16_t minuteColour=myColours.blue, uint16_t secondColour=myColours.yellow);
    
    ///
    /// @brief	Update and draw the clock
    /// @param	hour hours, 0..23
    /// @param	minute minutes, 0.59
    /// @param	second seconds, 0..49
    /// @param  text text to be displayed
    ///
    void draw(uint8_t hour, uint8_t minute, uint8_t second, String text="");
    
private:
    void _needle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t colour);
    uint16_t    _hourColour, _minuteColour, _secondColour;
    int32_t     _oldHourX, _oldHourY, _oldMinuteX, _oldMinuteY, _oldSecondX, _oldSecondY, _oldK;
};

///
/// @class	gGauge
/// @brief	Display a gauge
///
class gGauge: public mtgDial {
public:
    ///
    /// @brief	Constructor
    ///
    gGauge();
    
    ///
    /// @brief	Define the graphic, rectangle coordinates
    /// @param	lcd &LCD to which the graphic belongs
    /// @param	x0 center, point coordinate, x-axis
    /// @param	y0 center, point coordinate, y-axis
    /// @param	radius radius
    /// @param	valueMin minimum value for measures
    /// @param	valueMax maximum value for measures, already multiplied by unit
    /// @param  unit multiplier for all values, default=1, 10 or 100
    /// @param	memory keep highs and low during a number of measures, default=8, 0=no memory
    /// @param	grid number of areas for measures, default=8, 0=no grid
    /// @param	backColour 16-bit background colour, default=black, optional
    /// @param	frontColour 16-bit colour for text, default=white, optional
    /// @param	gridColour 16-bit colour for grid, default=yellow, optional
    /// @param	valueColour 16-bit colour for measures, default=blue, optional
    /// @param	minColour 16-bit colour for lows, default=green, optional
    /// @param	maxColour 16-bit colour for highs, default=red, optional
    ///
    /// @n @b	More: @ref Coordinates, @ref Colours, @ref Values
    ///
    void define(LCD_screen_virtual * lcd,
                uint16_t x0, uint16_t y0, uint16_t radius,
                int32_t valueMin, int32_t valueMax,
                int32_t unit=1,
                uint16_t memory=8, uint16_t grid=8,
                uint16_t backColour=myColours.black, uint16_t frontColour=myColours.white, uint16_t gridColour=myColours.yellow,
                uint16_t valueColour=myColours.blue, uint16_t minColour=myColours.green, uint16_t maxColour=myColours.red);
    
    ///
    /// @brief	Define the graphic, vector coordinates
    /// @param	lcd &LCD to which the graphic belongs
    /// @param	x0 point coordinate, x-axis
    /// @param	y0 point coordinate, y-axis
    /// @param	dx length, x-axis
    /// @param	dy height, y-axis
    /// @param	valueMin minimum value for measures, already multiplied by unit
    /// @param	valueMax maximum value for measures, already multiplied by unit
    /// @param  unit multiplier for all values, default=1, 10 or 100
    /// @param	memory keep highs and low during a number of measures, default=8, 0=no memory
    /// @param	grid number of areas for measures, default=8, 0=no grid
    /// @param	backColour 16-bit background colour, default=black, optional
    /// @param	frontColour 16-bit colour for text, default=white, optional
    /// @param	gridColour 16-bit colour for grid, default=yellow, optional
    /// @param	valueColour 16-bit colour for measures, default=blue, optional
    /// @param	minColour 16-bit colour for lows, default=green, optional
    /// @param	maxColour 16-bit colour for highs, default=red, optional
    ///
    /// @n @b	More: @ref Coordinates, @ref Colours, @ref Values
    ///
    void dDefine(LCD_screen_virtual * lcd,
                 uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy,
                 int32_t valueMin, int32_t valueMax,
                 int32_t unit=1,
                 uint16_t memory=8, uint16_t grid=8,
                 uint16_t backColour=myColours.black, uint16_t frontColour=myColours.white, uint16_t gridColour=myColours.yellow,
                 uint16_t valueColour=myColours.blue, uint16_t minColour=myColours.green, uint16_t maxColour=myColours.red);
    
    ///
    /// @brief	Update the graphic
    /// @param	value new measure, already multiplied by unit
    /// @param	stringValue label, default="", optional
    /// @note   If value=1234 with unit=100, actual value is 1234/100=12.34
    ///
    /// @n @b	More: @ref Values
    ///
    void draw(int32_t value, String stringValue="");
    
private:
    void needle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t colour);

    uint16_t    _dx, _dy;
    int32_t     _valueMin, _valueMax, _unit;
    int32_t     _oldZ, _minZ, _maxZ, _oldK;
    uint16_t    _n, _amnesiaMin, _amnesiaMax;
    uint32_t    _lapse;
    uint16_t    _memory, _grid;
    uint16_t    _minColour, _maxColour;
};

///
/// @class	gYaw
/// @brief	Specific display for yaw angle
///
class gYaw: public mtgDial {
public:
    ///
    /// @brief	Constructor
    ///
    gYaw();
    
    ///
    /// @brief	Define the graphic, vector coordinates
    /// @param	lcd &LCD to which the graphic belongs
    /// @param	x0 point coordinate, x-axis
    /// @param	y0 point coordinate, y-axis
    /// @param	dx length, x-axis
    /// @param	dy height, y-axis
    /// @param  unit multiplier for all values, default=1, 10 or 100
    /// @param	backColour 16-bit background colour, default=black, optional
    /// @param	frontColour 16-bit colour for text, default=white, optional
    /// @param	gridColour 16-bit colour for grid, default=yellow, optional
    /// @param	valueColour 16-bit colour for measures, default=blue, optional
    ///
    /// @n @b	More: @ref Coordinates, @ref Colours, @ref Values
    ///
    void dDefine(LCD_screen_virtual * lcd,
                 uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy,
                 int32_t unit=1,
                 uint16_t backColour=myColours.black, uint16_t frontColour=myColours.white,
                 uint16_t gridColour=myColours.yellow, uint16_t valueColour=myColours.blue);
    
    ///
    /// @brief	Define the graphic, rectangle coordinates
    /// @param	lcd &LCD to which the graphic belongs
    /// @param	x0 center, point coordinate, x-axis
    /// @param	y0 center, point coordinate, y-axis
    /// @param	radius radius
    /// @param  unit multiplier for all values, default=1, 10 or 100
    /// @param	backColour 16-bit background colour, default=black, optional
    /// @param	frontColour 16-bit colour for text, default=white, optional
    /// @param	gridColour 16-bit colour for grid, default=yellow, optional
    /// @param	valueColour 16-bit colour for measures, default=blue, optional
    ///
    /// @n @b	More: @ref Coordinates, @ref Colours, @ref Values
    ///
    void define(LCD_screen_virtual * lcd,
                uint16_t x0, uint16_t y0, uint16_t radius,
                int32_t unit=1,
                uint16_t backColour=myColours.black, uint16_t frontColour=myColours.white,
                uint16_t gridColour=myColours.yellow, uint16_t valueColour=myColours.blue);
    
    ///
    /// @brief	Update the graphic
    /// @param	valueDegrees yaw angle in degrees, already multiplied by unit
    /// @note   If valueDegrees=9000 with unit=100, actual angle is 9000 / 100 = 90.00
    ///
    /// @n @b	More: @ref Values
    ///
    void draw(int32_t valueDegrees);
    
private:
    int32_t     _oldDegrees, _unit;
};


///
/// @class	gPitch
/// @brief	Specific display for pitch angle
///
class gPitch: public mtgDial {
public:
    ///
    /// @brief	Constructor
    ///
    gPitch();
    
    ///
    /// @brief	Define the graphic, vector coordinates
    /// @param	lcd &LCD to which the graphic belongs
    /// @param	x0 point coordinate, x-axis
    /// @param	y0 point coordinate, y-axis
    /// @param	dx length, x-axis
    /// @param	dy height, y-axis
    /// @param  unit multiplier for all values, default=1, 10 or 100
    /// @param	backColour 16-bit background colour, default=black, optional
    /// @param	frontColour 16-bit colour for text, default=white, optional
    /// @param	gridColour 16-bit colour for grid, default=yellow, optional
    /// @param	valueColour 16-bit colour for measures, default=blue, optional
    ///
    /// @n @b	More: @ref Coordinates, @ref Colours, @ref Values
    ///
    void dDefine(LCD_screen_virtual * lcd,
                 uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy,
                 int32_t unit=1,
                 uint16_t backColour=myColours.black, uint16_t frontColour=myColours.white,
                 uint16_t gridColour=myColours.yellow, uint16_t valueColour=myColours.blue);
    
    ///
    /// @brief	Define the graphic, rectangle coordinates
    /// @param	lcd &LCD to which the graphic belongs
    /// @param	x0 center, point coordinate, x-axis
    /// @param	y0 center, point coordinate, y-axis
    /// @param	radius radius
    /// @param  unit multiplier for all values, default=1, 10 or 100
    /// @param	backColour 16-bit background colour, default=black, optional
    /// @param	frontColour 16-bit colour for text, default=white, optional
    /// @param	gridColour 16-bit colour for grid, default=yellow, optional
    /// @param	valueColour 16-bit colour for measures, default=blue, optional
    ///
    /// @n @b	More: @ref Coordinates, @ref Colours, @ref Values
    ///
    void define(LCD_screen_virtual * lcd,
                uint16_t x0, uint16_t y0, uint16_t radius,
                int32_t unit=1,
                uint16_t backColour=myColours.black, uint16_t frontColour=myColours.white,
                uint16_t gridColour=myColours.yellow, uint16_t valueColour=myColours.blue);
    
    ///
    /// @brief	Update the graphic
    /// @param	valueDegrees pitch angle in degrees, already multiplied by unit
    /// @note   If valueDegrees=9000 with unit=100, actual angle is 9000 / 100 = 90.00
    ///
    /// @n @b	More: @ref Values
    ///
    void draw(int32_t valueDegrees);
    
private:
    int32_t     _oldDegrees, _unit;
};


///
/// @class	gRoll
/// @brief	Specific display for roll angle
///
class gRoll: public mtgDial {
public:
    ///
    /// @brief	Constructor
    ///
    gRoll();
    
    ///
    /// @brief	Define the graphic, vector coordinates
    /// @param	lcd &LCD to which the graphic belongs
    /// @param	x0 point coordinate, x-axis
    /// @param	y0 point coordinate, y-axis
    /// @param	dx length, x-axis
    /// @param	dy height, y-axis
    /// @param  unit multiplier for all values, default=1, 10 or 100
    /// @param	backColour 16-bit background colour, default=black, optional
    /// @param	frontColour 16-bit colour for text, default=white, optional
    /// @param	gridColour 16-bit colour for grid, default=yellow, optional
    /// @param	valueColour 16-bit colour for measures, default=blue, optional
    ///
    /// @n @b	More: @ref Coordinates, @ref Colours, @ref Values
    ///
    void dDefine(LCD_screen_virtual * lcd,
                 uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy,
                 int32_t unit=1,
                 uint16_t backColour=myColours.black, uint16_t frontColour=myColours.white,
                 uint16_t gridColour=myColours.yellow, uint16_t valueColour=myColours.blue);
    
    ///
    /// @brief	Define the graphic, rectangle coordinates
    /// @param	lcd &LCD to which the graphic belongs
    /// @param	x0 center, point coordinate, x-axis
    /// @param	y0 center, point coordinate, y-axis
    /// @param	radius radius
    /// @param  unit multiplier for all values, default=1, 10 or 100
    /// @param	backColour 16-bit background colour, default=black, optional
    /// @param	frontColour 16-bit colour for text, default=white, optional
    /// @param	gridColour 16-bit colour for grid, default=yellow, optional
    /// @param	valueColour 16-bit colour for measures, default=blue, optional
    ///
    /// @n @b	More: @ref Coordinates, @ref Colours, @ref Values
    ///
    void define(LCD_screen_virtual * lcd,
                uint16_t x0, uint16_t y0, uint16_t radius,
                int32_t unit=1,
                uint16_t backColour=myColours.black, uint16_t frontColour=myColours.white,
                uint16_t gridColour=myColours.yellow, uint16_t valueColour=myColours.blue);
    
    ///
    /// @brief	Update the graphic
    /// @param	valueDegrees roll angle in degrees, already multiplied by unit
    /// @note   If valueDegrees=9000 with unit=100, actual angle is 9000 / 100 = 90.00
    ///
    /// @n @b	More: @ref Values
    ///
    void draw(int32_t valueDegrees);
    
private:
    int32_t     _oldDegrees, _unit;
};

///
/// @brief	Meta-Class for graphics based on a rectangular panel
/// @details	mtgPanel is a meta-class for panel-based graphics
///
class mtgPanel {
public:
    ///
    /// @brief	Constructor
    ///
    mtgPanel();
    
protected:
    void drawPanel(LCD_screen_virtual * lcd,
                   uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy,
                   uint16_t lapse, uint16_t gridX, uint16_t gridY, bool continous,
                   uint16_t backColour, uint16_t frontColour, uint16_t gridColour);
    void drawContinue();
    void drawLapse(uint32_t ms);
    
    LCD_screen_virtual  * _pscreen;
    uint16_t    _x0, _y0, _dx, _dy;
    uint16_t    _n;
    uint16_t    _lapse;
    bool        _continous;
    uint16_t    _gridX, _gridY, _gridC;
    uint16_t    _backColour, _frontColour, _gridColour;
    uint32_t    _chrono;
};


///
/// @class	gTable
/// @brief	Display a table
///
class gTable: public mtgPanel {
    
public:
    ///
    /// @brief	Constructor
    ///
    gTable();
    
    /// @brief	Define the graphic, vector coordinates
    /// @param	lcd &LCD to which the graphic belongs
    /// @param	x0 point coordinate, x-axis
    /// @param	y0 point coordinate, y-axis
    /// @param	dx length, x-axis
    /// @param	dy height, y-axis
    /// @param	gridX number of columns, default=4
    /// @param	gridY number of rows, default=4
    /// @param	backColour 16-bit background colour, default=black, optional
    /// @param	gridColour 16-bit colour for grid, default=yellow, optional
    ///
    /// @note 	The touch on table is disabled at initialisation.
    ///
    /// @n @b	More: @ref Coordinates, @ref Colours
    ///
    void dDefine(LCD_screen_virtual * lcd,
                 uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy,
                 uint8_t gridX=4, uint8_t gridY=4,
                 uint16_t backColour=myColours.black, uint16_t gridColour=myColours.yellow);
    
    /// @brief	Define the graphic, rectangle coordinates
    /// @param	lcd &LCD to which the graphic belongs
    /// @param	x1 top left coordinate, x-axis
    /// @param	y1 top left coordinate, y-axis
    /// @param	x2 bottom right coordinate, x-axis
    /// @param	y2 bottom right coordinate, y-axis    /// @param	gridX number of columns, default=4
    /// @param	gridY number of rows, default=4
    /// @param	backColour 16-bit background colour, default=black, optional
    /// @param	gridColour 16-bit colour for grid, default=yellow, optional
    ///
    /// @note 	The touch on table is disabled at initialisation.
    ///
    /// @n @b	More: @ref Coordinates, @ref Colours
    ///
    void define(LCD_screen_virtual * lcd,
                uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,
                uint8_t gridX=4, uint8_t gridY=4,
                uint16_t backColour=myColours.black, uint16_t gridColour=myColours.yellow);
    
    
    /// @brief	Update the graphic
    /// @param	ix cell to update, column number
    /// @param	iy cell to update, row number
    /// @param	colour colour for the cell default=myColours.black
    ///
    void draw(uint8_t ix, uint8_t iy, uint16_t colour=myColours.black);
    
    ///
    /// @brief	Define state of the table
    /// @param	flag default=true=area enabled, false=at initialisation
    ///
    void enable(bool flag=true);
    
    ///
    /// @brief	Check if area is pressed
    /// @param	instant default=false area waits for release, instant=true area is activated by press only
    /// @return	true if one cell pressed
    ///
    bool check(bool instant=false);
    
    ///
    /// @brief	Return column and row numbers of the cell touched
    /// @param	ix cell touched, column number
    /// @param	iy cell touched, row number
    ///
    void getIndex(uint8_t &ix, uint8_t &iy);
    
private:
    bool    _enable;
    uint8_t _ix, _iy;
};

///
/// @class	gHistogram
/// @brief	Display a time-based histogram
///
class gHistogram: public mtgPanel {
public:
    ///
    /// @brief	Constructor
    ///
    gHistogram();
    
    ///
    /// @brief	Define the graphic, vector coordinates
    /// @param	lcd &LCD to which the graphic belongs
    /// @param	x0 point coordinate, x-axis
    /// @param	y0 point coordinate, y-axis
    /// @param	dx length, x-axis
    /// @param	dy height, y-axis
    /// @param	valueMin minimum value for measures, already multiplied by unit
    /// @param	valueMax maximum value for measures, already multiplied by unit
    /// @param  unit multiplier for all values, default=1, 10 or 100
    /// @param	lapse period between two measures in ms
    /// @param	memory keep highs and low during a number of measures, default=8, 0=no memory
    /// @param	gridX number of areas for measures, default=4, 0=no grid
    /// @param	gridY grid on the time axis every specified number measures, default=0=no grid
    /// @param	continous true=keep previous measures, default=false=over-write
    /// @param	backColour 16-bit background colour, default=black, optional
    /// @param	frontColour 16-bit colour for text, default=white, optional
    /// @param	gridColour 16-bit colour for grid, default=yellow, optional
    /// @param	valueColour 16-bit colour for measures, default=blue, optional
    /// @param	minColour 16-bit colour for lows, default=green, optional
    /// @param	maxColour 16-bit colour for highs, default=red, optional
    /// @note @b 	Lapse
    /// @n	When default=0, all values are displayed and there's no control of the period.
    /// @n	When >0, a value received before that period is discarded.
    /// @n 	When >0, a value received after that period prompts the display of the period in reverse mode.
    ///
    /// @n @b	More: @ref Coordinates, @ref Colours, @ref Values
    ///
    void dDefine(LCD_screen_virtual * lcd,
                 uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy,
                 int32_t valueMin, int32_t valueMax,
                 int32_t unit=1,
                 uint32_t lapse=0, uint16_t memory=8,
                 uint16_t gridX=4, uint16_t gridY=0, bool continous=false,
                 uint16_t backColour=myColours.black, uint16_t frontColour=myColours.white, uint16_t gridColour=myColours.yellow,
                 uint16_t valueColour=myColours.blue, uint16_t minColour=myColours.green, uint16_t maxColour=myColours.red);
    
    ///
    /// @brief	Define the graphic, rectangle coordinates
    /// @param	lcd &LCD to which the graphic belongs
    /// @param	x1 top left coordinate, x-axis
    /// @param	y1 top left coordinate, y-axis
    /// @param	x2 bottom right coordinate, x-axis
    /// @param	y2 bottom right coordinate, y-axis
    /// @param	valueMin minimum value for measures, already multiplied by unit
    /// @param	valueMax maximum value for measures, already multiplied by unit
    /// @param  unit multiplier for all values, default=1, 10 or 100
    /// @param	lapse period between two measures in ms
    /// @param	memory keep highs and low during a number of measures, default=8, 0=no memory
    /// @param	gridX number of areas for measures, default=4, 0=no grid
    /// @param	gridY grid on the time axis every specified number measures, default=0=no grid
    /// @param	continous true=keep previous measures, default=false=over-write
    /// @param	backColour 16-bit background colour, default=black, optional
    /// @param	frontColour 16-bit colour for text, default=white, optional
    /// @param	gridColour 16-bit colour for grid, default=yellow, optional
    /// @param	valueColour 16-bit colour for measures, default=blue, optional
    /// @param	minColour 16-bit colour for lows, default=green, optional
    /// @param	maxColour 16-bit colour for highs, default=red, optional
    /// @note @b	Lapse
    /// @n	When lapse=0=default,
    /// *   all values are displayed and there's no control of the period.
    /// @n	When lapse>0,
    /// *   a value received before that period is discarded,
    /// *   a value received after that period prompts the display of the period in reverse mode.
    ///
    /// @n @b	More: @ref Coordinates, @ref Colours, @ref Values
    ///
    void define(LCD_screen_virtual * lcd,
                uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,
                int32_t valueMin, int32_t valueMax,
                int32_t unit=1,
                uint32_t lapse=0, uint16_t memory=8, uint16_t gridX=4, uint16_t gridY=0, bool continous=false,
                uint16_t backColour=myColours.black, uint16_t frontColour=myColours.white, uint16_t gridColour=myColours.yellow,
                uint16_t valueColour=myColours.blue, uint16_t minColour=myColours.green, uint16_t maxColour=myColours.red);
    
    ///
    /// @brief	Update the graphic
    /// @param	value new measure, already multiplied by unit
    /// @note   If value=1234 with unit=100, actual value is 1234/100=12.34
    ///
    /// @n @b	More: @ref Values
    ///
    void draw(int32_t value);
    
private:
    int32_t  _valueMin, _valueMax, _unit;
    uint16_t _min, _max;
    uint16_t _amnesiaMin, _amnesiaMax;
    uint16_t _memory;
    uint16_t _valueColour, _minColour, _maxColour;
};


///
/// @class	gMultiple
/// @brief	Display a graphic with up to 4 series
///
class gMultiple: public mtgPanel {
    
public:
    ///
    /// @brief	Constructor
    ///
    gMultiple();
    
    ///
    /// @brief	Define the graphic, vector coordinates
    /// @param	lcd &LCD to which the graphic belongs
    /// @param	x0 point coordinate, x-axis
    /// @param	y0 point coordinate, y-axis
    /// @param	dx length, x-axis
    /// @param	dy height, y-axis
    ///
    /// @param	serie1 serie 1: name of the serie
    /// @param	valueMin1 serie 1: minimum value for measures, already multiplied by unit
    /// @param	valueMax1 serie 1: maximum value for measures, already multiplied by unit
    /// @param  unit1 serie 1: multiplier for values, default=1, 10 or 100
    /// @param	valueColour1 serie 1: 16-bit colour for measures, default=blue, optional
    ///
    /// @param	serie2 serie 2: name of the serie, optional
    /// @param	valueMin2 serie 2: minimum value for measures, already multiplied by unit, optional
    /// @param	valueMax2 serie 2: maximum value for measures, already multiplied by unit, optional
    /// @param  unit2 serie 2: multiplier for values, default=1, 10 or 100
    /// @param	valueColour2 serie 2: 16-bit colour for measures, default=green, optional
    ///
    /// @param	serie3 serie 3: name of the serie, optional
    /// @param	valueMin3 serie 3: minimum value for measures, already multiplied by unit, optional
    /// @param	valueMax3 serie 3: maximum value for measures, already multiplied by unit, optional
    /// @param  unit3 serie 3: multiplier for values, default=1, 10 or 100
    /// @param	valueColour3 serie 3: 16-bit colour for measures, default=red, optional
    ///
    /// @param	serie4 serie 4: name of the serie, optional
    /// @param	valueMin4 serie 4: minimum value for measures, already multiplied by unit, optional
    /// @param	valueMax4 serie 4: maximum value for measures, already multiplied by unit, optional
    /// @param  unit4 serie 4: multiplier for values, default=1, 10 or 100
    /// @param	valueColour4 serie 4: 16-bit colour for measures, default=red, optional
    ///
    /// @param	lapse period between two measures in ms
    /// @param	gridX number of areas for measures, default=4, 0=no grid
    /// @param	gridY grid on the time axis every specified number measures, default=0=no grid
    /// @param	continous true=keep previous measures, default=false=over-write
    /// @param	backColour 16-bit background colour, default=black, optional
    /// @param	frontColour 16-bit colour for text, default=white, optional
    /// @param	gridColour 16-bit colour for grid, default=yellow, optional
    /// @note	This graphic doesn't feature memory.
    /// @note	For series 2, 3 and 4, minimum and maximum values can't be both=0.
    /// @b 	Lapse
    /// @n	When default=0, all values are displayed and there's no control of the period.
    /// @n	When >0, a value received before that period is discarded.
    /// @n 	When >0, a value received after that period prompts the display of the period in reverse mode.
    ///
    /// @n @b	More: @ref Coordinates, @ref Colours, @ref Values
    ///
    void dDefine(LCD_screen_virtual * lcd,
                 uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy,
                 String serie1, int32_t valueMin1, int32_t valueMax1, int32_t unit1=1, uint16_t valueColour1=myColours.blue,
                 String serie2="", int32_t valueMin2=0, int32_t valueMax2=0, int32_t unit2=1, uint16_t valueColour2=myColours.green,
                 String serie3="", int32_t valueMin3=0, int32_t valueMax3=0, int32_t unit3=1, uint16_t valueColour3=myColours.red,
                 String serie4="", int32_t valueMin4=0, int32_t valueMax4=0, int32_t unit4=1, uint16_t valueColour4=myColours.white,
                 uint32_t lapse=0, uint16_t gridX=4, uint16_t gridY=0, bool continous=false,
                 uint16_t backColour=myColours.black, uint16_t frontColour=myColours.white, uint16_t gridColour=myColours.yellow);
    
    ///
    /// @brief	Define the graphic, rectangle coordinates
    /// @param	lcd &LCD to which the graphic belongs
    /// @param	x1 top left coordinate, x-axis
    /// @param	y1 top left coordinate, y-axis
    /// @param	x2 bottom right coordinate, x-axis
    /// @param	y2 bottom right coordinate, y-axis
    ///
    /// @param	serie1 serie 1: name of the serie
    /// @param	valueMin1 serie 1: minimum value for measures, already multiplied by unit
    /// @param	valueMax1 serie 1: maximum value for measures, already multiplied by unit
    /// @param  unit1 serie 1: multiplier for values, default=1, 10 or 100
    /// @param	valueColour1 serie 1: 16-bit colour for measures, default=blue, optional
    ///
    /// @param	serie2 serie 2: name of the serie, optional
    /// @param	valueMin2 serie 2: minimum value for measures, already multiplied by unit, optional
    /// @param	valueMax2 serie 2: maximum value for measures, already multiplied by unit, optional
    /// @param  unit2 serie 2: multiplier for values, default=1, 10 or 100
    /// @param	valueColour2 serie 2: 16-bit colour for measures, default=green, optional
    ///
    /// @param	serie3 serie 3: name of the serie, optional
    /// @param	valueMin3 serie 3: minimum value for measures, already multiplied by unit, optional
    /// @param	valueMax3 serie 3: maximum value for measures, already multiplied by unit, optional
    /// @param  unit3 serie 3: multiplier for values, default=1, 10 or 100
    /// @param	valueColour3 serie 3: 16-bit colour for measures, default=red, optional
    ///
    /// @param	serie4 serie 4: name of the serie, optional
    /// @param	valueMin4 serie 4: minimum value for measures, already multiplied by unit, optional
    /// @param	valueMax4 serie 4: maximum value for measures, already multiplied by unit, optional
    /// @param  unit4 serie 4: multiplier for values, default=1, 10 or 100
    /// @param	valueColour4 serie 4: 16-bit colour for measures, default=red, optional
    ///
    /// @param	lapse period between two measures in ms
    /// @param	gridX number of areas for measures, default=4, 0=no grid
    /// @param	gridY grid on the time axis every specified number measures, default=0=no grid
    /// @param	continous true=keep previous measures, default=false=over-write
    /// @param	backColour 16-bit background colour, default=black, optional
    /// @param	frontColour 16-bit colour for text, default=white, optional
    /// @param	gridColour 16-bit colour for grid, default=yellow, optional
    /// @note	This graphic doesn't feature memory.
    /// @note	For series 2, 3 and 4, minimum and maximum values can't be both =0.
    /// @note @b 	Lapse
    /// @n	When default=0, all values are displayed and there's no control of the period.
    /// @n	When >0, a value received before that period is discarded.
    /// @n 	When >0, a value received after that period prompts the display of the period in reverse mode.
    ///
    /// @n @b	More: @ref Coordinates, @ref Colours, @ref Values
    ///
    void define(LCD_screen_virtual * lcd,
                uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,
                String serie1, int32_t valueMin1, int32_t valueMax1, int32_t unit1=1, uint16_t valueColour1=myColours.blue,
                String serie2="", int32_t valueMin2=0, int32_t valueMax2=0, int32_t unit2=1, uint16_t valueColour2=myColours.green,
                String serie3="", int32_t valueMin3=0, int32_t valueMax3=0, int32_t unit3=1, uint16_t valueColour3=myColours.red,
                String serie4="", int32_t valueMin4=0, int32_t valueMax4=0, int32_t unit4=1, uint16_t valueColour4=myColours.white,
                uint32_t lapse=0, uint16_t gridX=4, uint16_t gridY=0, bool continous=false,
                uint16_t backColour=myColours.black, uint16_t frontColour=myColours.white, uint16_t gridColour=myColours.yellow);
    
    ///
    /// @brief	Update the graphic
    /// @param	value1 serie 1: new measure, already multiplied by unit1
    /// @param	value2 serie 2: new measure, already multiplied by unit2
    /// @param	value3 serie 3: new measure, already multiplied by unit3
    /// @param	value4 serie 4: new measure, already multiplied by unit4
    /// @note   If value=1234 with unit=100, actual value is 1234/100=12.34
    ///
    /// @n @b	More: @ref Values
    ///
    void draw(int32_t value1, int32_t value2=0, int32_t value3=0, int32_t value4=0);
    
private:
    String      _name1, _name2, _name3, _name4;
    int32_t     _valueMin1, _valueMin2, _valueMin3,_valueMin4, _valueMax1, _valueMax2, _valueMax3, _valueMax4;
    int32_t     _unit1, _unit2, _unit3, _unit4;
    uint16_t    _valueColour1, _valueColour2, _valueColour3, _valueColour4;
};

#endif
