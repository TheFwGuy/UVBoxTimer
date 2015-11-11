//
// Screen_ER8875.cpp
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
// See Screen_ER8875.h and ReadMe.txt for references
//

// Done
// * initialisation
// * internal font
// * graphic primitives
// * orientation
// * read pixel
// * block functions
// * external font
// * SD card
//
// To do
// * fast RAM

// Library header
#include "Screen_ER8875.h"

//#if defined(__LM4F120H5QR__) || defined(__TM4C1230C3PM__) || defined(__TM4C129XNCZAD__)                                                   // Stellaris only

SPIClass SPI_LCD;


// SPI screen select management
#if (SPI_MODE == FAST_SPI)

#define CLEAR_SCREEN_LOW cbi(_portScreenCS, _bitScreenCS)
#define SET_SCREEN_HIGH  sbi(_portScreenCS, _bitScreenCS)

#else

#define CLEAR_SCREEN_LOW digitalWrite(_pinScreenCS, LOW)
#define SET_SCREEN_HIGH  digitalWrite(_pinScreenCS, HIGH)

#endif

///
/// @name	Commands for RA8875
/// @{

// Command/Data pins for SPI
#define RA8875_DATA_WRITE        0x00
#define RA8875_DATA_READ         0x40
#define RA8875_COMMAND_WRITE     0x80
#define RA8875_STATUS_READ       0xc0

// Registers and bits
#define RA8875_VERSION          0x00
#define RA8875_PWRR             0x01
#define RA8875_PWRR_DISPON      0x80
#define RA8875_PWRR_DISPOFF     0x00
#define RA8875_PWRR_SLEEP       0x02
#define RA8875_PWRR_NORMAL      0x00
#define RA8875_PWRR_SOFTRESET   0x01

#define RA8875_MRWC             0x02

#define RA8875_GPIOX            0xC7

#define RA8875_PLLC1            0x88
#define RA8875_PLLC1_PLLDIV2    0x80
#define RA8875_PLLC1_PLLDIV1    0x00

#define RA8875_PLLC2            0x89
#define RA8875_PLLC2_DIV1       0x00
#define RA8875_PLLC2_DIV2       0x01
#define RA8875_PLLC2_DIV4       0x02
#define RA8875_PLLC2_DIV8       0x03
#define RA8875_PLLC2_DIV16      0x04
#define RA8875_PLLC2_DIV32      0x05
#define RA8875_PLLC2_DIV64      0x06
#define RA8875_PLLC2_DIV128     0x07

#define RA8875_SYSR             0x10
#define RA8875_SYSR_8BPP        0x00
#define RA8875_SYSR_16BPP       0x0C
#define RA8875_SYSR_MCU8        0x00
#define RA8875_SYSR_MCU16       0x03

#define RA8875_PCSR             0x04
#define RA8875_PCSR_PDATR       0x00
#define RA8875_PCSR_PDATL       0x80
#define RA8875_PCSR_CLK         0x00
#define RA8875_PCSR_2CLK        0x01
#define RA8875_PCSR_4CLK        0x02
#define RA8875_PCSR_8CLK        0x03

#define RA8875_HDWR             0x14

#define RA8875_HNDFTR           0x15
#define RA8875_HNDFTR_DE_HIGH   0x00
#define RA8875_HNDFTR_DE_LOW    0x80

#define RA8875_HNDR             0x16
#define RA8875_HSTR             0x17
#define RA8875_HPWR             0x18
#define RA8875_HPWR_LOW         0x00
#define RA8875_HPWR_HIGH        0x80

#define RA8875_VDHR0            0x19
#define RA8875_VDHR1            0x1A
#define RA8875_VNDR0            0x1B
#define RA8875_VNDR1            0x1C
#define RA8875_VSTR0            0x1D
#define RA8875_VSTR1            0x1E
#define RA8875_VPWR             0x1F
#define RA8875_VPWR_LOW         0x00
#define RA8875_VPWR_HIGH        0x80

// Text
#define RA8875_DPCR             0x20
#define RA8875_FNCR0            0x21
#define RA8875_FNCR0_CGROM_8859_1   0x00
#define RA8875_FNCR0_CGROM_8859_2   0x01
#define RA8875_FNCR0_CGROM_8859_3   0x02
#define RA8875_FNCR0_CGROM_8859_4   0x03
#define RA8875_FNCR1            0x22
#define RA8875_FNCR1_BACKGROUND     0x00
#define RA8875_FNCR1_TRANSPARENT    0x40
#define RA8875_FNCR1_ROTATED    0x10
#define RA8875_CGSR             0x23
#define RA8875_HOFS0            0x24
#define RA8875_HOFS1            0x25
#define RA8875_VOFS0            0x26
#define RA8875_VOFS1            0x27
#define RA8875_FLDR             0x29
#define RA8875_F_CURXL          0x2a
#define RA8875_F_CURXH          0x2b
#define RA8875_F_CURYL          0x2c
#define RA8875_F_CURYH          0x2d
#define RA8875_F_SET            0x2e

#define RA8875_HSAW0            0x30
#define RA8875_HSAW1            0x31
#define RA8875_VSAW0            0x32
#define RA8875_VSAW1            0x33
#define RA8875_HEAW0            0x34
#define RA8875_HEAW1            0x35
#define RA8875_VEAW0            0x36
#define RA8875_VEAW1            0x37

