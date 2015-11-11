//
// Screen_PicasoSGC.cpp
// Library C++ code
// ----------------------------------
// Developed with embedXcode
// http://embedXcode.weebly.com
//
// Project LCD_screen Library Suite
//
// Created by Rei VILO, mars 07, 2013 09:21
// embedXcode.weebly.com
//
//
// Copyright © Rei VILO, 2013-2014
// License CC = BY NC SA
//
// See Screen_PicasoSGC.h and ReadMe.txt for references
//

// Library header
#include "Screen_PicasoSGC.h"

// Class
Screen_PicasoSGC::Screen_PicasoSGC(int8_t resetPin, Stream * proxyPort)
{
	_resetPin = resetPin;
	_port = proxyPort;
}

void Screen_PicasoSGC::begin()
{
	pinMode(_resetPin, OUTPUT);
    
	// Screen reset
	if (_resetPin>-1) {
		delay(3000);
		pinMode(_resetPin, OUTPUT);
		digitalWrite(_resetPin, LOW);
		delay(10);
		digitalWrite(_resetPin, HIGH);
		delay(10);
		_port->flush();
	}
    
	// Screen initialisation
	delay(3000);
	_port->print('U');    // connect
	while (_port->read()!=0x06) delay(100);
    
	_writeCommand8('o', 0x04);                      // clear touch + touch state
	//  _port->flush();
	delay(100);
	while (_port->available()) _port->read();
    
	// Standard
	setBacklight(true);
	setOrientation(0);
    
	_writeCommand16('Y', 0x0403);                   // set orientation
	_a = nacAck();
    
	_port->print('d');    // get display resolution
	while (_port->available()<4);
	_screenHeigth = _read16();
	_screenWidth  = _read16();
    
    _screenDiagonal = 0;
    
	//    Serial.print("_screenWidth  ");
	//    Serial.println(_screenWidth, DEC);
	//    Serial.print("_screenHeigth ");
	//    Serial.println(_screenHeigth, DEC);
    
	_penSolid = false;
	_fontSolid = false;
	_touchTrim = 1;                                 // touch
    _flagRead = true;                               // screen readable
    
	clear();
    
	_writeCommand16('Y', 0x0500);                   // enable touch
	_a = nacAck();
	_writeCommand16('Y', 0x0502);                   // full screen active
	_a = nacAck();
}

void Screen_PicasoSGC::setSpeed(uint32_t speed) {
	_a=0x06;
	if      (speed==(uint32_t)  9600) _a=0x06;
	else if (speed==(uint32_t) 19200) _a=0x08;
	else if (speed==(uint32_t) 38400) _a=0x0a;      // max for Arduino and software serial
	else if (speed==(uint32_t) 57600) _a=0x0c;
	else if (speed==(uint32_t)115200) _a=0x0d;      // ok with chipKIT and Arduino mega hardware serial
	else if (speed==(uint32_t)256000) _a=0x11;
	else _a= 0x15;
    
	delay(10);
	if (_a != 0x15) _writeCommand8('Q', _a);
    
	//    Serial.print("setSpeed ");
	//    Serial.println(a, HEX);
    
	//    _a = nacAck();
	while (!_port->available());
	_a = _port->read();
}

void Screen_PicasoSGC::clear(uint16_t colour)
{
	if (colour == myColours.black) {
		_port->print('E');
		_a = nacAck();
	} else {
		_fastFill(0, 0, screenSizeX(), screenSizeY(), colour);
	}
}

