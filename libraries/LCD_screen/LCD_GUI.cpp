//
// LCD_GUI.cpp
// Library C++ code
// ----------------------------------
// Developed with embedXcode
// http://embedXcode.weebly.com
//
// Project LCD_screen Library Suite
//
// Created by Rei VILO, Jun 14, 2013
// http://embedXcode.weebly.com
//
//
// Copyright © Rei VILO, 2013-2014
// License CC = BY NC SA
//
// See LCD_GUI.h and ReadMe.txt for references
//

// Library header
#include "LCD_GUI.h"

#if (CHRONO_DEBUG==1)
uint32_t chrono_ms;
#endif


// ---- utility
item setItem(uint8_t index, String text) {
    item item;
    item.index = index;
    text.toCharArray(item.label, 16);
    return item;
}


// ---- area
area::area() {
    ;
}

void area::dDefine(LCD_screen_virtual  * lcd,
                   uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy,
                   uint16_t index)
{
    _pscreen = lcd;
    
    _x0 = x0;
    _y0 = y0;
    _dx = dx;
    _dy = dy;
    
    _index = index;
    _enable = false;
    _option = false;
}


bool area::state() {
    return _enable;
}

void area::enable(bool flag)
{
    _enable = flag;
}

void area::setOption(bool flag)
{
    _option = flag;
}

bool area::check(bool instant)
{
    if (!_enable) return false;
//    bool flag;
    
    uint16_t x, y, z;
    
    // down
    if (_pscreen->getTouch(x, y, z)) {
        
        // pressed
        if ((x>=_x0) && (x<=_x0+_dx) && (y>=_y0) && (y<=_y0+_dy)) {
            if (instant) return true;
            
            // debounce
            //            do {
            //                delay(10);
            //                flag = _pscreen->getTouch(x, y, z);
            //                Serial.print(".");
            //            } while (flag);
            delay(50);
            
            while (_pscreen->getTouch(x, y, z)) {
                //                Serial.print(".a");
                delay(100);
            }
            // up
            if ((x>=_x0) && (x<=_x0+_dx) && (y>=_y0) && (y<=_y0+_dy)) return true;
        } // if pressed
    } // if down
    return false;
}

uint16_t area::getIndex()
{
    return _index;
}


// ---- button
button::button() {
    ;
}

// string-based button
void button::dStringDefine(LCD_screen_virtual * lcd,
                           uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy,
                           String text0, uint16_t textColour1, uint16_t highColour1)
{
    dStringDefine(lcd, x0, y0, dx, dy, text0, textColour1, highColour1, myColours.halve(highColour1), 9);
}


void button::dStringDefine(LCD_screen_virtual * lcd,
                           uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy,
                           String text0, uint16_t textColour1, uint16_t highColour1, uint16_t lowColour1, uint8_t size0)
{
    dDefine(lcd, x0, y0, dx, dy, setItem(0, text0), textColour1, highColour1, lowColour1, size0);
}


void button::dDefine(LCD_screen_virtual * lcd,
                     uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy,
                     item item0, uint16_t textColour1, uint16_t highColour1)
{
    dDefine(lcd, x0, y0, dx, dy, item0, textColour1, highColour1, myColours.halve(highColour1), 9);
}


void button::dDefine(LCD_screen_virtual  * lcd,
                     uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy,
                     item item0, uint16_t textColour1, uint16_t highColour1, uint16_t lowColour1, uint8_t size0)
{
    area::dDefine(lcd, x0, y0, dx, dy, item0.index);
    
    _text = item0.label;
    _textColour = textColour1;
    _highColour = highColour1;
    _lowColour  = lowColour1;
    _size = size0;
    _value = false;
    
    setOption(false);
}


void button::draw(bool flag)
{
    // 4 states
    // enable = true,  bool flag = true  : pressed button
    // enable = true,  bool flag = false : available button
    // enable = false, bool flag = true  : statement button
    // enable = false, bool flag = false : unavailable button
    uint16_t colourText, colourLabel, colourRectangle;
    
    colourText = _textColour;
    colourRectangle = _highColour;
    colourLabel = _highColour;
    
    if (_enable) {
        if ( flag || ( _option && _value ) ) {
            colourRectangle = _lowColour;
        } else {
            colourLabel = _lowColour;
        }
    } else {
        if (flag) {
            ;
        } else {
            colourText = myColours.halve(_textColour);
            colourLabel = myColours.halve(_lowColour);
            colourRectangle = myColours.halve(_highColour);
        }
    }
    
    dLabel(_pscreen, _x0, _y0, _dx, _dy, _text, colourText, colourLabel, 0, 0, _size);
    _pscreen->setPenSolid(false);
    _pscreen->dRectangle(_x0+2, _y0+2, _dx-4, _dy-4, colourRectangle);
}


//void button::enable(bool flag)
//{
//    _enable = flag;
//}

bool button::check(bool instant)
{
    uint16_t x, y, z;
    bool flag = false;
    
    if (!_enable) return false;
    
    // down
    if (_pscreen->getTouch(x, y, z)) {
        
        // pressed
        if ((x>=_x0) && (x<=_x0+_dx) && (y>=_y0) && (y<=_y0+_dy)) {
            // instant
            if (instant) {
                if (_option) _value = ~_value;
                flag = true;
            } else {
                draw(true);
                
                delay(50);
                // debounce
                //                do {
                //                    Serial.println(z, DEC);
                //                    delay(10);
                //                }
                while (_pscreen->getTouch(x, y, z)) {
                    delay(100);
                }
                //                Serial.print("*");
                
                // released
                delay(50);
                
                if ((x>=_x0) && (x<=_x0+_dx) && (y>=_y0) && (y<=_y0+_dy)) {
                    flag = true;
                    if (_option) _value = ~_value;
                }
            } // if instant
            
            draw(false || (_option && _value) );
        } // if pressed
    } // if down
    return flag;
}

