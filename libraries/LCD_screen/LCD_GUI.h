///
/// @file       LCD_GUI.h
/// @brief      GUI with label, button, dialog box, menu, slider
/// @details	GUI provides high-level elements for user graphic interface
/// @n
/// @n
/// @n @a	Member of LCD_screen Library Suite for 4D Systems uLCD-uLED-uVGA screens
/// @n @a	on Arduino 0023 and 1.0, chipKIT MPIDE 0023, Wiring 1.0
///
/// @deprecated	Rectangle coordinates
/// @n		Rectangle coordinates are no longer supported. Use vector coordinates instead.
/// @ref    Coordinates
/// @n		button::define, button::stringDefine and Label are deprecated;
/// @n		Consider button::dDefine, button::dStringDefine and dLabel instead.
///
/// @a          Developed with [embedXcode](http://embedXcode.weebly.com)
/// @n
/// @author     Rei VILO
/// @author     http://embeddedcomputing.weebly.com
/// @date       Jan 15, 2013
/// @version 	release 421
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
/// @see	ReadMe.txt for references
///


#ifndef LCD_GUI_RELEASE
///
/// @brief	Library release number
///
#define LCD_GUI_RELEASE 421

// Core library - IDE-based
#if defined(WIRING) // Wiring specific
#include "Wiring.h"
#elif defined(MAPLE_IDE) // Maple specific
#include "WProgram.h"
#elif defined(MPIDE) // chipKIT specific
#include "WProgram.h"
#elif defined(ENERGIA) // LaunchPad specific
#include "Energia.h"
#elif defined(ARDUINO) // Arduino 1.0 specific
#include "Arduino.h"
#elif defined(ARDUINO) && (ARDUINO < 100) // Arduino 23 specific
#include "WProgram.h"
#else // error
#error Board not supported.
#endif

// Other libraries
#include "LCD_screen_virtual.h"

#if LCD_SCREEN_VIRTUAL_RELEASE < 114
#error Required LCD_SCREEN_VIRTUAL_RELEASE 114
#endif

///
/// @brief      Set time monitoring
/// @details    1=print ms in console, 0=no
///
//#define CHRONO_DEBUG 0


///
/// @brief	Item combines index and text for GUI elements
///
/// @code	(.cpp)
/// item myMenuItems[] = {
/// 	{     0x0000, "Menu 0"        }  ,
/// 	{     0x1000, "Item 1"        }  ,
/// 	{     0x1100, "Item 11"       }  ,
/// 	...
/// 	{     0x1f00, "Item 1f"       }  ,
/// 	{     0x2000, "Item 2"        }  ,
/// 	{     0x2100, "Item 21"       }  ,
/// 	{     0x2110, "Item 211"      }  ,
/// 	{     0x2121, "Item 2111"     }  ,
/// };
/// @endcode
/// @n		Use setItem to convert a string into an item.
struct item {
    uint16_t    index;     ///< index 0x1000..0xffff, 0x0000 is reserved for menu root
    char        label[16]; ///< text, up to 16 characters
};

///
/// @brief	Set item
/// @details	Packs index and text into an item
/// @param	index index
/// @param	text text
/// @return	item
///
item setItem(uint8_t index, String text);

///
/// @brief	ARRAY_SIZE calculates the number of elements in an array
///
#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

///
/// @class	area
/// @brief	Area
/// @details	Area defines an active zone on the screen to manage touch
///
class area {
public:
    ///
    /// @brief	Constructor
    ///
    area();
    
    ///
    /// @brief	Define area, vector coordinates
    /// @param	lcd &LCD to which the button belongs
    /// @param	x0 point coordinate, x-axis
    /// @param	y0 point coordinate, y-axis
    /// @param	dx length, x-axis
    /// @param	dy height, y-axis
    /// @param  index index, optional
    ///
    /// @n @b	More: @ref Coordinates
    ///
    void dDefine(LCD_screen_virtual * lcd,
                 uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy,
                 uint16_t index=0x0000);
    