#define RA8875_CURHS            0x4e
#define RA8875_CURVS            0x4f

#define RA8875_GCHP0            0x80
#define RA8875_GCHP1            0x81
#define RA8875_GCVP0            0x82
#define RA8875_GCVP1            0x83
#define RA8875_GCC0             0x84
#define RA8875_GCC1             0x85

#define RA8875_MCLR             0x8e
#define RA8875_MCLR_START       0x80
#define RA8875_MCLR_STOP        0x00
#define RA8875_MCLR_READSTATUS  0x80
#define RA8875_MCLR_FULL        0x00
#define RA8875_MCLR_ACTIVE      0x40

#define RA8875_DCR              0x90
#define RA8875_DLHSR0           0x91
#define RA8875_DLHSR1           0x92
#define RA8875_DLVSR0           0x93
#define RA8875_DLVSR1           0x94
#define RA8875_DLHER0           0x95
#define RA8875_DLHER1           0x96
#define RA8875_DLVER0           0x97
#define RA8875_DLVER1           0x98
#define RA8875_DCHR0            0x99
#define RA8875_DCHR1            0x9a
#define RA8875_DCVR0            0x9b
#define RA8875_DCVR1            0x9c
#define RA8875_DCRR             0x9d
#define RA8875_DSCR             0x9e
#define RA8875_DTPH0            0xa9
#define RA8875_DTPH1            0xaa
#define RA8875_DTPV0            0xab
#define RA8875_DTPV1            0xac

// Constants for DCR
#define RA8875_DCR_LINESQUTRI_START   0x80
#define RA8875_DCR_LINESQUTRI_STOP    0x00
#define RA8875_DCR_LINESQUTRI_STATUS  0x80
#define RA8875_DCR_CIRCLE_START       0x40
#define RA8875_DCR_CIRCLE_STATUS      0x40
#define RA8875_DCR_CIRCLE_STOP        0x00

#define RA8875_DCR_FILL               0x20
#define RA8875_DCR_NOFILL             0x00

#define RA8875_DCR_DRAWLINE           0x00
#define RA8875_DCR_DRAWTRIANGLE       0x01
#define RA8875_DCR_DRAWSQUARE         0x10

#define RA8875_ELLIPSE                0xa0
#define RA8875_ELLIPSE_STATUS         0x80

#define RA8875_MWCR0            0x40
#define RA8875_MWCR1            0x41
#define RA8875_MWCR0_GFXMODE    0x00
#define RA8875_MWCR0_TXTMODE    0x80

#define RA8875_CURH0            0x46
#define RA8875_CURH1            0x47
#define RA8875_CURV0            0x48
#define RA8875_CURV1            0x49
#define RA8875_RCURH0           0x4a
#define RA8875_RCURH1           0x4b
#define RA8875_RCURV0           0x4c
#define RA8875_RCURV1           0x4d

#define RA8875_P1CR             0x8A
#define RA8875_P1CR_ENABLE      0x80
#define RA8875_P1CR_DISABLE     0x00
#define RA8875_P1CR_CLKOUT      0x10
#define RA8875_P1CR_PWMOUT      0x00

#define RA8875_P1DCR            0x8B

#define RA8875_P2CR             0x8C
#define RA8875_P2CR_ENABLE      0x80
#define RA8875_P2CR_DISABLE     0x00
#define RA8875_P2CR_CLKOUT      0x10
#define RA8875_P2CR_PWMOUT      0x00

#define RA8875_P2DCR            0x8D

#define RA8875_PWM_CLK_DIV1     0x00
#define RA8875_PWM_CLK_DIV2     0x01
#define RA8875_PWM_CLK_DIV4     0x02
#define RA8875_PWM_CLK_DIV8     0x03
#define RA8875_PWM_CLK_DIV16    0x04
#define RA8875_PWM_CLK_DIV32    0x05
#define RA8875_PWM_CLK_DIV64    0x06
#define RA8875_PWM_CLK_DIV128   0x07
#define RA8875_PWM_CLK_DIV256   0x08
#define RA8875_PWM_CLK_DIV512   0x09
#define RA8875_PWM_CLK_DIV1024  0x0A
#define RA8875_PWM_CLK_DIV2048  0x0B
#define RA8875_PWM_CLK_DIV4096  0x0C
#define RA8875_PWM_CLK_DIV8192  0x0D
#define RA8875_PWM_CLK_DIV16384 0x0E
#define RA8875_PWM_CLK_DIV32768 0x0F

#define RA8875_BECR0_BTE            0x50
#define RA8875_BECR1_BTE            0x51
#define RA8875_LTPR0_BTE            0x52
#define RA8875_LTPR1_BTE            0x53
#define RA8875_HSBE0_BTE            0x54
#define RA8875_HSBE1_BTE            0x55
#define RA8875_VSBE0_BTE            0x56
#define RA8875_VSBE1_BTE            0x57
#define RA8875_HDBE0_BTE            0x58
#define RA8875_HDBE1_BTE            0x59
#define RA8875_VDBE0_BTE            0x5a
#define RA8875_VDBE1_BTE            0x5b
#define RA8875_BEWR0_BTE            0x5c
#define RA8875_BEWR1_BTE            0x5d
#define RA8875_BEHR0_BTE            0x5e
#define RA8875_BEHR1_BTE            0x5f

