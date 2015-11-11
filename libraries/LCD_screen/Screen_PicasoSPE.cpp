//
// Screen_PicasoSPE.cpp
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
// See Screen_PicasoSPE.h and ReadMe.txt for references
//

// Library header
#include "Screen_PicasoSPE.h"

// Class
Screen_PicasoSPE::Screen_PicasoSPE(int8_t resetPin, Stream * proxyPort)
{
	_resetPin = resetPin;
	_port = proxyPort;
}

void Screen_PicasoSPE::begin()
{
	pinMode(_resetPin, OUTPUT);
    
	// Screen reset
	if (_resetPin != NULL) {
		delay(300);
		pinMode(_resetPin, OUTPUT);
		digitalWrite(_resetPin, LOW);
		delay(10);
		digitalWrite(_resetPin, HIGH);
		delay(10);
	}
    
	// Screen initialisation
	delay(3000);
    
	//  _port->flush();
	while (_port->available()) _port->read();
    
	// Standard
	setBacklight(true);
	setOrientation(0);

    delay(10);
	_writeCommand16(0xffa6, 0x0000);                                            // get display resolution
    _a = _nacAckData(_screenWidth);
    _screenWidth++;
    
	_writeCommand16(0xffa6, 0x0001);                                            // get display resolution
    _a = _nacAckData(_screenHeigth);
    _screenHeigth++;

    _screenDiagonal = 0;

	_penSolid  = false;
	_fontSolid = false;
	_touchTrim = 1;                                                             // touch
    _flagRead  = true;                                                          // screen readable
    _fsmArea = 0;
    
	clear();
    
	_writeCommand16(0xff38, 0x0000);                                            // enable touch
	_a = _nacAck();
	_writeCommand16(0xff38, 0x0002);                                            // full screen active
	_a = _nacAck();
}

void Screen_PicasoSPE::setSpeed(uint32_t speed) {
	_a=0x06;
	if      (speed==(uint32_t)  9600) _a = 0x06;
	else if (speed==(uint32_t) 19200) _a = 0x08;
	else if (speed==(uint32_t) 38400) _a = 0x0a;                                // max for Arduino and software serial
	else if (speed==(uint32_t) 57600) _a = 0x0c;
	else if (speed==(uint32_t)115200) _a = 0x0d;                                // ok with chipKIT and Arduino mega hardware serial
	else if (speed==(uint32_t)256000) _a = 0x0f;
	else                              _a = 0x15;
    
	delay(10);
	if (_a != 0x15) _writeCommand16(0x0026, _a);
    
	//    Serial.print("setSpeed ");
	//    Serial.println(a, HEX);
    
	_a = _nacAck();
    delay(100);
}

void Screen_PicasoSPE::clear(uint16_t colour)
{
	if (colour==myColours.black) {
		_writeCommand0(0xffcd);
        delay(30);
		_a = _nacAck();
        _a = _a + 1;
	} else {
        bool oldPenSolid = _penSolid;
        setPenSolid();
		rectangle(0, 0, screenSizeX(), screenSizeY(), colour);
        setPenSolid(oldPenSolid);
	}
}

String Screen_PicasoSPE::WhoAmI()
{
//    Serial.println(_port->available(), DEC);
    String s = "Screen_PicasoSPE ";
	_writeCommand0(0x001a);

	_a = _nacAckData(_b);

	while (_port->available() < _b);
	for (_a=0; _a<_b; _a++) s += (char)_port->read();

	return s;
}

#if (BUILT_IN_FONTS == 1)

void Screen_PicasoSPE::gText(uint16_t x0, uint16_t y0,
                          String s,
                          uint16_t textColour, uint16_t backColour,
                          uint8_t ix, uint8_t iy)
{
	_writeCommand16(0xffe7, textColour);
	_a = _nacAckData(_b);
    
	_writeCommand16(0xffe6, backColour);
	_a = _nacAckData(_b);
    
	_writeCommand16(0xffdf, _fontSolid);                                        // font solid
	_a = _nacAckData(_b);
    
    _writeCommand0(0xffcc);                                                     // set x0, y0
	_printXY(x0, y0);
	_a = _nacAck();
    
    _writeCommand16(0xffe4, ix);                                                // x multiplier
	_a = _nacAckData(_b);
    
    _writeCommand16(0xffe3, iy);                                                // y multiplier
	_a = _nacAckData(_b);
    
    _writeCommand0(0x0018);
	_port->print(s);
	_port->print((char)0x00);
	_a = _nacAckData(_b);
}

