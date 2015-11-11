///
/// @file       LCD_utilities.h
/// @brief      Library header
/// @details	Utilities for LCD_screen
/// @n	
/// @n @b       Project LCD_screen Library Suite
/// @n @a       Developed with [embedXcode](http://embedXcode.weebly.com)
/// 
/// @author     Rei VILO
/// @author     http://embedXcode.weebly.com
/// @date       Feb 16, 2014
/// @version	104
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

#ifndef LCD_UTILITIES_RELEASE
///
/// @brief	Library release number
///
#define LCD_UTILITIES_RELEASE 104

///
/// @name	Utilities
///
/// @{

///
/// @brief	Cosinus
/// @param	degreesX100 angle in degrees, x100
/// @return	cosinus value, x100
/// @note   This function uses integers only.
///
int32_t cos32x100(int32_t degreesX100);

///
/// @brief	Sinus
/// @param	degreesX100 angle in degrees, x100
/// @return	sinus value, x100
/// @note   This function uses integers only.
///
int32_t sin32x100(int32_t degreesX100);

///
/// @brief	UTF-8 to ISO-8859-1 Converter
/// @param	s UTF-8 string, input
/// @return	ISO-8859-1 string, output
///
/// @see    The Unicode Consortium. The Unicode Standard, Version 6.2.0,
/// (Mountain View, CA: The Unicode Consortium, 2012. ISBN 978-1-936213-07-8)
/// http://www.unicode.org/versions/Unicode6.2.0/
///
String utf2iso(String s);

///
/// @brief      Format character array
/// @details
/// @param      format format with standard codes
/// @param      ... list of values
/// @return     character array with values formatted
/// @see        http://www.cplusplus.com/reference/cstdio/printf/?kw=printf for codes
///
char * formatChar(const char * format, ... );

///
/// @brief      Format string
/// @details
/// @param      format format with standard codes
/// @param      ... list of values
/// @return     string with values formatted
/// @see        http://www.cplusplus.com/reference/cstdio/printf/?kw=printf for codes
///
String formatString(const char * format, ... );

/// @}

///
/// @name	Format
/// @brief	Utilities to format float, 64-bit unsigned integer, hexadecimal and period into string
/// @{

///
/// @brief	Convert hexadecimal to string
/// @param	number hexadecimal value
/// @param	size total number of digits, default=0=no check
/// @return	formated string
///
String htoa(uint32_t number, uint8_t size=0);

///
/// @brief	Convert binary to string
/// @param	number binary value
/// @param	size total number of digits, default=0=no check
/// @return	formated string
///
String btoa(uint32_t number, uint8_t size=8);

///
/// @brief	Convert time is ms to string
/// @param	number ms
/// @param	size total number of digits, default=0=free size, no check
/// @return	formated string with time unit, ms, s, mn, h
/// @note	Automatic selection of the time unit: ms, s, mn, h
/// @note	In case of insufficient place or overflow, # is returned
///
String ttoa(uint32_t number, uint8_t size=0);

///
/// @brief	Convert int32_t to string
/// @param	number value, int32_t, already multiplied by unit
/// @param  unit default=1, 10 or 100
/// @param	decimal number of decimal digits, default=0
/// @param	size total number of digits, default=0=free size, no check
/// @note	size >= integer digits + 1 for decimal separator . + decimal=decimal digits
/// @return	formated string
/// @note	In case of insufficient place or overflow, # is returned
///
String i32toa(int32_t number, int32_t unit=1, uint8_t decimal=0, uint8_t size=0);

///
/// @brief	Check range
/// @param	x value to be check
/// @param	xmin range minimum
/// @param	xmax range maximum
/// @return	value with range
/// Assuming xmin<xmax
/// * if x<xmin, x=xmin
/// * if x>xmax, x=xmax
/// Otherwise, xmin and xmax are swapped
///
uint16_t checkRange(uint16_t x, uint16_t xmin, uint16_t xmax);


/// @}



#endif