void button::setValue(bool value)
{
    _value = value;
}

bool button::getValue()
{
    return _value;
}


// ---- cursor
cursor::cursor() {
    ;
}

void cursor::dDefine(LCD_screen_virtual * lcd,
                     uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy,
                     int32_t minValue, int32_t maxValue,
                     uint16_t cursorColour, uint16_t highColour, uint16_t lowColour)
{
    area::dDefine(lcd, x0, y0, dx, dy, 0);
    _cursorColour = cursorColour;
    _highColour = highColour;
    _lowColour = lowColour;
    
    _minValue = minValue;
    _maxValue = maxValue;
    setOption(false);
}


void cursor::setOption(bool flag)
{
    area::setOption(flag);
    
    if (_option) {              // vertical
        //        Serial.println("cursor::setOption vertical");
        //
        _gMin = _y0 + 7;
        _gMax = _y0 + _dy - 7;
        _gLine = _x0 + _dx/2;
    } else {                    // horizontal
        //        Serial.println("cursor::setOption horizontal");
        //
        _gMin = _x0 + 7;
        _gMax = _x0 + _dx - 7;
        _gLine = _y0 + _dy/2;
    }
    
    _gPosition = _gMin;
}


void cursor::draw(bool flag)
{
    // 4 states
    // enable = true,  bool flag = true  : pressed button
    // enable = true,  bool flag = false : available button
    // enable = false, bool flag = true  : statement button
    // enable = false, bool flag = false : unavailable button
    uint16_t colourCursor, colourBackground, colourRectangle;
    
    colourCursor = _cursorColour;
    colourRectangle = _highColour;
    colourBackground = _highColour;
    
    if (_enable) {
        if (flag) {
            colourRectangle = _lowColour;
        } else {
            colourBackground = _lowColour;
        }
    } else {
        if (flag) {
            ;
        } else {
            colourCursor = myColours.halve(_cursorColour);
            colourBackground = myColours.halve(_lowColour);
            colourRectangle = myColours.halve(_highColour);
        }
    }
    
    _pscreen->setPenSolid(true);
    _pscreen->dRectangle(_x0, _y0, _dx, _dy, colourBackground);
    _pscreen->setPenSolid(false);
    _pscreen->dRectangle(_x0+2, _y0+2, _dx-4, _dy-4, colourRectangle);
    
    _pscreen->setPenSolid(true);
    if (_option) {              // vertical
        //        Serial.println("cursor::draw vertical");
        //
        _pscreen->dRectangle(_gLine, _gMin, 2, _gMax-_gMin, colourCursor);
        _pscreen->dRectangle(_x0+4, _gPosition - 3, _dx-8, 6, colourCursor);
    } else {                    // horizontal
        //        Serial.println("cursor::draw horizontal");
        //
        _pscreen->dRectangle(_gMin, _gLine, _gMax-_gMin, 2, colourCursor);
        _pscreen->dRectangle(_gPosition - 3, _y0+4, 6, _dy-8, colourCursor);
    }
}


bool cursor::check(bool instant)
{
    uint16_t x, y, z, _gLast, _gOld;
    bool flag = false;
    
    if (!_enable) return false;
    
    // down
    if (_pscreen->getTouch(x, y, z)) {
        
        if (_option) {                                                          // vertical
            if (y < min(_gMin, _gMax)) y = min(_gMin, _gMax);
            if (y > max(_gMin, _gMax)) y = max(_gMin, _gMax);
        } else {                                                                // horizontal
            if (x < min(_gMin, _gMax)) x = min(_gMin, _gMax);
            if (x > max(_gMin, _gMax)) x = max(_gMin, _gMax);
        }
        
        // pressed
        if ((x>=_x0) && (x<=_x0+_dx) && (y>=_y0) && (y<=_y0+_dy)) {
            _gOld = _gPosition;
            _gLast = _gOld;
            
            // instant
            if (instant) {
                if (_option) {                                                  // vertical
                    if (y != _gOld) {
                        _gPosition = y;
                        flag = true;
                    }
                } else {                                                        // horizontal
                    if (x != _gOld) {
                        _gPosition = x;
                        flag = true;
                    }
                }
            } else {
                delay(50);
                // debounce
                while (_pscreen->getTouch(x, y, z)) {
                    if (_option) {                                              // vertical
                        if (y < min(_gMin, _gMax)) y = min(_gMin, _gMax);
                        if (y > max(_gMin, _gMax)) y = max(_gMin, _gMax);
                    } else {                                                    // horizontal
                        if (x < min(_gMin, _gMax)) x = min(_gMin, _gMax);
                        if (x > max(_gMin, _gMax)) x = max(_gMin, _gMax);
                    }
                    
                    if ((x>=_x0) && (x<=_x0+_dx) && (y>=_y0) && (y<=_y0+_dy)) {
                        if (_option) {                                          // vertical
                            if (y != _gPosition) _gPosition = y;
                        } else {                                                // horizontal
                            if (x != _gPosition) _gPosition = x;
                        }
                        
                    } else {
                        _gPosition = _gOld;
                    }
                    
                    if (_gPosition != _gLast) {
                        _gLast = _gPosition;
                        draw(true);
                    }
                }
                
                // up
                if (z == 2)  {
                    if ((x>=_x0) && (x<=_x0+_dx) && (y>=_y0) && (y<=_y0+_dy)) {
                        if (_option) {                                          // vertical
                            if (y != _gOld) {
                                _gPosition = y;
                                flag = true;
                            }
                        } else {                                                // horizontal
                            if (x != _gOld) {
                                _gPosition = x;
                                flag = true;
                            }
                        }
                    } else {
                        _gPosition = _gLast;
                    }
                } // if up
            } // if instant
            draw();
        } // if pressed
    } // if down
    return flag;
}

