//
// LCD_graphics.cpp
// Library C++ code
// ----------------------------------
// Developed with embedXcode
// http://embedXcode.weebly.com
//
// Project LCD_screen_font_main
//
// Created by Rei VILO, mai 31, 2013 18:31
// http://embedXcode.weebly.com
//
//
// Copyright © Rei VILO, 2013-2014
// License CC = BY NC SA
//
// See LCD_graphics.h and ReadMe.txt for references
//


// Library header
#include "LCD_graphics.h"

// Code
mtgDial::mtgDial() {
    ;
}

void mtgDial::drawDial(LCD_screen_virtual * lcd,
                       uint16_t x0, uint16_t y0, uint16_t radius,
                       uint16_t backColour, uint16_t frontColour,
                       uint16_t gridColour, uint16_t valueColour)
{
    _pscreen = lcd;
    
    _x0     = x0;
    _y0     = y0;
    _radius = radius;
    
    _backColour  = backColour;
    _frontColour = frontColour;
    _gridColour  = gridColour;
    _valueColour = valueColour;
    
    _pscreen->setPenSolid(true);
    _pscreen->circle(_x0, _y0, _radius, _backColour);
    _pscreen->setPenSolid(false);
    _pscreen->circle(_x0, _y0, _radius, _frontColour);
}



gClock::gClock() {
    ;
}

void gClock::dDefine(LCD_screen_virtual * lcd,
                     uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy,
                     uint16_t backColour, uint16_t frontColour,
                     uint16_t hourColour, uint16_t minuteColour, uint16_t secondColour)
{
    uint16_t radius = min(dx, dy)/2;
    define(lcd, x0+dx/2, y0+dy/2, radius-2, backColour, frontColour, hourColour, minuteColour, secondColour);
}

void gClock::define(LCD_screen_virtual * lcd,
                    uint16_t x0, uint16_t y0, uint16_t radius,
                    uint16_t backColour, uint16_t frontColour,
                    uint16_t hourColour, uint16_t minuteColour, uint16_t secondColour)
{
    drawDial(lcd, x0, y0, radius, backColour, frontColour, 0, 0);
    
    _hourColour = hourColour;
    _minuteColour = minuteColour;
    _secondColour = secondColour;
    
    _oldHourX = x0;
    _oldHourY = y0;
    _oldMinuteX = x0;
    _oldMinuteY = y0;
    _oldSecondX = x0;
    _oldSecondY = y0;
    
    int32_t aa;
    _pscreen->setPenSolid(true);
    for (int8_t i=0; i<12; i++) {
        aa = i*3000;
        _pscreen->circle(_x0-(_radius-2)*cos32x100( aa )/100, _y0-(_radius-2)*sin32x100( aa )/100, 2, _frontColour);
    }
    _pscreen->circle(_x0, _y0-_radius+4, 4, _frontColour);
}

void gClock::_needle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t colour)
{
    _pscreen->line(x1, y1-1, x2, y2-1, colour);
    _pscreen->line(x1, y1,   x2, y2,   colour);
    _pscreen->line(x1, y1+1, x2, y2+1, colour);
}

void gClock::draw(uint8_t hour, uint8_t minute, uint8_t second, String text)
{
    int32_t hourX, hourY, minuteX, minuteY, secondX, secondY;
    int32_t _newHour, _newMinute, _newSecond;
    boolean flagH, flagM, flagS;
    
    _newHour   = hour;
    _newMinute = minute;
    _newSecond = second;
    
    hourX   = int32_t( _x0+_radius*6/10*cos32x100( (100*_newHour + 100*_newMinute/60                  +  900) * 360/12 )/100 );
    hourY   = int32_t( _y0+_radius*6/10*sin32x100( (100*_newHour + 100*_newMinute/60                  +  900) * 360/12 )/100 );
    minuteX = int32_t( _x0+_radius*8/10*cos32x100( (               100*_newMinute + 100*_newSecond/60 + 4500) * 360/60 )/100 );
    minuteY = int32_t( _y0+_radius*8/10*sin32x100( (               100*_newMinute + 100*_newSecond/60 + 4500) * 360/60 )/100 );
    secondX = int32_t( _x0+_radius*8/10*cos32x100( (                                100*_newSecond    + 4500) * 360/60 )/100 );
    secondY = int32_t( _y0+_radius*8/10*sin32x100( (                                100*_newSecond    + 4500) * 360/60 )/100 );
    
    flagH = (hourX!=_oldHourX) || (hourY!=_oldHourY);
    flagM = (minuteX!=_oldMinuteX) || (minuteY!=_oldMinuteY);
    flagS = (secondX!=_oldSecondX) || (secondY!=_oldSecondY);
    
    // old time with _backColour
    if ( flagS ) {
        _pscreen->line(_x0, _y0, _oldSecondX, _oldSecondY, _backColour);
    }
    if ( flagM ) {
        _needle(_x0,   _y0, _oldMinuteX,   _oldMinuteY, _backColour);
    }
    if ( flagH ) {
        _needle(_x0-1, _y0,   _oldHourX-1, _oldHourY,   _backColour);
        _needle(_x0,   _y0,   _oldHourX,   _oldHourY,   _backColour);
        _needle(_x0+1, _y0,   _oldHourX+1, _oldHourY,   _backColour);
    }
    
    if ( (flagH || flagM || flagS) && (text != "") ) {
        _pscreen->setFontSize(1);
        _pscreen->setFontSolid(true);
        
        String _s = text.substring(0, min(text.length(), 16));
        int32_t k = _s.length()*_pscreen->fontSizeX()/2;
        
        if (k != _oldK) {
            // Label
            _pscreen->setPenSolid(true);
            _pscreen->dRectangle(_x0-_oldK-1, _y0+_radius/2-2, _oldK*2+2, _pscreen->fontSizeY()+3, _backColour);
            
            _oldK = k;
        }
        
        _pscreen->setPenSolid(false);
        _pscreen->dRectangle(_x0-k-1, _y0+_radius/2-2, k*2+2, _pscreen->fontSizeY()+3, _frontColour);
        _pscreen->gText(_x0-k, _y0+_radius/2, _s, _frontColour, _backColour);
        
    }
    
    // new time with _hourColour, _minuteColour, _secondColour
    _needle(_x0-1, _y0, hourX-1, hourY,   _hourColour);
    _needle(_x0,   _y0, hourX,   hourY,   _hourColour);
    _needle(_x0+1, _y0, hourX+1, hourY,   _hourColour);
    
    _needle(_x0,   _y0, minuteX, minuteY, _minuteColour);
    
    _pscreen->line(_x0, _y0, secondX, secondY, _secondColour);
    
    _pscreen->setPenSolid(true);
    _pscreen->circle(_x0, _y0, 3, _frontColour);
    
    _oldHourX = hourX;
    _oldHourY = hourY;
    _oldMinuteX = minuteX;
    _oldMinuteY = minuteY;
    _oldSecondX = secondX;
    _oldSecondY = secondY;
    
}