String Screen_PicasoSGC::WhoAmI()
{
	String s="Screen_PicasoSGC";
	_writeCommand8('V', 0x00);    // clear touch + touch state
	delay(10);
    
	uint8_t i=0;
	uint8_t c=0;
    
	while(_port->available()!=0) {
		c=_port->read();
        //		s += String(c, HEX);
        s += " ";
        
        switch (i) {
            case 0:
                if (c==0) s += "uOLED";
                else if (c==1) s += "uLCD";
                else if (c==2) s+= "uVGA";
                else s+= "u?";
                s += " " + String(_screenWidth, DEC) + "x" + String(_screenHeigth, DEC);
                break;
                
            case 1:
                s += "hw" + String(c, HEX);
                break;
                
            case 2:
                s += "fw" + String(c, HEX);
                break;
                
            default:
                ;
        }
        
        //		        if ( i==0 )  _checkedScreenType = c; // uLED=0, uLCD=1, uVGA=2
        //		        if ( i==1 )  _checkedHardwareVersion = c;
        //		        if ( i==2 )  _checkedSoftwareVersion = c;
        //		        if ( i==3 )  _maxX = _size(c, 3); // standard
        //		        if ( i==4 )  _maxY = _size(c, 4); // standard
		i++;
	}
    
	//    ///
	//    /// @note 	Assumption: controller type based on screen size
	//    ///
	//    if ( (_maxX>0xff) || (_maxY>0xff) ) {
	//        _checkedControllerType = __Picaso__; // 16 bits
	//    } else {
	//        _checkedControllerType = __Goldelox__; // 8 bits
	//    }
	//    //  _port->setXY16( _checkedControllerType>__Goldelox__ );
	//    _XY16 = ( _checkedControllerType>__Goldelox__ );
	return s;
}

#ifdef BUILT_IN_FONTS
void Screen_PicasoSGC::gText(uint16_t x0, uint16_t y0,
                             String s,
                             uint16_t textColour, uint16_t backColour,
                             uint8_t ix, uint8_t iy)
{
	_writeCommand16('K', backColour);
	_a = nacAck();
    
	_writeCommand8('O', _fontSolid); // pen solid
	_a = nacAck();
    
	_port->print('S');
	_printXY(x0, y0);
	_port->print(_font);
	_print16(textColour);
	_port->print((char)ix);   // multiplier
	_port->print((char)iy);
	_port->print(s);
	_port->print((char)0x00);
	_a = nacAck();
}

void Screen_PicasoSGC::setFontSize(uint8_t size)
{
	// 00hex : 6x8 (5x7 false) small size font set
	// 01hex : 8x8 medium size font set
	// 02hex : 8x12 large size font set
	// 03hex : 12x16 largest size font set - not on uOLED
    
	if ( size>3 ) size=3;
    
	_writeCommand8('F', size);
	_font = size;
	switch (size) {
        case 0:
            _fontX=5;
            _fontY=8;
            break;
        case 1:
            _fontX=8;
            _fontY=8;
            break;
        case 2:
            _fontX=8;
            _fontY=12;
            break;
        case 3:
            _fontX=12;
            _fontY=16;
	}
	_a = nacAck();
}

uint8_t Screen_PicasoSGC::fontSizeX()
{
	return _fontX;
}

uint8_t Screen_PicasoSGC::fontSizeY()
{
	return _fontY;
}
#endif

void Screen_PicasoSGC::rectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t colour)
{
	_writeCommand8('p', (_penSolid) ? 0x00 : 0x01); // pen solid
	_a = nacAck();
    
	_port->print('r');
	_printXY(x1, y1);
	_printXY(x2, y2);
	_print16((uint16_t)colour);
    
	_a = nacAck();
}

void Screen_PicasoSGC::line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t colour)
{
	_port->print('L');
	_printXY(x1, y1);
	_printXY(x2, y2);
	_print16((uint16_t)colour);
    
	_a = nacAck();
}

void Screen_PicasoSGC::triangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, uint16_t colour)
{
    if ((x1 == x2) && (y1 == y2)) {
        line(x3, y3, x1, y1, colour);
        
    } else if ((x1 == x3) && (y1 == y3)) {
        line(x2, y2, x3, y3, colour);
        
    } else if ((x2 == x3) && (y2 == y3)) {
        line(x1, y1, x2, y2, colour);
        
    } else {
        bool b=true;
        
        // Graham Scan + Andrew's Monotone Chain Algorithm
        // 1. Sort by ascending x
        while (b) {  // required x2 < x1 : x3 > x2 : y2 > y1 : y3 > y1
            b = false;
            if (!b && (x1>x2)) {
                _swap(x1, x2);
                _swap(y1, y2);
                b = true;
            }
            if (!b && (x2>x3)) {
                _swap(x3, x2);
                _swap(y3, y2);
                b = true;
            }
        }
        
        // Graham Scan + Andrew's Monotone Chain Algorithm
        // 2. Sort by ascending y
        while (b) {  // required x2 < x1 : x3 > x2 : y2 > y1 : y3 > y1
            b = false;
            if (!b && (y1>y2)) {
                _swap(x1, x2);
                _swap(y1, y2);
                b = true;
            }
            if (!b && (y3>y2)) {
                _swap(x3, x2);
                _swap(y3, y2);
                b = true;
            }
        }
        
        // Graham Scan + Andrew's Monotone Chain Algorithm
        // 3. check counter-clockwise, clockwise, collinear
        long l = (x2 - x1)*(y3 - y1) - (y2 - y1)*(x3 - x1);
        
        if (l==0) {
            line(x1, y1, x3, y3, colour);
            return;
        }
        
        if (l>0) {
            _swap(x1, x2);
            _swap(y1, y2);
        }
        l = (x2 - x1)*(y3 - y1) - (y2 - y1)*(x3 - x1);
        
        _writeCommand8('p', (_penSolid) ? 0x00 : 0x01); // pen solid
        _a = nacAck();

        _port->print('G');
        _printXY(x1, y1);
        _printXY(x2, y2);
        _printXY(x3, y3);
        _port->print(colour);
        
        _a = nacAck();
    }
}