void cursor::setValue(int32_t value)
{
    if (value < min(_minValue, _maxValue)) value = min(_minValue, _maxValue);
    if (value > max(_minValue, _maxValue)) value = max(_minValue, _maxValue);
    
    _gPosition = map(value, _minValue, _maxValue, _gMin, _gMax);
    
    //    Serial.print(value, DEC);
    //    Serial.print(" => ");
    //    Serial.println(_gPosition, DEC);
}

int32_t cursor::getValue()
{
    return map(_gPosition, _gMin, _gMax, _minValue, _maxValue);
}


// ---- dialog
uint8_t dialog(LCD_screen_virtual * lcd,
               String text0, uint8_t kind0, uint16_t textColour0, uint16_t highColour0, uint16_t lowColour0,
               String text1, String button1, uint16_t textColour1, uint16_t highColour1, uint16_t lowColour1,
               String text2, String button2, uint16_t textColour2, uint16_t highColour2, uint16_t lowColour2,
               String text3, String button3, uint16_t textColour3, uint16_t highColour3, uint16_t lowColour3) {
    
    LCD_screen_virtual * _pscreen = lcd;
    uint32_t localAddress = 0;
//    uint32_t chrono;
    
    // Check SD
    //    if ( !_pscreen->checkSD() ) a = _pscreen->initSD();
    
    // landscape and portrait coordinates
    uint16_t _x0 = (_pscreen->screenSizeX() - 200) /2;
    uint16_t _y0 = (_pscreen->screenSizeY() - 200) /2;
    
    // Save initial screen
    //    if ( _pscreen->checkSD() ) {
    //        if ( _pscreen->checkRAW() ) {
    //            a = _pscreen->dSaveScreenRAW((uint32_t)0, _x0, _y0, 200, 200);
    //            //      Serial.print("\r\n dialog dSaveScreenRAW \t");
    //            //      Serial.print(a, HEX);
    //            //      Serial.print("\r\n");
    //        }
    //        else {
    //            a = _pscreen->dSaveScreenFAT("dialog.tmp", _x0, _y0, 200, 200);
    //            //      Serial.print("\r\n dialog dSaveScreenFAT \t");
    //            //      Serial.print(a, HEX);
    //            //      Serial.print("\r\n");
    //        }
    //    }
    if (_pscreen->isReadable() && _pscreen->isStorage()) {
        localAddress = 0;
        
#if (CHRONO_DEBUG==1)
        Serial.print("dialog copyArea (ms)\t");
        chrono_ms = millis();
#endif
        
        _pscreen->copyArea(_x0, _y0, 200, 200, localAddress);
        
        //        _pscreen->setPenSolid(false);
        //        _pscreen->dRectangle(_x0,  _y0, 200, 200, yellowColour);
        
#if (CHRONO_DEBUG==1)
        Serial.println(millis() - chrono_ms, DEC);
#endif
    }
    
    // Draw window
    _pscreen->setPenSolid(true);
    _pscreen->dRectangle(_x0, _y0, 200, 200, lowColour0);
    _pscreen->setPenSolid(false);
    _pscreen->dRectangle(_x0+2, _y0+2, 200-2-2, 200-2-2, highColour0);
    
    // String &result0, String text0, uint8_t kind0
    // uint16_t textColour0, uint16_t highColour0, uint16_t lowColour0,
    uint16_t _length;
    _length = max(text1.length(), text2.length());
    _length = max(_length, text3.length());
   
        uint16_t _size= 2;
        do {
            _size--;
            _pscreen->setFontSize(_size);
        }
        while ((_length * _pscreen->fontSizeX() > 200-60-10) && (_size > 0));

    dLabel(_pscreen, _x0+ 60, _y0+ 20, 200-60-10, 20, text0, textColour0, lowColour0, 1, 1, 9);
    dLabel(_pscreen, _x0+ 10, _y0+ 70, 200-10-10, 20, text1, textColour0, lowColour0, 1, 1, _size);
    dLabel(_pscreen, _x0+ 10, _y0+ 90, 200-10-10, 20, text2, textColour0, lowColour0, 1, 1, _size);
    dLabel(_pscreen, _x0+ 10, _y0+110, 200-10-10, 20, text3, textColour0, lowColour0, 1, 1, _size);
    
    // uint8_t kind0,
    uint16_t colour;
    char label;
    switch (kind0) {
        case 1:
            colour = myColours.blue;  // blue !
            label ='!';
            break;
        case 2:
            colour = myColours.yellow;  // yellow ?
            label ='?';
            break;
        case 3:
            colour = myColours.red;  // red X
            label ='X';
            break;
        default:
            colour = myColours.green;  // green +
            label ='+';
    }
    
    _pscreen->setPenSolid(false);
    _pscreen->circle(_x0+30, _y0+30, 18, colour);
    _pscreen->circle(_x0+30, _y0+30, 16, colour);
    _pscreen->circle(_x0+30, _y0+30, 15, colour);
    
    _pscreen->setFontSize(1);
    _pscreen->setFontSolid(false);
    _pscreen->gText(_x0+30- (_pscreen->fontSizeX()>>1)   , _y0+30- (_pscreen->fontSizeY()>>1)   , String(label), colour);
    _pscreen->gText(_x0+30- (_pscreen->fontSizeX()>>1) +1, _y0+30- (_pscreen->fontSizeY()>>1)   , String(label), colour);
    _pscreen->gText(_x0+30- (_pscreen->fontSizeX()>>1)   , _y0+30- (_pscreen->fontSizeY()>>1) +1, String(label), colour);
    _pscreen->gText(_x0+30- (_pscreen->fontSizeX()>>1) +1, _y0+30- (_pscreen->fontSizeY()>>1) +1, String(label), colour);
    
    button b1;
    button b2;
    button b3;
    
    b1.dDefine(_pscreen, _x0+130, _y0+150, 60, 40, setItem( 1, button1), textColour1, highColour1, lowColour1, 9);
    b2.dDefine(_pscreen, _x0+ 70, _y0+150, 60, 40, setItem( 2, button2), textColour2, highColour2, lowColour2, 9);
    b3.dDefine(_pscreen, _x0+ 10, _y0+150, 60, 40, setItem( 3, button3), textColour3, highColour3, lowColour3, 9);
    
    b1.enable(true);
    b1.draw();
    if (button2.length()>0) {
        b2.enable(true);
        b2.draw();
    }
    if (button3.length()>0) {
        b3.enable(true);
        b3.draw();
    }
    
    // dialog main loop
    uint8_t answer = 0;
    while ( answer==0 ) {
        if      (b1.check()) answer = b1.getIndex();
        else if (b2.check()) answer = b2.getIndex();
        else if (b3.check()) answer = b3.getIndex();
    }
    
    if (_pscreen->isReadable() && _pscreen->isStorage()) {
        localAddress = 0;

#if (CHRONO_DEBUG==1)
        Serial.print("dialog pasteArea (ms)\t");
        chrono_ms = millis();
#endif
        
        _pscreen->pasteArea(_x0, _y0, 200, 200, localAddress);
        
#if (CHRONO_DEBUG==1)
        Serial.println(millis() - chrono_ms, DEC);
#endif
    }
    
    return answer;
}


