//
// LCD_colours565.cpp
// Class library C++ code
// ----------------------------------
// Developed with embedXcode+
// http://embedXcode.weebly.com
//
// Project      LCD_screen Library Suite
//
// Created by 	Rei Vilo, 31/12/2013 18:49
// 				http://embeddedcomputing.weebly.com
//
// Copyright 	(c) Rei Vilo, 2013-2014
// License		All rights reserved
//
// See 			LCD_colours565.h and ReadMe.txt for references
//


// Library header
#include "LCD_colours565.h"

// Code
LCD_colours565::LCD_colours565() {
    ;
}

// Colour
uint16_t LCD_colours565::calculate(uint8_t red, uint8_t green, uint8_t blue)
{
    return (red >> 3) << 11 | (green >> 2) << 5 | (blue >> 3);
}

void LCD_colours565::split(uint16_t rgb, uint8_t &red, uint8_t &green, uint8_t &blue)
{
    // rgb16 = red5 green6 blue5
    red   = ((rgb & 0b1111100000000000) >> 11) *255/31 ; // << 3;
    green = ((rgb & 0b0000011111100000) >>  5) *255/63 ; // << 2;
    blue  = ((rgb & 0b0000000000011111)      ) *255/31 ; // << 3;
}

uint16_t LCD_colours565::halve(uint16_t rgb) {
	// rgb16 = red5 green6 blue5
	return (((rgb & 0b1111100000000000) >> 12) << 11 | \
            ((rgb & 0b0000011111100000) >>  6) <<  5 | \
            ((rgb & 0b0000000000011111) >>  1));
}

uint16_t LCD_colours565::average(uint16_t rgb1, uint16_t rgb2)
{
	uint8_t r1, g1, b1, r2, g2, b2;
	uint16_t r, g, b;
	split(rgb1, r1, g1, b1);
	split(rgb2, r2, g2, b2);
	r = (uint16_t)(r1 + r2)/2;
	g = (uint16_t)(g1 + g2)/2;
	b = (uint16_t)(b1 + b2)/2;
	return calculate((uint8_t)r, (uint8_t)g, (uint8_t)b);
}

uint16_t LCD_colours565::reverse(uint16_t rgb) {
    // rgb16 = red5 green6 blue5
    return (uint16_t)(rgb ^ 0b1111111111111111);
}

uint16_t LCD_colours565::code(uint8_t value09) {
    bool flag = (value09 > 9);
    value09 %= 10;
    uint16_t colour;
    
    if (value09 == 0) colour = black;
    if (value09 == 1) colour = brown;
    if (value09 == 2) colour = red;
    if (value09 == 3) colour = orange;
    if (value09 == 4) colour = yellow;
    if (value09 == 5) colour = green;
    if (value09 == 6) colour = blue;
    if (value09 == 7) colour = violet;
    if (value09 == 8) colour = grey;
    if (value09 == 9) colour = white;
    
    if (flag) return halve(colour);
    else return colour;
}

LCD_colours565 myColours = LCD_colours565();

