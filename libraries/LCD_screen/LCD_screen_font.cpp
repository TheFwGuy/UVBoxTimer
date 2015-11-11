//
// LCD_screen_font.cpp
// Class library C++ code
// ----------------------------------
// Developed with embedXcode
// http://embedXcode.weebly.com
//
// Project LCD_screen Library Suite
//
// Created by Rei VILO, May 30, 2013
// http://embedXcode.weebly.com
//
//
// Copyright © Rei VILO, 2013-2014
// License All rights reserved
//
// See LCD_screen_font.h and ReadMe.txt for references
//
// Last update: Mar 26, 2014 release    121 Patch for arc()

// Library header
#include "LCD_screen_font.h"

// Code
LCD_screen_font::LCD_screen_font()
{
    _fontSize       = 0;
    _fontSolid      = true;
    _penSolid       = false;
    _flagRead       = false;
    _flagStorage    = false;
    _flagEnergy     = false;
    _touchTrim      = 0;
}

void LCD_screen_font::showInformation(uint16_t &x0, uint16_t &y0)
{
    setFontSize(2);
    gText(x0 *2, y0, "LCD_screen Library Suite");
    y0 += fontSizeY() +1;
    setFontSize(1);
    gText(x0, y0, formatString("%12s %i", "Release", LCD_SCREEN_FONT_RELEASE));
    y0 += fontSizeY() +1;
    gText(x0, y0, formatString("%12s %s", "Copyright", "Rei Vilo, 2010-2015"));
    y0 += fontSizeY() +1;
    gText(x0, y0, formatString("%12s %s", "License", "All rights reserved"));
    y0 += fontSizeY() +1;
    gText(x0, y0, formatString("%12s %s", "Website", "http://embeddedcomputing.weebly.com"));
    y0 += fontSizeY() +1;
    String s = WhoAmI();
    
    char * c[24];
    s.toCharArray((char *)c, s.length() +1);
    gText(x0, y0, formatString("%12s %s", "Screen", c));
    y0 += fontSizeY() +1;
    gText(x0, y0, formatString("%12s %i x %i", "Pixels", screenSizeX(), screenSizeY()));
    y0 += fontSizeY() +1;
    gText(x0, y0, formatString("%12s %i.%i\"", "Diagonal", screenDiagonal() /10, screenDiagonal() %10));
    y0 += fontSizeY() +1;
    gText(x0, y0, formatString("%12s %s", "Readable", isReadable() ? "yes" : "no"));
    y0 += fontSizeY() +1;
    gText(x0, y0, formatString("%12s %s", "Touch", isTouch() ? "yes" : "no"));
    y0 += fontSizeY() +1;
    gText(x0, y0, formatString("%12s %s", "Storage", isStorage() ? "yes" : "no"));
    y0 += fontSizeY() +1;
    gText(x0, y0, formatString("%12s %s", "Energy", isEnergy() ? "yes" : "no"));
    y0 += fontSizeY() +1;
    gText(x0, y0, formatString("%12s %s", "Intensity", isIntensity() ? "yes" : "no"));
    y0 += fontSizeY() +1;
    y0 += fontSizeY() +1;
    
    //#ifdef BUILT_IN_FONTS
    //    gText(x0, y0, "Fonts:  built-in");
    //#else
    //    gText(x0, y0, "Fonts:  " +String(MAX_FONT_SIZE+1) + " header files");
    //#endif
    //    y0 += fontSizeY() +1;
}

void LCD_screen_font::clear(uint16_t colour)
{
    uint8_t oldOrientation = _orientation;
    bool oldPenSolid = _penSolid;
    setOrientation(0);
    setPenSolid();
    rectangle(0, 0, screenSizeX()-1, screenSizeY()-1, colour);
    setOrientation(oldOrientation);
    setPenSolid(oldPenSolid);
}

void LCD_screen_font::setOrientation(uint8_t orientation)
{
    _orientation = orientation % 4;
    _setOrientation(_orientation);
}

uint8_t LCD_screen_font::getOrientation()
{
    return _orientation;
}

uint16_t LCD_screen_font::screenSizeX()
{
    switch (_orientation)
    {
        case 0:
        case 2:
            return _screenWidth; // _maxX
            //            return _screenHeigth; // _maxY
            break;
        case 1:
        case 3:
            return _screenHeigth; // _maxX
            //            return _screenWidth; // _maxY
            break;
    }
    return(-1);	// (invalid return RR commented)
}

