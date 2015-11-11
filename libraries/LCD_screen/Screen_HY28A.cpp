//
// Screen_HY28A.cpp
// Library C++ code
// ----------------------------------
// Developed with embedXcode
// http:// embedXcode.weebly.com
//
// Project LCD_screen Library Suite
//
// Created by Rei VILO, mars 06, 2013 19:33
// embedXcode.weebly.com
//
//
// Copyright © Rei VILO, 2013-2014
// License All rights reserved
//
// See Screen_HY28A.h and ReadMe.txt for references
//
    /// August 12 Roberto Romano
// added conditional wrap to Serialprintf on copy to leave debug code where it is but disable.
// this code was printing readed data slowing down diplay area copy
//
// New define conditional to share or not SPI for touch
// addeed CS HIGH on begin to avoid contention on MISO MOSI
// changed confusing macro names CLEAR_SCREEN_LOW SET_SCREEN_HIGH with SET_SCREEN_CS_LOW SET_SCREEN_CS_HIGH


// Library header
#include "Screen_HY28A.h"

// SPI screen select management
#if (SPI_MODE == FAST_SPI)

#define SET_SCREEN_CS_LOW cbi(_portScreenCS, _bitScreenCS)
#define SET_SCREEN_CS_HIGH  sbi(_portScreenCS, _bitScreenCS)

#else

#define SET_SCREEN_CS_LOW digitalWrite(_pinScreenChipSelect, LOW)
#define SET_SCREEN_CS_HIGH  digitalWrite(_pinScreenChipSelect, HIGH)

#endif

///
/// @name	HY28A size
/// @{

#define HY28A_WIDTH  240
#define HY28A_HEIGHT 320

/// @}

///
/// @name	HY28A constants
/// @{

#define ILIGRAMMODE 0x03 ///< Define scan mode
#define TOUCH_X     0xd0 ///< Read X value
#define TOUCH_Y     0x90 ///< Read Y value
#define TOUCH_Z1    0xb0 ///< Read Z1 value
#define TOUCH_Z2    0xc0 ///< Read Z2 value
#define TOUCH_TRIM  0x10 ///< Touch threshold
#define LCD_HEIGHT  320  ///< Vertical
#define LCD_WIDTH   240  ///< Horizontal

// #define LCD_SERIAL_DEBUG 1

#define SPI_START   0x70              ///< Start byte for SPI transfer
#define SPI_RD      0x01              ///< WR bit 1 within start
#define SPI_WR      0x00              ///< WR bit 0 within start
#define SPI_DATA    0x02              ///< RS bit 1 within start byte
#define SPI_INDEX   0x00              ///< RS bit 0 within start byte 0x00

/// @}

///
/// @name	HY28A commands
/// @{

// #define HY28A_NOP     0x00

/// @}


// Class
Screen_HY28A::Screen_HY28A(uint8_t pinScreenChipSelect, uint8_t pinScreenReset,
                           uint8_t pinTouchChipSelect
			#ifdef SPI_IS_SHAREDBYTP
				)
			#else
			   , uint8_t pinTouchSerialClock,
                           uint8_t pinTouchSerialDataMOSI, uint8_t pinTouchSerialDataMISO)
			#endif // SPI SHARE
{
    _pinScreenChipSelect    = pinScreenChipSelect;
    _pinScreenReset         = pinScreenReset;
    _pinTouchChipSelect     = pinTouchChipSelect;
#ifndef SPI_IS_SHAREDBYTP
    _pinTouchSerialClock    = pinTouchSerialClock;
    _pinTouchSerialDataMOSI = pinTouchSerialDataMOSI;
    _pinTouchSerialDataMISO = pinTouchSerialDataMISO;
#endif // SPI SHARE
};


void Screen_HY28A::_orientCoordinates(uint16_t &x1, uint16_t &y1)
{
	switch (_orientation) {
        case 0:  // ok
            break;
        case 1: // ok
            y1 = screenSizeY() - y1 -1;
            _swap(x1, y1);
            break;
        case 2: // ok
            x1 = screenSizeX() - x1 -1;
            y1 = screenSizeY() - y1 -1;
            break;
        case 3: // ok
            x1 = screenSizeX() - x1 -1;
            _swap(x1, y1);
            break;
	}
}