void Screen_PicasoSPE::setFontSize(uint8_t size)
{
	// 00hex : 6x8 (5x7 false) small size font set
	// 01hex : 8x8 medium size font set
	// 02hex : 8x12 large size font set
	// 03hex : 12x16 largest size font set - not on uOLED
    
	if (size > 2) size=2;
	//_font = size;

	_writeCommand16(0xffe5, (uint16_t)size);
	_a = _nacAckData(_b);

	switch (size) {
	case 0:
		_fontX=7;
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

	//	_writeCommand8(0x001e, 'A');
	//    _a = _nacAckData(_b);
	//    _fontX = (uint8_t) _b;
	//
	//	_writeCommand8(0x001d, 'A');
	//    _a = _nacAckData(_b);
	//    _fontY = (uint8_t) _b;
}

//void Screen_PicasoSPE::setFontSolid(bool flag)
//{
//    _fontSolid = flag;
//}

uint8_t Screen_PicasoSPE::fontSizeX()
{
	return _fontX;
}

uint8_t Screen_PicasoSPE::fontSizeY()
{
	return _fontY;
}

uint8_t Screen_PicasoSPE::fontMax()
{
    return 2;
}

#endif

void Screen_PicasoSPE::rectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t colour)
{
	if (_penSolid) _writeCommand0(0xffc4); // pen solid
    else _writeCommand0(0xffc5);
    
	_printXY(x1, y1);
	_printXY(x2, y2);
	_print16((uint16_t)colour);
    
	_a = _nacAck();
}

void Screen_PicasoSPE::line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t colour)
{
    _writeCommand0(0xffc8);
	_printXY(x1, y1);
	_printXY(x2, y2);
	_print16((uint16_t)colour);
    
	_a = _nacAck();
}

void Screen_PicasoSPE::invert(boolean flag)
{
	//    _writeCommand(flag ? Template_INVON : Template_INVOFF);
}

void Screen_PicasoSPE::setBacklight(boolean flag)
{
	//    if (_backlight!=NULL) digitalWrite(_backlight, flag);
}

void Screen_PicasoSPE::setScreen(boolean flag)
{
	//    if (_backlight!=NULL) setBacklight(flag);
}

void Screen_PicasoSPE::_setOrientation(uint8_t orientation)
{
//	_orientation = orientation %4;
	switch (_orientation) {
        case 0:
            _writeCommand16(0xff9e, 0x02);
            break;
            
        case 1:
            _writeCommand16(0xff9e, 0x00);
            break;
            
        case 2:
            _writeCommand16(0xff9e, 0x03);
            break;
            
        case 3:
            _writeCommand16(0xff9e, 0x01);
            break;
            
        default:

            break;
	}
	_a = _nacAckData(_b);
}

void Screen_PicasoSPE::triangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, uint16_t colour)
{
	if (_penSolid) _writeCommand0(0xffa9);      // pen solid
    else _writeCommand0(0xffbf);
    
	_printXY(x1, y1);
	_printXY(x2, y2);
	_printXY(x3, y3);
	_print16(colour);
    
	_a = _nacAck();
}


void Screen_PicasoSPE::circle(uint16_t x0, uint16_t y0, uint16_t radius, uint16_t colour)
{
	if (_penSolid) _writeCommand0(0xffc2);      // pen solid
    else _writeCommand0(0xffc3);
    
	_printXY(x0, y0);
	_print16(radius);
	_print16(colour);
    
	_a = _nacAck();
}

void Screen_PicasoSPE::_fastFill(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t colour)
{
    _penSolid = true;
	rectangle(x1, y1, x2, y2, colour);
}

void Screen_PicasoSPE::_setPoint(uint16_t x1, uint16_t y1, uint16_t colour)
{
    _writeCommand0(0xffc1);
    _printXY(x1, y1);
	_print16(colour);
    
	_a = _nacAck();
}

uint16_t Screen_PicasoSPE::readPixel(uint16_t x1, uint16_t y1)
{
    _writeCommand0(0xffc0);
    _printXY(x1, y1);
    
    while (_port->available() < 3);
    
    _a = _nacAck();
    _b = _read16();
    
    return _b;
}

void Screen_PicasoSPE::copyPasteArea(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t dx, uint16_t dy)
{
    _writeCommand0(0xffad);
    _printXY(x1, y1);
    _printXY(x2, y2);
    _printXY(dx, dy);

	_a = _nacAck();
}

void Screen_PicasoSPE::_getRawTouch(uint16_t &x0, uint16_t &y0, uint16_t &z0)
{
//    while (_port->available() > 0) _port->read();
    
	_writeCommand16(0xff37, 0x0000);           // read state
	_a = _nacAckData(_b);

	if ((_b == 1) || (_b == 3)) z0 = 0x10;     // press or move
	else z0 = 0;
    
	if (z0 > 0) {
        _writeCommand16(0xff37, 0x0001);       // read x coordinate
        _a = _nacAckData(x0);
        
        _writeCommand16(0xff37, 0x0002);       // read y coordinate
        _a = _nacAckData(y0);
    }
}

bool Screen_PicasoSPE::getTouch(uint16_t &x, uint16_t &y, uint16_t &z)
{
	_getRawTouch(x, y, z);
	return (z > 0);
}

uint8_t Screen_PicasoSPE::_nacAck() {
//    _chrono = millis() + 10;
	while (_port->available() < 1);
	_a = _port->read();
	return _a;
}

