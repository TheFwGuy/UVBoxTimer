//
// LCD_screen_standard.cpp
// Class library C++ code
// ----------------------------------
// Developed with embedXcode
// http://embedXcode.weebly.com
//
// Project LCD_screen Library Suite
//
// Created by Rei VILO, mars 06, 2013 18:12
// embedXcode.weebly.com
//
//
// Copyright © Rei VILO, 2013-2014
// License All rights reserved
//
// See LCD_screen_standard.h and ReadMe.txt for references
//
// Last update: Mar 26, 2014 release    121 Patch for arc()


// Library header
#include "LCD_screen_standard.h"

// Code
LCD_screen_standard::LCD_screen_standard()
{
    _fontSize       = 0;
    _fontSolid      = true;
    _penSolid       = false;
    _flagRead       = false;
    _flagStorage    = false;
    _flagEnergy     = false;
    _touchTrim      = 0;
}

void LCD_screen_standard::showInformation(uint16_t &x0, uint16_t &y0)
{
    setFontSize(2);
    gText(x0 *2, y0, "LCD_screen Library Suite");
    y0 += fontSizeY() +1;
    setFontSize(1);
    gText(x0, y0, formatString("%12s %i", "Release", LCD_SCREEN_STANDARD_RELEASE));
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
//    gText(x0, y0, "Fonts:  " +String(fontmax() +1) + " header files");
//#endif
//    y0 += fontSizeY() +1;
}

void LCD_screen_standard::clear(uint16_t colour)
{
    uint8_t oldOrientation = _orientation;
    bool oldPenSolid = _penSolid;
    setOrientation(0);
    setPenSolid();
    rectangle(0, 0, screenSizeX()-1, screenSizeY()-1, colour);
    setOrientation(oldOrientation);
    setPenSolid(oldPenSolid);
}

void LCD_screen_standard::setOrientation(uint8_t orientation)
{
    _orientation = orientation % 4;
    _setOrientation(_orientation);
}

uint8_t LCD_screen_standard::getOrientation()
{
    return _orientation;
}