void Screen_HY28A::_setCursor(uint16_t x0, uint16_t y0)
{
	_orientCoordinates(x0, y0);
   	_writeRegister(0x0020, x0);
	_writeRegister(0x0021, y0);
}


void Screen_HY28A::_setWindow(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    _orientCoordinates(x1, y1);
	_orientCoordinates(x2, y2);
    
    _writeRegister(0x20, x1);
	_writeRegister(0x21, y1);
    
    if (x1>x2) _swap(x1, x2);
    if (y1>y2) _swap(y1, y2);
    
	_writeRegister(0x50, x1);
	_writeRegister(0x52, y1);
	_writeRegister(0x51, x2);
	_writeRegister(0x53, y2);
	_writeCommand(0x22);
}

void Screen_HY28A::_closeWindow()
{
    _setWindow(0, 0, screenSizeX()-1, screenSizeY()-1);
}


void Screen_HY28A::begin()
{
    // Hardware SPI for the screen
    pinMode(_pinScreenChipSelect, OUTPUT);
    pinMode(_pinScreenReset, OUTPUT);
    
    //    _portScreenCS   = portOutputRegister(digitalPinToPort(_pinScreenChipSelect));
    //    _bitScreenCS    = digitalPinToBitMask(_pinScreenChipSelect);
    
#if (SPI_MODE == FAST_SPI)
    _pinScreenSerialClock    = PB_4;
    _pinScreenSerialDataMOSI = PB_7;
    _pinScreenSerialDataMISO = PB_6;
    
    _portScreenCS   = portOutputRegister(digitalPinToPort(_pinScreenChipSelect));
    _bitScreenCS    = digitalPinToBitMask(_pinScreenChipSelect);
    _portScreenMOSI = portOutputRegister(digitalPinToPort(_pinScreenSerialDataMOSI));
    _bitScreenMOSI	= digitalPinToBitMask(_pinScreenSerialDataMOSI);
    _portScreenMISO	= portOutputRegister(digitalPinToPort(_pinScreenSerialDataMISO));
    _bitScreenMISO	= digitalPinToBitMask(_pinScreenSerialDataMISO);
    _portScreenSCK	= portOutputRegister(digitalPinToPort(_pinScreenSerialClock));
    _bitScreenSCK   = digitalPinToBitMask(_pinScreenSerialClock);
    _portScreenRST	= portOutputRegister(digitalPinToPort(_pinScreenReset));
    _bitScreenRST   = digitalPinToBitMask(_pinScreenReset);
    pinMode(_pinScreenSerialDataMOSI, OUTPUT);
    pinMode(_pinScreenSerialClock, OUTPUT);
    delay(100);
    
#elif (SPI_MODE == SOFTWARE_SPI)
    _pinScreenSerialClock    = PB_4;
    _pinScreenSerialDataMOSI = PB_7;
    _pinScreenSerialDataMISO = PB_6;
    
    pinMode(_pinScreenSerialDataMOSI, OUTPUT);
    pinMode(_pinScreenSerialDataMISO, INPUT);
    pinMode(_pinScreenSerialClock, OUTPUT);
    
#else                                                                           // normal hardware SPI
    
#if defined(__MSP430G2553__) || defined(__MSP430F5529__)
    SPI.begin();
    SPI.setClockDivider(SPI_CLOCK_DIV2); // for MSP430G2553
    SPI.setBitOrder(MSBFIRST);
    SPI.setDataMode(SPI_MODE3); // SPI_MODE2 or SPI_MODE3
    
#elif defined(__LM4F120H5QR__)
    SPI.setModule(2);
    SPI.begin();
    SPI.setClockDivider(SPI_CLOCK_DIV2); // for LM4F with 16000000
    SPI.setBitOrder(MSBFIRST);
    SPI.setDataMode(SPI_MODE3); // SPI_MODE2 or SPI_MODE3
    
    // Arduino specific
#elif defined(__AVR_ATmega328P__) || defined(__AVR_ATmega2560__) || defined(__AVR_ATmega32U4__) || defined(__SAM3X8E__)
    SPI.begin();
    SPI.setClockDivider(SPI_CLOCK_DIV2);
    SPI.setBitOrder(MSBFIRST);
    SPI.setDataMode(SPI_MODE3); // SPI_MODE2 or SPI_MODE3
    
#else
  // #error Board not supported
    
#endif // end MCU
#endif // end FAST_SPI
    
    pinMode(_pinTouchChipSelect, OUTPUT);
#ifndef SPI_IS_SHAREDBYTP
    // Software SPI for the touch controller
    pinMode(_pinTouchSerialClock, OUTPUT);
    pinMode(_pinTouchSerialDataMOSI, OUTPUT);
    pinMode(_pinTouchSerialDataMISO, INPUT);
#endif    
	uint16_t DeviceCode;

    // RR Add to avoid MISO MOSI bus contention
    digitalWrite(_pinScreenChipSelect, HIGH);
    digitalWrite(_pinTouchChipSelect, HIGH);

    digitalWrite(_pinScreenReset, LOW);
    delay(10);
    digitalWrite(_pinScreenReset, HIGH);
    delay(10);
    
    // DeviceCode = LCD_ReadReg(0x0000);		// Read ID
    // Serial.println(DeviceCode, HEX);
    
    _writeRegister(0x00, 0x0000); // Start oscillation
    _writeRegister(0x01, 0x0100); // Driver Output Contral
    _writeRegister(0x02, 0x0700); // LCD Driver Waveform Contral
    _writeRegister(0x03, 0x1038); // Set the scan mode
    _writeRegister(0x04, 0x0000); // Scalling Contral
    _writeRegister(0x08, 0x0202); // Display Contral 2
    _writeRegister(0x09, 0x0000); // Display Contral 3
    _writeRegister(0x0a, 0x0000); // Frame Cycle Contal
    _writeRegister(0x0c, (1<<0)); // Extern Display Interface Contral 1
    _writeRegister(0x0d, 0x0000); // Frame Maker Position
    _writeRegister(0x0f, 0x0000); // Extern Display Interface Contral 2
    delay(50);
    _writeRegister(0x07, 0x0101); // Display Contral
    delay(50);
    _writeRegister(0x10, (1<<12)|(0<<8)|(1<<7)|(1<<6)|(0<<4)); // Power Control 1
    _writeRegister(0x11, 0x0007); // Power Control 2
    _writeRegister(0x12, (1<<8)|(1<<4)|(0<<0)); // Power Control 3
    _writeRegister(0x13, 0x0b00); // Power Control 4
    _writeRegister(0x29, 0x0000); // Power Control 7
    _writeRegister(0x2b, (1<<14)|(1<<4));
	
    _writeRegister(0x50, 0); // Set X Start
    _writeRegister(0x51, 239);	 // Set X End
    _writeRegister(0x52, 0);	 // Set Y Start
    _writeRegister(0x53, 319);	 // Set Y End
    delay(50);
    
    _writeRegister(0x60, 0x2700); // Driver Output Control
    _writeRegister(0x61, 0x0001); // Driver Output Control
    _writeRegister(0x6a, 0x0000); // Vertical Srcoll Control
    
    _writeRegister(0x80, 0x0000); // Display Position? Partial Display 1
    _writeRegister(0x81, 0x0000); // RAM Address Start? Partial Display 1
    _writeRegister(0x82, 0x0000); // RAM Address End-Partial Display 1
    _writeRegister(0x83, 0x0000); // Displsy Position? Partial Display 2
    _writeRegister(0x84, 0x0000); // RAM Address Start? Partial Display 2
    _writeRegister(0x85, 0x0000); // RAM Address End? Partial Display 2
    
    _writeRegister(0x90, (0<<7)|(16<<0)); // Frame Cycle Contral
    _writeRegister(0x92, 0x0000); // Panel Interface Contral 2
    _writeRegister(0x93, 0x0001); // Panel Interface Contral 3
    _writeRegister(0x95, 0x0110); // Frame Cycle Contral
    _writeRegister(0x97, (0<<8));
    _writeRegister(0x98, 0x0000); // Frame Cycle Contral
    _writeRegister(0x07, 0x0133);
    delay(100); // delay 50 ms
    
    setBacklight(true);
    setOrientation(0);
    
    _screenWidth = HY28A_WIDTH;
    _screenHeigth = HY28A_HEIGHT;
    _screenDiagonal = 28;

    _penSolid  = false;
    _fontSolid = true;
    _flagRead = true;                               // screen readable
    
    _touchXmin = 108;
    _touchXmax = 1919;
    _touchYmin = 88;
    _touchYmax = 1880;
    _touchTrim = TOUCH_TRIM;                        // touch
    
    clear();
}