uint16_t LCD_screen_font::screenSizeY()
{
    switch (_orientation)
    {
        case 0:
        case 2:
            //            return _screenWidth; // _maxX
            return _screenHeigth; // _maxY
            break;
        case 1:
        case 3:
            //            return _screenHeigth; // _maxX
            return _screenWidth; // _maxY
            break;
    }
    return (-1); // (invalid return RR commented)
}

uint16_t LCD_screen_font::screenDiagonal()
{
    return _screenDiagonal;
}


void LCD_screen_font::circle(uint16_t x0, uint16_t y0, uint16_t radius, uint16_t colour)
{
    int16_t f = 1 - radius;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * radius;
    int16_t x = 0;
    int16_t y = radius;
    
    if (_penSolid == false)
    {
        point(x0, y0+radius, colour);
        point(x0, y0-radius, colour);
        point(x0+radius, y0, colour);
        point(x0-radius, y0, colour);
        
        while (x<y)
        {
            if (f >= 0)
            {
                y--;
                ddF_y += 2;
                f += ddF_y;
            }
            
            x++;
            ddF_x += 2;
            f += ddF_x;
            
            point(x0 + x, y0 + y, colour);
            point(x0 - x, y0 + y, colour);
            point(x0 + x, y0 - y, colour);
            point(x0 - x, y0 - y, colour);
            point(x0 + y, y0 + x, colour);
            point(x0 - y, y0 + x, colour);
            point(x0 + y, y0 - x, colour);
            point(x0 - y, y0 - x, colour);
        }
    } else {
        while (x<y)
        {
            if (f >= 0)
            {
                y--;
                ddF_y += 2;
                f += ddF_y;
            }
            
            x++;
            ddF_x += 2;
            f += ddF_x;
            
            line(x0 + x, y0 + y, x0 - x, y0 + y, colour); // bottom
            line(x0 + x, y0 - y, x0 - x, y0 - y, colour); // top
            line(x0 + y, y0 - x, x0 + y, y0 + x, colour); // right
            line(x0 - y, y0 - x, x0 - y, y0 + x, colour); // left
        }
        
        setPenSolid(true);
        rectangle(x0-x, y0-y, x0+x, y0+y, colour);
    }
}

void LCD_screen_font::dLine(uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy, uint16_t colour)
{
    line(x0, y0, x0+dx-1, y0+dy-1, colour);
}

void LCD_screen_font::line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t colour)
{
    //    Serial.print("LCD_screen_font::line \t");
    //    Serial.print(x1, DEC);
    //    Serial.print("\t");
    //    Serial.print(y1, DEC);
    //    Serial.print("\t");
    //    Serial.print(x2, DEC);
    //    Serial.print("\t");
    //    Serial.print(y2, DEC);
    //    Serial.println();
    
    if ((x1 == x2) && (y1 == y2))
    {
        _setPoint(x1, y1, colour);
    } else if ((x1 == x2) || (y1 == y2))
    {
        _fastFill(x1, y1, x2, y2, colour);
    } else {
        int16_t wx1 = (int16_t)x1;
        int16_t wx2 = (int16_t)x2;
        int16_t wy1 = (int16_t)y1;
        int16_t wy2 = (int16_t)y2;
        
        boolean flag = abs(wy2 - wy1) > abs(wx2 - wx1);
        if (flag)
        {
            _swap(wx1, wy1);
            _swap(wx2, wy2);
        }
        
        if (wx1 > wx2)
        {
            _swap(wx1, wx2);
            _swap(wy1, wy2);
        }
        
        int16_t dx = wx2 - wx1;
        int16_t dy = abs(wy2 - wy1);
        int16_t err = dx / 2;
        int16_t ystep;
        
        if (wy1 < wy2) ystep = 1;
        else ystep = -1;
        
        for (; wx1<=wx2; wx1++)
        {
            if (flag) _setPoint(wy1, wx1, colour);
            else _setPoint(wx1, wy1, colour);
            
            err -= dy;
            if (err < 0)
            {
                wy1 += ystep;
                err += dx;
            }
        }
    }
}

void LCD_screen_font::setPenSolid(bool flag)
{
    _penSolid = flag;
}

void LCD_screen_font::point(uint16_t x1, uint16_t y1, uint16_t colour)
{
    _setPoint(x1, y1, colour);
}

void LCD_screen_font::rectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t colour)
{
    if (_penSolid == false)
    {
        line(x1, y1, x1, y2, colour);
        line(x1, y1, x2, y1, colour);
        line(x1, y2, x2, y2, colour);
        line(x2, y1, x2, y2, colour);
    } else {
        _fastFill(x1, y1, x2, y2, colour);
    }
}