    ///
    /// @brief	Return state of the area
    /// @return	true if area enabled, false otherwise
    ///
    bool state();
    
    ///
    /// @brief	Define state of the area
    /// @param	flag default=true=area enabled, false=otherwise
    ///
    void enable(bool flag=true);
    
    ///
    /// @brief	Set option of the area
    /// @details	Exact usage of the option is defined by the object.
    /// @param	flag default=true=option enabled, false=otherwise
    ///
    void setOption(bool flag=true);
    
    ///
    /// @brief	Check if area is pressed
    /// @param	instant default=false area waits for release, instant=true area is activated by press only
    /// @return	true if area pressed
    ///
    bool check(bool instant=false);
    
    ///
    /// @brief	Return index
    /// @return	index of the area
    ///
    uint16_t getIndex();
    
protected:
    uint16_t    _x0, _y0, _dx, _dy;
    bool        _enable, _option;
    uint16_t    _index;
    LCD_screen_virtual * _pscreen;
};


///
/// @class	button
/// @brief	Button
/// @details	Button is an area with a label
///
class button: public area {
public:
    ///
    /// @brief	Constructor
    ///
    button();
    
    ///
    /// @brief	Define item-based button, vector coordinates
    /// @param	lcd &LCD to which the button belongs
    /// @param	x0 point coordinate, x-axis
    /// @param	y0 point coordinate, y-axis
    /// @param	dx length, x-axis
    /// @param	dy height, y-axis
    /// @param	item0 item (including index and text)
    /// @param	textColour1 16-bit colour for text
    /// @param	highColour1 16-bit colour for highlighting
    ///
    /// @n @b	More: @ref Coordinates, @ref Colours
    ///
    void dDefine(LCD_screen_virtual * lcd,
                 uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy,
                 item item0,
                 uint16_t textColour1, uint16_t highColour1);
    
    ///
    /// @brief	Define item-based button, vector coordinates
    /// @param	lcd &LCD to which the button belongs
    /// @param	x0 point coordinate, x-axis
    /// @param	y0 point coordinate, y-axis
    /// @param	dx length, x-axis
    /// @param	dy height, y-axis
    /// @param	item0 item (including index and text)
    /// @param	textColour1 16-bit colour for text
    /// @param	highColour1 16-bit colour for highlighting
    /// @param	lowColour1 16-bit colour for lowlighting
    /// @param	size0 size for text, default=9=automatic
    ///
    /// @n @b	More: @ref Coordinates, @ref Colours
    ///
    void dDefine(LCD_screen_virtual * lcd,
                 uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy,
                 item item0,
                 uint16_t textColour1, uint16_t highColour1, uint16_t lowColour1,
                 uint8_t size0=9);
    
    ///
    /// @brief	Define string-based button, vector coordinates
    /// @param	lcd &LCD to which the button belongs
    /// @param	x0 point coordinate, x-axis
    /// @param	y0 point coordinate, y-axis
    /// @param	dx length, x-axis
    /// @param	dy height, y-axis
    /// @param	text0 text
    /// @param	textColour1 16-bit colour for text
    /// @param	highColour1 16-bit colour for highlighting
    ///
    /// @n @b	More: @ref Coordinates, @ref Colours
    ///
    void dStringDefine(LCD_screen_virtual * lcd,
                       uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy,
                       String text0,
                       uint16_t textColour1, uint16_t highColour1);
    