String Screen_HY28A::WhoAmI()
{
    return "HY28A screen";
}


void Screen_HY28A::invert(boolean flag) {
    // _writeCommand(flag ? HY28A_INVON : HY28A_INVOFF);
}


void Screen_HY28A::_setBacklight(bool flag) {
    // if (_backlight!=NULL) digitalWrite(_backlight, flag);
}

void Screen_HY28A::_setIntensity(uint8_t intensity)
{
    ;
}

void Screen_HY28A::_setOrientation(uint8_t orientation)
{
    // _orientation = orientation % 4;
//    LCD_screen_font::setOrientation(orientation);
    
    switch (_orientation) {
        case 0:
            //                                     hvO
            _writeRegister(ILIGRAMMODE, 0x1000 + 0b00110000);
            break;
        case 1:
            //                                     hvO
            _writeRegister(ILIGRAMMODE, 0x1000 + 0b00101000);
            break;
        case 2:
            //                                     hvO
            _writeRegister(ILIGRAMMODE, 0x1000 + 0b00000000);
            break;
        case 3:
            //                                     hvO
            _writeRegister(ILIGRAMMODE, 0x1000 + 0b00011000);
            break;
    }
}

uint16_t Screen_HY28A::readPixel(uint16_t x1, uint16_t y1)
{
	uint16_t w;
    
	_setCursor(x1, y1);
    _writeCommand(0x0022);
	
	w = _readData();   // An empty read
	w = _readData();
    // Serial.print("**");
    // Serial.print(w, BIN);
    // Serial.print(".");
    // Serial.println(_readData(), BIN);
    
	return w;
}