void LCD_screen_font::dRectangle(uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy, uint16_t colour)
{
    rectangle(x0, y0, x0+dx-1, y0+dy-1, colour);
}

void LCD_screen_font::_triangleArea(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, uint16_t colour)
{
    int16_t wx1 = (int16_t)x1;
    int16_t wy1 = (int16_t)y1;
    int16_t wx2 = (int16_t)x2;
    int16_t wy2 = (int16_t)y2;
    int16_t wx3 = (int16_t)x3;
    int16_t wy3 = (int16_t)y3;
    int16_t wx4 = wx1;
    int16_t wy4 = wy1;
    int16_t wx5 = wx1;
    int16_t wy5 = wy1;
    
    //    Serial.println("*** area");
    //    Serial.print("1 =\t");
    //    Serial.print(x1, DEC);
    //    Serial.print(",\t");
    //    Serial.println(y1, DEC);
    //    Serial.print("2 =\t");
    //    Serial.print(x2, DEC);
    //    Serial.print(",\t");
    //    Serial.println(y2, DEC);
    //    Serial.print("3 =\t");
    //    Serial.print(x3, DEC);
    //    Serial.print(",\t");
    //    Serial.println(y3, DEC);
    
    bool changed1 = false;
    bool changed2 = false;
    
    int16_t dx1 = abs(wx2 - wx1);
    int16_t dy1 = abs(wy2 - wy1);
    
    int16_t dx2 = abs(wx3 - wx1);
    int16_t dy2 = abs(wy3 - wy1);
    
    int16_t signx1 = (wx2 >= wx1) ? +1 : -1;
    int16_t signx2 = (wx3 >= wx1) ? +1 : -1;
    
    int16_t signy1 = (wy2 >= wy1) ? +1 : -1;
    int16_t signy2 = (wy3 >= wy1) ? +1 : -1;
    
    if (dy1 > dx1)
    {   // swap values
        _swap(dx1, dy1);
        changed1 = true;
    }
    
    if (dy2 > dx2)
    {   // swap values
        _swap(dx2, dy2);
        changed2 = true;
    }
    
    int16_t e1 = 2 * dy1 - dx1;
    int16_t e2 = 2 * dy2 - dx2;
    
    for (int i = 0; i <= dx1; i++)
    {
        line(wx4, wy4, wx5, wy5, colour);
        
        while (e1 >= 0)
        {
            if (changed1) wx4 += signx1;
            else wy4 += signy1;
            e1 = e1 - 2 * dx1;
        }
        
        if (changed1) wy4 += signy1;
        else wx4 += signx1;
        
        e1 = e1 + 2 * dy1;
        
        while (wy5 != wy4)
        {
            while (e2 >= 0)
            {
                if (changed2) wx5 += signx2;
                else wy5 += signy2;
                e2 = e2 - 2 * dx2;
            }
            
            if (changed2) wy5 += signy2;
            else wx5 += signx2;
            
            e2 = e2 + 2 * dy2;
        }
    }
}

void LCD_screen_font::triangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, uint16_t colour)
{
    if ((x1 == x2) && (y1 == y2))
    {
        line(x3, y3, x1, y1, colour);
        
    } else if ((x1 == x3) && (y1 == y3))
    {
        line(x2, y2, x3, y3, colour);
        
    } else if ((x2 == x3) && (y2 == y3))
    {
        line(x1, y1, x2, y2, colour);
        
    } else if (_penSolid)
    {
        bool b = true;
        
        // Graham Scan + Andrew's Monotone Chain Algorithm
        // Sort by ascending y
        while (b)
        {
            b = false;
            if ((b == false) && (y1 > y2))
            {
                _swap(x1, x2);
                _swap(y1, y2);
                b = true;
            }
            if ((b == false) && (y2 > y3))
            {
                _swap(x3, x2);
                _swap(y3, y2);
                b = true;
            }
        }
        
        //        Serial.println("*** triangle");
        //        Serial.print("1 =\t");
        //        Serial.print(x1, DEC);
        //        Serial.print(",\t");
        //        Serial.println(y1, DEC);
        //        Serial.print("2 =\t");
        //        Serial.print(x2, DEC);
        //        Serial.print(",\t");
        //        Serial.println(y2, DEC);
        //        Serial.print("3 =\t");
        //        Serial.print(x3, DEC);
        //        Serial.print(",\t");
        //        Serial.println(y3, DEC);
        
        if (y2 == y3)
        {
            _triangleArea(x1, y1, x2, y2, x3, y3, colour);
            
        } else if (y1 == y2)
        {
            _triangleArea(x3, y3, x1, y1, x2, y2, colour);
            
        } else {
            uint16_t x4 = (uint16_t)( (int32_t)x1 + (y2 - y1) * (x3 - x1) / (y3 - y1) );
            uint16_t y4 = y2;
            
            _triangleArea(x1, y1, x2, y2, x4, y4, colour);
            delay(1);
            _triangleArea(x3, y3, x2, y2, x4, y4, colour);
            delay(1);
        }
    } else {
        line(x1, y1, x2, y2, colour);
        line(x2, y2, x3, y3, colour);
        line(x3, y3, x1, y1, colour);
    }
}