    ///
    /// @brief	Define item-based button, vector coordinates
    /// @param	lcd &LCD to which the button belongs
    /// @param	x0 point coordinate, x-axis
    /// @param	y0 point coordinate, y-axis
    /// @param	dx length, x-axis
    /// @param	dy height, y-axis
    /// @param	text0 text
    /// @param	textColour1 16-bit colour for text
    /// @param	highColour1 16-bit colour for highlighting
    /// @param	lowColour1 16-bit colour for lowlighting
    /// @param	size0 size for text, default=9=automatic
    ///
    /// @n @b	More: @ref Coordinates, @ref Colours
    ///
    void dStringDefine(LCD_screen_virtual * lcd,
                       uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy,
                       String text0,
                       uint16_t textColour1, uint16_t highColour1, uint16_t lowColour1,
                       uint8_t size0=9);
    
    //  ///
    //  /// @brief	Return state of the button
    //  /// @return	true if button enabled, false otherwise
    //  ///
    //  bool state();
    
    ///
    /// @brief	Draw the button
    /// @param	flag default=false=no overriding
    /// @note	4 states
    /// @n *	if button enabled,
    /// @n 	flag=true: pressed button, flag=false: available button
    /// @n * 	if button disabled,
    /// @n 	flag=true: statement button, flag=false: unavailable button
    ///
    void draw(bool flag=false);
    
    ///
    /// @brief	Check button is pressed
    /// @details	With button animation
    /// @param	instant default=false button waits for release, instant=true button is activated by press only
    /// @return	true if button pressed
    ///
    bool check(bool instant=false);
    
    ///
    /// @brief	Set current value of the flip-flop button
    /// @param	value value true=default, false
    /// @note	Only if setOption=true for flip-flop button
    ///
    void setValue(bool value=true);
    
    ///
    /// @brief	Return current value of the flip-flop button
    /// @return	current value true or false
    /// @note	Only if setOption=true for flip-flop button
    ///
    bool getValue();

    
private:
    uint16_t    _textColour, _highColour, _lowColour;
    String      _text;
    uint8_t     _size;
    bool        _value;
};


///
/// @class	cursor
/// @brief	Cursor
/// @details	Cursor is an area with a cursor
///
class cursor: public area {
public:
    ///
    /// @brief	Constructor
    ///
    cursor();
    
    ///
    /// @brief	Define cursor, vector coordinates
    /// @param	lcd &LCD to which the button belongs
    /// @param	x0 point coordinate, x-axis
    /// @param	y0 point coordinate, y-axis
    /// @param	dx length, x-axis
    /// @param	dy height, y-axis
    /// @param	minValue minimum value, int32
    /// @param	maxValue maximum value, int32
    /// @param	cursorColour 16-bit colour for cursor
    /// @param	highColour 16-bit colour for highlighting
    /// @param	lowColour 16-bit colour for lowlighting
    ///
    /// @n @b	More: @ref Coordinates, @ref Colours
    ///
    void dDefine(LCD_screen_virtual * lcd,
                 uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy,
                 int32_t minValue, int32_t maxValue,
                 uint16_t cursorColour, uint16_t highColour, uint16_t lowColour);
    
    ///
    /// @brief	Set orientation of the cursor
    /// @details	The cursor is intialised with false=horizontal. Use setOption to change the orientation.
    /// @param	flag default=true=vertical, false=horizontal
    ///
    void setOption(bool flag=true);

    ///
    /// @brief	Draw the cursor
    /// @param	flag default=false=no overriding
    /// @note	4 states
    /// @n *	if cursor enabled,
    /// @n 	flag=true: pressed cursor, flag=false: available cursor
    /// @n * 	if cursor disabled,
    /// @n 	flag=true: statement cursor, flag=false: unavailable cursor
    ///
    void draw(bool flag=false);
    
    ///
    /// @brief	Check cursor is pressed
    /// @details	With cursor animation
    /// @param	instant default=false cursor waits for release, instant=true cursor is activated by press only
    /// @return	true if cursor pressed
    ///
    bool check(bool instant=false);
    
    ///
    /// @brief	Set current value of the cursor
    /// @param	value value between minValue and maxValue
    ///
    void setValue(int32_t value);
    