uint8_t Screen_PicasoSPE::_nacAckData(uint16_t &data) {
//    _chrono = millis() + 10;
	while (_port->available() < 3);
	_a = _port->read();
	data   = _read16();
	return _a;
}

void Screen_PicasoSPE::_writeCommand0(uint16_t command)
{
//    while (millis() < _chrono);
    _print16(command);
}

void Screen_PicasoSPE::_writeCommand8(uint16_t command, uint8_t data)
{
//    while (millis() < _chrono);
    _print16(command);
	_print8(data);
}

void Screen_PicasoSPE::_writeCommand88(uint16_t command, uint8_t data0, uint8_t data1)
{
//    while (millis() < _chrono);
    _print16(command);
	_print8(data0);
	_print8(data1);
}

void Screen_PicasoSPE::_writeCommand16(uint16_t command, uint16_t data)
{
//    while (millis() < _chrono);
    _print16(command);
	_print16(data);
}

void Screen_PicasoSPE::_writeCommandString0(uint16_t command, String s)
{
	_print16(command);
	_port->print(s);
	_port->print((char)0x00);
}

void Screen_PicasoSPE::_print16(uint16_t ui)
{
	_print8(highByte(ui));
	_print8(lowByte(ui));
};

void Screen_PicasoSPE::_print8(uint8_t ui)
{
	_port->print((char)ui);
};


uint16_t Screen_PicasoSPE::_read16()
{
	uint16_t ui;
	ui   = _port->read();
	ui <<= 8;
	ui  |= _port->read();
	return ui;
};

void Screen_PicasoSPE::_printXY(uint16_t x, uint16_t y)
{
	_print16(x);
	_print16(y);
};

bool Screen_PicasoSPE::fileSystemBegin()
{
	_writeCommand0(0xff03);
	_a = _nacAckData(_b);
	return (_b != 0);
}

bool Screen_PicasoSPE::fileSystemEnd()
{
	_writeCommand0(0xff02);
	_a = _nacAck();
	return (_a == 0x06);
}

uint8_t Screen_PicasoSPE::fileError()
{
	_writeCommand0(0xff1f);
	_a = _nacAckData(_b);
	return (uint8_t)_b;
}

bool Screen_PicasoSPE::fileDelete(String fileName)
{
	_writeCommandString0(0x0003, fileName);
	_a = _nacAckData(_b);
	return (_b != 0);
}

bool Screen_PicasoSPE::fileFind(String fileName)
{
	_writeCommandString0(0x0005, fileName);
	_a = _nacAckData(_b);
	return (_b != 0);
}

bool Screen_PicasoSPE::fileOpenAppend(String fileName, uint16_t &handler)
{
	_writeCommandString0(0x000a, fileName);
	_print8('a');
	_a = _nacAckData(_b);
	handler = _b;
	return (handler > 0);
}

bool Screen_PicasoSPE::fileOpenWrite(String fileName, uint16_t &handler)
{
	if (fileFind(fileName)) fileDelete(fileName);
	_writeCommandString0(0x000a, fileName);
	_print8('w');
	_a = _nacAckData(_b);
	handler = _b;
	return (handler > 0);
}

bool Screen_PicasoSPE::fileOpenRead(String fileName, uint16_t &handler)
{
	_writeCommandString0(0x000a, fileName);
	_print8('r');
	_a = _nacAckData(_b);
	handler = _b;
	return (handler > 0);
}

bool Screen_PicasoSPE::fileClose(uint16_t handler)
{
	_writeCommand16(0xff18, handler);
	_a = _nacAckData(_b);
	return (_b != 0);
}

bool Screen_PicasoSPE::fileWrite(uint16_t handler, char c)
{
	_writeCommand16(0x001f, (uint16_t)c);
	_print16(handler);
	_a = _nacAckData(_b);
	return (_b == 1);
}

bool Screen_PicasoSPE::fileWrite(uint16_t handler, String s)
{
	_writeCommandString0(0x0020, s);
	_print16(handler);
	_a = _nacAckData(_b);
	return (_b == s.length());
}

bool Screen_PicasoSPE::fileRead(uint16_t handler, char &c)
{
	_writeCommand16(0xff0e, handler);
	_a = _nacAckData(_b);
	c  = lowByte(_b);
	return (_a == 0x06);
}

uint8_t Screen_PicasoSPE::getOrientation()
{
    return _orientation;
}

void Screen_PicasoSPE::_orientCoordinates(uint16_t &x1, uint16_t &y1) { ; } // compulsory

void Screen_PicasoSPE::_setWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) { ; } // compulsory
void Screen_PicasoSPE::_writeData88(uint8_t dataHigh8, uint8_t dataLow8) { ; }  // compulsory


void Screen_PicasoSPE::_setBacklight(bool state)
{
    ;
}

void Screen_PicasoSPE::_setIntensity(uint8_t intensity)
{
    ;
}