void LCD_screen_font::arc(uint16_t x0, uint16_t y0, uint16_t radius, uint16_t start, uint16_t end, uint16_t colour)
{
    ;
}

// Touch
bool LCD_screen_font::isTouch()
{
    return (_touchTrim > 0);
}
bool LCD_screen_font::isReadable()
{
    return _flagRead;
}
bool LCD_screen_font::isStorage()
{
    return _flagStorage;
}
uint16_t LCD_screen_font::readPixel(uint16_t x1, uint16_t y1)
{
    return 0;
}
void LCD_screen_font::copyPasteArea(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t dx, uint16_t dy)
{
    
}
void LCD_screen_font::copyArea(uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy, uint32_t &address)
{
    
}
void LCD_screen_font::pasteArea(uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy, uint32_t &address, bool option)
{
    
}

void LCD_screen_font::_displayTarget(uint16_t x0, uint16_t y0, uint16_t colour)
{
    setPenSolid(false);
    circle(x0, y0, 8, colour);
    circle(x0, y0, 7, colour);
    circle(x0, y0, 6, colour);
    
    //    line(x0-9, y0, x0+9, y0, colour);
    //    line(x0, y0-9, x0, y0+9, colour);
}

bool LCD_screen_font::getTouch(uint16_t &x, uint16_t &y, uint16_t &z)
{
    if (_touchTrim == 0) return false;
    
    uint16_t x0, y0, z0;
    
    _getRawTouch(x0, y0, z0);
    z = z0;
    
    if (z > _touchTrim)
    {
        x0 = _check(x0, _touchXmin, _touchXmax);
        y0 = _check(y0, _touchYmin, _touchYmax);
        
        switch (_orientation)
        {
            case 0: // ok
                x = map(x0, _touchXmin, _touchXmax, 0, _screenWidth);
                y = map(y0, _touchYmin, _touchYmax, 0, _screenHeigth);
                break;
            case 1: // ok
                x = map(y0, _touchYmin, _touchYmax, 0, _screenHeigth);
                y = map(x0, _touchXmin, _touchXmax, _screenWidth, 0);
                break;
            case 2: // ok
                x = map(x0, _touchXmin, _touchXmax, _screenWidth, 0);
                y = map(y0, _touchYmin, _touchYmax, _screenHeigth, 0);
                break;
            case 3: // ok
                x = map(y0, _touchYmin, _touchYmax, _screenHeigth, 0);
                y = map(x0, _touchXmin, _touchXmax, 0, _screenWidth);
                break;
        }
        return true;
    } else {
        return false;
    }
}