gGauge::gGauge() {
    ;
}

void gGauge::dDefine(LCD_screen_virtual * lcd,
                     uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy,
                     int32_t valueMin, int32_t valueMax,
                     int32_t unit,
                     uint16_t memory, uint16_t grid,
                     uint16_t backColour, uint16_t frontColour, uint16_t gridColour,
                     uint16_t valueColour, uint16_t minColour, uint16_t maxColour)
{
    uint16_t radius = min(dx, dy)/2;
    define(lcd,
           x0+dx/2, y0+dy/2, radius-2,
           valueMin, valueMax,
           unit,
           memory, grid,
           backColour, frontColour, gridColour,
           valueColour, minColour, maxColour);
    
}

void gGauge::define(LCD_screen_virtual * lcd,
                    uint16_t x0, uint16_t y0, uint16_t radius,
                    int32_t valueMin, int32_t valueMax,
                    int32_t unit,
                    uint16_t memory, uint16_t grid,
                    uint16_t backColour, uint16_t frontColour, uint16_t gridColour,
                    uint16_t valueColour, uint16_t minColour, uint16_t maxColour)
{
    drawDial(lcd, x0, y0, radius, backColour, frontColour, gridColour, valueColour);
    
    _oldZ = 45000;
    _oldK = 0;
    
    _minColour = minColour;
    _maxColour = maxColour;
    
    _valueMin  = valueMin;
    _valueMax  = valueMax;
    _unit      = unit;
    _memory    = memory;
    _grid      = grid;
    
    _n   = 0; // number of values shown
    _minZ = 0; // y coordinate of min for memory
    _maxZ = 0; // y coordinate of max for memory
    _amnesiaMin = 0;
    _amnesiaMax = 0;
    
    int32_t aa;
    _pscreen->setPenSolid(false);
    if (_grid > 0)
        for (int32_t i=0; i<=_grid; i++) {
            aa = 31500 + i*27000/_grid;
            _pscreen->circle(_x0-(_radius-2)*cos32x100( aa )/100, _y0-(_radius-2)*sin32x100( aa )/100, 2, _gridColour);
        }
}

void gGauge::needle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t colour)
{
    _pscreen->line(x1, y1-1, x2, y2-1, colour);
    _pscreen->line(x1, y1,   x2, y2,   colour);
    _pscreen->line(x1, y1+1, x2, y2+1, colour);
}