#define RA8875_BGCR_BTE_RED         0x60
#define RA8875_BGCR_BTE_GREEN       0x61
#define RA8875_BGCR_BTE_BLUE        0x62
#define RA8875_FGCR_BTE_RED         0x63
#define RA8875_FGCR_BTE_GREEN       0x64
#define RA8875_FGCR_BTE_BLUE        0x65

#define RA8875_TPCR0                  0x70
#define RA8875_TPCR0_ENABLE           0x80
#define RA8875_TPCR0_DISABLE          0x00
#define RA8875_TPCR0_WAIT_512CLK      0x00
#define RA8875_TPCR0_WAIT_1024CLK     0x10
#define RA8875_TPCR0_WAIT_2048CLK     0x20
#define RA8875_TPCR0_WAIT_4096CLK     0x30
#define RA8875_TPCR0_WAIT_8192CLK     0x40
#define RA8875_TPCR0_WAIT_16384CLK    0x50
#define RA8875_TPCR0_WAIT_32768CLK    0x60
#define RA8875_TPCR0_WAIT_65536CLK    0x70
#define RA8875_TPCR0_WAKEENABLE       0x08
#define RA8875_TPCR0_WAKEDISABLE      0x00
#define RA8875_TPCR0_ADCCLK_DIV1      0x00
#define RA8875_TPCR0_ADCCLK_DIV2      0x01
#define RA8875_TPCR0_ADCCLK_DIV4      0x02
#define RA8875_TPCR0_ADCCLK_DIV8      0x03
#define RA8875_TPCR0_ADCCLK_DIV16     0x04
#define RA8875_TPCR0_ADCCLK_DIV32     0x05
#define RA8875_TPCR0_ADCCLK_DIV64     0x06
#define RA8875_TPCR0_ADCCLK_DIV128    0x07

#define RA8875_TPCR1            0x71
#define RA8875_TPCR1_AUTO       0x00
#define RA8875_TPCR1_MANUAL     0x40
#define RA8875_TPCR1_VREFINT    0x00
#define RA8875_TPCR1_VREFEXT    0x20
#define RA8875_TPCR1_DEBOUNCE   0x04
#define RA8875_TPCR1_NODEBOUNCE 0x00
#define RA8875_TPCR1_IDLE       0x00
#define RA8875_TPCR1_WAIT       0x01
#define RA8875_TPCR1_LATCHX     0x02
#define RA8875_TPCR1_LATCHY     0x03

#define RA8875_TPXH             0x72
#define RA8875_TPYH             0x73
#define RA8875_TPXYL            0x74
#define RA8875_TPXYL_ADET       0x80



#define RA8875_DMA_SSAR0        0xb0
#define RA8875_DMA_SSAR1        0xb1
#define RA8875_DMA_SSAR2        0xb2
#define RA8875_DMA_BWR0         0xb4
#define RA8875_DMA_BWR1         0xb5
#define RA8875_DMA_BHR0         0xb6
#define RA8875_DMA_BHR1         0xb7
#define RA8875_DMA_SPWR0        0xb8
#define RA8875_DMA_SPWR1        0xb9
#define RA8875_DMA_DMACR        0xbf

// Serial flash
#define RA8875_SACS_MODE        0xe0
#define RA8875_SACS_ADDR        0xe1
#define RA8875_SACS_DATA        0xb2
#define RA8875_SROC             0x05
#define RA8875_SFCLR            0x06

#define RA8875_INTC1            0xF0
#define RA8875_INTC1_KEY        0x10
#define RA8875_INTC1_DMA        0x08
#define RA8875_INTC1_TP         0x04
#define RA8875_INTC1_BTE        0x02

#define RA8875_INTC2            0xf1
#define RA8875_INTC2_KEY        0x10
#define RA8875_INTC2_DMA        0x08
#define RA8875_INTC2_TP         0x04
#define RA8875_INTC2_BTE        0x02

/// @}


///
/// @name	RA8875 size
/// @{

#define RA8875_WIDTH  800
#define RA8875_HEIGHT 480

/// @}

inline uint16_t absDiff(uint16_t a, uint16_t b) { return (a > b) ? a-b : b-a; }

// Class
//Screen_ER8875::Screen_ER8875() {
//#if defined(ENERGIA)
//    _pinScreenCS     = 10; // select
//#elif defined(__AVR_ATmega328P__) || defined(__AVR_ATmega2560__) || defined(__AVR_ATmega644P__)
//    _pinScreenCS     = NULL; // select
//#elif defined(__MK20DX128__)
//    _pinScreenCS     = NULL; // select
//
//#else
//#error Platform not supported
//#endif
//
//}

Screen_ER8875::Screen_ER8875(uint8_t pinScreenCS)
{
    _pinScreenCS = pinScreenCS; // select
};


