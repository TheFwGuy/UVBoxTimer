//
// Screen_ER8875_SD.cpp
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
// See Screen_ER8875_SD.h and ReadMe.txt for references
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

#if defined(__LM4F120H5QR__) || defined(__TM4C1230C3PM__) || defined(__TM4C129XNCZAD__)
// Stellaris only


// Library header
#include "Screen_ER8875_SD.h"

// Choice of SD card library
#define SD_LIBRARY 1
#define FATFS_LIBRARY 2
#define FILE_LIBRARY SD_LIBRARY

#if (FILE_LIBRARY == FATFS_LIBRARY)
#include "fatfs.h"                                                              // with FatFs on SPI already available
#elif (FILE_LIBRARY == SD_LIBRARY)
#include "SD.h"
#else
#error File library not selected
#endif


///
/// @name	RA8875 size
/// @{

#define RA8875_SD_WIDTH  800
#define RA8875_SD_HEIGHT 480

/// @}

// Local variables
uint16_t RA8875_buffer[max(RA8875_SD_WIDTH, RA8875_SD_HEIGHT)];

#if (FILE_LIBRARY == SD_LIBRARY)
uint8_t *RA8875_buffer8 = reinterpret_cast<uint8_t*>(RA8875_buffer);
#endif

//// Class
//Screen_ER8875_SD::Screen_ER8875_SD() {
////    Screen_ER8875_SD(10, 9);
//_pinScreenCS = pinScreenChipSelect;
//_pinSDChipSelect = pinSDChipSelect;
//}

Screen_ER8875_SD::Screen_ER8875_SD(uint8_t pinScreenChipSelect, uint8_t pinSDChipSelect, int8_t pinSDcardDetection)
{
    _pinScreenCS = pinScreenChipSelect;
    _pinSDChipSelect = pinSDChipSelect;
    _pinSDcardDetection = pinSDcardDetection;
};

String Screen_ER8875_SD::WhoAmI()
{
    return "RA8875 + SD screen";
}

void Screen_ER8875_SD::copyArea(uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy, uint32_t &address)
{
    _fastCopyAreaToSD(x0, y0, dx, dy, address);
}

void Screen_ER8875_SD::pasteArea(uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy, uint32_t &address, bool option)
{
    _fastPasteAreaFromSD(x0, y0, dx, dy, address, option);
}

#if (FILE_LIBRARY == FATFS_LIBRARY)

void Screen_ER8875_SD::begin()
{
    pinMode(_pinSDChipSelect, OUTPUT);
    digitalWrite(_pinSDChipSelect, HIGH);
    Screen_ER8875::begin();
    _flagStorage = (FatFs.begin(_pinSDChipSelect, SPI_CLOCK_DIV4, 3) == FR_OK);
}

void Screen_ER8875_SD::_fastCopyAreaToSD(uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy, uint32_t &address)
{
    uint8_t colour;
    FRESULT result;
    size_t  bytesWritten;
    String  fileName = htoa(address, 8);
    char    filePath[20];
    
    memset(&RA8875_buffer, 0xff, sizeof(RA8875_buffer));
    fileName.toCharArray(filePath, fileName.length()+1);
    strcat (filePath,".tmp");
    result = FatFs.open(filePath, FA_CREATE_ALWAYS | FA_WRITE);
    
    if (result == FR_OK) {
        _setWindowRead(x0, y0, x0+dx-1, y0+dy-1); // only to read!
        
        for (uint16_t y=0; y<dy; y++) {
            for (uint16_t x=0; x<dx; x++) {
                RA8875_buffer[x] = _readData16();
            }
            result = FatFs.write(RA8875_buffer, dx*2, &bytesWritten);
            if (result || !bytesWritten) break;
        }
        result = FatFs.close();
    }
    
    if (result == FR_OK) address += dx*dy;
}

// Communication, fast read
void Screen_ER8875_SD::_fastPasteAreaFromSD(uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy, uint32_t &address, bool option)
{
    FRESULT result;
    size_t  bytesRead;
    String  fileName = htoa(address, 8);
    char    filePath[20];
    
    memset(&RA8875_buffer, 0xff, sizeof(RA8875_buffer));
    fileName.toCharArray(filePath, fileName.length()+1);
    strcat (filePath,".tmp");
    result = FatFs.open(filePath, FA_READ);
    
    if (result == FR_OK) {
        _setWindow(x0, y0, x0+dx-1, y0+dy-1);
        for (uint16_t y=0; y<dy; y++) {
            
            result = FatFs.read(RA8875_buffer, dx*2, &bytesRead);
            if (result || !bytesRead) break;
            
            for (uint16_t x=0; x<dx; x++) {
                if (option) _writeData16(myColours.halve(RA8875_buffer[x]));
                else        _writeData16(RA8875_buffer[x]);
            }
        }
        result = FatFs.close();
    }
    
    if (result == FR_OK) address += dx*dy;
}

#elif (FILE_LIBRARY == SD_LIBRARY)

void Screen_ER8875_SD::begin()
{
    pinMode(_pinSDChipSelect, OUTPUT);
    digitalWrite(_pinSDChipSelect, HIGH);
    _flagStorage = SD.begin(_pinSDChipSelect, SPI_CLOCK_DIV2, 3, _pinSDcardDetection, LOW);     // SPI(3)
    
    Screen_ER8875::begin();
}

void Screen_ER8875_SD::_fastCopyAreaToSD(uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy, uint32_t &address)
{
    uint8_t result;
    size_t  bytesWritten;
    String  fileName = htoa(address, 8);
    char    filePath[20];
    File    myFileW;
    
    memset(&RA8875_buffer, 0xff, sizeof(RA8875_buffer));
    fileName.toCharArray(filePath, fileName.length()+1);
    strcat(filePath, ".tmp");
    if (SD.exists(filePath)) SD.remove(filePath);
    
    myFileW = SD.open(filePath, FILE_WRITE);
    
    if (SD.getErrorCode() == 0) {
        for (uint16_t y=0; y<dy; y++) {
            _setWindowRead(x0, y0, x0+dx-1, y0+dy-1); // only to read!
            
            for (uint16_t y=0; y<dy; y++) {
                for (uint16_t x=0; x<dx; x++) {
                    RA8875_buffer[x] = _readData16();
                }
                bytesWritten = myFileW.write(RA8875_buffer8, dx*2);
                if (bytesWritten != dx*2) break;
            }
        }
        myFileW.close();
    }
    if (result > 0) address += dx*dy;
}


void Screen_ER8875_SD::_fastPasteAreaFromSD(uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy, uint32_t &address, bool option)
{
    uint8_t result;
    size_t  bytesRead;
    String  fileName = htoa(address, 8);
    char    filePath[20];
    File    myFileR;
    
    memset(&RA8875_buffer, 0xff, sizeof(RA8875_buffer));
    fileName.toCharArray(filePath, fileName.length()+1);
    strcat(filePath, ".tmp");
    myFileR = SD.open(filePath, FILE_READ);
    
    if (myFileR.available()) {
        _setWindow(x0, y0, x0+dx-1, y0+dy-1);
        for (uint16_t y=0; y<dy; y++) {
            
            bytesRead = myFileR.read(RA8875_buffer8, dx*2);
            if (bytesRead != dx*2) break;
            
            for (uint16_t x=0; x<dx; x++) {
                if (option) _writeData16(myColours.halve(RA8875_buffer[x]));
                else        _writeData16(RA8875_buffer[x]);
            }
        }
        myFileR.close();
    }
    if (result > 0) address += dx*dy;
}

#endif

#endif