void gGauge::draw(int32_t value, String text)
{
    //    int32_t z = 31500+27000*(value-_valueMin)/(_valueMax-_valueMin);
    String _s;
    int32_t z = map(value, _valueMin, _valueMax, 31500, 58500);
    if ( z < 31500 )   z = 31500;
    if ( z > 58500 )   z = 58500;
    int32_t k = min(_radius*8/10, _radius-8-3);
    int32_t l = 0;
    uint16_t vX  = _x0-k*cos32x100(z)/100;
    uint16_t vY  = _y0-k*sin32x100(z)/100;
    uint16_t _vX = _x0-k*cos32x100(_oldZ)/100;
    uint16_t _vY = _y0-k*sin32x100(_oldZ)/100;
    
    _oldZ = z;
    if ( (vX == _vX) && (vY == _vY) ) return;
    
    //    Serial.print("line(");
    //    Serial.print(_x0, DEC);
    //    Serial.print(", ");
    //    Serial.print(_y0, DEC);
    //    Serial.print(", ");
    //    Serial.print(vX, DEC);
    //    Serial.print(", ");
    //    Serial.print(vY, DEC);
    //    Serial.println(", myColours.white);");
    
    // value
    //    needle(_x0-1, _y0,   vX-1, vY,   _valueColour);
    //    needle(_x0,   _y0,   vX,   vY,   _valueColour);
    //    needle(_x0+1, _y0,   vX+1, vY,   _valueColour);
    
    needle(_x0-1, _y0,   _vX-1, _vY,   _backColour);
    needle(_x0,   _y0,   _vX,   _vY,   _backColour);
    needle(_x0+1, _y0,   _vX+1, _vY,   _backColour);
    
    needle(_x0-1, _y0,   vX-1, vY,   _valueColour);
    needle(_x0,   _y0,   vX,   vY,   _valueColour);
    needle(_x0+1, _y0,   vX+1, vY,   _valueColour);
    
    // min and max memory
    if (_memory>0) {
        _pscreen->setPenSolid(true);
        // first time
        if (_n==0) {
            _maxZ = z;
            _minZ = z;
            _n++;
        }
        
        boolean fMax = false;
        boolean fMin = false;
        int32_t pxMax = _x0-(_radius-8)*cos32x100(_maxZ)/100;
        int32_t pyMax = _y0-(_radius-8)*sin32x100(_maxZ)/100;
        int32_t pxMin = _x0-(_radius-8)*cos32x100(_minZ)/100;
        int32_t pyMin = _y0-(_radius-8)*sin32x100(_minZ)/100;
        
        // max - coordinates in normal scale
        if (z >= _maxZ) {
            _pscreen->circle(pxMax, pyMax, 2, _backColour);
            _maxZ = z;
            _amnesiaMax = _memory;
        } else if (_amnesiaMax > 1) {
            fMax = true;
            _amnesiaMax--;
        } else {
            _pscreen->circle(pxMax, pyMax, 2, _backColour);
            _maxZ = z;
        } // max
        
        // min - coordinates in normal scale
        if (z <= _minZ) {
            _pscreen->circle(pxMin, pyMin, 2, _backColour);
            _minZ = z;
            _amnesiaMin = _memory;
        } else if (_amnesiaMin > 0) {
            fMin = true;
            _amnesiaMin--;
        } else {
            _pscreen->circle(pxMin, pyMin, 2, _backColour);
            _minZ = z;
        } // min
        
        if (fMin) _pscreen->circle(_x0-(_radius-8)*cos32x100(_minZ)/100, _y0-(_radius-8)*sin32x100(_minZ)/100, 2, _minColour);
        if (fMax) _pscreen->circle(_x0-(_radius-8)*cos32x100(_maxZ)/100, _y0-(_radius-8)*sin32x100(_maxZ)/100, 2, _maxColour);
    } // min and max memory
    
    // min and max values
    _pscreen->setFontSize(0);
    _pscreen->setFontSolid(true);
    _pscreen->gText(_x0-_radius, _y0+_radius-_pscreen->fontSizeY(), String(_valueMin), _frontColour);
    _pscreen->gText(_x0+_radius-_pscreen->fontSizeX()*String(_valueMax).length(), _y0+_radius-_pscreen->fontSizeY(), String(_valueMax), _frontColour);
    
    // string display
    if (text!="") {
        _pscreen->setPenSolid(false);
        _pscreen->setFontSolid(true);
        
        //        uint8_t i=2;
        //        do {
        //            i--;
        //            _pscreen->setFont(i);
        //        } while ( (text.length()*_pscreen->fontSizeX() > (2*_radius*8/10)) && (i>0) );
        //
        //        String _s = text.substring(0, min(text.length(), (2*_radius*8/10) / _pscreen->fontSizeX()));
        //        k = text.length()*_pscreen->fontSizeX()/2;
        //        l = _radius*9/10-_pscreen->fontSizeY();
        //
        //        _pscreen->dRectangle(_x0-k-1, _y0+l-2, 2*k+3, _pscreen->fontSizeY()+3, _gridColour);
        //        _pscreen->gText(_x0-k, _y0+l, _s, _frontColour);
        
        
        _pscreen->setFontSize(1);
        _pscreen->setFontSolid(true);
        
        _s = text.substring(0, min(text.length(), _radius / _pscreen->fontSizeX() / 2 ));
        k = _s.length()*_pscreen->fontSizeX();
        l = _radius*9/10-2*_pscreen->fontSizeY();
        
        if (k != _oldK) {
            // Label
            _pscreen->setPenSolid(true);
            _pscreen->dRectangle(_x0-_radius/2, _y0+l-2, _radius, _pscreen->fontSizeY()+3, _backColour);
            
            _pscreen->setPenSolid(false);
            _pscreen->dRectangle(_x0-_radius/2, _y0+l-2, _radius, _pscreen->fontSizeY()+3, _gridColour);
            
            _oldK = k;
        }
        
        _pscreen->gText(_x0-k/2, _y0+l, _s, _frontColour, _backColour); // , 2, 2);
        
        
        
    } // string display
    
    _pscreen->setPenSolid(true);
    _pscreen->circle(_x0, _y0, 3, _valueColour);
    
    //// debug
    //  Serial.print(value, 2);
    //  Serial.print("\t>");
    //  Serial.print(z, 2);
    //  Serial.print("\t min:");
    //  Serial.print(_minZ, 2);
    //  Serial.print("\t(");
    //  Serial.print(_amnesiaMin, DEC);
    //  Serial.print(")\t max:");
    //  Serial.print(_maxZ, 2);
    //  Serial.print("\t(");
    //  Serial.print(_amnesiaMax, DEC);
    //  Serial.print(")\n");
}

gYaw::gYaw() {
    ;
}

void gYaw::dDefine(LCD_screen_virtual * lcd,
                   uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy,
                   int32_t unit,
                   uint16_t backColour, uint16_t frontColour,
                   uint16_t gridColour, uint16_t valueColour)
{
    uint16_t radius = min(dx, dy)/2;
    define(lcd, x0-radius, y0-radius, radius-2, backColour, frontColour, gridColour, valueColour);
}

void gYaw::define(LCD_screen_virtual * lcd,
                  uint16_t x0, uint16_t y0, uint16_t radius,
                  int32_t unit,
                  uint16_t backColour, uint16_t frontColour,
                  uint16_t gridColour, uint16_t valueColour)
{
    drawDial(lcd, x0, y0, radius, backColour, frontColour, gridColour, valueColour);
    
    //  _pscreen = lcd;
    //
    //  _x0 = x0;
    //  _y0 = y0;
    //  _radius = radius;
    //  _backColour = backColour;
    //  _frontColour = frontColour;
    //  _gridColour = gridColour;
    //  _valueColour = valueColour;
    
    //  _pscreen->setPenSolid(true);
    //  _pscreen->circle(_x0, _y0, _radius, _backColour);
    //  _pscreen->setPenSolid(false);
    //  _pscreen->circle(_x0, _y0, _radius, _gridColour);
    
    _oldDegrees = 0;
    _unit = unit;
}