void Screen_ER8875::begin()
{
    // Interface initialisation
#if (SPI_MODE == FAST_SPI)
    _portScreenCS   = portOutputRegister(digitalPinToPort(_pinScreenCS));
    _bitScreenCS    = digitalPinToBitMask(_pinScreenCS);
#endif
    
#if defined(__LM4F120H5QR__) || defined(__TM4C1230C3PM__) || defined(__TM4C129XNCZAD__)
//    SPI_LCD.setModule(2);
    SPI_LCD.setModule(5);
#endif
    
    SPI_LCD.begin();
    SPI_LCD.setClockDivider(SPI_CLOCK_DIV128);
    SPI_LCD.setBitOrder(MSBFIRST);
    SPI_LCD.setDataMode(SPI_MODE0);
    
    pinMode(_pinScreenCS, OUTPUT);
    SET_SCREEN_HIGH;
    
    //    Serial.print(_readRegister8(0), HEX);
    
    // Screen reset
    delay(200);
    
    // PLL initialisation
    _writeRegister8(RA8875_PLLC1, 0x88); // RA8875_PLLC1_PLLDIV1 + 10);
    delay(1);
    _writeRegister8(RA8875_PLLC2, 0x02); // RA8875_PLLC2_DIV4);
    delay(1);
    
    // Screen initialisation
    _writeRegister8(RA8875_SYSR, RA8875_SYSR_16BPP | RA8875_SYSR_MCU8);
    
    // Timing values
    uint8_t hsync_start = 32;
    uint8_t hsync_pw = 96;
    uint8_t hsync_finetune = 0;
    uint8_t hsync_nondisp = 26;
    uint8_t vsync_pw = 2;
    uint16_t vsync_nondisp = 32;
    uint16_t vsync_start = 23;
    
    // Set the correct values for the display being used
    _writeRegister8(RA8875_PCSR, RA8875_PCSR_PDATL | RA8875_PCSR_2CLK);
    delay(1);
    
    // Horizontal settings registers
    _writeRegister8(RA8875_HDWR, (RA8875_WIDTH / 8) - 1);                       // H width: (HDWR + 1) * 8 = 480
    _writeRegister8(RA8875_HNDFTR, RA8875_HNDFTR_DE_HIGH + hsync_finetune);
    _writeRegister8(RA8875_HNDR, (hsync_nondisp - hsync_finetune - 2)/8);       // H non-display: HNDR * 8 + HNDFTR + 2 = 10
    _writeRegister8(RA8875_HSTR, hsync_start/8 - 1);                            // Hsync start: (HSTR + 1)*8
    _writeRegister8(RA8875_HPWR, RA8875_HPWR_LOW + (hsync_pw/8 - 1));           // HSync pulse width = (HPWR+1) * 8
    
    // Vertical settings registers
    _writeRegister16(RA8875_VDHR0, (uint16_t)(RA8875_HEIGHT - 1));
    _writeRegister16(RA8875_VNDR0, vsync_nondisp-1);                            // V non-display period = VNDR + 1
    _writeRegister16(RA8875_VSTR0, vsync_start-1);                              // Vsync start position = VSTR + 1
    _writeRegister8(RA8875_VPWR, RA8875_VPWR_LOW + vsync_pw - 1);               // Vsync pulse width = VPWR + 1
    
    _writeRegister16(RA8875_HSAW0, 0);                                          // horizontal start point
    _writeRegister16(RA8875_HEAW0, (uint16_t)(RA8875_WIDTH - 1));               // horizontal end point
    _writeRegister16(RA8875_VSAW0, 0);                                          // vertical start point
    _writeRegister16(RA8875_VEAW0, (uint16_t)(RA8875_HEIGHT - 1));              // horizontal end point
    
    // Clear
    _setBackColour(myColours.black);
    _writeRegister8(RA8875_MCLR, RA8875_MCLR_START | RA8875_MCLR_FULL);
    delay(500);
    
    _writeRegister8(RA8875_PWRR, RA8875_PWRR_DISPON);
    _writeRegister8(RA8875_TPCR0, RA8875_TPCR0_ENABLE);
    _writeRegister8(RA8875_TPCR1, RA8875_TPCR1_AUTO);
    
    // Standard
    _writeRegister8(RA8875_GPIOX, 1);
    _writeRegister8(RA8875_P1CR, RA8875_P1CR_ENABLE | RA8875_PWM_CLK_DIV1024);
    _writeRegister8(RA8875_P1DCR, 0xff);
    
    clear();
    setOrientation(0);
    
#if defined(__LM4F120H5QR__) || defined(__TM4C1230C3PM__) || defined(__TM4C129XNCZAD__)
    SPI_LCD.setClockDivider(SPI_CLOCK_DIV2);
#else
    SPI_LCD.setClockDivider(SPI_CLOCK_DIV4);
#endif
    
    _touchXmin = 483;
    _touchXmax = 38;
    _touchYmin = 449;
    _touchYmax = 41;
    
    _screenWidth = RA8875_WIDTH;
    _screenHeigth = RA8875_HEIGHT;
    _screenDiagonal = 50;
    
    _penSolid = false;
    _fontSolid = false;
    _touchTrim = 0x10; // touch
    _fsmArea = 0x01;
    
    // Touch
    _writeRegister8(RA8875_TPCR0, RA8875_TPCR0_ENABLE | RA8875_TPCR0_WAIT_4096CLK  |
                    RA8875_TPCR0_WAKEDISABLE | RA8875_TPCR0_ADCCLK_DIV64);      // 10mhz max!
    _writeRegister8(RA8875_TPCR1, RA8875_TPCR1_DEBOUNCE | RA8875_TPCR1_DEBOUNCE);
    _writeRegister8(RA8875_INTC1, _readRegister8(RA8875_INTC1) | RA8875_INTC1_TP); // enable
    _writeRegister8(RA8875_INTC2, _readRegister8(RA8875_INTC2) | RA8875_INTC2_TP); // rearm
}