void Screen_PicasoSGC::invert(boolean flag)
{
	//    _writeCommand(flag ? Template_INVON : Template_INVOFF);
}

void Screen_PicasoSGC::setBacklight(boolean flag)
{
	//    if (_backlight!=NULL) digitalWrite(_backlight, flag);
}

void Screen_PicasoSGC::setScreen(boolean flag)
{
	//    if (_backlight!=NULL) setBacklight(flag);
}

void Screen_PicasoSGC::_setOrientation(uint8_t orientation)
{
    //	_orientation = orientation %4;
	switch (_orientation) {
        case 0:
            _writeCommand88('Y', 0x04, 0x02);
            break;
            
        case 1:
            _writeCommand88('Y', 0x04, 0x03);
            break;
            
        case 2:
            _writeCommand88('Y', 0x04, 0x01);
            break;
            
        case 3:
            _writeCommand88('Y', 0x04, 0x04);
            break;
            
        default:

            break;
	}
	_a = nacAck();
}

void Screen_PicasoSGC::circle(uint16_t x0, uint16_t y0, uint16_t radius, uint16_t colour)
{
	_writeCommand8('p', (_penSolid) ? 0x00 : 0x01); // pen solid
	_a = nacAck();
    
	_port->print('C');
	_printXY(x0, y0);
	_print16(radius);
	_print16(colour);
    
	_a = nacAck();
}

void Screen_PicasoSGC::_fastFill(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t colour)
{
	_writeCommand8('p', 0x00); // pen solid
	_a = nacAck();
    
	_port->print('r');
	_printXY(x1, y1);
	_printXY(x2, y2);
	_print16(colour);
    
	_a = nacAck();
    
    _writeCommand8('p', (_penSolid) ? 0x00 : 0x01); // pen solid
	_a = nacAck();
}

void Screen_PicasoSGC::_setPoint(uint16_t x1, uint16_t y1, uint16_t colour)
{
	_port->print('P');
	_printXY(x1, y1);
	_print16(colour);
    
	_a = nacAck();
}

// Utilities
//void Screen_PicasoSGC::_setWindow(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1)
//{
//    if (_tab == TemplateR_GREEN_TAB) {
//        switch (_orientation)
//        {
//            case 0:
//            case 2:
//                x0 += 2;
//                y0 += 1;
//                x1 += 2;
//                y1 += 1;
//                break;
//            case 1:
//            case 3:
//                x0 += 1;
//                y0 += 2;
//                x1 += 1;
//                y1 += 2;
//                break;
//            default:
//                break;
//        }
//    }
//
//    _writeRegister(0x2a, 0, x0, 0, x1, -1);
//    _writeRegister(0x2b, 0, y0, 0, y1, -1);
//
//    _writeCommand(0x2C);
//}

//void Screen_PicasoSGC::_writeRegister(uint8_t b, int8_t arg1, ...)
//{
//    va_list argList;
//    va_start(argList, arg1);
//    int16_t data;
//
//    _writeCommand((uint8_t)b);
//
//    va_start(argList, arg1);
//    for (data = arg1; data >= 0; data = va_arg(argList, int16_t)) {
//        _writeData((uint8_t)data);
//    }
//    va_end(argList);
//}

