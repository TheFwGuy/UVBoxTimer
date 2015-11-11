//
// Screen_HY28A_SRAM.cpp
// Library C++ code
// ----------------------------------
// Developed with embedXcode
// http:// embedXcode.weebly.com
//
// Project LCD_screen Library Suite
//
// Created by Rei VILO, Mar 06, 2013
// embedXcode.weebly.com
//
// Copyright © Rei VILO, 2013-2014
// License All rights reserved
//
// See HY28A_SRAM_screen.h and ReadMe.txt for references
//

#if defined(__LM4F120H5QR__)

// Library header
#include "Screen_HY28A_SRAM.h"

///
/// @name	HY28A constants
/// @{

#define SPI_START   0x70              ///< Start byte for SPI transfer
#define SPI_RD      0x01              ///< WR bit 1 within start
#define SPI_WR      0x00              ///< WR bit 0 within start
#define SPI_DATA    0x02              ///< RS bit 1 within start byte
#define SPI_INDEX   0x00              ///< RS bit 0 within start byte 0x00

/// @}

///
/// @name	HY28A size
/// @{

#define HY28A_WIDTH  240
#define HY28A_HEIGHT 320

/// @}

uint8_t HY28A_buffer[max(HY28A_WIDTH, HY28A_HEIGHT)][2];


///
/// @name	Instruction Set
///
/// @{

#define SRAM_READ         0b00000011  ///< Read data from memory array beginning at selected address
#define SRAM_WRITE        0b00000010  ///< Write data to memory array beginning at selected address
#define SRAM_READ_STATUS  0b00000101  ///< Read STATUS register
#define SRAM_WRITE_STATUS 0b00000001  ///< Write STATUS register

/// @}

/// @section Initialisation
/// @name   Status Register Instruction
///
/// @{

#define SRAM_BYTE_MODE     0b00000000  ///< Byte mode (default operation)
#define SRAM_PAGE_MODE     0b10000000  ///< Page mode
#define SRAM_SEQUENCE_MODE 0b01000000  ///< Sequential mode
#define SRAM_RESERVED_MODE 0b11000000  ///< Reserved
#define SRAM_HOLD_MODE     0b00000001  ///< Set this bit to DISABLE hold mode

/// @}

// Class
Screen_HY28A_SRAM::Screen_HY28A_SRAM(uint8_t pinScreenChipSelect, uint8_t pinScreenReset,
                               uint8_t pinTouchChipSelect, uint8_t pinTouchSerialClock,
                               uint8_t pinTouchSerialDataMOSI, uint8_t pinTouchSerialDataMISO)
{
//    Screen_HY28A::Screen_HY28A(pinScreenChipSelect, pinTouchChipSelect, pinTouchSerialClock, pinTouchSerialDataMOSI, pinTouchSerialDataMISO);
    Screen_HY28A::_pinScreenChipSelect    = pinScreenChipSelect;
    Screen_HY28A::_pinScreenReset         = pinScreenReset;
    Screen_HY28A::_pinTouchChipSelect     = pinTouchChipSelect;
    Screen_HY28A::_pinTouchSerialClock    = pinTouchSerialClock;
    Screen_HY28A::_pinTouchSerialDataMOSI = pinTouchSerialDataMOSI;
    Screen_HY28A::_pinTouchSerialDataMISO = pinTouchSerialDataMISO;
    Screen_HY28A::_pinScreenChipSelect    = pinScreenChipSelect;
};



void Screen_HY28A_SRAM::begin()
{
    Screen_HY28A::begin();
    
    _flagStorage = false;
    
#if (USE_SPI_SRAM == 1)
    _beginSRAM();
    _flagStorage = true;
#endif                                                                          // end USE_SPI_SRAM
    
//    // Software SPI for the touch controller
////    _pinTouchChipSelect.begin(_pinTouchChipSelectNumber);
////    pinMode(_pinTouchSerialClock, OUTPUT);
//    pinMode(_pinTouchSerialDataMOSI, OUTPUT);
//    pinMode(_pinTouchSerialDataMISO, INPUT);
    
    //    digitalWrite(_pinScreenReset, LOW);
    //    delay(10);
    //    digitalWrite(_pinScreenReset, HIGH);
    //    delay(10);
    
    clear();
}

