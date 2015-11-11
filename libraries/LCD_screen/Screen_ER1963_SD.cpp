//
// Screen_ER1963_SD.cpp
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
// See Screen_ER1963_SD.h and ReadMe.txt for references
//

// Done
// * initialisation
// * internal font
// * graphic primitives
// * orientation
// * read pixel
// * block functions
// * SD card
// * external font
//
// To do
// * fast RAM



// Library header
#include "Screen_ER1963_SD.h"
//#include "Debug.h"

#if (FILE_LIBRARY == FATFS_LIBRARY)
#include "fatfs.h"                                                              // with FatFs on SPI already available
#elif (FILE_LIBRARY == SD_LIBRARY)
#include "SD.h"
#else
#error File library not selected
#endif


/// moved to header and conditional defined from ER1963 header
/// @name	ER1963 size
/// @{
//#define ER1963_SD_WIDTH  800
//#define ER1963_SD_HEIGHT 480

/// @}

// Local variables
uint16_t ER1963_buffer[max(ER1963_SD_WIDTH, ER1963_SD_HEIGHT)];

#if (FILE_LIBRARY == SD_LIBRARY)
uint8_t *ER1963_buffer8 = reinterpret_cast<uint8_t*>(ER1963_buffer);
#endif

// Class
Screen_ER1963_SD::Screen_ER1963_SD(uint8_t pinScreenCS, uint8_t pinTouchCS, uint8_t pinSDcardCS, int8_t pinSDcardDetection)
{
    _pinScreenCS = pinScreenCS;
    _pinTouchCS  = pinTouchCS;
    _pinSDcardCS = pinSDcardCS;
    _pinSDcardDetection = pinSDcardDetection;
}


String Screen_ER1963_SD::WhoAmI()
{
    return "ER 5\" SSD1963 + SD screen";
}


void Screen_ER1963_SD::copyArea(uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy, uint32_t &address)
{
    _fastCopyAreaToSD(x0, y0, dx, dy, address);
}


void Screen_ER1963_SD::pasteArea(uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy, uint32_t &address, bool option)
{
    _fastPasteAreaFromSD(x0, y0, dx, dy, address, option);
}

#if (FILE_LIBRARY == FATFS_LIBRARY)

void Screen_ER1963_SD::begin()
{
    pinMode(_pinSDcardCS, OUTPUT);
    digitalWrite(_pinSDcardCS, HIGH);
    
    // begin requires SD-card already inserted
    _flagStorage = (FatFs.begin(_pinSDcardCS, SPI_CLOCK_DIV2, 3) == FR_OK);     // SPI(3)
//    debugln("*** FatFs.begin = %i (=%i?)", FatFs.begin(_pinSDcardCS, SPI_CLOCK_DIV2, 3), FR_OK);
    Screen_ER1963::begin();
}

bool Screen_ER1963_SD::detectSD()
{
    if (_flagStorage == 0) {                                                    // SD not yet initialised
        _flagStorage = (FatFs.begin(_pinSDcardCS, SPI_CLOCK_DIV2, 3) == FR_OK); // SPI(3)
        return _flagStorage;
    } else {
//        return FatFs.detectSD();                                                // hardaware detection
        return true;
    }
}


void Screen_ER1963_SD::_fastCopyAreaToSD(uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy, uint32_t &address)
{
    FRESULT result;
    size_t  bytesWritten;
    String  fileName = htoa(address, 8);
    char    filePath[20];
    
    
    fileName.toCharArray(filePath, fileName.length()+1);
    strcat(filePath, ".tmp");
    result = FatFs.open(filePath, FA_CREATE_ALWAYS | FA_WRITE);
    
    if (result == FR_OK) {
        
        for (uint16_t y=0; y<dy; y++) {
            _setWindowRead(x0, y0+y, x0+dx, y0+y);
            
            for (uint16_t x=0; x<dx; x++) {
                ER1963_buffer[x] = _readData16();
            }
            
            result = FatFs.write(ER1963_buffer, dx*2, &bytesWritten);
            if (result || !bytesWritten) break;
            
            _setWindow(x0, y0+y+36, x0+dx, y0+y+36);
        }
        result = FatFs.close();
    }
    
    if (result == FR_OK) address = address + dx*dy;
}