void Screen_PicasoSGC::_writeCommand8(uint8_t command, uint8_t data)
{
	_port->print((char)command);
	_port->print((char)data);
}

void Screen_PicasoSGC::_writeCommand88(uint8_t command, uint8_t data0, uint8_t data1)
{
	_port->print((char)command);
	_port->print((char)data0);
	_port->print((char)data1);
}

void Screen_PicasoSGC::_writeCommand16(uint8_t command, uint16_t data)
{
	_port->print((char)command);
	_port->print((char)highByte(data));
	_port->print((char)lowByte(data));
}

//void Screen_PicasoSGC::_writeData(uint8_t data)
//{
//    digitalWrite(_dataCommand, HIGH);
//    digitalWrite(_chipSelect, LOW);
//
//#if defined(HARDWARE_SPI)
//    SPI.transfer(data);
//#else
//    shiftOut(_pinSerialData, _pinSerialClock, MSBFIRST, data);
//#endif
//
//    digitalWrite(_chipSelect, HIGH);
//}

//void Screen_PicasoSGC::_writeData16(uint16_t data)
//{
//    _writeData(data >> 8);
//    _writeData(data & 0xff);
//}

uint16_t Screen_PicasoSGC::readPixel(uint16_t x1, uint16_t y1)
{
    _port->print('R');
    _printXY(x1, y1);
    
    uint16_t colour = _port->read();
    colour <<= 8;
    colour += _port->read();
    return colour;
}

void Screen_PicasoSGC::copyPasteArea(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t dx, uint16_t dy)
{
    if ( (x2+dx > screenSizeX()) || (y2+dy > screenSizeY()) ) return;
    
    _port->print('c');
    _printXY(x1, y1);
    _printXY(x2, y2);
    _printXY(dx, dy);
    
    nacAck();
}

void Screen_PicasoSGC::_getRawTouch(uint16_t &x0, uint16_t &y0, uint16_t &z0)
{
	_writeCommand8('o', 0x04);                  // read state
    
	uint8_t i=0;
	delay(10);    // delay required by chipKIT
	while ((_port->available()) && (i<4)) {
		_touch_buffer[i]=_port->read();
		i++;
	}
    
	// 	0x00= no touch activity
	// 	0x01= touch press
	// 	0x02= touch release
	// 	0x03= touch moving
	if ((_touch_buffer[1] == 1) || (_touch_buffer[1] == 3)) z0 = 0x10;
	else z0 = 0;
    
	if (z0 > 0) {
		_writeCommand8('o', 0x05);              // read coordinates
        
		i=0;
		delay(10);    // delay required by chipKIT
		while (_port->available() && (i<4)) {
			_touch_buffer[i] = _port->read();
			i++;
		}
        
		if (_touch_buffer[0]!=0x15) {
			x0 = (uint16_t)(_touch_buffer[0] << 8) + _touch_buffer[1];
			y0 = (uint16_t)(_touch_buffer[2] << 8) + _touch_buffer[3];
		}
	}
}

bool Screen_PicasoSGC::getTouch(uint16_t &x, uint16_t &y, uint16_t &z)
{
	_getRawTouch(x, y, z);
	return (z>0);
}

uint8_t Screen_PicasoSGC::nacAck() {
	uint8_t b=0x00; // 0x06;
	while (!(_port->available())) delay(2);
    
	b = _port->read();
	return b;
}

void Screen_PicasoSGC::_print16(uint16_t ui)
{
	_port->print((char)highByte(ui));
	_port->print((char)lowByte(ui));
};

uint16_t Screen_PicasoSGC::_read16()
{
	uint16_t ui;
	ui = _port->read();
	ui <<= 8;
	ui |= _port->read();
	return ui;
};

void Screen_PicasoSGC::_printXY(uint16_t x, uint16_t y)
{
	_print16(x);
	_print16(y);
};

void Screen_PicasoSGC::_setWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) { ; } // compulsory
void Screen_PicasoSGC::_writeData88(uint8_t dataHigh8, uint8_t dataLow8) { ; }  // compulsory

void Screen_PicasoSGC::_setBacklight(bool state)
{
    ;
}

void Screen_PicasoSGC::_setIntensity(uint8_t intensity)
{
    ;
}