String Screen_ER8875::WhoAmI()
{
#if (SPI_MODE == FAST_SPI)
    return "ER 5\" RA8875 (fast)";
#else
    return "ER 5\" RA8875 screen";
#endif
}

void Screen_ER8875::clear(uint16_t colour)
{
    //    if (colour == myColours.black) {
    //        _setDrawingCoordinates(0, 0, screenSizeX()-1, screenSizeY()-1);
    //        _writeRegister8(RA8875_MCLR, RA8875_MCLR_START | RA8875_MCLR_FULL);
    //    } else {
    _fastFill(0, 0, screenSizeX()-1, screenSizeY()-1, colour);
    //    }
}


void Screen_ER8875::_orientCoordinates(uint16_t &x1, uint16_t &y1)
{
    switch (_orientation) {
        case 0:
            break;
        case 1:
            _swap(x1, y1);
            break;
        case 2:
            break;
        case 3:
            _swap(x1, y1);
            break;
    }
}


void Screen_ER8875::_setOrientation(uint8_t orientation)
{
    switch (_orientation) {
        case 0:
            //                                 hv
            _writeRegister8(RA8875_DPCR, 0b00000000);
            break;
        case 1:
            //                                 hv
            _writeRegister8(RA8875_DPCR, 0b00001000);
            break;
        case 2:
            //                                 hv
            _writeRegister8(RA8875_DPCR, 0b00001100);
            break;
        case 3:
            //                                 hv
            _writeRegister8(RA8875_DPCR, 0b00000100);
            break;
    }
}


uint8_t Screen_ER8875::_readData8()
{
    CLEAR_SCREEN_LOW;
    
    SPI_LCD.transfer(RA8875_DATA_READ);

#if defined(__TM4C129XNCZAD__)
    delay(2);
#endif

    uint8_t data = SPI_LCD.transfer(0x00);
    
    SET_SCREEN_HIGH;
    
    return data;
}

void Screen_ER8875::_readData88(uint8_t &dataHigh8, uint8_t &dataLow8)
{
    CLEAR_SCREEN_LOW;
    
    SPI_LCD.transfer(RA8875_DATA_READ);
    dataHigh8 = SPI_LCD.transfer(0x00);
    dataLow8  = SPI_LCD.transfer(0x00);
    
    SET_SCREEN_HIGH;
}


uint16_t Screen_ER8875::_readData16()
{
    uint16_t data;
    
    CLEAR_SCREEN_LOW;
    
    SPI_LCD.transfer(RA8875_DATA_READ);
    data   = SPI_LCD.transfer(0x00);
    data <<= 8;
    data  += SPI_LCD.transfer(0x00);
    
    SET_SCREEN_HIGH;
    
    data <<= 1;                                                                 // seems to be necessary
    return data;
}


uint8_t Screen_ER8875::_readRegister8(uint8_t register8)
{
    _writeCommand8(register8);

#if defined(__TM4C129XNCZAD__)
    delay(2);
#endif

    return (uint8_t)_readData8();
}


void Screen_ER8875::_fastFill(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t colour)
{
    _setDrawingCoordinates(x1, y1, x2, y2);
    _setFrontColour(colour);
    _setBackColour(colour);
    
    _writeRegister8(RA8875_DCR, RA8875_DCR_LINESQUTRI_START | RA8875_DCR_FILL | RA8875_DCR_DRAWSQUARE);

    while (_readRegister8(RA8875_DCR) & RA8875_DCR_LINESQUTRI_STATUS);
}


// Utilities
void Screen_ER8875::_setDrawingCoordinates(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
    _orientCoordinates(x0, y0);
    _orientCoordinates(x1, y1);
    _writeRegister16(RA8875_DLHSR0, x0);
    _writeRegister16(RA8875_DLVSR0, y0);
    _writeRegister16(RA8875_DLHER0, x1);
    _writeRegister16(RA8875_DLVER0, y1);
}


void Screen_ER8875::_setPoint(uint16_t x0, uint16_t y0, uint16_t colour)
{
    _orientCoordinates(x0, y0);
    _writeRegister16(RA8875_CURH0, x0);
    _writeRegister16(RA8875_CURV0, y0);
    _writeCommand8(RA8875_MRWC);
    _writeData16(colour);
}

void Screen_ER8875::_writeRegister8(uint8_t register8, uint8_t data8)
{
    CLEAR_SCREEN_LOW;
    
    SPI_LCD.transfer(RA8875_COMMAND_WRITE);
    SPI_LCD.transfer(register8);
    
    //    SET_SCREEN_HIGH;
    //    CLEAR_SCREEN_LOW;
    
    SPI_LCD.transfer(RA8875_DATA_WRITE);
    SPI_LCD.transfer(data8);
    
    SET_SCREEN_HIGH;
    
    delay(1);                                                                   // required
}