// ---- textBox
textBox::textBox()
{
    ;
}

void textBox::dDefine(LCD_screen_virtual * lcd,
                      uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy,
                      uint16_t textColour, uint16_t backColour,
                      uint8_t size)
{
    area::dDefine(lcd, x0, y0, dx, dy, 0);
    _textColour = textColour;
    _backColour = backColour;
    
    _size = size;
    _pscreen->setFontSize(size);
    _tx = _pscreen->fontSizeX() + 0;
    _ty = _pscreen->fontSizeY() + 1;
    _maxColumns = dx / _tx - 0;
    _maxLines = dy / _ty   - 1;
    
    _line = 0;
    _column = 0;
    setOption(false);
}

void textBox::clear()
{
    _pscreen->setPenSolid(true);
    _pscreen->dRectangle(_x0, _y0, _dx, _dy, _backColour);
    _line = 0;
    _column = 0;
}


void textBox::scroll()
{
    //    Serial.println("scroll");
    _line++;
    if (_line > _maxLines) {
        //        Serial.print("scroll - copyPaste");
        //        Serial.print("\t source:");
        //        Serial.print(_x0, DEC);
        //        Serial.print("\t");
        //        Serial.print(_y0+_ty, DEC);
        //        Serial.print("\t target");
        //        Serial.print(_x0, DEC);
        //        Serial.print("\t");
        //        Serial.print(_y0, DEC);
        //        Serial.print("\t size:");
        //        Serial.print(_dx-1, DEC);
        //        Serial.print("\t");
        //        Serial.println(_dy-_ty-1);
        
#if (CHRONO_DEBUG==1)
        Serial.print("textBox scroll copyPasteArea (ms)\t");
        chrono_ms = millis();
#endif
        _pscreen->copyPasteArea(_x0, _y0+_ty, _x0, _y0, _dx-1, _dy-_ty-1);
        
        _pscreen->setPenSolid(true);
        _pscreen->rectangle(_x0, _y0+(_maxLines*_ty), _x0+_dx-1, _y0+_dy-1, _backColour);
        _line = _maxLines;
        
#if (CHRONO_DEBUG==1)
        Serial.println(millis() - chrono_ms, DEC);
#endif
        
    }
    _column = 0;
}