void gYaw::draw(int32_t valueDegrees)
{
    int32_t value, c, s;
    uint16_t x1, x2, x3, x4, y1, y2, y3, y4;
    
    valueDegrees *= 100 / _unit;
    
    if (valueDegrees!=_oldDegrees) {
        
        // old value with _backColour
        //        radians = (-_oldDegrees / 180.0 * PI + PI);
        //        c = cos(radians) * 0.6 * _radius;
        //        s = sin(radians) * 0.6 * _radius;
        value = -_oldDegrees + 18000;
        c = cos32x100(value) * 6/10 * _radius /100;
        s = sin32x100(value) * 6/10 * _radius /100;
        
        x1 = _x0 - c * 5/10    ;
        y1 = _y0 + s * 5/10    ;
        x2 = _x0 + c * 5/10    ;
        y2 = _y0 - s * 5/10    ;
        x3 = _x0 - c * 5/10 - s;
        y3 = _y0 + s * 5/10 - c;
        x4 = _x0 + c * 5/10 - s;
        y4 = _y0 - s * 5/10 - c;
        
        _pscreen->line(x1, y1, x3, y3, _backColour);
        _pscreen->line(x3, y3, x4, y4, _backColour);
        _pscreen->line(x2, y2, x4, y4, _backColour);
        
        x3 = _x0 - c;
        y3 = _y0 + s;
        x4 = _x0 + c;
        y4 = _y0 - s;
        _pscreen->line(x1, y1, x3, y3, _backColour);
        _pscreen->line(x2, y2, x4, y4, _backColour);
        
        x1 = _x0 + s;
        y1 = _y0 + c;
        _pscreen->line(x1, y1, x3, y3, _backColour);
        _pscreen->line(x1, y1, x4, y4, _backColour);
        
        
        // new value with _frontColour
        //        radians = (-degrees / 180.0 * PI + PI);
        //        c = cos(radians) * 0.6 * _radius;
        //        s = sin(radians) * 0.6 * _radius;
        value = -valueDegrees + 18000;
        c = cos32x100(value) * 6/10 * _radius /100;
        s = sin32x100(value) * 6/10 * _radius /100;
        
        x1 = _x0 - c * 5/10    ;
        y1 = _y0 + s * 5/10    ;
        x2 = _x0 + c * 5/10    ;
        y2 = _y0 - s * 5/10    ;
        x3 = _x0 - c * 5/10 - s;
        y3 = _y0 + s * 5/10 - c;
        x4 = _x0 + c * 5/10 - s;
        y4 = _y0 - s * 5/10 - c;
        
        _pscreen->line(x1, y1, x3, y3, _valueColour);
        _pscreen->line(x3, y3, x4, y4, _valueColour);
        _pscreen->line(x2, y2, x4, y4, _valueColour);
        
        x3 = _x0 - c;
        y3 = _y0 + s;
        x4 = _x0 + c;
        y4 = _y0 - s;
        _pscreen->line(x1, y1, x3, y3, _valueColour);
        _pscreen->line(x2, y2, x4, y4, _valueColour);
        
        x1 = _x0 + s;
        y1 = _y0 + c;
        _pscreen->line(x1, y1, x3, y3, _valueColour);
        _pscreen->line(x1, y1, x4, y4, _valueColour);
        
        _oldDegrees = valueDegrees;
    }
    
    _pscreen->setPenSolid(true);
    _pscreen->circle(_x0, _y0, 2, _frontColour);
}


gPitch::gPitch() {
    ;
}

void gPitch::dDefine(LCD_screen_virtual * lcd,
                     uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy,
                     int32_t unit,
                     uint16_t backColour, uint16_t frontColour,
                     uint16_t gridColour, uint16_t valueColour)
{
    uint16_t radius = min(dx, dy)/2;
    define(lcd, x0-radius, y0-radius, radius-2, unit, backColour, frontColour, gridColour, valueColour);
}

void gPitch::define(LCD_screen_virtual * lcd,
                    uint16_t x0, uint16_t y0, uint16_t radius,
                    int32_t unit,
                    uint16_t backColour, uint16_t frontColour,
                    uint16_t gridColour, uint16_t valueColour)
{
    drawDial(lcd, x0, y0, radius, backColour, frontColour, gridColour, valueColour);
    
    //  _pscreen = lcd;
    //
    //  _x0 = x0;
    //  _y0 = y0;
    //  _radius = radius;
    //  _backColour = backColour;
    //  _frontColour = frontColour;
    //  _gridColour = gridColour;
    //  _valueColour = valueColour;
    
    //  _pscreen->setPenSolid(true);
    //  _pscreen->circle(_x0, _y0, _radius, _backColour);
    //  _pscreen->setPenSolid(false);
    //  _pscreen->circle(_x0, _y0, _radius, _gridColour);
    
    _oldDegrees = 0;
    _unit = unit;
}

void gPitch::draw(int32_t valueDegrees)
{
    int32_t value,  c, s; // radians, (commented RR)
    uint16_t x1, x2, y1, y2;
    
    valueDegrees *= 100 / _unit;
    
    if (valueDegrees != _oldDegrees) {
        // old value with _backColour
        value = _oldDegrees;
        
        if (value > 18000)  value -= 36000;
        if (value < -18000) value += 36000;
        //        radians = (value / 180.0 * PI);
        c = cos32x100(value) * 6/10 * _radius /100;
        s = sin32x100(value) * 6/10 * _radius /100;
        
        x1 = _x0 + c;
        y1 = _y0 - s;
        x2 = _x0 - c;
        
        if (x1 > x2) {
            y2 = x2;
            x2 = x1;
            x1 = y2;
        }
        _pscreen->line(x1-_radius/10, y1, x2+_radius/10, y1, _backColour);
        
        x1 = _x0 + c * 7/10;
        x2 = _x0 - c * 7/10;
        
        if (x1 > x2) {
            y2 = x2;
            x2 = x1;
            x1 = y2;
        }
        
        x1 -= _radius/20;
        x2 += _radius/20;
        
        if ((valueDegrees>=-90) && (value<=90)) {
            y1 -= _radius/6;
        }
        else    {
            y1 += _radius/6;
        }
        _pscreen->line(x1, y1, x2, y1, _backColour);
        
        
        // new value with _frontColour
        value = valueDegrees;
        if (value > 18000)  value -= 36000;
        if (value < -18000) value += 36000;
        //        radians = (value / 180.0 * PI);
        c = cos32x100(value) * 6/10 * _radius /100;
        s = sin32x100(value) * 6/10 * _radius /100;
        
        x1 = _x0 + c;
        y1 = _y0 - s;
        x2 = _x0 - c;
        
        if (x1 > x2) {
            y2 = x2;
            x2 = x1;
            x1 = y2;
        }
        _pscreen->line(x1-_radius/10, y1, x2+_radius/10, y1, _valueColour);
        
        x1 = _x0 + c * 7/10;
        x2 = _x0 - c * 7/10;
        
        if (x1 > x2) {
            y2 = x2;
            x2 = x1;
            x1 = y2;
        }
        
        x1 -= _radius/20;
        x2 += _radius/20;
        
        if ((valueDegrees>=-90) && (value<=90)) {
            y1 -= _radius/6;
        }
        else    {
            y1 += _radius/6;
        }
        _pscreen->line(x1, y1, x2, y1, _valueColour);
        
        _pscreen->setPenSolid(true);
        _pscreen->circle(_x0, _y0, 2, _frontColour);
        
        _oldDegrees = valueDegrees;
    }
}