    ///
    /// @brief	Return current value
    /// @return	current value between minValue and maxValue
    ///
    int32_t getValue();
    
    
private:
    uint16_t    _cursorColour, _highColour, _lowColour;
    int32_t     _minValue, _maxValue;
    uint16_t    _gMin, _gMax, _gPosition, _gLine;
};


///
/// @class	textBox
/// @brief	TextBox
/// @details	textBox is an area for text with word-wrap and auto-scrolling
/// @n 		Special commands are accepted:
/// *	\ n = new line
/// *	\ r = for clear
///
class textBox: public area {
public:
    ///
    /// @brief	Constructor
    ///
    textBox();
    
    ///
    /// @brief	Define a text box, vector coordinates
    /// @param	lcd &LCD to which the button belongs
    /// @param	x0 point coordinate, x-axis
    /// @param	y0 point coordinate, y-axis
    /// @param	dx length, x-axis
    /// @param	dy height, y-axis
    /// @param	textColour0 16-bit colour for textBox, default=white
    /// @param	backColour0 16-bit colour for background, default=black
    /// @param	size size of the font, default=0
    ///
    /// @note	Goldelox default=0x00=small .. 0x02=large
    /// @note	Picaso default=0x00=small .. 0x03=largest
    /// @n 0x00=  6x8  small size font set
    /// @n 0x01=  8x8  medium size font set
    /// @n 0x02=  8x12 large size font set
    /// @n 0x03= 12x16 largest size font set -only on Picaso
    ///
    /// @n @b	More: @ref Coordinates, @ref Colours
    ///
    void dDefine(LCD_screen_virtual * lcd,
                 uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy,
                 uint16_t textColour0=myColours.white, uint16_t backColour0=myColours.black,
                 uint8_t size=0);

    ///
    /// @brief	Clear and draw the text box
    ///
    void clear();
    
    ///
    /// @brief	Scroll the text on line up
    ///
    void scroll();
    
    ///
    /// @brief	Print on the text box
    /// @param	text string to be printed
    /// @note	Special caracters includes
    /// * \ n new line
    /// * \ r clear
    ///
    void print(String text);
    
private:
    uint8_t     _size, _tx, _ty;
    uint16_t    _textColour, _backColour;
    int32_t     _maxLines, _maxColumns, _line, _column;
};


///
/// @brief	Dialog box
/// @details	The dialog box displays a title with an icon, and up to three buttons
/// @n		button 1 required, button 2 optional, button 2 and 3 optional
/// @param	lcd &LCD to which the dialog box belongs
/// @param	text0 title of the dialog box
/// @param	kind0 icon with 0=green + ; 1=blue ! ; 2=yellow ? ; 3=red X
/// @param	textColour0 16-bit colour for title
/// @param	highColour0 16-bit high colour for title
/// @param	lowColour0 16-bit low colour for title
/// @param	text1 text for button 1, required
/// @param	button1 comment for button 1, required
/// @param	textColour1 16-bit colour for text on button 1, required
/// @param	highColour1 16-bit high colour for button 1, required
/// @param	lowColour1 16-bit low colour for low button 1, required
/// @param	text2 text for button 2, optional
/// @param	button2 comment for button 2, optional
/// @param	textColour2 16-bit colour for text on button 2, optional
/// @param	highColour2 16-bit high colour for button 2, optional
/// @param	lowColour2 16-bit low colour for low button 2, optional
/// @param	text3 text for button 3, optional
/// @param	button3 comment for button 3, optional
/// @param	textColour3 16-bit colour for text on button 3, optional
/// @param	highColour3 16-bit high colour for button 3, optional
/// @param	lowColour3 16-bit low colour for low button 3, optional
/// @return	number of the button pressed
/// @note   If a storage is available, the initial screen is saved and restored after.
///
/// @n @b	More: @ref Colours, @ref Storage
///
uint8_t dialog(LCD_screen_virtual * lcd,
               String text0, uint8_t kind0, uint16_t textColour0, uint16_t highColour0, uint16_t lowColour0,
               String text1="Press OK", String button1="OK", uint16_t textColour1=myColours.white, uint16_t highColour1=myColours.blue, uint16_t lowColour1=myColours.black,
               String text2="\0", String button2="\0", uint16_t textColour2=0, uint16_t highColour2=0, uint16_t lowColour2=0,
               String text3="\0", String button3="\0", uint16_t textColour3=0, uint16_t highColour3=0, uint16_t lowColour3=0);