void textBox::print(String text)
{
    int8_t i;
    
    //    Serial.println("print");
    
    _pscreen->setFontSize(_size);
    _pscreen->setFontSolid(true);
    //_pscreen->setBackGroundColour(_backColour);
    
    while (text.length()) {
        i = text.indexOf(' ');
        if (i<0) i=text.length();
        i++;
        
        //        Serial.print("text.indexOf = ");
        //        Serial.print(text.indexOf(' '), DEC);
        //        Serial.print("    text.length = ");
        //        Serial.print(text.length(), DEC);
        //        Serial.print("    i = ");
        //        Serial.println(i, DEC);
        
        
        if (_column+i > _maxColumns) scroll();
        
        //        Serial.print("_column = ");
        //        Serial.print(_column, DEC);
        //        Serial.print("    _line = ");
        //        Serial.println(_line, DEC);
        //        Serial.print("x = ");
        //        Serial.print(_x0+_column*_tx, DEC);
        //        Serial.print("    y = ");
        //        Serial.print(_y0+_line*_ty, DEC);
        //        Serial.print("    >");
        //        Serial.print(text.substring(0, i+1));
        //        Serial.println("<");
        
        //        _pscreen->gText(_x0+_column*_tx, _y0+_line*_ty, text.substring(0, i+1), _textColour);
        //        _column += i;
        
        for (uint8_t k=0; k<i; k++) {
            char c = text[k];
            if (c == 0x0d) {
                clear();
            } else if (c == 0x0a) {
                scroll();
            } else {
                //                Serial.print("print - gText");
                //                Serial.print("\t source:");
                //                Serial.print(_x0+_column*_tx, DEC);
                //                Serial.print("\t");
                //                Serial.print(_y0+_line*_ty, DEC);
                //                Serial.println();
                
                _pscreen->gText(_x0+_column*_tx, _y0+_line*_ty, String(c), _textColour, _backColour);
                _column++;
            }
        }
        text = text.substring(i);
        
        //        Serial.print(">");
        //        Serial.print(text);
        //        Serial.println("<");
        //        Serial.println();
    }
}

void dFill(LCD_screen_virtual * lcd, uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy)
{
    for (uint16_t i=x0; i<x0+dx; i++) {
        for (uint16_t j=y0; j<y0+dy; j++) {
            if ((i+j)%2 == 0) lcd->point(i, j, myColours.black);
        }
    }
    
}