String Screen_HY28A_SRAM::WhoAmI()
{
    return "HY28A + SRAM screen";
}

//void Screen_HY28A_SRAM::_writeBeginSPI()
//{
//#if (FAST_DIGITALWRITE == 1)
//    _pinScreenChipSelect.setLOW();
//    
//#else
//    digitalWrite(_pinScreenChipSelect, LOW);
//    
//#endif
//}
//
//void Screen_HY28A_SRAM::_writeEndSPI()
//{
//#if (FAST_DIGITALWRITE == 1)
//    _pinScreenChipSelect.setHIGH();
//    
//#else
//    digitalWrite(_pinScreenChipSelect, HIGH);
//    
//#endif
//}

#if (USE_SPI_SRAM == 1)
SPIClass SPI_RAM(1);

void Screen_HY28A_SRAM::_beginSRAM()
{
    //    Serial.println("*** _beginSRAM");
    
    _fastPinSRAMChipSelectMSB.begin(PE_2);
    _fastPinSRAMChipSelectLSB.begin(PE_3);
    
    //    SPI_RAM.setModule(0);
    SPI_RAM.begin();
    SPI_RAM.setClockDivider(SPI_CLOCK_DIV4);                                    // for LM4F with 16000000
    SPI_RAM.setBitOrder(MSBFIRST);
    SPI_RAM.setDataMode(SPI_MODE0);                                             // SPI_MODE0
    
    _fastPinSRAMChipSelectMSB.setLOW();
    SPI_RAM.transfer(SRAM_WRITE_STATUS);
    SPI_RAM.transfer(SRAM_SEQUENCE_MODE);
    _fastPinSRAMChipSelectMSB.setHIGH();
    
    _fastPinSRAMChipSelectLSB.setLOW();
    SPI_RAM.transfer(SRAM_WRITE_STATUS);
    SPI_RAM.transfer(SRAM_SEQUENCE_MODE);
    _fastPinSRAMChipSelectLSB.setHIGH();
}