/// @brief	Menu
/// @details	The menu displays up to 4 levels with 8 options per level
/// @param	lcd &LCD to which the menu belongs
/// @param	menuItem0 menuItem structure with index and label
/// @param	nItems0 number of items in passed menuItem
/// @param	textColour0 16-bit colour for title
/// @param	highColourMain0 16-bit hight colour for main menu
/// @param	highColourSub0 16-bit high colour for sub-menus
/// @return	index of the selected menu, 0=cancel
/// @note   If a storage is available, the initial screen is saved and restored after.
///
/// @n @b	More: @ref Colours, @ref Storage
///
uint16_t menu(LCD_screen_virtual * lcd,
              item menuItem0[], uint8_t nItems0,
              uint16_t textColour0, uint16_t highColourMain0, uint16_t highColourSub0);


/// @brief	Label, vector coordinates
/// @param	lcd &LCD to which the label belongs
/// @param	x0 point coordinate, x-axis
/// @param	y0 point coordinate, y-axis
/// @param	dx length, x-axis
/// @param	dy height, y-axis
/// @param	text0 text for the label
/// @param	textColour0 16-bit colour for text, default=white, optional
/// @param	backColour0 16-bit colour for background, default=black, optional
/// @param	horizontal0 horizontal position with 0=center, 1=left, 2=right, optional
/// @param	vertical0 vertical position with 0=center, 1=top, 2=bottom, optional
/// @param	size0 size of the font, default=9=automatic, optional
///
/// @n @b	More: @ref Coordinates, @ref Colours
///
void dLabel(LCD_screen_virtual * lcd,
            uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy,
            String text0,
            uint16_t textColour0=myColours.white, uint16_t backColour0=myColours.black,
            uint8_t horizontal0=0, uint8_t vertical0=0,
            uint8_t size0=9);


/// @brief	Slider
/// @param	lcd &LCD to which the sliders belongs
/// @param	&value value to be changed
/// @param	min minimum value, default=0, optional
/// @param	max maximum value, default=511, optional
/// @param	step step value, default=8, optional
/// @param	minColour0 16-bit for minimum value, default=green, optional
/// @param	maxColour0 16-bit for maximum value, default=red, optional
/// @param	okText0 label for OK button, default=OK, optional
/// @param	okTextColour0 16-bit colour for text on OK button, default=white, optional
/// @param	okColour0 16-bit colour for OK button, default=blue, optional
/// @param	cancelText0 label for Cancel button, default=Cancel, optional
/// @param	cancelTextColour0 16-bit colour for text on Cancel button, default=white, optional
/// @param	cancelColour0 16-bit colour for Cancel button, default=yellow, optional
/// @return	true=OK, false=Cancel
/// @note   If a storage is available, the initial screen is saved and restored after.
///
/// @n @b	More: @ref Coordinates, @ref Colours, @ref Storage
///
bool slider(LCD_screen_virtual * lcd,
               uint16_t &value, uint16_t min=0, uint16_t max=511, uint16_t step=8,
               uint16_t minColour0=myColours.green, uint16_t maxColour0=myColours.red,
               String okText0="OK", uint16_t okTextColour0=myColours.white, uint16_t okColour0=myColours.blue,
               String cancelText0="Cancel", uint16_t cancelTextColour0=myColours.white, uint16_t cancelColour0=myColours.yellow);

#endif










