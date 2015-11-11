///
/// @file       LCD_documentation.h
/// @brief      Documentation for the LCD_screen Library Suite
/// @details 	Additional documentation on coordinates, fonts and colours
///
/// @n  The LCD_screen Library Suite is the continuation of the [Serial_LCD Library Suite](http://embeddedcomputing.weebly.com/serial_lcd-library-suite-for-4d-systems-screens.html).
/// @n  The Serial_LCD Library Suite is now obsolete and no longer maintained. 4D Systems has launched a new series of screens and provides the libraries for the new serial SPE2 protocol, which is not compatible with former SGC serial protocol.
/// @n  While Serial_LCD Library Suite was limited to 4D Systems screens in SGC mode, the LCD_screen Library Suite adresses a larger audience of basic screens, with a variety of sizes, connections and features, at much more affordable prices.
///
/// @n @a 	Member of LCD_screen Library Suite
/// @n @a	LCD_screen Library Suite
/// @n 		For Arduino 1.0, chipKIT MPIDE 0023, Wiring 1.0, Energia 009
///
/// @a 		Developed with [embedXcode](http://embedXcode.weebly.com)
///
/// @author     Rei VILO
/// @author     http://embeddedcomputing.weebly.com
/// @date       May 20, 2013
/// @version	release	109
/// @n
/// @copyright 	(c) Rei VILO, 2010-2015
/// @copyright 	All rights reserved
///
/// @n  Dual license:
/// *   For hobbyists and for personal usage: Attribution-NonCommercial-ShareAlike 3.0 Unported (CC BY-NC-SA 3.0)
/// *   For professionals or organisations or for commercial usage: All rights reserved
///
/// @n  For any enquiry about license, http://embeddedcomputing.weebly.com/contact
///