gRoll::gRoll() {
    ;
}

void gRoll::dDefine(LCD_screen_virtual * lcd,
                    uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy,
                    int32_t unit,
                    uint16_t backColour, uint16_t frontColour,
                    uint16_t gridColour, uint16_t valueColour)
{
    uint16_t radius = min(dx, dy)/2;
    define(lcd, x0-radius, y0-radius, radius-2, unit, backColour, frontColour, gridColour, valueColour);
}

void gRoll::define(LCD_screen_virtual * lcd,
                   uint16_t x0, uint16_t y0, uint16_t radius,
                   int32_t unit,
                   uint16_t backColour, uint16_t frontColour,
                   uint16_t gridColour, uint16_t valueColour)
{
    drawDial(lcd, x0, y0, radius, backColour, frontColour, gridColour, valueColour);
    
    //  _pscreen = lcd;
    //
    //  _x0 = x0;
    //  _y0 = y0;
    //  _radius = radius;
    //  _backColour = backColour;
    //  _frontColour = frontColour;
    //  _gridColour = gridColour;
    //  _valueColour = valueColour;
    
    //  _pscreen->setPenSolid(true);
    //  _pscreen->circle(_x0, _y0, _radius, _backColour);
    //  _pscreen->setPenSolid(false);
    //  _pscreen->circle(_x0, _y0, _radius, _gridColour);
    
    _oldDegrees = 0;
    _unit = unit;
}

void gRoll::draw(int32_t valueDegrees)
{
    int32_t value, c, s; // radians, (commented RR)
    //  uint16_t x1, x2, y1, y2;
    
    valueDegrees *= 100 / _unit;
    
    if (valueDegrees!=_oldDegrees) {
        // old value with _backColour
        //radians = (-_oldDegrees / 180.0 * PI + PI);
        value = -_oldDegrees + 18000;
        c = cos32x100(value) * 6/10 * _radius /100;
        s = sin32x100(value) * 6/10 * _radius /100;
        
        //    x1 = _x0 - c;
        //    y1 = _y0 + s;
        //    x2 = _x0 + c;
        //    y2 = _y0 - s;
        //    _pscreen->line(x1, y1, x2, y2, _backColour);
        
        _pscreen->line(_x0 - c, _y0 + s,
                       _x0 + c, _y0 - s, _backColour);
        
        
        //    x1 = _x0 - c * 7/10 + s * 3/10;
        //    y1 = _y0 + s * 7/10 + c * 3/10;
        //    x2 = _x0 + c * 7/10 + s * 3/10;
        //    y2 = _y0 - s * 7/10 + c * 3/10;
        //    _pscreen->line(x1, y1, x2, y2, _valueColour);
        
        _pscreen->line(_x0 - c * 7/10 + s * 3/10, _y0 + s * 7/10 + c * 3/10,
                       _x0 + c * 7/10 + s * 3/10, _y0 - s * 7/10 + c * 3/10, _backColour);
        
        
        // new value with _frontColour
        //radians = (-degrees / 180.0 * PI + PI);
        //c = cos(radians) * 0.6 * _radius;
        //s = sin(radians) * 0.6 * _radius;
        value = -valueDegrees + 18000;
        c = cos32x100(value) * 6/10 * _radius /100;
        s = sin32x100(value) * 6/10 * _radius /100;
        
        _pscreen->line(_x0 - c, _y0 + s,
                       _x0 + c, _y0 - s, _valueColour);
        
        _pscreen->line(_x0 - c * 7/10 + s * 3/10, _y0 + s * 7/10 + c * 3/10,
                       _x0 + c * 7/10 + s * 3/10, _y0 - s * 7/10 + c * 3/10, _valueColour);
        
        _pscreen->setPenSolid(true);
        _pscreen->circle(_x0, _y0, 2, _frontColour);
        
        _oldDegrees = valueDegrees;
    }
}

mtgPanel::mtgPanel() {
    ;
}

void mtgPanel::drawPanel(LCD_screen_virtual * lcd,
                         uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy,
                         uint16_t lapse, uint16_t gridX, uint16_t gridY, bool continous,
                         uint16_t backColour, uint16_t frontColour, uint16_t gridColour)
{
    _pscreen = lcd;
    
    _x0 = x0;
    _y0 = y0;
    _dx = dx;
    _dy = dy;
    
    _backColour  = backColour;
    _frontColour = frontColour;
    _gridColour  = gridColour;
    
    _continous = (continous && (_pscreen->isReadable()));
    _lapse     = lapse;
    _gridX     = gridX;
    _gridY     = gridY;
    _gridC     = 0;
    _chrono    = millis();
    _n         = 0; // number of values shown
    
    
    _pscreen->setPenSolid(true);
    _pscreen->dRectangle(_x0, _y0, _dx, _dy, _backColour);
    
    _pscreen->setPenSolid(false);
    _pscreen->dRectangle(_x0, _y0, _dx, _dy, _gridColour);
    
    if (_gridX>0) for (uint16_t i=1; i<_gridX; i++) {
        _pscreen->dLine(_x0+1, _y0+_dy*i/_gridX, _dx-2, 1, myColours.halve(_gridColour));
    }
    
}