//void Screen_HY28A::move(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t dx, uint16_t dy)
//{
//    uint16_t buffer[dx];
//    
//    setPenSolid(false);
//    dRectangle(x1, y1, dx, dy, myColours.green);
//    dRectangle(x2, y2, dx, dy, myColours.red);
//    
//    // for (uint16_t i=0; i < dx; i++) {
//    // for (uint16_t j=0; j < dy; j++) {
//    // point(x2+i, y2+j, readPixel(x1+i, y1+j));
//    // }
//    // }
//    for (uint16_t j=0; j < dy; j++) {
//        // read
//        _setWindow(x1, y1+j, x1+dx, y1+j);
//        _writeCommand(0x0022);
//        
//        // digitalWrite(_pinScreenChipSelect, LOW);
//        SET_SCREEN_CS_LOW;
//        
//        // SPI.transfer(SPI_START | SPI_RD | SPI_DATA); // write : RS = 1, RW = 0
//        _writeSPI(SPI_START | SPI_RD | SPI_DATA); // write : RS = 1, RW = 0
//        
//        // SPI.transfer(0);   // An empty read
//        _readSPI();   // An empty read
//        
//        for (uint16_t i=0; i < dx; i++) {
//            // buffer[i]   = SPI.transfer(0);
//            // buffer[i] <<= 8;
//            // buffer[i]  += SPI.transfer(0);
//            buffer[i]   = _readSPI();
//            buffer[i] <<= 8;
//            buffer[i]  += _readSPI();
//        }
//        // digitalWrite(_pinScreenChipSelect, HIGH);
//        SET_SCREEN_CS_HIGH;
//        
//        // write
//        _setWindow(x2, y2+j, x2+dx, y2+j);
//        _writeCommand(0x0022);
//        
//        // digitalWrite(_pinScreenChipSelect, LOW);
//        SET_SCREEN_CS_LOW;
//        
//        SPI.transfer(SPI_START | SPI_WR | SPI_DATA); // write : RS = 1, RW = 0
//        for (uint16_t i=0; i < dx; i++) {
//            // SPI.transfer((buffer[i] >> 8)); // write D8..D15
//            // SPI.transfer((buffer[i] & 0xff)); // write D0..D7
//            _writeSPI((buffer[i] >> 8)); // write D8..D15
//            _writeSPI((buffer[i] & 0xff)); // write D0..D7
//        }
//        
//        // digitalWrite(_pinScreenChipSelect, HIGH);
//        SET_SCREEN_CS_HIGH;
//        
//    }
//    _closeWindow();
//}