//void Screen_HY28A_SRAM::_writeSRAM(uint32_t &address, uint8_t dataHigh8, uint8_t dataLow8)
//{
//    // 131070 = 1024 * 1024 bits / 8 = 131072 bytes - 2 to be written
//    bool flag = false;
//    uint32_t localAddress = address;
//    
//    if (address < (uint32_t)131070) {
//        flag = true;
//    } else {
//        localAddress = address - (uint32_t)131070;
//    }
//    
//#if (FAST_DIGITALWRITE==1)
//    if (flag) _pinSRAMChipSelectLSB.setLOW();
//    else      _pinSRAMChipSelectMSB.setLOW();
//    
//    SPI_RAM.transfer(SRAM_WRITE);
//    SPI_RAM.transfer((localAddress >> 16) & 0xff);
//    SPI_RAM.transfer((localAddress >> 8) & 0xff);
//    SPI_RAM.transfer(localAddress);
//    SPI_RAM.transfer(dataHigh8);
//    SPI_RAM.transfer(dataLow8);
//    
//    if (flag) _pinSRAMChipSelectLSB.setHIGH();
//    else      _pinSRAMChipSelectMSB.setHIGH();
//    
//#else
//    if (flag) _pinSRAMChipSelectMSB.setLOW();
//    else      _pinSRAMChipSelectLSB.setLOW();
//    
//    SPI_RAM.transfer(SRAM_READ);
//    SPI_RAM.transfer((localAddress >> 16) & 0xff);
//    SPI_RAM.transfer((localAddress >> 8) & 0xff);
//    SPI_RAM.transfer(localAddress);
//    SPI_RAM.transfer(dataHigh8);
//    SPI_RAM.transfer(dataLow8);
//    
//    if (flag) _pinSRAMChipSelectMSB.setHIGH();
//    else      _pinSRAMChipSelectLSB.setHIGH();
//    
//#endif
//    
//    address = address + 2;
//}
//
//
//void Screen_HY28A_SRAM::_readSRAM(uint32_t &address, uint8_t &dataHigh8, uint8_t &dataLow8)
//{
//    bool flag = false;
//    uint32_t localAddress = address;
//    
//    if (address < (uint32_t)131070) {
//        flag = true;
//    } else {
//        localAddress = address - (uint32_t)131070;
//    }
//    
//#if (FAST_DIGITALWRITE==1)
//    if (flag) _pinSRAMChipSelectLSB.setLOW();
//    else      _pinSRAMChipSelectMSB.setLOW();
//    
//    SPI_RAM.transfer(SRAM_READ);
//    SPI_RAM.transfer((localAddress >> 16) & 0xff);
//    SPI_RAM.transfer((localAddress >> 8) & 0xff);
//    SPI_RAM.transfer(localAddress);
//    dataHigh8 = SPI_RAM.transfer(0);
//    dataLow8  = SPI_RAM.transfer(0);
//    
//    if (flag) _pinSRAMChipSelectLSB.setHIGH();
//    else      _pinSRAMChipSelectMSB.setHIGH();
//    
//#else
//    if (flag) _pinSRAMChipSelectMSB.setLOW();
//    else      _pinSRAMChipSelectLSB.setLOW();
//    
//    SPI_RAM.transfer(SRAM_READ);
//    SPI_RAM.transfer((localAddress >> 16) & 0xff);
//    SPI_RAM.transfer((localAddress >> 8) & 0xff);
//    SPI_RAM.transfer(localAddress);
//    dataHigh8 = SPI_RAM.transfer(0);
//    dataLow8  = SPI_RAM.transfer(0);
//    
//    if (flag) _pinSRAMChipSelectMSB.setHIGH();
//    else      _pinSRAMChipSelectLSB.setHIGH();
//    
//#endif
//    
//    address = address + 2;
//}

void Screen_HY28A_SRAM::_fastCopyAreaToSRAM(uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy, uint32_t &address)
{
    uint16_t data;
    uint32_t localAddress = address;
    
    if ((uint32_t)address+dx*dy < (uint32_t)131070) {
        
        for (uint16_t y=0; y<dy; y++) {
            localAddress = address + y*dx;
            
            _fastPinSRAMChipSelectMSB.setLOW();
            SPI_RAM.transfer(SRAM_WRITE);
            SPI_RAM.transfer((localAddress >> 16) & 0xff);
            SPI_RAM.transfer((localAddress >> 8) & 0xff);
            SPI_RAM.transfer(localAddress);
            
            for (uint16_t x=0; x<dx; x++) {
                _setCursor(x0+x, y0+y);
                data = _readData();
                
                HY28A_buffer[x][0] = highByte(data);
                HY28A_buffer[x][1] = lowByte(data);
                
                SPI_RAM.transfer(HY28A_buffer[x][0]);
            }
            _fastPinSRAMChipSelectMSB.setHIGH();
        
            _fastPinSRAMChipSelectLSB.setLOW();
            SPI_RAM.transfer(SRAM_WRITE);
            SPI_RAM.transfer((localAddress >> 16) & 0xff);
            SPI_RAM.transfer((localAddress >> 8) & 0xff);
            SPI_RAM.transfer(localAddress);

            for (uint16_t x=0; x<dx; x++) {
                SPI_RAM.transfer(HY28A_buffer[x][1]);
            }
            _fastPinSRAMChipSelectLSB.setHIGH();
        }
        _closeWindow();
        
        address = address + dx*dy;
    }
}