void Screen_ER1963_SD::_fastPasteAreaFromSD(uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy, uint32_t &address, bool option)
{
    FRESULT result;
    size_t  bytesRead;
    String  fileName = htoa(address, 8);
    char    filePath[20];
    
    fileName.toCharArray(filePath, fileName.length()+1);
    strcat(filePath, ".tmp");
    result = FatFs.open(filePath, FA_READ);
    
    if (result == FR_OK) {
        for (uint16_t y=0; y<dy; y++) {
            _setWindow(x0, y0+y, x0+dx, y0+y);
            
            result = FatFs.read(ER1963_buffer, dx*2, &bytesRead);
            if (result || !bytesRead) break;
            
            for (uint16_t x=0; x<dx; x++) {
                
                if (option) _writeData16(myColours.halve(ER1963_buffer[x]));
                else        _writeData16(ER1963_buffer[x]);
            }
        }
        result = FatFs.close();
    }
    
    if (result == FR_OK) address = address + dx*dy;
    
}

#elif (FILE_LIBRARY == SD_LIBRARY)

void Screen_ER1963_SD::begin()
{
    pinMode(_pinSDcardCS, OUTPUT);
    digitalWrite(_pinSDcardCS, HIGH);
    
    // begin requires SD-card available RR debug it was stuck @28
    _flagStorage = SD.begin(_pinSDcardCS, SPI_CLOCK_DIV2, 3, _pinSDcardDetection, LOW);     // SPI(3)
    //    debugln("*** SD.begin = %i (=%i?)", SD.begin(28, SPI_CLOCK_DIV2, 3, _pinSDcardDetection, LOW), true);
    Screen_ER1963::begin();
}


void Screen_ER1963_SD::_fastCopyAreaToSD(uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy, uint32_t &address)
{
    uint8_t result;
    size_t  bytesWritten;
    String  fileName = htoa(address, 8);
    char    filePath[20];
    File    myFileW;
    
    
    fileName.toCharArray(filePath, fileName.length()+1);
    strcat(filePath, ".tmp");
    if (SD.exists(filePath)) SD.remove(filePath);
    
    myFileW = SD.open(filePath, FILE_WRITE);
    
    if (SD.getErrorCode() == 0) {
        for (uint16_t y=0; y<dy; y++) {
            _setWindowRead(x0, y0+y, x0+dx, y0+y);
            
            for (uint16_t x=0; x<dx; x++) {
                ER1963_buffer[x] = _readData16();
            }
            
            bytesWritten = myFileW.write(ER1963_buffer8, dx*2);
            if (bytesWritten != dx*2) break;
            
            _setWindow(x0, y0+y+36, x0+dx, y0+y+36);
        }
        myFileW.close();
    }
    if (result > 0) address = address + dx*dy;
}


void Screen_ER1963_SD::_fastPasteAreaFromSD(uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy, uint32_t &address, bool option)
{
    uint8_t result;
    size_t  bytesRead;
    String  fileName = htoa(address, 8);
    char    filePath[20];
    File    myFileR;

    
    fileName.toCharArray(filePath, fileName.length()+1);
    strcat(filePath, ".tmp");
    myFileR = SD.open(filePath, FILE_READ);
    
    if (myFileR.available()) {
        for (uint16_t y=0; y<dy; y++) {
            _setWindow(x0, y0+y, x0+dx, y0+y);
            
            bytesRead = myFileR.read(ER1963_buffer8, dx*2);
            if (bytesRead != dx*2) break;
            
            for (uint16_t x=0; x<dx; x++) {
                if (option) _writeData16(myColours.halve(ER1963_buffer[x]));
                else        _writeData16(ER1963_buffer[x]);
            }
        }
        myFileR.close();
    }
    if (result > 0) address += dx*dy;
}

#endif