void Screen_HY28A::copyPasteArea(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t dx, uint16_t dy)
{
    uint16_t data;
    uint8_t HY28A_buffer[dx][2];
    
    _closeWindow();
    
    for (uint16_t y=0; y<dy; y++) {
        for (uint16_t x=0; x<dx; x++) {
            _setCursor(x1+x, y1+y);
            data = _readData();
            
            HY28A_buffer[x][0] = highByte(data);
            HY28A_buffer[x][1] = lowByte(data);
        }
        
        _setWindow(x2, y2+y, x2+dx, y2+y);
        _writeCommand(0x0022);
        
        SET_SCREEN_CS_LOW;
        _writeSPI(SPI_START | SPI_WR | SPI_DATA);                               // write : RS = 1, RW = 0
        for (uint16_t x=0; x<dx; x++) {
            _writeSPI(HY28A_buffer[x][0]);                                            // write D8..D15
            _writeSPI(HY28A_buffer[x][1]);                                            // write D0..D7
        }
        SET_SCREEN_CS_HIGH;
    }
}


void Screen_HY28A::_setPoint(uint16_t x1, uint16_t y1, uint16_t colour)
{
    _setWindow(x1, y1, x1, y1);
    _writeData88(highByte(colour), lowByte(colour));
}

// Utilities
void Screen_HY28A::_writeCommand(uint8_t command8)
{
    // digitalWrite(_pinScreenChipSelect, LOW);
    SET_SCREEN_CS_LOW;
    
    // SPI write data
    // SPI.transfer(SPI_START | SPI_WR | SPI_INDEX); // write : RS = 0, RW = 0
    // SPI.transfer(0x00); // write D8..D15
    // SPI.transfer(command8); // write D0..D7
    _writeSPI(SPI_START | SPI_WR | SPI_INDEX); // write : RS = 0, RW = 0
    _writeSPI(0x00); // write D8..D15
    _writeSPI(command8); // write D0..D7
    
    // digitalWrite(_pinScreenChipSelect, HIGH);
    SET_SCREEN_CS_HIGH;
    
}

void Screen_HY28A::_writeData16(uint16_t data16)
{
    // digitalWrite(_pinScreenChipSelect, LOW);
    SET_SCREEN_CS_LOW;
    
    // SPI.transfer(SPI_START | SPI_WR | SPI_DATA); // write : RS = 1, RW = 0
    // SPI.transfer(highByte(data16)); // write D8..D15
    // SPI.transfer(lowByte(data16)); // write D0..D7
    _writeSPI(SPI_START | SPI_WR | SPI_DATA); // write : RS = 1, RW = 0
    _writeSPI(highByte(data16)); // write D8..D15
    _writeSPI(lowByte(data16)); // write D0..D7
    
    // digitalWrite(_pinScreenChipSelect, HIGH);
    SET_SCREEN_CS_HIGH;
    
}

void Screen_HY28A::_writeData88(uint8_t dataHigh8, uint8_t dataLow8)
{
    // digitalWrite(_pinScreenChipSelect, LOW);
    SET_SCREEN_CS_LOW;
    
    // SPI.transfer(SPI_START | SPI_WR | SPI_DATA); // write : RS = 1, RW = 0
    // SPI.transfer(dataHigh8); // write D8..D15
    // SPI.transfer(dataLow8); // write D0..D7
    _writeSPI(SPI_START | SPI_WR | SPI_DATA); // write : RS = 1, RW = 0
    _writeSPI(dataHigh8); // write D8..D15
    _writeSPI(dataLow8); // write D0..D7
    
    // digitalWrite(_pinScreenChipSelect, HIGH);
    SET_SCREEN_CS_HIGH;
    
}