void mtgPanel::drawContinue() {
    // continuity management
    if (_continous) {
        // right-most side
        if (_n>_dx-4) {
            // move
            _pscreen->copyPasteArea(_x0+2, _y0, _x0+1, _y0, _dx-3, _dy);
            // new
            _pscreen->line(_x0+_n+1, _y0+1, _x0+_n+1, _y0+_dy-2, _backColour);
            
            // gridY
            if (_gridY>0) {
                if (_gridC==0) _pscreen->line(_x0+_n+1, _y0+1, _x0+_n+1, _y0+_dy-2, myColours.halve(_gridColour));
                _gridC++;
                _gridC %= _gridY;
            } // gridY
            
            // gridX
            if (_gridX>0) for (uint16_t i=1; i<_gridX; i++)
                _pscreen->point(_x0+_n+1, _y0+map(i, 0, _gridX, 0, _dy), myColours.halve(_gridColour));
        } else {
            _n++;
        } // right-most side
    } else {
        // right
        if (_n>_dx-4) {
            _n=1;
        } else {
            _n++;
        } // right
        
        _pscreen->line(_x0+_n+1, _y0+1, _x0+_n+1, _y0+_dy-2, _backColour);
        _pscreen->line(_x0+_n+2, _y0+1, _x0+_n+2, _y0+_dy-2, _gridColour);
        
        // gridY
        if (_gridY>0) {
            if (_gridC==0) _pscreen->line(_x0+_n+1, _y0+1, _x0+_n+1, _y0+_dy-2, myColours.halve(_gridColour));
            _gridC++;
            _gridC %= _gridY;
        } // gridY
        
        // gridX
        if (_gridX>0) for (uint16_t i=1; i<_gridX; i++) {
            _pscreen->point(_x0+_n+1, _y0+map(i, 0, _gridX, 0, _dy), myColours.halve(_gridColour));
        }
    } // continuity management
}

void mtgPanel::drawLapse(uint32_t ms)
{
    uint16_t colourFront, colourBack;
    
    // lapse error
    if (_lapse>0) {
        
        if (ms>_lapse) {
            colourFront = _backColour; // reverse mode
            colourBack = _frontColour;
        } else {
            colourFront = _frontColour; // normal mode
            colourBack = _backColour;
        }
        
        //        _pscreen->setBackGroundColour(colourBack);
        
        if (_gridY>0) {
            _pscreen->gText(_x0+_dx-2-7*_pscreen->fontSizeX(), _y0+_dy-2-_pscreen->fontSizeY(), ttoa(ms*_gridY, 7), colourFront, colourBack);
        } else {
            _pscreen->gText(_x0+_dx-2-7*_pscreen->fontSizeX(), _y0+_dy-2-_pscreen->fontSizeY(),
                            ttoa(ms, 7), colourFront, colourBack);
        } // _gridY
        
    } else {
        
        //        _pscreen->setBackGroundColour(_frontColour);
        _pscreen->gText(_x0+_dx-2-7*_pscreen->fontSizeX(), _y0+_dy-2-_pscreen->fontSizeY(),
                        ttoa(ms, 7), _backColour, _frontColour);
    } // end _lapse>0
}


gTable::gTable() {
    ;
}

void gTable::dDefine(LCD_screen_virtual * lcd,
                     uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy,
                     uint8_t gridX, uint8_t gridY,
                     uint16_t backColour, uint16_t gridColour)
{
    drawPanel(lcd,
              x0, y0, dx, dy,
              0, 0, 0, 0,
              backColour, 0x0000, gridColour);
    
    _gridX = gridX;
    _gridY = gridY;
    _dx--;
    _dy--;
    _enable = false;
    
    for (uint8_t i=0; i<_gridX; i++) {
        _pscreen->dLine(_x0+_dx*i/_gridX, _y0, 1, _dy, _gridColour);
    }
    for (uint8_t j=0; j<_gridY; j++) {
        _pscreen->dLine(_x0, _y0+_dy*j/_gridY, _dx, 1, _gridColour);
    }
}


void gTable::define(LCD_screen_virtual * lcd,
                    uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,
                    uint8_t gridX, uint8_t gridY,
                    uint16_t backColour, uint16_t gridColour)
{
    dDefine(lcd,
            x1, y1, x2-x1, y2-y1,
            gridX, gridY,
            backColour, gridColour);
}

void gTable::draw(uint8_t ix, uint8_t iy, uint16_t colour)
{
    _pscreen->setPenSolid(true);
    _pscreen->rectangle(_x0+_dx*ix/_gridX+3, _y0+_dy*iy/_gridY+3, _x0+_dx*(ix+1)/_gridX-3, _y0+_dy*(iy+1)/_gridY-3, colour);
}

void gTable::enable(bool flag)
{
    _enable = flag;
}

bool gTable::check(bool instant)
{
    if (!_enable) return false;
    
    uint16_t x, y, z;
    
    // down
    if (_pscreen->getTouch(x, y, z)) {
        
        // pressed
        if ((x>=_x0) && (x<=_x0+_dx) && (y>=_y0) && (y<=_y0+_dy)) {
            _ix = (x - _x0) * _gridX / _dx;
            _iy = (y - _y0) * _gridY / _dy;
            
            if (instant) return true;
            
            // debounce
            while (_pscreen->getTouch(x, y, z)) delay(100);
            
            // up
            //                _pscreen->getTouchXY(x, y);
            if ((x>=_x0) && (x<=_x0+_dx) && (y>=_y0) && (y<=_y0+_dy)) return true;
            
            
            Serial.print("failed");
            Serial.println(millis(), DEC);
        } // if pressed
    } // if down
    return false;
}