///
/// @mainpage 	LCD_screen Library Suite Documentation
/// @details	LCD_screen is a modular suite of libraries for screens.
///
/// @n
/// @n 	The LCD_screen Library Suite
/// *   supports
///     *       SSD1963-based 5" screen from East Rising
///     *       RA8875-based 5" screen from East Rising
///     *       SSD2119-based 3.5" inch TFT LCD from Kentec
///     *       SSD1963-based 7" inch TFT LCD from Kentec
///     *       ILI9481-based 3.2" 480x320 screen
///     *       HX8352A-based 3.2" 400x200 screen
///     *       HX8353E-based screen Educational BoosterPack MKII
///     * 		ST7735-based RobG's universal colour LCD BoosterPack
///     *       ILI9320-based 2.8" HY28A screen from T-Haoyu
///     *       ILI9225B-based RobG's 2.2" LCD+Touch Panel BoosterPack
///     *       ILI9340-based RobG's Color LCD Booster Pack
///     *       4D Systems Picaso-based screens on SGC mode (*),
///     *       4D Systems Picaso-based screens on SPE mode,
/// * 	manages display and touch, SD write and read (*),
/// * 	is based on 3 levels
/// 	1.		top-level with dedicated GUI and Graphics libraries
/// 	2. 		intermediate-level with screen-specific code
/// 	3. 		low-level with virtual classes
/// * 	has been tested on Arduino 1.0.x and Energia 012
///
/// @n  (*) roadmap, future possible enhancements
///
/// <hr><b>	If you enjoy this library, please help me!
/// @n		See how to contribute at http://embeddedcomputing.weebly.com/contact
/// </b><hr>
/// @n
///
/// @a          Developed with [embedXcode](http://embedXcode.weebly.com)
/// @n
/// @author     Rei VILO
/// @author     http://embeddedcomputing.weebly.com
/// @date       May 20, 2013
/// @version	release	118
/// @n
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
/// @page       Documentation	Additional documentation
/// @brief      This section includes additional documentation on copyright and License, structure, initialisation, coordinates, colour, SD-card and resources
///
/// @section	Copyright	Copyright and License
/// @brief      Copyright and License
/// @details	The LCD_screen Library Suite is shared under dual license:
/// *   For hobbyists and for personal usage: Attribution-NonCommercial-ShareAlike 3.0 Unported (CC BY-NC-SA 3.0)
/// *   For professionals or organisations or for commercial usage: All rights reserved
/// @n
/// @n For any enquiry about copyright and License, please use the [contact form](http://embeddedcomputing.weebly.com/contact.html).
///
/// @section    Structure	Library Suite Structure
/// @details	The LCD_screen Library Suite contains three levels of libraries:
/// *	top level end-user libraries like label, button, dialog, menu or slider with GUI.h, or graphics with Graphics.h
/// * 		intermediate level screen-specific libraries, i.e. HY28A_sccren.h
/// * 		low level virtual classes
///
///
/// @section	Version Version management
/// @brief      This section details the management and control of each library part of the Serial_LCD Library Suite.
/// @details	Each library has a release number that can be check at pre-processing.
///
/// @n		Each library has its own release number.
/// 	@code{.cpp}
/// 	#define LCD_FONT_RELEASE	105
/// 	@endcode
///
/// @n		The release	number is checked at pre-processing
/// 	@code{.cpp}
/// 	// Other libraries
/// 	#include "LCD_screen_font.h"
///
/// 	// Test
/// 	#if LCD_FONT_RELEASE	< 106
/// 	#error required LCD_FONT_RELEASE	106
/// 	#endif
/// 	@endcode
///
/// @n  In this example, if the LCD_screen_font library release	is 105, the pre-processor prompts an error message:
/// 	@code{.cpp}
/// 	#error required LCD_FONT_RELEASE	106
/// 	@endcode
///
///
/// @section    Screens Screens
/// @brief      This section explains the different screen features used by the library.
/// @details	Each screen is driven by a controller, and each controller has different features.
///
/// @n  A readable screen allows to get the colour of one specific pixel.
/// 	@code{.cpp}
///     uint16_t colour;
///     if (myScreen.isReadable()) {
///         colour = myScreen.readPixel(10, 10);
///     }
/// 	@endcode
///
/// If the screen isn't readable, LCD_screen::isReadable() is false and LCD_screen::readPixel() returns 0.
/// @n  The LCD_screen::readPixel() function is required by the LCD_screen::copyPasteArea() and LCD_screen::copyArea() functions.
///
///
/// @section    Storage Storage
/// @brief      This section explains the different kinds of storage used used by the library.
/// @details	The GUI library saves the initial screen before displaying a dialog box, a menu or a slider, to restore it afterwards.
///
/// @n  A storage can be:
/// *      external SRAM
/// *      SD-card
///
/// @n      The function LCD_screen::isStorage() returns true is a storage is available.
///
/// @n  The storage is required by the LCD_screen::copyArea() and LCD_screen::pasteArea() functions.
/// * LCD_screen::copyArea() copies an area from the screen and saves it to the SRAM or SD-card
/// * LCD_screen::pasteArea() reads an area from the SRAM or SD-card and pastes it to the screen
///
/// The MCU SRAM is used for the LCD_screen::copyPasteArea() function.
///
///
/// @section	Coordinates Coordinates systems
/// @brief      This section explains the rectangle and vector coordinates systems.
/// @details	Two systems of coordinates are used, rectangle and vector coordinates.
///
/// @n
/// @b      Rectangle coordinates include two points P1 and P2.
/// *	P1 is a pixel on the top left, with (x1, y1) coordinates.
/// *	P2 is a pixel on the bottom right, with (x2, y2) coordinates.
///
/// @n @a 	Example rectangle (0, 0) - (319, 239)
///
/// @n
/// @b		Vector coordinates include one point P0 and one distance.
/// *	P0 is a pixel and the origin, with (x0, y0) coordinates.
/// *	The distance (dx, dy) is specified for the horizontal and the vertical axis.
///
/// @n @a 	Example vector (0, 0) - (320, 240)
/// @n		Going from pixel 0 to pixel 319 represents 320 pixels in total
///
///
/// @section	Areas Areas
/// @brief      This section describes the area-related functions.
/// @details	The area functions allow fast drawing in sequential mode.
///
/// @n
/// *   isArea    True if area optimised functions are provided by the screen
/// *   openArea  Open an area for fast sequential writing, in vector coordinates, with size = dx*dy pixels
/// *   feedArea  Feed the area with colour, maximum dx*dy pixels, no check
/// *   closeArea Close the area previously opened
///
///
/// @section	Fonts   Fonts
/// @brief      This section explains how to use the fonts.
/// @details	Four extended fonts are supplied:
///
/// *   Font 0 or Terminal6x8e  	fixed	 6 x  8,	size=  1344 bytes,	cumulated=  1344 bytes
/// *   Font 1 or Terminal8x12e 	fixed	 8 x 12,	size=  3584 bytes,	cumulated=  4928 bytes
/// *   Font 2 or Terminal12x16e	fixed	12 x 16, 	size=  5376 bytes,	cumulated= 10304 bytes
/// *   Font 3 or Terminal16x24e	fixed	16 x 24, 	size= 10752 bytes,	cumulated= 21056 bytes (not released)
///
/// @n      All the fonts include the extended characters 0x80~0xff corresponding to the ISO-8859-1 fonts page.
/// @n      To convert UTF-8 strings to ISO-8859-1 strings, use the utf2iso() utility.
///
/// @note   First font is numbered 0, second 1, ... The latest font is numbered LCD_screen::fontMax()-1.
/// @n      MAX_FONT_SIZE=0 means no font.
///
/// @brief	Number of fonts
/// @return	number of fonts available
/// @note   First font is numbered 0, second 1, ...
/// @n      The latest font is numbered LCD_screen_font::fontMax()-1
///
/// @see
/// *       MikroElektronika GLCD Font Creator 1.2.0.0
/// @n      http://www.mikroe.com
/// *       The Unicode Consortium. The Unicode Standard, Version 6.2.0,
/// @n      (Mountain View, CA: The Unicode Consortium, 2012. ISBN 978-1-936213-07-8)
/// @n      http://www.unicode.org/versions/Unicode6.2.0/
///
///
/// @section    Values Values using integer numbers
/// @brief      This section explains how values are coded using integer numbers only.
/// @details    Using integers only allows to avoid loading the library for real numbers, which requires 6 KB of memory.
///
/// @n      A value are coded using two numbers:
/// *   a significand, int32_t number, already multiplied by unit
/// *   plus a multiplier, int32_t unit, with default=1, 10 or 100
/// @n      value = number / unit = significand / multiplier
/// The unit provides the scale of the degrees passed.
/// @n      The following calls of the draw() function are equivalent:
/// @code{.cpp}
///     draw(90);        // = 90 / 1
///     draw(90, 1);     // = 90 / 1
///     draw(9000, 100); // = 9000 / 100
/// @endcode
///
/// @n  Functions like cos32x100 and sin32x100 receive and return values multiplied by 100.
/// The unit is set at 100.
/// *   int32_t cos32x100(int32_t degreesX100)
/// *   int32_t sin32x100(int32_t degreesX100)
///
/// @n  int32_t are used instead of int64_t because some platforms don't manage 64-bit numbers.
///
/// @see
/// *   Wikipedia on Floating points and Significand
/// @n  https://en.wikipedia.org/wiki/Floating_point and https://en.wikipedia.org/wiki/Significand
///
///
/// @section	Colours Colours
/// @brief      This section explains how the colours are coded in 16-bit colours and 8-bits Red-Green-Blue components.
/// @details	Colours are coded internally on 16 bits, with 5 bits for red, 6 bits for green and 5 bits for blue, or called RGB565.
///
/// @n 		The Red-Green-Blue components are 8-bit sized and 0x00..0xff scaled.
///
/// @n 		Two functions are available to convert 16-bit colours and 8-bit Red-Green-Blue components:
/// *   LCD_screen::calculateColour calculates 16-bit colour from 8-bit Red-Green-Blue components
/// * 	LCD_screen::splitColour calculates 8-bit Red-Green-Blue components from 16-bit colour
/// @n 		Two functions provide additional calculations:
/// * 	LCD_screen::halveColour halves a 16-bit colour
/// * 	LCD_screen::reverseColour reverses a 16-bit colour
///
/// @see	from Embedded Computing website:
/// *	[Intermediate Level: Colour Functions](http://embeddedcomputing.weebly.com/colour-functions.html)
///
///
/// @section	Resources Other resources
/// @brief      More resources are available online.
/// @details	The LCD_screen Library Suite is supported by the dedicated Embedded Computing website at http://embeddedcomputing.weebly.com
///
/// @see	from Embedded Computing website:
/// *	[Main page](http://embeddedcomputing.weebly.com/serial-lcd.html)
/// *	[Download](http://embeddedcomputing.weebly.com/download.html)
/// *	Former [Tutorials](http://embeddedcomputing.weebly.com/tutorials.html)
/// *	Former [Examples](http://embeddedcomputing.weebly.com/examples.html)
/// *	Former [Tutorial 3: FAQ](http://embeddedcomputing.weebly.com/tutorial-3-faq.html)
/// *	Former [LCD_screen Library Suite](http://embeddedcomputing.weebly.com/lcd_screen-library-suite.html)
/// * 	[Fonts and font generator](http://www.mikroe.com) except for Terminal16e font
///
///
/// @page	Core_History	Version history for core libraries
///
/// @section	LCD_screen_virtual LCD_screen_virtual.h
/// @brief      Version history for LCD_screen_virtual
/// @note       Unified numbering for LCD_screen_virtual, LCD_screen_standard and LCD_screen_font
///
/// *	May 26, 2013 release	104	Built-in fonts and separate LCD_screen_font.h
/// *   Jan 06, 2014 release    118 LCD_screen_standard without primitives, LCD_screen_font with primitives
/// *   Feb 15, 2014 release    119 Stability improvement
/// *   Mar 19, 2014 release    120 Manage triangle with two points equal
/// *   Apr 02, 2014 release    122 New _screenColourBits variable for colour depth
/// *   Apr 13, 2014 release    123 SD-card with FatFs or SD.h (recommended) library
/// *   May 13, 2014 release    124 Energy management
///
///
/// @section	LCD_screen_standard LCD_screen_standard.h
/// @brief      Version history for LCD_screen_standard, without graphic and text primitives
/// @note       Numbering on par with LCD_screen_virtual
///
/// *	May 26, 2013 release	104	Built-in fonts and separate LCD_screen_font.h
/// *	May 26, 2013 release	105	Virtual functions
/// *	May 26, 2013 release	106	Initial release
/// *	Jul 02, 2013 release	107 SRAM integration
/// *	Jul 06, 2013 release	108 SRAM speed optimisation
/// *	Jul 10, 2013 release	109 GUI integration
/// *	Aug 16, 2013 release	110 Storage integration
/// *   Oct 26, 2013 release    113 New screen added
/// *   Dec 10, 2013 release    114 Text functions refactoring
/// *   Dec 31, 2013 release    115 Virtual screen class
/// *   Dec 31, 2013 release    116 Colours refactoring
/// *   Jan 04, 2014 release    117 General layout improvement
/// *   Jan 06, 2014 release    118 New name LCD_screen_standard
/// *   Feb 15, 2014 release    119 Stability improvement
/// *   Mar 19, 2014 release    120 Manage triangle with two points equal
/// *   Mar 26, 2014 release	121 Patch for arc()
/// *   Apr 02, 2014 release    122 New _screenColourBits variable for colour depth
/// *   Apr 13, 2014 release    123 SD-card with FatFs or SD.h (recommended) library
/// *   May 13, 2014 release    124 Energy management
///
///
/// @section	LCD_screen_font LCD_screen_font.h
/// @brief      Version history for LCD_screen_font, with graphic and text primitives
/// @note       Numbering on par with LCD_screen_virtual
///
/// *	May 26, 2013 release	103	Virtual functions
/// *	May 26, 2013 release	104	Integration of LCD_font
/// *	May 26, 2013 release	105	Initial release
/// *	May 26, 2013 release	106	Initial release
/// *	Jul 02, 2013 release	107 SRAM integration
/// *	Jul 06, 2013 release	108 SRAM speed optimisation
/// *	Jul 10, 2013 release	109 GUI integration
/// *	Aug 16, 2013 release	110 Storage integration
/// *   Aug 24, 2013 release    111 Stability enhancement
/// *   Sep 08, 2013 release    112 uint8_t for unsigned char
/// *   Oct 26, 2013 release    113 New screen added
/// *   Dec 10, 2013 release    114 Text functions refactoring
/// *   Jan 04, 2014 release    117 General layout improvement
/// *   Feb 15, 2014 release    119 Stability improvement
/// *   Mar 19, 2014 release    120 Manage triangle with two points equal
/// *   Mar 26, 2014 release	121 Patch for arc()
/// *   Apr 02, 2014 release    122 New _screenColourBits variable for colour depth
/// *   Apr 13, 2014 release    123 SD-card with FatFs or SD.h (recommended) library
/// *   May 13, 2014 release    124 Energy management
///
///
/// @section	LCD_utilities LCD_utilities.h
/// @brief      Version history for the utilities
///
/// *	May 10, 2012 release	100	Initial release
/// *	Jul 10, 2013 release	101 Better algorithms
/// *	Sep 18, 2013 release	102	Use of char[] and C functions
/// *   Dec 10, 2013 release    103 Fixed i32toa function
/// *   Feb 16, 2014 release	104 Added formatString function
///
///
/// @section	Fonts Fonts
/// @brief      Version history for fonts
///
/// *	May 25, 2013 release	101	Initial sets of fonts Terminal6, Terminal8 and Terminal12
/// *	May 27, 2013 release	102	Extended sets of fonts Terminal6e, Terminal8e and Terminal12e
/// *	Jun 04, 2013 release	103	Added 16x24 font Terminal16e (not released)
///
///
/// @section	LCD_colours565 LCD_colours565.h
/// @brief      Version history for colours
///
/// *	Dec 31, 2013 release	101	Initial set of 5-6-5 colours
/// *   Feb 15, 2014 release    102 Added standard colours 0..9 with codes used for resistors
/// *   May 15, 2014 release    103 Added dark colours 10..19 with 0..9 codes used for resistors
///
///
/// @page	High_History	Version history for high-level libraries
///
/// @section	LCD_graphics LCD_graphics.h
/// @brief      Version history for the high-level graphics
///
/// *	Jan 22, 2012 release	1	New Graphics library with example Graphics_main
/// *	Jan 25, 2012 release	2	(x0, y0, dx, fy) functions added to (x1, x2, y1, y2) = (x0, y0, x0+dx, y0+dy)
/// *	Jan 27, 2012 release	3	Histogram graphic with example Histogram_main
/// *	Jan 28, 2012 release	103	New index
/// *	Jan 28, 2012 release	104	Default colours for each graphic
/// *	Jan 30, 2012 release	105	Improved consistency
/// *	Feb 01, 2012 release	106	ttoa used
/// *	Feb 12, 2012 release    106a	__AVR_ATmega328P__ added
/// *	Feb 14, 2012 release	107	gGauge graphic
/// *	Feb 16, 2012 release	108	Yaw, pitch, roll graphics
/// *	Mar 19, 2012 release	209	Arduino 1.0 compatible
/// *	May 01, 2012 release	109	Support for Wiring 1.0
/// *	Jun 14, 2012 release	110	Unified library for Arduino 23 & 1.0, chipKIT and Wiring
/// *	Jul 06, 2012 release	111	More compact library
/// *	Jul 08, 2012 release	112	Meta-classes mtgDial and mtfPane
/// *	Jul 09, 2012 release	113	Graphic for multiple values
/// *	Jul 13, 2012 release	114	Better dial for clock
/// * 	Jul 30, 2012 release	314	Unified release	numbering version 3xx
/// *	Aug 21, 2012 release	315	__AVR_ATmega1280__ added
/// *	Sep 01, 2012 release	316	__AVR_ATmega32U4__ added
/// * 	Sep 12, 2012 release	317	Gauge stability enhancement
/// *	Mar 03, 2013 release	318	gTable graphic
/// *	Jun 03, 2013 release	419	clock, gauge and yaw-pitch-roll for LCD_screen
/// *	Jun 04, 2013 release	420	Full library for LCD_screen
///
///
/// @section	LCD_GUI LCD_GUI.h
/// @brief      Version history for the high-level GUI elements
///
/// *   Sep 18, 2011 release	1	Dialog window with up to 3 buttons
/// *	Nov 25, 2011 release	2	Faster dialog show/hide and optional area for screen copy to/read from SD
/// *	Nov 27, 2011 release	3	Bugs fixed
/// *	Dec 15, 2011 release	3.1	Arduino 1.0 implementation test	no longer compatible with 0022
/// *	Dec 27, 2011 release	4	Ready for GUI = button + dialog box + menu + label
/// *	Dec 28, 2011 release	5	Item-based refactoring for dialog box, menu and label
/// *	Dec 29, 2011 release	6	Button library deprecated, superseeded by GUI library
/// *	Jan 05, 2012 release	7	(x0, y0, dx, fy) functions added to (x1, x2, y1, y2) = (x0, y0, x0+dx, y0+dy)
/// *	Jan 25, 2012 release	8	Button with instant option (no de-bouncing)
/// *	Jan 28, 2012 release	108	New index
/// *	Feb 12, 2012 release	108a	__AVR_ATmega328P__ __AVR_ATmega2560__ added
/// *	Mar 19, 2012 release	209	Arduino 1.0 compatible
/// *	Apr 22, 2012 release	109	Slider added
/// *	Apr 28, 2012 release	110	Better menu
/// *	May 01, 2012 release	111	Support for Wiring 1.0
/// *	Jun 04, 2012 release	112	RAW image bug fixed
/// *	Jun 14, 2012 release	113	Unified library for Arduino 23 & 1.0, chipKIT and Wiring
/// *	Jul 05, 2012 release	114	More compact library
/// * 	Jul 30, 2012 release	315	Unified release	numbering version 3xx
/// *	Aug 21, 2012 release	316	__AVR_ATmega1280__ added
/// *	Sep 01, 2012 release	317	__AVR_ATmega32U4__ added
/// *	Dec 01, 2012 release	318	New area object = zone for touch
/// *	Jan 08, 2013 release	319	New cursor object
/// *	Jan 15, 2013 release	320 New text box object
/// *	Jul 06, 2013 release	421 First release for LCD_screen
///
///
/// @page	Screen_History	Version history for screens drivers
///
/// @section	Screen_ER1963 Screen_ER1963.h
/// @brief      Version history for the 800x400 East Rising 5" SSD1963-based screen
///
/// *	Feb 14, 2014 release	101	Graphics and fonts implemented
/// *   Feb 15, 2014 release    102 Touch implemented
/// *   Mar 23, 2014 release    103 setBacklight added
/// *   May 12, 2014 release    104 Power management and energy saving
///
///
/// @section	Screen_ER1963_SD Screen_ER1963_SD.h
/// @brief      Version history for the 800x400 East Rising 5" SSD1963-based screen with SD-card
/// @note       Numbering on par with Screen_ER1963
///
/// *   Feb 15, 2014 release    102 SD-card write-read
/// *   Apr 13, 2014 release    106 SD-card with FatFs or SD.h (recommended) library
/// *   Feb 08, 2015 release    107 Cleaner ER1963_buffer8
///
///
/// @section	Screen_ILI9340 Screen_ILI9340.h
/// @brief      Version history for the 320x240 2.2" ILI9340-based + SD BoosterPack
///
/// *	Feb 18, 2014 release	101	First release, no pixel reading
///
///
/// @section	Screen_ER8875 Screen_ER8875.h
/// @brief      Version history for the 800x400 East Rising 5" RA8875-based screen
///
/// *	Jan 06, 2014 release	101	Graphics and fonts implemented
/// *   Jan 08, 2014 release    102 Touch implemented
/// *   Jan 10, 2014 release    103 Area functions implemented
/// *   Jan 10, 2014 release    104 Fast SPI
/// *   Jan 12, 2014 release    105 SD-card write-read
///
///
/// @section	Screen_ER8875_SD Screen_ER8875_SD.h
/// @brief      Version history for the 800x400 East Rising 5" RA8875-based screen with SD card
/// @note       Numbering on par with Screen_ER8875
///
/// *   Jan 12, 2014 release    105 SD-card write-read
/// *   Apr 13, 2014 release    106 SD-card with FatFs or SD.h (recommended) library
/// *   Feb 08, 2015 release    107 Cleaner ER8875_buffer8
///
///
/// @section	Screen_HI32 Screen_HI32.h
/// @brief      Version history for the 480x320 screen
///
/// *	May 29, 2013 release	101 Stable release
/// *   Dec 10, 2013 release    102 Text functions refactoring
///
///
/// @section	Screen_HX8353E Screen_HX8353E.h
/// @brief      Version history for the HX8353E-based screen
///
/// *	Dec 06, 2013 release	101	Initial release
/// *   Dec 10, 2013 release    102 Text functions refactoring
/// *   Jan 04, 2014 release    117 Better consistency with standard functions
///
///
/// @section	Screen_HY28A_SRAM Screen_HY28A_SRAM.h
/// @brief      Version history for the HY28A-based screen
///
/// *	May 26, 2013 release	105	Dual SPI
/// *	May 26, 2013 release	106	Faster text (6x)
/// *	May 26, 2013 release	107	Initial release
/// *	May 26, 2013 release	108 _setPoint fixed
/// *	Jun 02, 2013 release	109 Fast software SPI evaluation
/// *	Jul 02, 2013 release	110 SRAM copy paste
/// *   Jul 07, 2013 release    111 Improved SPI library by reaper7
/// *   Aug 10, 2013 release	112 Improved SRAM management
/// *   Dec 10, 2013 release    113 Text functions refactoring
///
///
/// @section	Screen_ILI9225B Screen_ILI9225B.h
/// @brief      Version history for the ILI9225B-based screen
///
/// *	May 26, 2013 release	105	SPI speed fixed for screen and touch
/// *	May 26, 2013 release	105	Faster text (10x)
/// *	May 26, 2013 release	106	Initial release
/// *	May 26, 2013 release	107	gText fixed
/// *   Sep 09, 2013 release    108 Added support for F5529
/// *   Dec 10, 2013 release    109 Text functions refactoring
/// *   Apr 03, 2014 release    110 Added support for TM4C129
///
///
/// @section	Screen_K35 Screen_K35.h
/// @brief      Version history for the Kentec 3.5 screen
///
/// *	Aug 16, 2013 release	103	Initial release
/// *   Dec 10, 2013 release    104 Text functions refactoring
///
///
/// @section	Screen_K70 Screen_K70.h
/// @brief      Version history for the Kentec 7.0 screen
///
/// *	Dec 14, 2013 release    101	Initial release
/// *   Feb 01, 2014 release    102 check x1<x2 and y1<y2 for _fastFill
///
///
/// @section	Screen_HX8353E Screen_HX8353E.h
/// @brief      Version history for the 3.2" wide screen
///
/// *   Dec 10, 2013 release    100 Educational BoosterPack MKII (not released)
/// *   Dec 10, 2013 release    101 Text functions refactoring
///
///
/// @section	Screen_PicasoSGC Screen_PicasoSGC.h
/// @brief      Version history for 4D Systems Picaso-based screen on SGC mode
///
/// *	May 27, 2013 release	099	Proof of concept (not released)
/// *   Jun 25, 2013 release    100 Interim version (not released)
/// *   Jun 25, 2013 release    101 Interim version (not released)
/// *   Jun 25, 2013 release    102 Interim version (not released)
/// *   Sep 25, 2013 release    103 readPixel and copyPaste added (not released)
/// *   Dec 10, 2013 release    104 Text functions refactoring (not released)
///
///
/// @section	Screen_PicasoSPE Screen_PicasoSPE.h
/// @brief      Version history for 4D Systems Picaso-based screen on SPE mode
///
/// *	May 27, 2013 release	099	Proof of concept (not released)
/// *   Jun 09, 2013 release    100 Proof of concept (not released)
/// *   Jun 09, 2013 release    101 Proof of concept (not released)
/// *   Jun 09, 2013 release    102 Proof of concept (not released)
/// *   Jun 09, 2013 release    103 Proof of concept (not released)
/// *   Jun 09, 2013 release    104 Proof of concept (not released)
/// *   Sep 25, 2013 release    105 readPixel and copyPaste added (not released)
/// *   Oct 20, 2013 release    106 Support for Energia
/// *   Oct 20, 2013 release    106 First release
/// *   Dec 10, 2013 release    107 Text functions refactoring
///
///
/// @section	Screen_ST7735 Screen_ST7735.h
/// @brief      Version history for the ST7735-based screen
///
/// *	May 26, 2013 release	105	Initial release
/// *   Dec 10, 2013 release    106 Text functions refactoring
///
///
/// @section	Screen_W32 Screen_W32.h
/// @brief      Version history for the 3.2" wide screen
///
/// *	May 29, 2013 release	101 Stable release
/// *   Oct 05, 2013 release    102 Fix for orientation
/// *   Dec 10, 2013 release    103 Text functions refactoring
///
///