// ---- menu
uint16_t menu(LCD_screen_virtual * lcd,
              item menuItem0[], uint8_t nItems0,
              uint16_t textColour0, uint16_t highColourMain0, uint16_t highColourSub0)
{
    uint8_t i, k, nMain, nSub, nItems;
    uint16_t current, mask;
    button main[8];
    button sub[8];
    bool mainFlag, subFlag;
    bool oldState0, oldState7;
    uint32_t sizeRAW;
    LCD_screen_virtual * _pscreen = lcd;
    uint32_t localAddress = 0;
//    uint32_t interimAddress;
    
    uint16_t maxY = _pscreen->screenSizeY();
    if (maxY > 240) maxY = 240;
    
    // Check SD
    nItems = nItems0;
    //    if ( !_pscreen->checkSD() ) _pscreen->initSD();
    
    // landscape and portrait coordinates
    
    // Save initial screen
    //    if ( _pscreen->checkSD() ) {
    //        if ( _pscreen->checkRAW() ) {
    //            sizeRAW = _pscreen->getSectors(80, 30);
    //            for (i=0; i<8; i++) {
    //                _pscreen->dSaveScreenRAW((uint32_t) sizeRAW*(i+0),  0, i*30, 80, 30 ); // main
    //                _pscreen->dSaveScreenRAW((uint32_t) sizeRAW*(i+8), 80, i*30, 80, 30 ); // sub
    //            }
    //            _pscreen->dSaveScreenRAW((uint32_t) sizeRAW*(i+9), 0, _pscreen->screenSizeY()-60, 80, 60 ); // OK and Cancel
    //        }
    //        else {
    //            for (i=0; i<8; i++) {
    //                _pscreen->dSaveScreenFAT( "main" + String(i, HEX) + ".tmp",  0, i*30, 80, 30 );
    //                _pscreen->dSaveScreenFAT( "sub"  + String(i, HEX) + ".tmp", 80, i*30, 80, 30 );
    //            }
    //            _pscreen->dSaveScreenFAT( "low.tmp" , 0, _pscreen->screenSizeY()-60, 80, 60 ); // OK and Cancel
    //        }
    //    }
    if (_pscreen->isReadable() && _pscreen->isStorage()) {
        localAddress = 0;
        // sizeRAW = _pscreen->getSectors(80, 30);
        sizeRAW = 80*30*2;
        
#if (CHRONO_DEBUG==1)
        Serial.print("menu copyArea (ms)\t");
        chrono_ms = millis();
#endif
        
        for (i=0; i<8; i++) {
            localAddress = sizeRAW*(i+0);
            _pscreen->copyArea(  0, i*30, 80, 30, localAddress); // main
            
            //            _pscreen->setPenSolid(false);
            //            _pscreen->dRectangle(  0, i*30, 80, 30, yellowColour);
            
            localAddress = sizeRAW*(i+8);
            _pscreen->copyArea( 80, i*30, 80, 30, localAddress); // sub
        }
        localAddress = sizeRAW*(i+9);
        _pscreen->copyArea( 0, maxY-60, 80, 60, localAddress); // OK and Cancel

        if (_pscreen->screenSizeY()-60 > 7*30) {
            _pscreen->copyArea( 0, 7*30, 80*2, maxY-7*30, localAddress); // interim space
        }
        
#if (CHRONO_DEBUG==1)
        Serial.println(millis() - chrono_ms, DEC);
#endif
    }
    
    //    Serial.print("\r\n ! end of saveScreen... \n");
    
    main[0].dDefine(      _pscreen, 0, 0*30, 80, 30, menuItem0[0], myColours.white, highColourMain0);
    main[6].dStringDefine(_pscreen, 0, maxY-2*30, 80, 30, "Cancel", myColours.white, myColours.yellow);
    main[7].dStringDefine(_pscreen, 0, maxY-30, 80, 30, "OK", myColours.white, myColours.green);
    
    current = 0x0000;
    nMain=0;
    
    mainFlag = false;
    
    // persistent buttons
    main[0].enable(false);
    oldState0 = false;
    main[6].enable(true);
    main[7].enable(false);
    oldState7 = false;
    
    main[0].draw();
    main[6].draw();
    main[7].draw();
    
    // main loop
    while ( !mainFlag ) {
        for (i=0; i<8; i++) sub[i].enable(false);
        for (i=0; i<6; i++) main[i].enable(false);
        
        // main menu column
        // menu title
        nMain = 0;
        main[nMain].dDefine(_pscreen,  0, nMain*30,  80, 30, menuItem0[nMain], myColours.white, highColourMain0);
        nMain++;
        
        // other menus
        for (k=0; k<4; k++) {
            mask = (current >> (4*(3-k)));
            if ( (mask & 0xf) >0 ) {
                for (i=1; i<nItems; i++) {
                    if ( menuItem0[i].index == (mask << (4*(3-k))) ) {
                        main[nMain].dDefine(_pscreen,  0, nMain*30,  80, 30, menuItem0[i], myColours.white, highColourMain0);
                        main[nMain].enable(true);
                        main[nMain].draw();
                        nMain++;
                    }
                }
            }
        }
        
        //        if ( _pscreen->checkRAW() )
        //            for (i=1; i<6; i++) {
        //                if (i>=nMain) _pscreen->readScreenRAW( sizeRAW*(i+0),  0, i*30 ); // main
        //            }
        //        else for (i=1; i<6; i++) {
        //            if (i>=nMain) _pscreen->readScreenFAT( "main"  + String(i, HEX) + ".tmp", 0, i*30 );
        //        }
        if (_pscreen->isReadable() && _pscreen->isStorage()) {
            for (i=1; i<6; i++) {
                if (i>=nMain) {
                    localAddress = sizeRAW*(i+0);
                    _pscreen->pasteArea(0, i*30, 80, 30, localAddress, true);
                    //                    dFill(_pscreen, 0, i*30, 80, 30);
                }
            }
        }
        
        // sub-menu column
        mask = 0x0000;
        for (uint8_t k=0; k<4; k++) {
            if ( (current & (0xf << (4*k))) > 0 ) mask |= ((uint16_t)0xf << (4*k));
        }
        mask ^= 0xffff;
        
        nSub = 0;
        for (i=0; i<nItems; i++) {
            if (nSub<8) {
                
                if ( ((menuItem0[i].index & (mask >> 4)) == 0) && (menuItem0[i].index > current) && (menuItem0[i].index < current+mask) ) {
                    sub[nSub].dDefine(_pscreen, 80, nSub*30, 80, 30, menuItem0[i], myColours.white, highColourSub0);
                    sub[nSub].enable(true);
                    sub[nSub].draw();
                    nSub++;
                }
            }
        }
        
        //        for (i=0; i<8; i++) {
        //            if (i>=nSub) {
        //                if (_pscreen->checkRAW()) _pscreen->readScreenRAW( sizeRAW*(i+8), 80, i*30 ); // sub
        //                else _pscreen->readScreenFAT( "sub"  + String(i, HEX) + ".tmp", 80, i*30 );
        //            }
        //        }
        if (_pscreen->isReadable() && _pscreen->isStorage()) {
            for (i=0; i<8; i++) {
                if (i>=nSub) {
                    localAddress = sizeRAW*(i+8);
                    _pscreen->pasteArea(80, i*30, 80, 30, localAddress, true);
                    //                    dFill(_pscreen, 80, i*30, 80, 30);
                }
            }
        }
        
        // final preparation
        // hide / show menu Cancel OK buttons
        if (nMain>1) {
            main[nMain-1].enable(false);
            main[nMain-1].draw(true);
        }
        // menu title
        main[0].enable(nMain>1);
        if ( oldState0 != main[0].state() ) {
            main[0].draw();
            oldState0 = main[0].state();
        }
        
        // OK button
        main[7].enable(nSub==0);
        if ( oldState7 != main[7].state() ) {
            main[7].draw();
            oldState7 = main[7].state();
        }
        
        // touch loop
        subFlag=false;
        
        do {
            for (i=0; i<nSub; i++) {
                if (sub[i].check()) {
                    current=sub[i].getIndex();
                    subFlag = true;
                };
            };
            
            for (i=0; i<nMain; i++) {
                if (main[i].check()) {
                    current = main[i].getIndex();
                    subFlag = true;
                }
            }
            
            if (main[6].check()) {          // Cancel
                mainFlag = true;
                current = 0x0000;
            }
            else if (main[7].check()) {     // OK
                mainFlag = true;
            }
        }
        while ( !(subFlag || mainFlag) );
    }
    
    // Restore initial screen
    //    if ( _pscreen->checkSD() ) {
    //        if ( _pscreen->checkRAW() ) {
    //            for (i=0; i<8; i++) {
    //                _pscreen->readScreenRAW( sizeRAW*(i+0),  0, i*30 ); // main
    //                _pscreen->readScreenRAW( sizeRAW*(i+8), 80, i*30 ); // sub
    //            }
    //            _pscreen->readScreenRAW( sizeRAW*(i+9), 0, _pscreen->screenSizeY()-60 ); // OK and Cancel
    //
    //        } else {
    //            for (i=0; i<8; i++) {
    //                _pscreen->readScreenFAT( "main" + String(i, HEX) + ".tmp",  0, i*30 );
    //                _pscreen->readScreenFAT( "sub"  + String(i, HEX) + ".tmp", 80, i*30 );
    //            }
    //            _pscreen->readScreenFAT( "low.tmp" , 0, _pscreen->screenSizeY()-60 ); // OK and Cancel
    //        }
    //    }
    if (_pscreen->isReadable() && _pscreen->isStorage()) {
        localAddress = 0;
        //        sizeRAW = _pscreen->getSectors(80, 30);
        sizeRAW = 80*30*2;
        
#if (CHRONO_DEBUG==1)
        Serial.print("menu pasteArea (ms)\t");
        chrono_ms = millis();
#endif
        for (i=0; i<8; i++) {
            localAddress = sizeRAW*(i+0);
            _pscreen->pasteArea(  0, i*30, 80, 30, localAddress); // main
            localAddress = sizeRAW*(i+8);
            _pscreen->pasteArea( 80, i*30, 80, 30, localAddress); // sub
        }
        localAddress = sizeRAW*(i+9);
        _pscreen->pasteArea( 0, maxY-60, 80, 60, localAddress); // OK and Cancel
        
#if (CHRONO_DEBUG==1)
        Serial.println(millis() - chrono_ms, DEC);
#endif
    }
    
    return current;
}