// Communication, fast read
void Screen_HY28A_SRAM::_fastPasteAreaFromSRAM(uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy, uint32_t &address, bool option)
{
    uint8_t dataHigh8, dataLow8;
    uint32_t localAddress = address;
    
    if ((uint32_t)address+dx*dy < (uint32_t)131070) {
        
        _setWindow(x0, y0, x0+dx-1, y0+dy-1);
        _setCursor(x0, y0);
        
//        _writeBeginSPI();
        digitalWrite(_pinScreenChipSelect, LOW);
        _writeSPI(SPI_START | SPI_WR | SPI_DATA);                                   // write : RS = 1, RW = 0
        
        for (uint16_t y=0; y<dy; y++) {
            localAddress = address + y*dx;
            
            _fastPinSRAMChipSelectMSB.setLOW();
            SPI_RAM.transfer(SRAM_READ);
            SPI_RAM.transfer((localAddress >> 16) & 0xff);
            SPI_RAM.transfer((localAddress >> 8) & 0xff);
            SPI_RAM.transfer(localAddress);
            for (uint16_t x=0; x<dx; x++) {
                HY28A_buffer[x][0] = SPI_RAM.transfer(0);
            }
            _fastPinSRAMChipSelectMSB.setHIGH();
            
            _fastPinSRAMChipSelectLSB.setLOW();
            SPI_RAM.transfer(SRAM_READ);
            SPI_RAM.transfer((localAddress >> 16) & 0xff);
            SPI_RAM.transfer((localAddress >> 8) & 0xff);
            SPI_RAM.transfer(localAddress);
            for (uint16_t x=0; x<dx; x++) {
                HY28A_buffer[x][1]  = SPI_RAM.transfer(0);
                
                if (option) {
                    _writeSPI(((HY28A_buffer[x][0] >> 1) & 0b11111000) | ((HY28A_buffer[x][0] & 0b00000111) >> 1));
                    _writeSPI(((HY28A_buffer[x][1] >> 1) & 0b11100000) | ((HY28A_buffer[x][1] & 0b00011111) >> 1));
                } else {
                    _writeSPI(HY28A_buffer[x][0]);                                              // write D8..D15
                    _writeSPI(HY28A_buffer[x][1]);                                               // write D0..D7
                }
            }
            _fastPinSRAMChipSelectLSB.setHIGH();
            
        }
//        _writeEndSPI();
        digitalWrite(_pinScreenChipSelect, HIGH);
        
        address = address + dx*dy;
    }
}