uint16_t Screen_HY28A::_readData()
{
	uint16_t data;
    
    // digitalWrite(_pinScreenChipSelect, LOW);
    SET_SCREEN_CS_LOW;
    
    // SPI.transfer(SPI_START | SPI_RD | SPI_DATA);    // read: RS = 1, RW = 1
	_writeSPI(SPI_START | SPI_RD | SPI_DATA);    // read: RS = 1, RW = 1
    
    // SPI.transfer(0);                                // Dummy read 1
    // data   = SPI.transfer(0);                      // read D8..D15
    // data <<= 8;
    // data  |= SPI.transfer(0);                      // read D0..D7
	data   = _readSPI();                                // Dummy read 1
	data   = _readSPI();                      // read D8..D15
#ifdef LCD_SERIAL_DEBUG
    Serial.print(data, BIN);
#endif
	data <<= 8;
	data  |= _readSPI();                      // read D0..D7
#ifdef LCD_SERIAL_DEBUG
    Serial.print("\t");
#endif    
    
    // digitalWrite(_pinScreenChipSelect, HIGH);
    SET_SCREEN_CS_HIGH;
#ifdef LCD_SERIAL_DEBUG    
    Serial.print("1:");
    Serial.println(data, BIN);
    // Convert B5G5R5 into R5G6B5
    // Colour         BBBBBxGGGGGRRRRR                     BBBBBxGGGGGRRRRR                     BBBBBxGGGGGRRRRR
    data = ((data & 0b1111100000000000) >>11) + ((data & 0b0000001111100000)<<1) + ((data & 0b0000000000011111)<<11);
    Serial.print("2:");
    Serial.println(data, BIN);
#endif    
	return data;
}

void Screen_HY28A::_writeRegister(uint8_t command8, uint16_t data16)
{
    _writeCommand(command8);
    _writeData16(data16);
}


void Screen_HY28A::_writeSPI(uint8_t data8)
{
#if (SPI_MODE==FAST_SPI)
    // bit 7
 	if (data8 & 0x80) sbi(_portScreenMOSI, _bitScreenMOSI); else cbi(_portScreenMOSI, _bitScreenMOSI);
	pulse_low(_portScreenSCK, _bitScreenSCK);
    
    // bit 6
	if (data8 & 0x40) sbi(_portScreenMOSI, _bitScreenMOSI); else cbi(_portScreenMOSI, _bitScreenMOSI);
	pulse_low(_portScreenSCK, _bitScreenSCK);
    
    // bit 5
	if (data8 & 0x20) sbi(_portScreenMOSI, _bitScreenMOSI); else cbi(_portScreenMOSI, _bitScreenMOSI);
	pulse_low(_portScreenSCK, _bitScreenSCK);
    
    // bit 4
	if (data8 & 0x10) sbi(_portScreenMOSI, _bitScreenMOSI); else cbi(_portScreenMOSI, _bitScreenMOSI);
	pulse_low(_portScreenSCK, _bitScreenSCK);
    
    // bit 3
	if (data8 & 0x08) sbi(_portScreenMOSI, _bitScreenMOSI); else cbi(_portScreenMOSI, _bitScreenMOSI);
	pulse_low(_portScreenSCK, _bitScreenSCK);
    
    // bit 2
	if (data8 & 0x04) sbi(_portScreenMOSI, _bitScreenMOSI); else cbi(_portScreenMOSI, _bitScreenMOSI);
	pulse_low(_portScreenSCK, _bitScreenSCK);
    
    // bit 1
	if (data8 & 0x02) sbi(_portScreenMOSI, _bitScreenMOSI); else cbi(_portScreenMOSI, _bitScreenMOSI);
	pulse_low(_portScreenSCK, _bitScreenSCK);
	
    // bit 0
    if (data8 & 0x01) sbi(_portScreenMOSI, _bitScreenMOSI); else cbi(_portScreenMOSI, _bitScreenMOSI);
	pulse_low(_portScreenSCK, _bitScreenSCK);
    
#elif (SPI_MODE==SOFTWARE_SPI)
    shiftOut(_pinScreenSerialDataMOSI, _pinTouchSerialClock, MSBFIRST, data8);
    
#else
    SPI.transfer(data8);
    
#endif
}