void LCD_screen_font::calibrateTouch()
{
    if (_touchTrim == 0) return;
    
    uint16_t x00, y00, x10, x01, x11, y10, y01, y11, z0;
    uint16_t x0, x1, y0, y1;
    uint8_t  old = _orientation;
    
    setOrientation(0);
    clear();
    
    setFontSize(0);
    gText(screenSizeX()/2-17*fontSizeX()/2, screenSizeY()/2-fontSizeY(), "TOUCH CALIBRATION", myColours.red, myColours.black);
    gText(screenSizeX()/2-16*fontSizeX()/2, screenSizeY()/2,             "Press the centre", myColours.red, myColours.black);
    gText(screenSizeX()/2-17*fontSizeX()/2, screenSizeY()/2+fontSizeY(), "of the red circle", myColours.red, myColours.black);
    
    _displayTarget(10, 10, myColours.red);
    z0 = 0;
    do _getRawTouch(x00, y00, z0); while (z0 < _touchTrim);
    _displayTarget(10, 10, myColours.green);
    //    do _getRawTouch(x0, y0, z0); while (z0 > 0);
    
    delay(500);
    _displayTarget(_screenWidth-10, 10, myColours.red);
    z0 = 0;
    do _getRawTouch(x10, y10, z0); while (z0<_touchTrim);
    _displayTarget(_screenWidth-10, 10, myColours.green);
    //    do _getRawTouch(x0, y0, z0); while (z0 > 0);
    
    delay(500);
    _displayTarget(_screenWidth-10, _screenHeigth-10, myColours.red);
    z0 = 0;
    do _getRawTouch(x11, y11, z0); while (z0<_touchTrim);
    _displayTarget(_screenWidth-10, _screenHeigth-10, myColours.green);
    //    do _getRawTouch(x0, y0, z0); while (z0 > 0);
    
    delay(500);
    _displayTarget(10, _screenHeigth-10, myColours.red);
    z0 = 0;
    do _getRawTouch(x01, y01, z0); while (z0<_touchTrim);
    _displayTarget(10, _screenHeigth-10, myColours.green);
    //    do _getRawTouch(x0, y0, z0); while (z0 > 0);
    
    Serial.print("// *** calibrateTouch00\t");
    Serial.print(x00, DEC);
    Serial.print("\t");
    Serial.println(y00, DEC);
    Serial.print("// calibrateTouch01\t");
    Serial.print( x01, DEC);
    Serial.print("\t");
    Serial.println(y01, DEC);
    Serial.print("// calibrateTouch10\t");
    Serial.print( x10, DEC);
    Serial.print("\t");
    Serial.println(y10, DEC);
    Serial.print("// calibrateTouch11\t");
    Serial.print( x11, DEC);
    Serial.print("\t");
    Serial.println(y11, DEC);
    
    Serial.print("// _screenWidth\t");
    Serial.println( _screenWidth, DEC);
    Serial.print("// _screenHeigth\t");
    Serial.println( _screenHeigth, DEC);
    
    x0 = (x00+x01)/2;
    x1 = (x10+x11)/2;
    y0 = (y00+y10)/2;
    y1 = (y01+y11)/2;
    
    _touchXmin = x0 - 10 * (x1-x0) / (_screenWidth -10-10);
    _touchXmax = x1 + 10 * (x1-x0) / (_screenWidth -10-10);
    _touchYmin = y0 - 10 * (y1-y0) / (_screenHeigth-10-10);
    _touchYmax = y1 + 10 * (y1-y0) / (_screenHeigth-10-10);
    
    Serial.println(">>> 2");
    Serial.println("// Touch calibration");
    Serial.print("_touchXmin = ");
    Serial.print(_touchXmin, DEC);
    Serial.println(";");
    Serial.print("_touchXmax = ");
    Serial.print(_touchXmax, DEC);
    Serial.println(";");
    Serial.print("_touchYmin = ");
    Serial.print(_touchYmin, DEC);
    Serial.println(";");
    Serial.print("_touchYmax = ");
    Serial.print(_touchYmax, DEC);
    Serial.println(";");
    
    clear();
    setOrientation(old);
}

// Utilities
void LCD_screen_font::_swap(uint16_t &a, uint16_t &b)
{
    uint16_t w = a;
    a = b;
    b = w;
}
void LCD_screen_font::_swap(int16_t &a, int16_t &b)
{
    int16_t w = a;
    a = b;
    b = w;
}
//void LCD_screen_font::_swap(uint8_t &a, uint8_t &b)
//{
//    uint8_t w = a;
//    a = b;
//    b = w;
//}

uint16_t LCD_screen_font::_check(uint16_t x0, uint16_t xmin, uint16_t xmax)
{
    if (xmin < xmax)
    {
        if (x0 < xmin) return xmin;
        else if (x0 > xmax) return xmax;
        else return x0;
    } else {
        if (x0 < xmax) return xmax;
        else if (x0 > xmin) return xmin;
        else return x0;
    }
}

// Area functions
bool LCD_screen_font::isArea()
{
    return (_fsmArea > 0);
}

void LCD_screen_font::openArea(uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy)
{
    _setWindow(x0, y0, x0+dx-1, y0+dy-1);
    _fsmArea |= 0xf0;
}

void LCD_screen_font::feedArea(uint16_t colour)
{
    if (_fsmArea & 0xf0) _writeData88(highByte(colour), lowByte(colour));
}

void LCD_screen_font::closeArea()
{
    _closeWindow();
    _fsmArea &= ~0xf0;
}


// Font functions
void LCD_screen_font::setFontSolid(bool flag)
{
    _fontSolid = flag;
}

void LCD_screen_font::setFontSize(uint8_t size)
{
    if (size < MAX_FONT_SIZE) _fontSize = size;
    else _fontSize = MAX_FONT_SIZE -1;
}

uint8_t LCD_screen_font::fontMax()
{
    return MAX_FONT_SIZE;
}