//void Screen_HY28A_SRAM::_fastCopyAreaToSRAM(uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy, uint32_t &address)
//{
//    bool flag = false;
//    uint32_t localAddress = (uint32_t)address;
//    uint16_t data;
//
//    if ((uint32_t)address+dx*dy < (uint32_t)131070) {
//        flag = true;
//    } else if ((uint32_t)address < (uint32_t)131070) {
//        localAddress = (uint32_t)131070;
//    } else {
//        localAddress = (uint32_t)address - (uint32_t)131070;
//    }
//    Serial.print("address       =");
//    Serial.println(address, DEC);
//    Serial.print("address+dx*dy =");
//    Serial.println(address+dx*dy, DEC);
//    Serial.print("localAddress  =");
//    Serial.println(localAddress, DEC);
//    Serial.print("flag          =");
//    Serial.println(flag, DEC);
//
//#if (FAST_DIGITALWRITE==1)
//    if (flag) _pinSRAMChipSelectLSB.setLOW();
//    else      _pinSRAMChipSelectMSB.setLOW();
//#else
//    if (flag) _pinSRAMChipSelectMSB.setLOW();
//    else      _pinSRAMChipSelectLSB.setLOW();
//#endif
//
//    SPI_RAM.transfer(SRAM_WRITE);
//    SPI_RAM.transfer((localAddress >> 16) & 0xff);
//    SPI_RAM.transfer((localAddress >> 8) & 0xff);
//    SPI_RAM.transfer(localAddress);
//
//
//    _closeWindow();
//
//    for (uint16_t y=y0; y<y0+dy; y++) {
//        for (uint16_t x=x0; x<x0+dx; x++) {
//            _setCursor(x, y);
//            data = _readData();
//
//            SPI_RAM.transfer(highByte(data));
//            SPI_RAM.transfer(lowByte(data));
//        }
//    }
//
//#if (FAST_DIGITALWRITE==1)
//    if (flag) _pinSRAMChipSelectLSB.setHIGH();
//    else      _pinSRAMChipSelectMSB.setHIGH();
//#else
//    if (flag) _pinSRAMChipSelectMSB.setHIGH();
//    else      _pinSRAMChipSelectLSB.setHIGH();
//#endif
//
//    address = address + dx*dy;
//}
//
//// Communication, fast read
//void Screen_HY28A_SRAM::_fastPasteAreaFromSRAM(uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy, uint32_t &address, bool option)
//{
//    uint8_t highColour, lowColour;
//
//    bool flag = false;
//    uint32_t localAddress = address;
//    uint8_t dataHigh8, dataLow8;
//
//    if ((uint32_t)address+dx*dy < (uint32_t)131070) {
//        flag = true;
//    } else if ((uint32_t)address < (uint32_t)131070) {
//        localAddress = (uint32_t)131070;
//    } else {
//        localAddress = (uint32_t)address - (uint32_t)131070;
//    }
//
//    Serial.print("address       =");
//    Serial.println(address, DEC);
//    Serial.print("address+dx*dy =");
//    Serial.println(address+dx*dy, DEC);
//    Serial.print("localAddress  =");
//    Serial.println(localAddress, DEC);
//    Serial.print("flag          =");
//    Serial.println(flag, DEC);
//
//#if (FAST_DIGITALWRITE==1)
//    if (flag) _pinSRAMChipSelectLSB.setLOW();
//    else      _pinSRAMChipSelectMSB.setLOW();
//#else
//    if (flag) _pinSRAMChipSelectMSB.setLOW();
//    else      _pinSRAMChipSelectLSB.setLOW();
//#endif
//
//    SPI_RAM.transfer(SRAM_READ);
//    SPI_RAM.transfer((localAddress >> 16) & 0xff);
//    SPI_RAM.transfer((localAddress >> 8) & 0xff);
//    SPI_RAM.transfer(localAddress);
//
//    _setWindow(x0, y0, x0+dx-1, y0+dy-1);
//    _setCursor(x0, y0);
//
//    _writeBeginSPI();
//    _writeSPI(SPI_START | SPI_WR | SPI_DATA);                                   // write : RS = 1, RW = 0
//
//    for (uint16_t y=0; y<dy; y++) {
//        for (uint16_t x=0; x<dx; x++) {
//            dataHigh8 = SPI_RAM.transfer(0);
//            dataLow8  = SPI_RAM.transfer(0);
//
//            if (option) {
//                _writeSPI(((dataHigh8 >> 1) & 0b11111000) | ((dataHigh8 & 0b00000111) >> 1));
//                _writeSPI(((dataLow8  >> 1) & 0b11100000) | ((dataLow8  & 0b00011111) >> 1));
//            } else {
//                _writeSPI(dataHigh8);                                              // write D8..D15
//                _writeSPI(dataLow8);                                               // write D0..D7
//            }
//        }
//    }
//    _writeEndSPI();
//    _closeWindow();
//
//#if (FAST_DIGITALWRITE==1)
//    if (flag) _pinSRAMChipSelectLSB.setHIGH();
//    else      _pinSRAMChipSelectMSB.setHIGH();
//#else
//    if (flag) _pinSRAMChipSelectMSB.setHIGH();
//    else      _pinSRAMChipSelectLSB.setHIGH();
//#endif
//
//    address = address + dx*dy;
//}

void Screen_HY28A_SRAM::copyArea(uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy, uint32_t &address)
{
    _fastCopyAreaToSRAM(x0, y0, dx, dy, address);
}

void Screen_HY28A_SRAM::pasteArea(uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy, uint32_t &address, bool option)
{
    _fastPasteAreaFromSRAM(x0, y0, dx, dy, address, option);
}

#endif                                                                          // end USE_SPI_SRAM

#endif