uint8_t Screen_HY28A::_readSPI()
{
#if (SPI_MODE==FAST_SPI)
    uint8_t data8 = 0;
    data8 = shiftIn(_pinScreenSerialDataMISO, _pinScreenSerialClock, MSBFIRST);
    return data8;    
#elif (SPI_MODE==SOFTWARE_SPI)
    uint8_t data8 = 0;
    data8 = shiftIn(_pinScreenSerialDataMISO, _pinScreenSerialClock, MSBFIRST);
    return data8; 
#else
    return SPI.transfer(0);   
#endif
}


// Communication, fast write
void Screen_HY28A::_fastFill(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t colour)
{
    if (x1 > x2) _swap(x1, x2);
    if (y1 > y2) _swap(y1, y2);
    
    _setWindow(x1, y1, x2, y2);
    _writeCommand(0x0022);
    
    // digitalWrite(_pinScreenChipSelect, LOW);
    SET_SCREEN_CS_LOW;
    
    // SPI.transfer(SPI_START | SPI_WR | SPI_DATA); // write : RS = 1, RW = 0
    _writeSPI(SPI_START | SPI_WR | SPI_DATA); // write : RS = 1, RW = 0
    
    uint8_t highColour = highByte(colour);
    uint8_t lowColour  = lowByte(colour);
    
    for (uint32_t t=(uint32_t)(y2-y1+1)*(x2-x1+1); t>0; t--) {
        // SPI.transfer(highColour); // write D8..D15
        // SPI.transfer(lowColour); // write D0..D7
        _writeSPI(highColour); // write D8..D15
        _writeSPI(lowColour); // write D0..D7
    }
    
    // digitalWrite(_pinScreenChipSelect, HIGH);
    SET_SCREEN_CS_HIGH;
    
    _closeWindow();
}


// Touch
void Screen_HY28A::_getOneTouch(uint8_t command, uint8_t &a, uint8_t &b)
{
    digitalWrite(_pinTouchChipSelect, LOW);
    
#ifdef SPI_IS_SHAREDBYTP
    _writeSPI(command);
    a = _readSPI();
    b = _readSPI();
#else
    shiftOut(_pinTouchSerialDataMOSI, _pinTouchSerialClock, MSBFIRST, command);
    a = shiftIn(_pinTouchSerialDataMISO, _pinTouchSerialClock, MSBFIRST);
    b = shiftIn(_pinTouchSerialDataMISO, _pinTouchSerialClock, MSBFIRST);
#endif    
    digitalWrite(_pinTouchChipSelect, HIGH);
}

void Screen_HY28A::_getRawTouch(uint16_t &x0, uint16_t &y0, uint16_t &z0)
{
    uint16_t z1, z2;
    uint8_t a, aa, b, c, cc, d;
    
    // Need for cleaning screen SPI communication for LM4F
    // #if defined(__LM4F120H5QR__)
    // _setPoint(0, 0, 0);
    // #endif
    
    delay(10);
    _getOneTouch(TOUCH_Z1 + 0x08 + 0x01, a, b); // 8-bit
    _getOneTouch(TOUCH_Z2 + 0x08 + 0x01, c, d);
    
    z0 = a + (0x7f - c);
    if (z0 == 0x7f) z0 = 0;
    
    if (z0 > _touchTrim) {
        do {
            _getOneTouch(TOUCH_X + 0x00 + 0x01, a, b); // 12-bit
            _getOneTouch(TOUCH_Y + 0x00 + 0x01, c, d);
            delay(10);
            _getOneTouch(TOUCH_X + 0x00 + 0x01, aa, b); // 12-bit
            _getOneTouch(TOUCH_Y + 0x00 + 0x01, cc, d);
        } while ( (a != aa) || (c != cc) );
        
        x0 = a <<4 | b >>4;
        y0 = c <<4 | d >>4;
    }
}