uint8_t LCD_screen_font::fontSizeX()
{
#if (MAX_FONT_SIZE > 0)
    if (_fontSize == 0) return 6;
#if (MAX_FONT_SIZE > 1)
    else if (_fontSize == 1) return 8;
#if (MAX_FONT_SIZE > 2)
    else if (_fontSize == 2) return 12;
#if (MAX_FONT_SIZE > 3)
    else if (_fontSize == 3) return 16;
#endif // end MAX_FONT_SIZE > 3
#endif // end MAX_FONT_SIZE > 2
#endif // end MAX_FONT_SIZE > 1
#endif // end MAX_FONT_SIZE > 0
    else return 0;
}

//uint8_t LCD_screen_font::_fontSize
//{
//    return _fontSize;
//}

uint8_t LCD_screen_font::fontSizeY()
{
#if (MAX_FONT_SIZE > 0)
    if (_fontSize == 0) return 8;
#if (MAX_FONT_SIZE > 1)
    else if (_fontSize == 1) return 12;
#if (MAX_FONT_SIZE > 2)
    else if (_fontSize == 2) return 16;
#if (MAX_FONT_SIZE > 3)
    else if (_fontSize == 3) return 24;
#endif // end MAX_FONT_SIZE > 3
#endif // end MAX_FONT_SIZE > 2
#endif // end MAX_FONT_SIZE > 1
#endif // end MAX_FONT_SIZE > 0
    else return 0;
}

// Code
uint8_t LCD_screen_font::_getCharacter(uint8_t c, uint8_t i)
{
    
#if defined(ENERGIA)
    
#if (MAX_FONT_SIZE > 0)
    if (_fontSize == 0)      return Terminal6x8e[c][i];
#if (MAX_FONT_SIZE > 1)
    else if (_fontSize == 1) return Terminal8x12e[c][i];
#if (MAX_FONT_SIZE > 2)
    else if (_fontSize == 2) return Terminal12x16e[c][i];
#if (MAX_FONT_SIZE > 3)
    else if (_fontSize == 3) return Terminal16x24e[c][i];
#endif // end MAX_FONT_SIZE > 3
#endif // end MAX_FONT_SIZE > 2
#endif // end MAX_FONT_SIZE > 1
#endif // end MAX_FONT_SIZE > 0
    
    else return 0;
    
#else
    
#if (MAX_FONT_SIZE > 0)
    if (_fontSize == 0)      return pgm_read_byte(&Terminal6x8e[c][i]);
#if (MAX_FONT_SIZE > 1)
    else if (_fontSize == 1) return pgm_read_byte(&Terminal8x12e[c][i]);
#if (MAX_FONT_SIZE > 2)
    else if (_fontSize == 2) return pgm_read_byte(&Terminal12x16e[c][i]);
#if (MAX_FONT_SIZE > 3)
    else if (_fontSize == 3) return pgm_read_byte(&Terminal16x24e[c][i]);
#endif // end MAX_FONT_SIZE > 3
#endif // end MAX_FONT_SIZE > 2
#endif // end MAX_FONT_SIZE > 1
#endif // end MAX_FONT_SIZE > 0
    
    else return 0;
#endif
}