void Screen_ER8875::_writeRegister16(uint8_t register8, uint16_t data16)
{
    _writeRegister8(register8,    lowByte(data16));
    _writeRegister8(register8 +1, highByte(data16));
}


void Screen_ER8875::_writeCommand8(uint8_t command8)
{
    CLEAR_SCREEN_LOW;
    
    SPI_LCD.transfer(RA8875_COMMAND_WRITE);
    SPI_LCD.transfer(command8);
    
    SET_SCREEN_HIGH;
}


void Screen_ER8875::_writeData8(uint8_t data8)
{
    CLEAR_SCREEN_LOW;
    
    SPI_LCD.transfer(RA8875_DATA_WRITE);
    SPI_LCD.transfer(data8);
    
    SET_SCREEN_HIGH;
}


void Screen_ER8875::_writeData88(uint8_t dataHigh8, uint8_t dataLow8)
{
    CLEAR_SCREEN_LOW;
    
    SPI_LCD.transfer(RA8875_DATA_WRITE);
    SPI_LCD.transfer(dataHigh8);
    SPI_LCD.transfer(dataLow8);
    
    SET_SCREEN_HIGH;
}


void Screen_ER8875::_writeData16(uint16_t data16)
{
    CLEAR_SCREEN_LOW;
    
    SPI_LCD.transfer(RA8875_DATA_WRITE);
    SPI_LCD.transfer(highByte(data16));
    SPI_LCD.transfer(lowByte(data16));
    
    SET_SCREEN_HIGH;
}


void Screen_ER8875::_getRawTouch(uint16_t &x0, uint16_t &y0, uint16_t &z0)
{
    SPI_LCD.setClockDivider(SPI_CLOCK_DIV4);

    uint16_t x00, y00;
    
    
    /// @bug    0b00000010 for LM4F120 but 0b00000100 for TM4C129
    /// Seems to be linked to dummy read and delay
    ///
    if (_readRegister8(RA8875_INTC2) & 0b00000010) {
        
        do {
            Serial.print("2");
            _writeRegister8(RA8875_INTC2, _readRegister8(RA8875_INTC2) | RA8875_INTC2_TP); // rearm
            delay(1);
            Serial.print("3");
            x0 = _readRegister8(RA8875_TPXH);                                       // x: bits 9..2
            delay(1);
            Serial.print("4");
            y0 = _readRegister8(RA8875_TPYH);                                       // y: bits 9..2
            delay(1);
            
            _writeRegister8(RA8875_INTC2, _readRegister8(RA8875_INTC2) | RA8875_INTC2_TP); // rearm
            delay(1);
            x00 = _readRegister8(RA8875_TPXH);                                       // x: bits 9..2
            delay(1);
            y00 = _readRegister8(RA8875_TPYH);                                       // y: bits 9..2
            delay(1);

        } while (absDiff(x0, x00) + absDiff(y0, y00) > 0);
        
        x0 &= 0b01111111;
        x0 <<= 2;
        y0 &= 0b01111111;
        y0 <<= 2;
        
        z0 = _readRegister8(RA8875_TPXYL);
        x0 |= z0 & 0b00000011;                                                  // x: bits 1..0
        y0 |= (z0 >> 2) & 0b00000011;                                           // y: bits 1..0
        
        z0 = 0xff;
        
    } else {
        x0 = 0;
        y0 = 0;
        z0 = 0;
    }
    _writeRegister8(RA8875_INTC2, _readRegister8(RA8875_INTC2) | RA8875_INTC2_TP); // rearm
    
#if defined(__LM4F120H5QR__) || defined(__TM4C1230C3PM__) || defined(__TM4C129XNCZAD__)
    SPI_LCD.setClockDivider(SPI_CLOCK_DIV2);
#else
    SPI_LCD.setClockDivider(SPI_CLOCK_DIV4);
#endif
}


void Screen_ER8875::_setFrontColour(uint16_t colour)
{
    _writeRegister8(RA8875_FGCR_BTE_RED,   (colour & 0xf800) >> 11);
    _writeRegister8(RA8875_FGCR_BTE_GREEN, (colour & 0x07e0) >> 5);
    _writeRegister8(RA8875_FGCR_BTE_BLUE,  (colour & 0x001f));
}


void Screen_ER8875::_setBackColour(uint16_t colour)
{
    _writeRegister8(RA8875_BGCR_BTE_RED,   (colour & 0xf800) >> 11);
    _writeRegister8(RA8875_BGCR_BTE_GREEN, (colour & 0x07e0) >> 5);
    _writeRegister8(RA8875_BGCR_BTE_BLUE,  (colour & 0x001f));
}



void Screen_ER8875::line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t colour)
{
    _setDrawingCoordinates(x1, y1, x2, y2);
    _setFrontColour(colour);
    
    _writeRegister8(RA8875_DCR, RA8875_DCR_LINESQUTRI_START | RA8875_DCR_DRAWLINE);
    
    while (_readRegister8(RA8875_DCR) & RA8875_DCR_LINESQUTRI_STATUS);
}