void gTable::getIndex(uint8_t &ix, uint8_t &iy)
{
    ix = _ix;
    iy = _iy;
}


gHistogram::gHistogram() {
    ;
}

void gHistogram::define(LCD_screen_virtual * lcd,
                        uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,
                        int32_t valueMin, int32_t valueMax,
                        int32_t unit,
                        uint32_t lapse, uint16_t memory,
                        uint16_t gridX, uint16_t gridY, bool continous,
                        uint16_t backColour, uint16_t frontColour, uint16_t gridColour,
                        uint16_t valueColour, uint16_t minColour, uint16_t maxColour)
{
    dDefine(lcd,
            x1, y1, x2-x1, y2-y1,
            valueMin, valueMax,
            unit,
            lapse, memory,
            gridX, gridY, continous,
            backColour, frontColour, gridColour,
            valueColour, minColour, maxColour);
}

void gHistogram::dDefine(LCD_screen_virtual * lcd,
                         uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy,
                         int32_t valueMin, int32_t valueMax,
                         int32_t unit,
                         uint32_t lapse, uint16_t memory,
                         uint16_t gridX, uint16_t gridY, bool continous,
                         uint16_t backColour, uint16_t frontColour, uint16_t gridColour,
                         uint16_t valueColour, uint16_t minColour, uint16_t maxColour)
{
    drawPanel(lcd,
              x0, y0, dx, dy,
              lapse, gridX, gridY, continous,
              backColour, frontColour, gridColour);
    
    _valueColour = valueColour;
    _minColour   = minColour;
    _maxColour   = maxColour;
    
    _valueMin  = valueMin;
    _valueMax  = valueMax;
    _unit      = unit;
    _memory    = memory;
    
    _min = 0; // y coordinate of min for memory
    _max = 0; // y coordinate of max for memory
    _amnesiaMin=0;
    _amnesiaMax=0;
}


void gHistogram::draw(int32_t value)
{
    uint32_t period = millis()-_chrono;
    if ( (_lapse > 0) && (period < _lapse) ) return;
    _chrono = millis();
    
    uint16_t y = _y0+1+_dy-2 - (uint16_t)( (value-_valueMin)*(_dy-2)/(_valueMax-_valueMin) );
    if (y < _y0+2)     y = _y0+2;
    if (y > _y0+_dy-4) y = _y0+_dy-4;
    
    drawContinue(); // continuity management
    
    // value
    _pscreen->setPenSolid(true);
    _pscreen->dRectangle(_x0+_n, y, 2, 2, _valueColour);
    
    // min and max memory
    if (_memory > 0) {
        
        // first time
        if (_n == 0) {
            _max = y;
            _min = y;
        }
        
        // max - coordinates in reverse scale
        if (y <= _max) {
            _max = y;
            _amnesiaMax = _memory;
        }
        else if (_amnesiaMax > 0) {
            _pscreen->dRectangle(_x0+_n, _max, 2, 2, _maxColour);
            _amnesiaMax--;
        }
        else {
            _max = y;
        } // max
        
        // min - coordinates in reverse scale
        if (y >= _min) {
            _min = y;
            _amnesiaMin = _memory;
        }
        else if (_amnesiaMin > 0) {
            _pscreen->dRectangle(_x0+_n, _min, 2, 2, _minColour);
            _amnesiaMin--;
        }
        else {
            _min = y;
        } // min
    } // min and max memory
    
    // units
    _pscreen->setFontSize(0);
    _pscreen->setFontSolid(true);
    
    Serial.println(_valueMax, DEC);
    Serial.println(_unit, DEC);
    Serial.println(i32toa(_valueMax, _unit, 1, 0));
    
    _pscreen->gText(_x0+3, _y0+2, i32toa(_valueMax, _unit, 1, 0), _frontColour);
    _pscreen->gText(_x0+3, _y0+_dy-2-_pscreen->fontSizeY(), i32toa(_valueMin, _unit, 1, 0), _frontColour);
    
    drawLapse(period); // lapse error
    
} // end gHistogram


gMultiple::gMultiple() {
    ;
}

void gMultiple::define(LCD_screen_virtual * lcd,
                       uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,
                       String serie1, int32_t valueMin1, int32_t valueMax1, int32_t unit1, uint16_t valueColour1,
                       String serie2, int32_t valueMin2, int32_t valueMax2, int32_t unit2, uint16_t valueColour2,
                       String serie3, int32_t valueMin3, int32_t valueMax3, int32_t unit3, uint16_t valueColour3,
                       String serie4, int32_t valueMin4, int32_t valueMax4, int32_t unit4, uint16_t valueColour4,
                       uint32_t lapse, uint16_t gridX, uint16_t gridY, bool continous,
                       uint16_t backColour, uint16_t frontColour, uint16_t gridColour)
{
    dDefine(lcd,
            x1, y1, x2-x1, y2-y1,
            serie1, valueMin1, valueMax1, unit1, valueColour1,
            serie2, valueMin2, valueMax2, unit2, valueColour2,
            serie3, valueMin3, valueMax3, unit3, valueColour3,
            serie4, valueMin4, valueMax4, unit4, valueColour4,
            lapse, gridX, gridY, continous,
            backColour, frontColour, gridColour);
}