// ---- label
void dLabel(LCD_screen_virtual  * lcd,
            uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy,
            String text0, uint16_t textColour0, uint16_t backColour0,
            uint8_t horizontal0, uint8_t vertical0, uint8_t size0)
{
    uint8_t _size;
    uint16_t _xt, _yt;
    String _text;
    LCD_screen_virtual * _pscreen = lcd;
    
    if (size0 == 9) {
        uint16_t i=4;
        do {
            i--;
            _pscreen->setFontSize(i);
        }
        //    while ( ( (text0.length() * _pscreen->fontSizeX() > (x2-x1)) || (_pscreen->fontSizeY() > (y2-y1)) ) && (i>0) );
        while ( ( (text0.length() * _pscreen->fontSizeX() > (dx-6)) || (_pscreen->fontSizeY() > (dy-6)) ) && (i>0) );
        _size = i;
    }
    else {
        _size = size0;
    }
    _pscreen->setFontSize(_size);
    _text = text0.substring(0, min(text0.length(), (dx-6) / _pscreen->fontSizeX()));

//#if (defined(CHIPKIT) || (defined(ARDUINO) && (ARDUINO < 100)) ) //  Arduino 23 and chipKIT specific
//    _text = _text.trim();
//#else // Wiring, Maple, Energia specific
//    _text.trim();
//#endif

    /// @bug    trim() doesn't work on Stellaris
    /// @see    http://github.com/energia/Energia/issues/257
    ///
    uint8_t i = 0;
    uint8_t j = _text.length();
    
    while ((_text.charAt(i)==' ') && (i<j)) {
        i++;
    }
    while ((_text.charAt(j)==' ') && (j>0)) {
        j--;
    }
    if (j>i) _text = _text.substring(i, j);
    else _text = "";
    
//#if (defined(CHIPKIT) || (defined(ARDUINO) && (ARDUINO < 100)) ) //  Arduino 23 and chipKIT specific
//    _text = _text.trim();
//#else // Wiring, Maple, Energia specific
//    _text.trim();
//#endif
    
    //  // horizontal
    //  // default = 0 = center, 1 = left, 2 = right
    //  if ( horizontal0==1 ) _xt = x1;
    //  else if ( horizontal0==2 ) _xt = x2 - _text.length() * _pscreen->fontSizeX();
    //  else _xt = x1 + ( (x2-x1) - (_text.length() * _pscreen->fontSizeX()) ) /2;
    //
    //  // vertical
    //  // default = 0 = center, 1 = up, 2 = down
    //  if (vertical0==1)   _yt = y1;
    //  else if (vertical0==2)   _yt = y2 - _pscreen->fontSizeY();
    //  else   _yt = y1 + ( (y2-y1) - _pscreen->fontSizeY() ) /2;
    //
    //  _pscreen->setPenSolid(true);
    //  _pscreen->rectangle(x1, y1, x2, y2, backColour0);
    //  delay(10);    // delay required by chipKIT
    
    // horizontal
    // default = 0 = center, 1 = left, 2 = right
    if ( horizontal0==1 ) _xt = x0;
    else if ( horizontal0==2 ) _xt = x0 + dx - _text.length() * _pscreen->fontSizeX();
    else _xt = x0 + ( dx - (_text.length() * _pscreen->fontSizeX()) ) /2;
    
    // vertical
    // default = 0 = center, 1 = up, 2 = down
    if (vertical0==1)   _yt = y0;
    else if (vertical0==2)   _yt = y0 + dy - _pscreen->fontSizeY();
    else _yt = y0 + ( dy - _pscreen->fontSizeY() ) /2;
    
    _pscreen->setPenSolid(true);
    _pscreen->dRectangle(x0, y0, dx, dy, backColour0);
    delay(10);    // delay required by chipKIT
    
    _pscreen->setFontSize(_size);
    _pscreen->setFontSolid(false);
    _pscreen->gText(_xt, _yt, _text, textColour0);
}