void LCD_screen_font::gText(uint16_t x0, uint16_t y0,
                            String s,
                            uint16_t textColour, uint16_t backColour,
                            uint8_t ix, uint8_t iy)
{
    uint8_t c;
    uint8_t line, line1;	//, line2, line3; //(unused RR commented)
    uint16_t x, y;
    uint8_t i, j, k;
    //    Serial.print("gText ");
    
    if ((ix > 1) || (iy > 1) || !_fontSolid)
    {
        
        //    Serial.print("LCD_screen_font::gText (");
        //    Serial.print(_fontSize, DEC);
        //    Serial.print(") ");
        //    Serial.println(s);
        
        if ((_fontSize == 0) && ((ix > 1) || (iy > 1)))
        {
            bool oldPenSolid = _penSolid;
            setPenSolid(true);
            
            for (k=0; k<s.length(); k++)
            {
                x = x0 + 6 * k * ix;
                y = y0;
                c = s.charAt(k)-' ';
                
                for ( i=0; i<6; i++ )
                {
                    line = _getCharacter(c, i);
                    for ( j = 0; j<8; j++)
                    {
                        if (bitRead(line, j)) rectangle(x+i*ix, y+j*iy, x+i*ix+ix-1, y+j*iy+iy-1, textColour);
                        else if (_fontSolid)  rectangle(x+i*ix, y+j*iy, x+i*ix+ix-1, y+j*iy+iy-1, backColour);
                    }
                }
            }
            setPenSolid(oldPenSolid);
        } else {
            
            
#if (MAX_FONT_SIZE > 0)
            if (_fontSize == 0)
            {
                
                for (k=0; k<s.length(); k++)
                {
                    c = s.charAt(k)-' ';
                    
                    for (i=0; i<6; i++)
                    {
                        line = _getCharacter(c, i);
                        
                        for (j=0; j<8; j++)
                            if (bitRead(line, j)) point(x0 + 6*k + i, y0 + j, textColour);
                            else if (_fontSolid)  point(x0 + 6*k + i, y0 + j, backColour);
                    }
                }
            }
#if (MAX_FONT_SIZE > 1)
            else if (_fontSize == 1)
            {
                
                for (k=0; k<s.length(); k++)
                {
                    c = s.charAt(k)-' ';
                    
                    for (i=0; i<8; i++)
                    {
                        line = _getCharacter(c, 2*i);
                        line1 =_getCharacter(c, 2*i+1);
                        
                        for (j=0; j<8; j++)
                        {
                            if (bitRead(line, j))   point(x0 + 8*k + i, y0 + j, textColour);
                            else if (_fontSolid)    point(x0 + 8*k + i, y0 + j, backColour);
                            if (bitRead(line1, j))  point(x0 + 8*k + i, y0 +8 + j, textColour);
                            else if ((_fontSolid) && (j<4)) point(x0 + 8*k + i, y0 +8 + j, backColour);
                        }
                    }
                }
            }
#if (MAX_FONT_SIZE > 2)
            else if (_fontSize == 2)
            {
                
                for (k=0; k<s.length(); k++)
                {
                    c = s.charAt(k)-' ';
                    
                    for (i=0; i<12; i++)
                    {
                        line = _getCharacter(c, 2*i);
                        line1 =_getCharacter(c, 2*i+1);
                        
                        for (j=0; j<8; j++)
                        {
                            if (bitRead(line, j))  point(x0 + 12*k + i, y0 + j,    textColour);
                            else if (_fontSolid)   point(x0 + 12*k + i, y0 + j,    backColour);
                            if (bitRead(line1, j)) point(x0 + 12*k + i, y0 +8 + j, textColour);
                            else if (_fontSolid)   point(x0 + 12*k + i, y0 +8 + j, backColour);
                        }
                    }
                }
            }
#if (MAX_FONT_SIZE > 3)
            else if (_fontSize == 3)
            {
                
                for (k=0; k<s.length(); k++)
                {
                    c = s.charAt(k)-' ';
                    
                    for (i=0; i<16; i++)
                    {
                        line  =_getCharacter(c, 3*i);
                        line1 =_getCharacter(c, 3*i+1);
                        line2 =_getCharacter(c, 3*i+2);
                        
                        for (j=0; j<8; j++)
                        {
                            if (bitRead(line, j))  point(x0 + 16*k + i, y0 + j,     textColour);
                            else if (_fontSolid)   point(x0 + 16*k + i, y0 + j,     backColour);
                            if (bitRead(line1, j)) point(x0 + 16*k + i, y0 +8 + j,  textColour);
                            else if (_fontSolid)   point(x0 + 16*k + i, y0 +8 + j,  backColour);
                            if (bitRead(line2, j)) point(x0 + 16*k + i, y0 +16 + j, textColour);
                            else if (_fontSolid)   point(x0 + 16*k + i, y0 +16 + j, backColour);
                        }
                    }
                }
            }
#endif // end MAX_FONT_SIZE > 3
#endif // end MAX_FONT_SIZE > 2
#endif // end MAX_FONT_SIZE > 1
#endif // end MAX_FONT_SIZE > 0
        }
        
    } else {
        
        uint8_t c;
        uint8_t line; //  line1; (unused RR commented)
//        uint16_t x, y; // (unused RR commented)
        uint8_t i, j, k;
        uint8_t highTextColour = highByte(textColour);
        uint8_t lowTextColour  = lowByte(textColour);
        uint8_t highBackColour = highByte(backColour);
        uint8_t lowBackColour  = lowByte(backColour);
        
#if (MAX_FONT_SIZE > 0)
        if (_fontSize == 0)
        {
            
            for (k=0; k<s.length(); k++)
            {
                c = s.charAt(k)-' ';
                _setWindow(x0 +fontSizeX()*k, y0, x0 +fontSizeX()*(k+1)-1, y0+fontSizeY()-1);
                
                for (j=0; j<8; j++)
                {
                    for (i=0; i<6; i++)
                    {
                        line = _getCharacter(c, i);
                        
                        if (bitRead(line, j))   _writeData88(highTextColour, lowTextColour);
                        else                    _writeData88(highBackColour, lowBackColour);
                    }
                }
            }
        }
#if (MAX_FONT_SIZE > 1)
        else if (_fontSize == 1)
        {
            
            for (k=0; k<s.length(); k++)
            {
                c = s.charAt(k)-' ';
                _setWindow(x0 +fontSizeX()*k, y0, x0 +fontSizeX()*(k+1)-1, y0+fontSizeY()-1);
                
                for (j=0; j<8; j++)
                {
                    for (i=0; i<8; i++)
                    {
                        line = _getCharacter(c, 2*i);
                        
                        if (bitRead(line, j))   _writeData88(highTextColour, lowTextColour);
                        else                    _writeData88(highBackColour, lowBackColour);
                    }
                }
                for (j=0; j<4; j++)
                {
                    for (i=0; i<8; i++)
                    {
                        line = _getCharacter(c, 2*i+1);
                        
                        if (bitRead(line, j))   _writeData88(highTextColour, lowTextColour);
                        else                    _writeData88(highBackColour, lowBackColour);
                    }
                }
            }
        }
#if (MAX_FONT_SIZE > 2)
        else if (_fontSize == 2)
        {
            
            for (k=0; k<s.length(); k++)
            {
                c = s.charAt(k)-' ';
                _setWindow(x0 +fontSizeX()*k, y0, x0 +fontSizeX()*(k+1)-1, y0+fontSizeY()-1);
                
                for (j=0; j<8; j++)
                {
                    for (i=0; i<12; i++)
                    {
                        line = _getCharacter(c, 2*i);
                        
                        if (bitRead(line, j))   _writeData88(highTextColour, lowTextColour);
                        else                    _writeData88(highBackColour, lowBackColour);
                    }
                }
                for (j=0; j<8; j++)
                {
                    for (i=0; i<12; i++)
                    {
                        line = _getCharacter(c, 2*i+1);
                        
                        if (bitRead(line, j))   _writeData88(highTextColour, lowTextColour);
                        else                    _writeData88(highBackColour, lowBackColour);
                    }
                }
            }
        }
#if (MAX_FONT_SIZE > 3)
        else if (_fontSize == 3)
        {
            
            for (k=0; k<s.length(); k++)
            {
                c = s.charAt(k)-' ';
                _setWindow(x0 +fontSizeX()*k, y0, x0 +fontSizeX()*(k+1)-1, y0+fontSizeY()-1);
                
                for (j=0; j<8; j++)
                {
                    for (i=0; i<16; i++)
                    {
                        line = _getCharacter(c, 3*i);
                        
                        if (bitRead(line, j))   _writeData88(highTextColour, lowTextColour);
                        else                    _writeData88(highBackColour, lowBackColour);
                    }
                }
                for (j=0; j<8; j++)
                {
                    for (i=0; i<16; i++)
                    {
                        line = _getCharacter(c, 3*i+1);
                        
                        if (bitRead(line, j))   _writeData88(highTextColour, lowTextColour);
                        else                    _writeData88(highBackColour, lowBackColour);
                    }
                }
                for (j=0; j<8; j++)
                {
                    for (i=0; i<16; i++)
                    {
                        line = _getCharacter(c, 3*i+2);
                        
                        if (bitRead(line, j))   _writeData88(highTextColour, lowTextColour);
                        else                    _writeData88(highBackColour, lowBackColour);
                    }
                }
            }
        }
#endif // end MAX_FONT_SIZE > 3
#endif // end MAX_FONT_SIZE > 2
#endif // end MAX_FONT_SIZE > 1
#endif // end MAX_FONT_SIZE > 0
    }
}

bool LCD_screen_font::isIntensity()
{
    return _flagIntensity;
}

void LCD_screen_font::setIntensity(uint8_t intensity)
{
    _setIntensity(intensity);
}

bool LCD_screen_font::isEnergy()
{
    return _flagEnergy;
}

void LCD_screen_font::setBacklight(bool state)
{
    _setBacklight(state);
}

void LCD_screen_font::setEnergy(uint32_t seconds)
{
    _msEnergy = (uint32_t)seconds*1000;
    _chronoEnergy = millis() + _msEnergy;
    _stateEnergy = true;
}

void LCD_screen_font::updateEnergy(bool state)
{
    if (_msEnergy > 0)
    {
        if (state)
        {
            _chronoEnergy = millis() + _msEnergy;
            if (!_stateEnergy) setBacklight(true);
            _stateEnergy = true;
        } else if (millis() > _chronoEnergy)
        {
            if (_stateEnergy) setBacklight(false);
            _stateEnergy = false;
        }
    }
}