void gMultiple::dDefine(LCD_screen_virtual * lcd,
                        uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy,
                        String serie1, int32_t valueMin1, int32_t valueMax1, int32_t unit1, uint16_t valueColour1,
                        String serie2, int32_t valueMin2, int32_t valueMax2, int32_t unit2, uint16_t valueColour2,
                        String serie3, int32_t valueMin3, int32_t valueMax3, int32_t unit3, uint16_t valueColour3,
                        String serie4, int32_t valueMin4, int32_t valueMax4, int32_t unit4, uint16_t valueColour4,
                        uint32_t lapse, uint16_t gridX, uint16_t gridY, bool continous,
                        uint16_t backColour, uint16_t frontColour, uint16_t gridColour)
{
    drawPanel(lcd,
              x0, y0, dx, dy,
              lapse, gridX, gridY, continous,
              backColour, frontColour, gridColour);
    
    _pscreen->setFontSize(0);
    
    uint8_t n = _dx / _pscreen->fontSizeX();
    if (n>6) n = 6;
    
    _name1 = serie1.substring(0, n);
    while (_name1.length()<n) _name1 = " " + _name1;
    _name2 = serie2.substring(0, n);
    while (_name2.length()<n) _name2 = " " + _name2;
    _name3 = serie3.substring(0, n);
    while (_name3.length()<n) _name3 = " " + _name3;
    _name4 = serie4.substring(0, n);
    while (_name4.length()<n) _name4 = " " + _name4;
    
    _valueMin1 = valueMin1;
    _valueMin2 = valueMin2;
    _valueMin3 = valueMin3;
    _valueMin4 = valueMin4;
    
    _valueMax1 = valueMax1;
    _valueMax2 = valueMax2;
    _valueMax3 = valueMax3;
    _valueMax4 = valueMax4;
    
    _unit1     = unit1;
    _unit2     = unit2;
    _unit3     = unit3;
    _unit4     = unit4;
    
    _valueColour1 = valueColour1;
    _valueColour2 = valueColour2;
    _valueColour3 = valueColour3;
    _valueColour4 = valueColour4;
}


void gMultiple::draw(int32_t value1, int32_t value2, int32_t value3, int32_t value4)
{
    uint32_t period = millis()-_chrono;
    if ( (_lapse > 0) && (period < _lapse) ) return;
    _chrono = millis();
    
    drawContinue(); // continuity management
    
    // values, units and names of the series
    _pscreen->setPenSolid(true);
    _pscreen->setFontSize(0);
    _pscreen->setFontSolid(true);
    
    uint16_t _y1 = _y0+1+_dy-2 - (uint16_t)( (value1-_valueMin1)*(_dy-2)/(_valueMax1-_valueMin1) );
    if (_y1 < _y0+2)     _y1 = _y0+2;
    if (_y1 > _y0+_dy-4) _y1 = _y0+_dy-4;
    _pscreen->dRectangle(_x0+_n, _y1, 2, 2, _valueColour1);
    _pscreen->gText(_x0+3, _y0+2,
                    i32toa(_valueMax1, _unit1, 0, 6), _valueColour1);
    _pscreen->gText(_x0+3, _y0+_dy-2-_pscreen->fontSizeY(),
                    i32toa(_valueMin1, _unit1, 0, 6), _valueColour1);
    _pscreen->gText(_x0+3, _y0+2+_pscreen->fontSizeY(),
                    _name1, _valueColour1);
    
    if ( (_valueMax2!=0) || (_valueMin2!=0) ) {
        uint16_t _y2 = _y0+1+_dy-2 - (uint16_t)( (value2-_valueMin2)*(_dy-2)/(_valueMax2-_valueMin2) );
        if (_y2<_y0+2)     _y2 = _y0+2;
        if (_y2>_y0+_dy-4) _y2 = _y0+_dy-4;
        _pscreen->dRectangle(_x0+_n, _y2, 2, 2, _valueColour2);
        _pscreen->gText(_x0+3 + 6*_pscreen->fontSizeX(), _y0+2,
                        i32toa(_valueMax2, _unit2, 0, 6), _valueColour2);
        _pscreen->gText(_x0+3 + 6*_pscreen->fontSizeX(), _y0+_dy-2-_pscreen->fontSizeY(),
                        i32toa(_valueMin2, _unit2, 0, 6), _valueColour2);
        _pscreen->gText(_x0+3 +  6*_pscreen->fontSizeX(), _y0+2+_pscreen->fontSizeY(),
                        _name2, _valueColour2);
    }
    
    if ( (_valueMax3!=0) || (_valueMin3!=0) ) {
        uint16_t _y3 = _y0+1+_dy-2 - (uint16_t)( (value3-_valueMin3)*(_dy-2)/(_valueMax3-_valueMin3) );
        if (_y3<_y0+2)     _y3 = _y0+2;
        if (_y3>_y0+_dy-4) _y3 = _y0+_dy-4;
        _pscreen->dRectangle(_x0+_n, _y3, 2, 2, _valueColour3);
        _pscreen->gText(_x0+3 + 12*_pscreen->fontSizeX(), _y0+2,
                        i32toa(_valueMax3, _unit3, 0, 6), _valueColour3);
        _pscreen->gText(_x0+3 + 12*_pscreen->fontSizeX(), _y0+_dy-2-_pscreen->fontSizeY(),
                        i32toa(_valueMin3, _unit3, 0, 6), _valueColour3);
        _pscreen->gText(_x0+3 + 12*_pscreen->fontSizeX(), _y0+2+_pscreen->fontSizeY(),
                        _name3, _valueColour3);
    }
    
    if ( (_valueMax4!=0) || (_valueMin4!=0) ) {
        uint16_t _y4 = _y0+1+_dy-2 - (uint16_t)( (value4-_valueMin4)*(_dy-2)/(_valueMax4-_valueMin4) );
        if (_y4<_y0+2)     _y4 = _y0+2;
        if (_y4>_y0+_dy-4) _y4 = _y0+_dy-4;
        _pscreen->dRectangle(_x0+_n, _y4, 2, 2, _valueColour4);
        _pscreen->gText(_x0+3 + 18*_pscreen->fontSizeX(), _y0+2,
                        i32toa(_valueMax4, _unit4, 0, 6), _valueColour4);
        _pscreen->gText(_x0+3 + 18*_pscreen->fontSizeX(), _y0+_dy-2-_pscreen->fontSizeY(),
                        i32toa(_valueMin4, _unit4, 0, 6), _valueColour4);
        _pscreen->gText(_x0+3 + 18*_pscreen->fontSizeX(), _y0+2+_pscreen->fontSizeY(),
                        _name4, _valueColour4);
    }
    
    drawLapse(period); // lapse error
} // end gMultiple