uint16_t LCD_screen_standard::screenSizeX()
{
    switch (_orientation) {
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
    return(-1);		// (invalid return RR commented)
}

uint16_t LCD_screen_standard::screenSizeY()
{
    switch (_orientation) {
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
    return(-1); //(inv return RR add & commented)
}

uint16_t LCD_screen_standard::screenDiagonal()
{
    return _screenDiagonal;
}



void LCD_screen_standard::dLine(uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy, uint16_t colour)
{
    line(x0, y0, x0+dx-1, y0+dy-1, colour);
}


void LCD_screen_standard::setPenSolid(bool flag)
{
    _penSolid = flag;
}

void LCD_screen_standard::point(uint16_t x1, uint16_t y1, uint16_t colour)
{
    _setPoint(x1, y1, colour);
}

void LCD_screen_standard::dRectangle(uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy, uint16_t colour)
{
    rectangle(x0, y0, x0+dx-1, y0+dy-1, colour);
}


void LCD_screen_standard::arc(uint16_t x0, uint16_t y0, uint16_t radius, uint16_t start, uint16_t end, uint16_t colour)
{
    ;
}

//void LCD_screen_standard::setFontSolid(bool flag)
//{
//    _fontSolid = flag;
//}

// Touch
bool LCD_screen_standard::isTouch()
{
    return (_touchTrim > 0);
}

bool LCD_screen_standard::isReadable()
{
    return _flagRead;
}

bool LCD_screen_standard::isStorage()
{
    return _flagStorage;
}

uint16_t LCD_screen_standard::readPixel(uint16_t x1, uint16_t y1)
{
    return 0;
}

void LCD_screen_standard::copyPasteArea(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t dx, uint16_t dy)
{
    
}

void LCD_screen_standard::copyArea(uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy, uint32_t &address)
{
    
}

void LCD_screen_standard::pasteArea(uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy, uint32_t &address, bool option)
{
    
}

bool LCD_screen_standard::isArea()
{
    return (_fsmArea > 0);
}

void LCD_screen_standard::setFontSolid(bool flag)
{
    _fontSolid = flag;
}



void LCD_screen_standard::_displayTarget(uint16_t x0, uint16_t y0, uint16_t colour)
{
    setPenSolid(false);
    circle(x0, y0, 8, colour);
    circle(x0, y0, 7, colour);
    circle(x0, y0, 6, colour);
    
//    line(x0-9, y0, x0+9, y0, colour);
//    line(x0, y0-9, x0, y0+9, colour);
}

bool LCD_screen_standard::getTouch(uint16_t &x, uint16_t &y, uint16_t &z)
{
    if (_touchTrim == 0) return false;
    
    uint16_t x0, y0, z0;
    
    _getRawTouch(x0, y0, z0);

    z = z0;
    
    if (z > _touchTrim) {
//        Serial.print(x0, DEC);
//        Serial.print("\t");
//        Serial.print(y0, DEC);
//        Serial.print("\t");
//        Serial.print(z0, DEC);
//        Serial.print("\t->\t");

        x0 = _check(x0, _touchXmin, _touchXmax);
        y0 = _check(y0, _touchYmin, _touchYmax);
        
        switch (_orientation) {
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
        
//        Serial.print(x, DEC);
//        Serial.print("\t");
//        Serial.print(y, DEC);
//        Serial.print("\t");
//        Serial.print(z, DEC);
//        Serial.println();

        return true;
    } else {
        return false;
    }
}

void LCD_screen_standard::calibrateTouch()
{
    if (_touchTrim == 0) return;
    
    uint16_t x00, y00, x10, x01, x11, y10, y01, y11;
    uint16_t x0, x1, y0, y1, z0;
    uint8_t  old = _orientation;
    
    setOrientation(0);
    clear();
    
    setFontSize(0);
    gText(screenSizeX()/2-17*fontSizeX()/2, screenSizeY()/2-fontSizeY(), "TOUCH CALIBRATION", myColours.red, myColours.black);
    gText(screenSizeX()/2-16*fontSizeX()/2, screenSizeY()/2,             "Press the centre",  myColours.red, myColours.black);
    gText(screenSizeX()/2-17*fontSizeX()/2, screenSizeY()/2+fontSizeY(), "of the red circle", myColours.red, myColours.black);
    
    _displayTarget(10, 10, myColours.red);
    z0 = 0;
    do _getRawTouch(x00, y00, z0); while (z0 < _touchTrim);
    _displayTarget(10, 10, myColours.green);
    do _getRawTouch(x0, y0, z0); while (z0 > 0);
    
    delay(500);
    _displayTarget(_screenWidth-10, 10, myColours.red);
    z0 = 0;
    do _getRawTouch(x10, y10, z0); while (z0<_touchTrim);
    _displayTarget(_screenWidth-10, 10, myColours.green);
    do _getRawTouch(x0, y0, z0); while (z0 > 0);
    
    delay(500);
    _displayTarget(_screenWidth-10, _screenHeigth-10, myColours.red);
    z0 = 0;
    do _getRawTouch(x11, y11, z0); while (z0<_touchTrim);
    _displayTarget(_screenWidth-10, _screenHeigth-10, myColours.green);
    do _getRawTouch(x0, y0, z0); while (z0 > 0);
    
    delay(500);
    _displayTarget(10, _screenHeigth-10, myColours.red);
    z0 = 0;
    do _getRawTouch(x01, y01, z0); while (z0<_touchTrim);
    _displayTarget(10, _screenHeigth-10, myColours.green);
    do _getRawTouch(x0, y0, z0); while (z0 > 0);
    
    x0 = (x00+x01)/2;
    x1 = (x10+x11)/2;
    y0 = (y00+y10)/2;
    y1 = (y01+y11)/2;
    
    _touchXmin = x0 - 10 * (x1-x0) / (_screenWidth -10-10);
    _touchXmax = x1 + 10 * (x1-x0) / (_screenWidth -10-10);
    _touchYmin = y0 - 10 * (y1-y0) / (_screenHeigth-10-10);
    _touchYmax = y1 + 10 * (y1-y0) / (_screenHeigth-10-10);
    
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
void LCD_screen_standard::_swap(uint16_t &a, uint16_t &b)
{
    uint16_t w = a;
    a = b;
    b = w;
}
void LCD_screen_standard::_swap(int16_t &a, int16_t &b)
{
    int16_t w = a;
    a = b;
    b = w;
}
void LCD_screen_standard::_swap(uint8_t &a, uint8_t &b)
{
    uint8_t w = a;
    a = b;
    b = w;
}

uint16_t LCD_screen_standard::_check(uint16_t x0, uint16_t xmin, uint16_t xmax)
{
    if (xmin < xmax)
    {
        if (x0 < xmin) return xmin;
        else if (x0 > xmax) return xmax;
        else return x0;
    }
    else
    {
        if (x0 < xmax) return xmax;
        else if (x0 > xmin) return xmin;
        else return x0;
    }
}

void LCD_screen_standard::openArea(uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy) { ; } // compulsory
void LCD_screen_standard::feedArea(uint16_t colour) { ; } // compulsory
void LCD_screen_standard::closeArea() { ; } // compulsory

bool LCD_screen_standard::isEnergy()
{
    return _flagEnergy;
}

void LCD_screen_standard::setBacklight(bool state)
{
    _setBacklight(state);
}

void LCD_screen_standard::setEnergy(uint32_t seconds)
{
    _msEnergy = (uint32_t)seconds*1000;
    _chronoEnergy = millis() + _msEnergy;
    _stateEnergy = true;
}

void LCD_screen_standard::updateEnergy(bool state)
{
    if (_msEnergy > 0) {
        if (state) {
            _chronoEnergy = millis() + _msEnergy;
            if (!_stateEnergy) setBacklight(true);
            _stateEnergy = true;
        } else if (millis() > _chronoEnergy) {
            if (_stateEnergy) setBacklight(false);
            _stateEnergy = false;
        }
    }
}

bool LCD_screen_standard::isIntensity()
{
    return _flagIntensity;
}

void LCD_screen_standard::setIntensity(uint8_t intensity)
{
    _setIntensity(intensity);
}