void Screen_ER8875::rectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t colour)
{
    _setDrawingCoordinates(x1, y1, x2, y2);
    _setFrontColour(colour);
    
    if (_penSolid) {
        _writeRegister8(RA8875_DCR, RA8875_DCR_LINESQUTRI_START | RA8875_DCR_FILL | RA8875_DCR_DRAWSQUARE);
    } else {
        _writeRegister8(RA8875_DCR, RA8875_DCR_LINESQUTRI_START | RA8875_DCR_NOFILL | RA8875_DCR_DRAWSQUARE);
    }
    
    while (_readRegister8(RA8875_DCR) & RA8875_DCR_LINESQUTRI_STATUS);
}


void Screen_ER8875::circle(uint16_t x0, uint16_t y0, uint16_t radius, uint16_t colour)
{
    _writeRegister16(RA8875_DCHR0, x0);
    _writeRegister16(RA8875_DCVR0, y0);
    _writeRegister8(RA8875_DCRR, (uint8_t)radius);
    _setFrontColour(colour);
    
    if (_penSolid) {
        _writeRegister8(RA8875_DCR, RA8875_DCR_CIRCLE_START | RA8875_DCR_FILL);
    } else {
        _writeRegister8(RA8875_DCR, RA8875_DCR_CIRCLE_START | RA8875_DCR_NOFILL);
    }
    
    while (_readRegister8(RA8875_DCR) & 0b100000);
}


void Screen_ER8875::triangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, uint16_t colour)
{
    _setDrawingCoordinates(x1, y1, x2, y2);
    _writeRegister16(RA8875_DTPH0, x3);
    _writeRegister16(RA8875_DTPV0, y3);
    _setFrontColour(colour);
    
    if (_penSolid) {
        _writeRegister8(RA8875_DCR, RA8875_DCR_LINESQUTRI_START | RA8875_DCR_FILL | RA8875_DCR_DRAWTRIANGLE);
    } else {
        _writeRegister8(RA8875_DCR, RA8875_DCR_LINESQUTRI_START | RA8875_DCR_NOFILL | RA8875_DCR_DRAWTRIANGLE);
    }
    
    while (_readRegister8(RA8875_DCR) & RA8875_DCR_LINESQUTRI_STATUS);
}

#if (BUILT_IN_FONTS == 1)
void Screen_ER8875::gText(uint16_t x0, uint16_t y0,
                          String s,
                          uint16_t textColour, uint16_t backColour,
                          uint8_t ix, uint8_t iy)
{
    uint8_t c;
    uint8_t line, line1, line2, line3;
    uint16_t x, y;
    uint8_t i, j, k;
    
    _orientCoordinates(x0, y0);
    
    _writeRegister8(RA8875_MWCR0, _readRegister8(RA8875_MWCR0) | RA8875_MWCR0_TXTMODE);
    _writeRegister8(RA8875_MWCR1, 0x00);
    _writeRegister8(RA8875_CURHS, 0x00);
    _writeRegister8(RA8875_CURVS, 0x00);
    
    _writeRegister8(RA8875_FNCR0, RA8875_FNCR0_CGROM_8859_1);
    
    _writeRegister16(RA8875_F_CURXL, x0);
    _writeRegister16(RA8875_F_CURYL, y0);
    _setFrontColour(textColour);
    _setBackColour(backColour);
    
    k = _readRegister8(RA8875_FNCR1);
    
    if (_fontSolid) k &= ~ RA8875_FNCR1_TRANSPARENT;
    else            k |=   RA8875_FNCR1_TRANSPARENT;
    
    if (_orientation %2 == 1)   k |=   0b00010000;
    else                        k &= ~ 0b00010000;
    
    k &=  0b11110000;
    k |= (0b00000101 * _fontSize);
    
    _writeRegister8(RA8875_FNCR1, k);
    
    _writeCommand8(RA8875_MRWC);
    for (i=0; i<s.length(); i++) {
        _writeData8(s[i]);
        delay(1);
    }
    _writeRegister8(RA8875_MWCR0, _readRegister8(RA8875_MWCR0) & ~RA8875_MWCR0_TXTMODE);
}


void Screen_ER8875::setFontSolid(bool flag)
{
    _fontSolid = flag;
}


void Screen_ER8875::setFontSize(uint8_t size)
{
    if (size < MAX_FONT_SIZE) _fontSize = size;
    else _fontSize = MAX_FONT_SIZE -1;
}


uint8_t Screen_ER8875::fontMax()
{
    return MAX_FONT_SIZE;
}


uint8_t Screen_ER8875::fontSizeX()
{
    return 8 * (1+ _fontSize);
}


uint8_t Screen_ER8875::fontSizeY()
{
    return 16 * (1+ _fontSize);
}
#endif