// ---- slider
bool slider(LCD_screen_virtual * lcd,
            uint16_t &value, uint16_t min, uint16_t max, uint16_t step,
            uint16_t minColour0, uint16_t maxColour0,
            String okText0, uint16_t okTextColour0, uint16_t okColour0,
            String cancelText0, uint16_t cancelTextColour0, uint16_t cancelColour0)
{
    LCD_screen_virtual * _pscreen = lcd;
    
    uint16_t i, j;
    
    uint16_t maxY = _pscreen->screenSizeY();
    if (maxY > 240) maxY = 240;

    uint16_t k = maxY-80;
    
    // Check SD
    //    if ( !_pscreen->checkSD() ) _pscreen->initSD();
    
    // landscape and portrait coordinates
    uint16_t _x0 = _pscreen->screenSizeX() - 60 -60;
    uint16_t _y0 = 0;
    uint32_t localAddress = 0;
    

    // save initial screen
    //    if ( _pscreen->checkSD() ) {
    //        if ( _pscreen->checkRAW() ) a = _pscreen->dSaveScreenRAW((uint32_t) 0, _x0,  _y0, 60, _pscreen->screenSizeY() );
    //        else a = _pscreen->dSaveScreenFAT( "slider.tmp", _x0, _y0, 60, _pscreen->screenSizeY() );
    //    }
    if (_pscreen->isReadable() && _pscreen->isStorage()) {
        localAddress = 0;
        
#if (CHRONO_DEBUG==1)
        Serial.print("slider copyArea (ms)\t");
        chrono_ms = millis();
#endif
        _pscreen->copyArea(_x0, _y0, 60, maxY, localAddress);
        
#if (CHRONO_DEBUG==1)
        Serial.println(millis() - chrono_ms, DEC);
#endif
        //        _pscreen->setPenSolid(false);
        //        _pscreen->dRectangle(_x0,  _y0, 60, _pscreen->screenSizeY(), yellowColour);
        
    }
    
    //    Serial.print("\r\n dSaveScreenRAW/FAT ");
    //    Serial.print(a, HEX);
    
    // buttons
    button sb1;
    sb1.dDefine(_pscreen, _x0, _y0, 60, 40-2, setItem( 1, okText0), okTextColour0, okColour0);
    sb1.enable(true);
    sb1.draw();
    
    button sb2;
    sb2.dDefine(_pscreen, _x0, maxY-40+2, 60, 40-2, setItem( 2, cancelText0), cancelTextColour0, cancelColour0);
    sb2.enable(true);
    sb2.draw();
    
    // variables for value management
    uint16_t x;
    uint16_t y = map(value, min, max, _y0+40, _y0+40+k);
    uint16_t z;
    uint16_t initialY = y;
    uint16_t oldY = y;
    
    // colour min, max and value
    uint8_t minRed, maxRed, valueRed;
    uint8_t minGreen, maxGreen, valueGreen;
    uint8_t minBlue, maxBlue, valueBlue;
    
    myColours.split(minColour0, minRed, minGreen, minBlue);
    myColours.split(maxColour0, maxRed, maxGreen, maxBlue);
    
    for (i=0; i<k; i++) {
        valueRed   = map(i, 0, k, minRed,   maxRed);
        valueGreen = map(i, 0, k, minGreen, maxGreen);
        valueBlue  = map(i, 0, k, minBlue,  maxBlue);
        
        j = myColours.calculate(valueRed, valueGreen, valueBlue);
        if ( (_y0+40+i>initialY-4) && (_y0+40+i<initialY+4) ) j = myColours.reverse(j);
        
        _pscreen->dLine(_x0+4, _y0+40+i, 60-8, 1, j);
    }
    _pscreen->setPenSolid();
    _pscreen->dRectangle(_x0,      38, 4, maxY-76, myColours.black);
    _pscreen->dRectangle(_x0+60-4, 38, 4, maxY-76, myColours.black);
    
    // slider main loop
    uint8_t answer = 0;
    
    while (answer == 0) {
        //        Serial.print(a, DEC);
        
        if (_pscreen->getTouch(x, y, z)) {
            if      (sb1.check()) answer = sb1.getIndex();
            else if (sb2.check()) answer = sb2.getIndex();
            else {
                
                if ( (x>_x0) && (x<_pscreen->screenSizeX()) && (y>_y0+40+4) && (y<_y0+40+k-4) ) {
                    for (i=min(oldY, y)-4+1; i<max(oldY, y)+4; i++) {
                        if ( ((i>oldY-4) && (i<oldY+4)) ^ ((i>y-4) && (i<y+4)) )
                            _pscreen->dLine(_x0+4, i, 60-8, 1, myColours.reverse(_pscreen->readPixel(_x0+4, i)));
                    }
                    oldY = y;
                }
            }
        }
        delay(100);
    }
    
    
    // restore initial screen
    //    if ( _pscreen->checkSD() ) {
    //        if ( _pscreen->checkRAW() ) a = _pscreen->readScreenRAW((uint32_t)0, _x0, _y0 );
    //        else a = _pscreen->readScreenFAT("slider.tmp", _x0, _y0 );
    //    }
    if (_pscreen->isReadable() && _pscreen->isStorage()) {
        localAddress = 0;
        
#if (CHRONO_DEBUG==1)
        Serial.print("slider pasteArea (ms)\t");
        chrono_ms = millis();
#endif
        _pscreen->pasteArea(_x0, _y0, 60, maxY, localAddress);
        
#if (CHRONO_DEBUG==1)
        Serial.println(millis() - chrono_ms, DEC);
#endif
    }
    
    //    Serial.print("\r\n readScreenRAW/FAT ");
    //    Serial.print(a, HEX);
    
    if (answer == sb1.getIndex()) {
        if ( oldY!=initialY ) value = map(oldY, _y0+40, _y0+40+k, min, max);
        return true;
    } else {
        return false;
    }
}