uint16_t Screen_ER8875::readPixel(uint16_t x1, uint16_t y1)
{
    _orientCoordinates(x1, y1);
    _writeRegister16(RA8875_RCURH0, x1);
    _writeRegister16(RA8875_RCURV0, y1);
    
    _writeCommand8(RA8875_MRWC);
    _readData16();                                                              // dummy
    
    return _readData16();
    
    //    uint16_t colour;
    //
    //    _writeRegister16(RA8875_HSBE0_BTE, x1);
    //    _writeRegister16(RA8875_VSBE0_BTE, y1);
    //    _writeRegister16(RA8875_BEWR0_BTE, 1);
    //    _writeRegister16(RA8875_BEHR0_BTE, 1);
    //
    //
    //    // ROP Bits REG[51h] Bit[7:4]   1100b   Source
    //    // BTE Operation REG[51h] Bits [3:0] 0010b    Move BTE in positive direction with ROP
    //    _writeRegister8(RA8875_BECR1_BTE, 0b00000001);
    //    _writeRegister8(RA8875_BECR0_BTE, _readRegister8(RA8875_HSBE0_BTE) | 0b10000000);
    ////    _writeRegister16(RA8875_MWCR0, RA8875_MWCR0_GFXMODE);
    //
    //    _writeCommand8(RA8875_MRWC);
    //    delay(1);
    ////    do {
    ////    CLEAR_SCREEN_LOW;
    ////    SPI_LCD.transfer(RA8875_STATUS_READ);
    ////    colour = SPI_LCD.transfer(0x00);
    ////    } while (colour & 0b10000000);
    //
    //    return _readData16() << 1;
}

void Screen_ER8875::copyPasteArea(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t dx, uint16_t dy)
{
    //    1. Setting source layer and address       REG[54h], [55h], [56h], [57h]
    //    2. Setting destination layer and address  REG[58h], [59h], [5Ah], [5Bh]
    //    3. Setting BTE width and height           REG[5Ch], [5Dh], [5Eh], [5Fh]
    //    4. Setting BTE operation and ROP function REG[51h] Bit[3:0] = 2h
    //    5. Enable BTE function                    REG[50h] Bit7 = 1
    //    6. Check STSR REG Bit6                    check 2D final
    
    _writeRegister16(RA8875_HSBE0_BTE, x1);
    _writeRegister16(RA8875_VSBE0_BTE, y1);
    _writeRegister16(RA8875_HDBE0_BTE, x2);
    _writeRegister16(RA8875_VDBE0_BTE, y2);
    _writeRegister16(RA8875_BEWR0_BTE, dx);
    _writeRegister16(RA8875_BEHR0_BTE, dy);
    
    
    // ROP Bits REG[51h] Bit[7:4]   1100b   Source
    // BTE Operation REG[51h] Bits [3:0] 0010b    Move BTE in positive direction with ROP
    _writeRegister8(RA8875_BECR1_BTE, 0b11000010);
    _writeRegister8(RA8875_BECR0_BTE, _readRegister8(RA8875_HSBE0_BTE) | 0b10000000);
    
    
    while (_readRegister8(RA8875_BECR0_BTE) & 0b10000000) {
        //        Serial.print(".");
    }
}

void Screen_ER8875::copyArea(uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy, uint32_t &address)
{
    
}


void Screen_ER8875::pasteArea(uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy, uint32_t &address, bool option)
{
    
}


void Screen_ER8875::_setWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
    _writeRegister16(RA8875_HDBE0_BTE, x0);
    _writeRegister16(RA8875_VDBE0_BTE, y0);
    _writeRegister16(RA8875_BEWR0_BTE, x1-x0+1);
    _writeRegister16(RA8875_BEHR0_BTE, y1-y0+1);
    
    _writeRegister8(RA8875_BECR1_BTE, 0b11000000);
    _writeRegister8(RA8875_BECR0_BTE, _readRegister8(RA8875_HSBE0_BTE) | 0b10000000);
    _writeCommand8(RA8875_MRWC);
}


void Screen_ER8875::_setWindowRead(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
    //        _setWindow(x0, y0, x0+dx-1, y0+dy-1); // only to write!
    _writeRegister16(RA8875_HSBE0_BTE, x0);
    _writeRegister16(RA8875_VSBE0_BTE, y0);
    _writeRegister16(RA8875_BEWR0_BTE, x1-x0+1);
    _writeRegister16(RA8875_BEHR0_BTE, y1-y0+1);
    
    
    // ROP Bits REG[51h] Bit[7:4]   1100b   Source
    // BTE Operation REG[51h] Bits [3:0] 0010b    Move BTE in positive direction with ROP
    _writeRegister8(RA8875_BECR1_BTE, 0b00000001);
    _writeRegister8(RA8875_BECR0_BTE, _readRegister8(RA8875_HSBE0_BTE) | 0b10000000);
    //    _writeRegister16(RA8875_MWCR0, RA8875_MWCR0_GFXMODE);
    
    _writeCommand8(RA8875_MRWC);
}


void Screen_ER8875::openArea(uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy)
{
    _setWindow(x0, y0, x0+dx-1, y0+dy-1);
    _fsmArea |= 0xf0;
}

void Screen_ER8875::feedArea(uint16_t colour)
{
    if (_fsmArea & 0xf0) _writeData16(colour);
}

void Screen_ER8875::closeArea()
{
    _writeRegister8(RA8875_BECR0_BTE, _readRegister8(RA8875_HSBE0_BTE) & ~ 0b10000000);
    _fsmArea &= ~0xf0;
}

void Screen_ER8875::_setBacklight(bool state)
{
    ;
}

void Screen_ER8875::_setIntensity(uint8_t intensity)
{
    ;
}



//#endif
