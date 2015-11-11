//
// Screen_K70.cpp
// Library C++ code
// ----------------------------------
// Developed with embedXcode
// http://embedXcode.weebly.com
//
// Project LCD_screen Library Suite
//
// Created by Rei VILO, Dec 14, 2013
// embedXcode.weebly.com
//
//
// Copyright © Rei VILO, 2013
// License All rights reserved
//
// See LCD_documentation.h and ReadMe.txt for references
//

#if defined(__LM4F120H5QR__) || defined(__TM4C123GH6PM__)
//#if defined(__LM4F120H5QR__) || defined(__TM4C1230C3PM__) || defined(__TM4C129XNCZAD__)

// Library header
#include "Screen_K70.h"
#include "inc/hw_gpio.h"
#include "driverlib/sysctl.h"

///
/// @name	SSD1963 constants
///
/// @{

#define SSD1963_NOP                     0x00
#define SSD1963_SOFT_RESET              0x01
#define SSD1963_GET_POWER_MODE          0x0a
#define SSD1963_GET_ADDRESS_MODE        0x0b
#define SSD1963_RESERVED                0x0c
#define SSD1963_GET_DISPLAY_MODE        0x0d
#define SSD1963_GET_TEAR_EFFECT_STATUS	0x0e
#define SSD1963_RESERVED                0x0f
#define SSD1963_ENTER_SLEEP_MODE        0x10
#define SSD1963_EXIT_SLEEP_MODE         0x11
#define SSD1963_ENTER_PARTIAL_MODE      0x12
#define SSD1963_ENTER_NORMAL_MODE       0x13
#define SSD1963_EXIT_INVERT_MODE        0x20
#define SSD1963_ENTER_INVERT_MODE       0x21
#define SSD1963_SET_GAMMA_CURVE         0x26
#define SSD1963_SET_DISPLAY_OFF         0x28
#define SSD1963_SET_DISPLAY_ON          0x29
#define SSD1963_SET_COLUMN_ADDRESS      0x2a
#define SSD1963_SET_PAGE_ADDRESS        0x2b
#define SSD1963_WRITE_MEMORY_START      0x2c
#define SSD1963_READ_MEMORY_START       0x2e
#define SSD1963_SET_PARTIAL_AREA        0x30
#define SSD1963_SET_SCROLL_AREA         0x33
#define SSD1963_SET_TEAR_OFF            0x34
#define SSD1963_SET_TEAR_ON             0x35
#define SSD1963_SET_ADDRESS_MODE        0x36
#define SSD1963_SET_SCROLL_START        0x37
#define SSD1963_EXIT_IDLE_MODE          0x38
#define SSD1963_ENTER_IDLE_MODE         0x39
#define SSD1963_RESERVED                0x3a
#define SSD1963_WRITE_MEMORY_CONTINUE	0x3c
#define SSD1963_READ_MEMORY_CONTINUE	0x3e
#define SSD1963_SET_TEAR_SCANLINE       0x44
#define SSD1963_GET_SCANLINE            0x45
#define SSD1963_READ_DDB                0xa1
#define SSD1963_RESERVED                0xa8
#define SSD1963_SET_LCD_MODE            0xb0
#define SSD1963_GET_LCD_MODE            0xb1
#define SSD1963_SET_HORI_PERIOD         0xb4
#define SSD1963_GET_HORI_PERIOD         0xb5
#define SSD1963_SET_VERT_PERIOD         0xb6
#define SSD1963_GET_VERT_PERIOD         0xb7
#define SSD1963_SET_GPIO_CONF			0xb8
#define SSD1963_GET_GPIO_CONF           0xb9
#define SSD1963_SET_GPIO_VALUE          0xba
#define SSD1963_GET_GPIO_STATUS         0xbb
#define SSD1963_SET_POST_PROC           0xbc
#define SSD1963_GET_POST_PROC           0xbd
#define SSD1963_SET_PWM_CONF            0xbe
#define SSD1963_GET_PWM_CONF            0xbf
#define SSD1963_SET_LCD_GEN0            0xc0
#define SSD1963_GET_LCD_GEN0            0xc1
#define SSD1963_SET_LCD_GEN1            0xc2
#define SSD1963_GET_LCD_GEN1            0xc3
#define SSD1963_SET_LCD_GEN2            0xc4
#define SSD1963_GET_LCD_GEN2            0xc5
#define SSD1963_SET_LCD_GEN3            0xc6
#define SSD1963_GET_LCD_GEN3            0xc7
#define SSD1963_SET_GPIO0_ROP           0xc8
#define SSD1963_GET_GPIO0_ROP           0xc9
#define SSD1963_SET_GPIO1_ROP           0xca
#define SSD1963_GET_GPIO1_ROP           0xcb
#define SSD1963_SET_GPIO2_ROP           0xcc
#define SSD1963_GET_GPIO2_ROP           0xcd
#define SSD1963_SET_GPIO3_ROP           0xce
#define SSD1963_GET_GPIO3_ROP           0xcf
#define SSD1963_SET_DBC_CONF            0xd0
#define SSD1963_GET_DBC_CONF            0xd1
#define SSD1963_SET_DBC_TH              0xd4
#define SSD1963_GET_DBC_TH              0xd5
#define SSD1963_SET_PLL                 0xe0
#define SSD1963_SET_PLL_MN              0xe2
#define SSD1963_GET_PLL_MN              0xe3
#define SSD1963_GET_PLL_STATUS          0xe4
#define SSD1963_SET_DEEP_SLEEP          0xe5
#define SSD1963_SET_LSHIFT_FREQ         0xe6
#define SSD1963_GET_LSHIFT_FREQ         0xe7
#define SSD1963_RESERVED                0xe8
#define SSD1963_RESERVED                0xe9
#define SSD1963_SET_PIXEL_DATA_INTERFACE	0xf0
#define SSD1963_GET_PIXEL_DATA_INTERFACE	0xf1
#define SSD1963_RESERVED                0xff

#define DEFAULT_ADDRESS_MODE            0b00000011

#define K70_WIDTH       800 // Vertical
#define K70_HEIGHT      480 // Horizontal

/// @}

//
// LCD control line GPIO definitions.
//
#define LCD_DATAH_PERIPH        SYSCTL_PERIPH_GPIOB
#define LCD_DATAH_BASE          GPIO_PORTB_BASE
#define LCD_DATAH_PINS          0xff

#define LCD_CS_PERIPH           SYSCTL_PERIPH_GPIOA
#define LCD_CS_BASE             GPIO_PORTA_BASE
#define LCD_CS_PIN              GPIO_PIN_7
#define LCD_DC_PERIPH           SYSCTL_PERIPH_GPIOA
#define LCD_DC_BASE             GPIO_PORTA_BASE
#define LCD_DC_PIN              GPIO_PIN_6
#define LCD_WR_PERIPH           SYSCTL_PERIPH_GPIOA
#define LCD_WR_BASE             GPIO_PORTA_BASE
#define LCD_WR_PIN              GPIO_PIN_5
#define LCD_RD_PERIPH           SYSCTL_PERIPH_GPIOA
#define LCD_RD_BASE             GPIO_PORTA_BASE
#define LCD_RD_PIN              GPIO_PIN_4

#define LCD_PE0_PERIPH           SYSCTL_PERIPH_GPIOA
#define LCD_PE0_BASE             GPIO_PORTE_BASE
#define LCD_PE0_PIN              GPIO_PIN_0

//#define LCD_BACKLIGHT_PERIPH    SYSCTL_PERIPH_GPIOF
//#define LCD_BACKLIGHT_BASE      GPIO_PORTF_BASE
//#define LCD_BACKLIGHT_PIN       GPIO_PIN_3

///
/// @brief	Low level write to 8-bit parallel bus
/// @param	data8 byte to write
///
inline void SET_LCD_DATA(uint8_t data8)
{
    HWREG(LCD_DATAH_BASE + GPIO_O_DATA + (LCD_DATAH_PINS << 2)) = (data8);
}
//#define SET_LCD_DATA(ucByte)                                                  \
//{                                                                             \
//    HWREG(LCD_DATAH_BASE + GPIO_O_DATA + (LCD_DATAH_PINS << 2)) = (ucByte);   \
//}

///
/// @brief	Low level read to 8-bit parallel bus
/// @return	value read, uint8_t
///
inline uint8_t GET_LCD_DATA()
{
    return HWREG(LCD_DATAH_BASE + GPIO_O_DATA + (LCD_DATAH_PINS << 2));
}
//#define GET_LCD_DATA() HWREG(LCD_DATAH_BASE + GPIO_O_DATA + (LCD_DATAH_PINS << 2))

///
/// @name   Touch constants
///
/// @{

#define TOUCH_TRIM  0x10  ///< Touch threshold
#define _pinTouchXR PE_4  ///< XP *analog
#define _pinTouchYU PE_5  ///< YP *analog
#define _pinTouchXL PA_2  ///< XN analog or *digital
#define _pinTouchYD PA_3  ///< YN analog or *digital

// TOUCH_XP PE4 Resistor touch screen terminal (Left)
// TOUCH_YP PE5 Resistor touch screen terminal (Top)
// TOUCH_XN PA3 Resistor touch screen terminal (Right)
// TOUCH_YN PA2 Resistor touch screen terminal (Bottom)
#define TOUCH_XP A8      // PE_5  // must be an analog pin, use "An" notation!
#define TOUCH_YP A9      // PE_4  // must be an analog pin, use "An" notation!
#define TOUCH_XN PA_2    // can be a digital pin
#define TOUCH_YN PA_3    // can be a digital pin

/// @}


// Code
Screen_K70::Screen_K70()
{
    ;
}

void Screen_K70::begin()
{
	//
    // Enable GPIOs
    //
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    SysCtlPeripheralEnable(LCD_DC_PERIPH);
    SysCtlPeripheralEnable(LCD_RD_PERIPH);
    SysCtlPeripheralEnable(LCD_WR_PERIPH);
    SysCtlPeripheralEnable(LCD_CS_PERIPH);
    
    //
    // GPIOs as OUTPUT
    //
    GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, 0xff);
    GPIOPinTypeGPIOOutput(LCD_DC_BASE, LCD_DC_PIN);
    GPIOPinTypeGPIOOutput(LCD_RD_BASE, LCD_RD_PIN);
    GPIOPinTypeGPIOOutput(LCD_WR_BASE, LCD_WR_PIN);
    GPIOPinTypeGPIOOutput(LCD_CS_BASE, LCD_CS_PIN);
    
    //    //
    //    // Enable the GPIO peripherals used to interface to the SSD2119.
    //    //
    //    SysCtlPeripheralEnable(LCD_DATAH_PERIPH);
    //    SysCtlPeripheralEnable(LCD_DC_PERIPH);
    //    SysCtlPeripheralEnable(LCD_RD_PERIPH);
    //    SysCtlPeripheralEnable(LCD_WR_PERIPH);
    //    SysCtlPeripheralEnable(LCD_CS_PERIPH);
    //    //    SysCtlPeripheralEnable(LCD_BACKLIGHT_PERIPH);
    //
    //    //
    //    // Perform low level interface initialization depending upon how the LCD
    //    // is connected to the Stellaris microcontroller.  This varies depending
    //    // upon the daughter board connected it is possible that a daughter board
    //    // can drive the LCD directly rather than via the basic GPIO interface.
    //    //
    //    //
    //    // Initialize the GPIOs used to interface to the LCD controller.
    //    // Configure the pins that connect to the LCD as GPIO outputs.
    //    //
    //    GPIODirModeSet(LCD_DATAH_BASE, LCD_DATAH_PINS, GPIO_DIR_MODE_OUT);
    GPIOPadConfigSet(LCD_DATAH_BASE, LCD_DATAH_PINS, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD);
    //
    //    GPIODirModeSet(LCD_DC_BASE, LCD_DC_PIN, GPIO_DIR_MODE_OUT);
    GPIOPadConfigSet(LCD_DC_BASE, LCD_DC_PIN, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD);
    //    GPIODirModeSet(LCD_RD_BASE, LCD_RD_PIN, GPIO_DIR_MODE_OUT);
    GPIOPadConfigSet(LCD_RD_BASE, LCD_RD_PIN, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD);
    //    GPIODirModeSet(LCD_WR_BASE, LCD_WR_PIN, GPIO_DIR_MODE_OUT);
    GPIOPadConfigSet(LCD_WR_BASE, LCD_WR_PIN, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD);
    //    GPIODirModeSet(LCD_CS_BASE, LCD_CS_PIN, GPIO_DIR_MODE_OUT);
    GPIOPadConfigSet(LCD_CS_BASE, LCD_CS_PIN, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD);
    //    //            GPIODirModeSet(LCD_BACKLIGHT_BASE, LCD_BACKLIGHT_PIN, GPIO_DIR_MODE_OUT);
    //    //            GPIOPadConfigSet(LCD_BACKLIGHT_BASE, LCD_BACKLIGHT_PIN, GPIO_STRENGTH_8MA,
    //    //                             GPIO_PIN_TYPE_STD);
    
    //
    // Set the LCD control pins to their default values.
    //
    GPIOPinWrite(LCD_CS_BASE, LCD_CS_PIN, LCD_CS_PIN);
    GPIOPinWrite(LCD_DATAH_BASE, LCD_DATAH_PINS, 0x00);
    GPIOPinWrite(LCD_DC_BASE, LCD_DC_PIN, 0x00);
    GPIOPinWrite(LCD_RD_BASE, LCD_RD_PIN, LCD_RD_PIN);
    GPIOPinWrite(LCD_WR_BASE, LCD_WR_PIN, LCD_WR_PIN);
    GPIOPinWrite(LCD_CS_BASE, LCD_CS_PIN, 0);
    
    delay(10); // Delay 10mS
    
    _writeCommand8(SSD1963_SOFT_RESET);  // software reset
    delay(10); // Delay 1mS
    
    _writeCommand8(SSD1963_SET_PLL_MN);	//PLL multiplier, set PLL clock to 120M
    _writeData8(0x3E);	    //N=0x36 for 6.5M, 0x23 for 10M crystal
    _writeData8(0x03);
    _writeData8(0x04);
    
    _writeCommand8(SSD1963_SET_PLL);  // PLL enable
    _writeData8(0x01);
    delay(1); // Delay 1mS
    
    _writeCommand8(SSD1963_SET_PLL);
    _writeData8(0x03);
    delay(5); // Delay 5mS
    
    _writeCommand8(SSD1963_SET_LSHIFT_FREQ);	//PLL setting for PCLK, depends on resolution
    _writeData8(0x03);
    _writeData8(0xFF);
    _writeData8(0xFF);
    
    _writeCommand8(SSD1963_SET_LCD_MODE);	//LCD SPECIFICATION
    _writeData8(0x20);//24
    _writeData8(0x00);
    _writeData8(0x03);  //Set HDP
    _writeData8(0x1F);
    _writeData8(0x01);
    _writeData8(0xDF);
    _writeData8(0x00);
    
    _writeCommand8(SSD1963_SET_HORI_PERIOD);	//HSYNC
    _writeData8(0x03);
    _writeData8(0xF8);//F8
    _writeData8(0x00);
    _writeData8(0xB4);//64
    _writeData8(0x59);//49
    _writeData8(0x00);
    _writeData8(0x00);
    _writeData8(0x00);
    
    _writeCommand8(SSD1963_SET_VERT_PERIOD);	//VSYNC
    _writeData8(0x01);
    _writeData8(0xFB);
    _writeData8(0);
    _writeData8(0x14);
    _writeData8(0x09);
    _writeData8(0);
    _writeData8(0);
    
    _writeCommand8(SSD1963_SET_GPIO_CONF);
    _writeData8(0x0F);    //set GPIO[3:0] as output
    _writeData8(0x01);    //GPIO0 normal
    
    _writeCommand8(SSD1963_SET_GPIO_VALUE);
    _writeData8(0x0B);    //set GPIO3(SHLR)=1, GPIO2(UPDN)=0, GPIO1=1, GPIO0(VGH_VGL)=1;
    
    _writeCommand8(SSD1963_SET_ADDRESS_MODE); //rotation
    _writeData8(DEFAULT_ADDRESS_MODE);
    
    _writeCommand8(SSD1963_SET_PIXEL_DATA_INTERFACE); //pixel data interface
    //	_writeData8(0x03);
    _writeData8(0x00);
    delay(5); // Delay 5mS
    
    _writeCommand8(SSD1963_SET_PWM_CONF); //set PWM for B/L
    _writeData8(0x01);//frequency
    _writeData8(0xFF);//duty
    _writeData8(0x09);//bit[3]DBC enable,bit[0]PWM enable
    _writeData8(0xFF);//C0 DBC manual brightness
    _writeData8(0x88);//10 DBC minimum brightness
    _writeData8(0x00);//Brightness prescaler
    
    _writeCommand8(SSD1963_SET_DBC_TH); //DBC
    _writeData8(0x00);
    _writeData8(0x1C);
    _writeData8(0x20);
    _writeData8(0x00);
    _writeData8(0x46);
    _writeData8(0x50);
    _writeData8(0x00);
    _writeData8(0xA8);
    _writeData8(0xC0);
    
    _writeCommand8(SSD1963_SET_DBC_CONF);
    //_writeData8(0x0d);//DBC Aggressive mode
    _writeData8(0x01);//DBC disable
    
    //_writeCommand8(SSD1963_ENTER_INVERT_MODE); //Enter Invert Mode
    
    _writeCommand8(SSD1963_SET_DISPLAY_ON); //display on
    
    _screenWidth  = K70_HEIGHT;
    _screenHeigth = K70_WIDTH;
    
    // Standard
    setOrientation(1);
    
    _screenWidth  = K70_HEIGHT;
    _screenHeigth = K70_WIDTH;
    _screenDiagonal = 70;
    
    // Touch
    uint16_t x0, y0, z0;
    _getRawTouch(x0, y0, z0);
    
    _touchTrim = TOUCH_TRIM;
    _touchXmin = 2781;
    _touchXmax = 1033;
    _touchYmin = 3440;
    _touchYmax = 554;
    
    _penSolid  = false;
    _fontSolid = true;
    _flagRead  = false;
    _flagEnergy = false;
    _touchTrim = 10;
    _fsmArea = 0x01;
    clear();
}

String Screen_K70::WhoAmI()
{
    return "Kentec 7\" screen";
}

//uint16_t Screen_K70::readPixel(uint16_t x1, uint16_t y1)
//{
//	_setWindowRead(x1, y1, x1, y1);
//	return _readData16();
//}


//void Screen_K70::copyPasteArea(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t dx, uint16_t dy)
//{
//    uint16_t buffer[dx];
//
//    _closeWindow();
//
//    for (uint16_t y=0; y<dy; y++) {
//
//        _setWindowRead(x1, y1+y, x1+dx, y1+y);
//        for (uint16_t x=0; x<dx; x++) {
//            //            _setCursor(x1+x, y1+y);
//            buffer[x] = _readData16();
//        }
//
//        _setWindow(x2, y2+y, x2+dx, y2+y);
//
//        for (uint16_t x=0; x<dx; x++) {
//            _writeData88(highByte(buffer[x]), lowByte(buffer[x]));
//        }
//    }
//    _closeWindow();
//}

void Screen_K70::_setOrientation(uint8_t orientation)
{
    //_orientation = orientation % 4;
    //    LCD_screen_font::setOrientation(orientation);
    
    //
    // default = 0x6830 = 0x68 <<8 + 0b00110000
    switch (_orientation)
    {
        case 0:
            //                                          hvOo
            _writeRegister8(SSD1963_SET_ADDRESS_MODE, 0b01100000 | DEFAULT_ADDRESS_MODE);              // ok
            break;
        case 1:
            //                                          hvOo
            _writeRegister8(SSD1963_SET_ADDRESS_MODE, 0b11000000 | DEFAULT_ADDRESS_MODE);              // ok
            break;
        case 2:
            //                                          hvOo
            _writeRegister8(SSD1963_SET_ADDRESS_MODE, 0b10100000 | DEFAULT_ADDRESS_MODE);              // ok
            break;
        case 3:
            //                                          hvOo
            _writeRegister8(SSD1963_SET_ADDRESS_MODE, 0b00000000 | DEFAULT_ADDRESS_MODE);              // nok
            break;
    }
}

// Utilities
void Screen_K70::_writeData8(uint8_t data8)
{
    // Writes a data word to the SSD2119.  This function implements the basic GPIO
    // interface to the LCD display.
    
    HWREG(LCD_CS_BASE + GPIO_O_DATA + (LCD_CS_PIN << 2)) = 0;
    
    //
    // Write the least significant byte of the data to the bus.
    //
    SET_LCD_DATA(data8);
    //
    // Assert the write enable signal.
    //
    HWREG(LCD_WR_BASE + GPIO_O_DATA + (LCD_WR_PIN << 2)) = 0;
    HWREG(LCD_WR_BASE + GPIO_O_DATA + (LCD_WR_PIN << 2)) = 0;
    
    //
    // Deassert the write enable signal.
    //
    HWREG(LCD_WR_BASE + GPIO_O_DATA + (LCD_WR_PIN << 2)) = LCD_WR_PIN;
    HWREG(LCD_WR_BASE + GPIO_O_DATA + (LCD_WR_PIN << 2)) = LCD_WR_PIN;
    
    HWREG(LCD_CS_BASE + GPIO_O_DATA + (LCD_CS_PIN << 2)) = LCD_CS_PIN;          // CS HIGH
}

void Screen_K70::_writeData88(uint8_t dataHigh8, uint8_t dataLow8)
{
    //    uint16_t colour = makeWord(dataHigh8, dataLow8);
    //
    //    uint8_t redColour8   =  (colour >>11) <<3;
    //    uint8_t greenColour8 =  (colour >> 5) <<2;
    //    uint8_t blueColour8  =   colour       <<3;
    
    // Fastest solution
    uint8_t redColour8   =    dataHigh8 & 0b11111000;
    uint8_t greenColour8 =  ((dataHigh8 & 0b00000111) <<3 | (dataLow8 >> 5)) <<2;
    uint8_t blueColour8  =    dataLow8  <<3;
    
    HWREG(LCD_CS_BASE + GPIO_O_DATA + (LCD_CS_PIN << 2)) = 0;
    _writeData888(redColour8, greenColour8, blueColour8);
    HWREG(LCD_CS_BASE + GPIO_O_DATA + (LCD_CS_PIN << 2)) = LCD_CS_PIN;          // CS HIGH
}


void Screen_K70::_writeData888(uint8_t dataRed8, uint8_t dataBlue8, uint8_t dataGreen8)
{
    // Writes the colour value to the SSD2119.
    // 3 bytes, one per component red-green-blue, so 24-bit depth
    // This function implements the basic GPIO interface to the LCD display.
    
    //	HWREG(LCD_CS_BASE + GPIO_O_DATA + (LCD_CS_PIN << 2)) = 0;
    
	//
    // Write the most significant byte of the data to the bus.
    //
    SET_LCD_DATA(dataRed8);
    
    //
    // Assert the write enable signal.  We need to do this 2 times to ensure
    // that we don't violate the timing requirements for the display.
    //
    HWREG(LCD_WR_BASE + GPIO_O_DATA + (LCD_WR_PIN << 2)) = 0;
    HWREG(LCD_WR_BASE + GPIO_O_DATA + (LCD_WR_PIN << 2)) = 0;
    
    //
    // Deassert the write enable signal.
    //
    HWREG(LCD_WR_BASE + GPIO_O_DATA + (LCD_WR_PIN << 2)) = LCD_WR_PIN;
    HWREG(LCD_WR_BASE + GPIO_O_DATA + (LCD_WR_PIN << 2)) = LCD_WR_PIN;
    
    //
    // Write the mid byte of the data to the bus.
    //
    SET_LCD_DATA(dataBlue8);
    
    //
    // Assert the write enable signal.  We need to do this 2 times to ensure
    // that we don't violate the timing requirements for the display.
    //
    HWREG(LCD_WR_BASE + GPIO_O_DATA + (LCD_WR_PIN << 2)) = 0;
    HWREG(LCD_WR_BASE + GPIO_O_DATA + (LCD_WR_PIN << 2)) = 0;
    
    //
    // Deassert the write enable signal.
    //
    HWREG(LCD_WR_BASE + GPIO_O_DATA + (LCD_WR_PIN << 2)) = LCD_WR_PIN;
    HWREG(LCD_WR_BASE + GPIO_O_DATA + (LCD_WR_PIN << 2)) = LCD_WR_PIN;
    
    //
    // Write the least significant byte of the data to the bus.
    //
    SET_LCD_DATA(dataGreen8);
    
    //
    // Assert the write enable signal.
    //
    HWREG(LCD_WR_BASE + GPIO_O_DATA + (LCD_WR_PIN << 2)) = 0;
    HWREG(LCD_WR_BASE + GPIO_O_DATA + (LCD_WR_PIN << 2)) = 0;
    
    //
    // Deassert the write enable signal.
    //
    HWREG(LCD_WR_BASE + GPIO_O_DATA + (LCD_WR_PIN << 2)) = LCD_WR_PIN;
    HWREG(LCD_WR_BASE + GPIO_O_DATA + (LCD_WR_PIN << 2)) = LCD_WR_PIN;
    
    //    HWREG(LCD_CS_BASE + GPIO_O_DATA + (LCD_CS_PIN << 2)) = LCD_CS_PIN;          // CS HIGH
}

//*****************************************************************************
//
// Writes a command to the SSD2119.  This function implements the basic GPIO
// interface to the LCD display.
//
//*****************************************************************************
void Screen_K70::_writeCommand8(uint8_t command8)
{
    //
    // Writes a command to the SSD2119.  This function implements the basic GPIO
    // interface to the LCD display.
    //
	HWREG(LCD_CS_BASE + GPIO_O_DATA + (LCD_CS_PIN << 2)) = 0;
    
    //
    // Write the most significant byte of the data to the bus.  This is always
    // 0 since commands are no more than 8 bits.
    //
    SET_LCD_DATA(0);
    
    //
    // Assert DC
    //
    HWREG(LCD_DC_BASE + GPIO_O_DATA + (LCD_DC_PIN << 2)) = 0;
    
    //
    // Assert the write enable signal.  We need to do this 2 times to ensure
    // that we don't violate the timing requirements for the display.
    //
    HWREG(LCD_WR_BASE + GPIO_O_DATA + (LCD_WR_PIN << 2)) = 0;
    HWREG(LCD_WR_BASE + GPIO_O_DATA + (LCD_WR_PIN << 2)) = 0;
    
    //
    // Deassert the write enable signal.
    //
    HWREG(LCD_WR_BASE + GPIO_O_DATA + (LCD_WR_PIN << 2)) = LCD_WR_PIN;
    HWREG(LCD_WR_BASE + GPIO_O_DATA + (LCD_WR_PIN << 2)) = LCD_WR_PIN;
    
    //
    // Write the least significant byte of the data to the bus.
    //
    SET_LCD_DATA(command8);
    
    //
    // Assert the write enable signal.
    //
    HWREG(LCD_WR_BASE + GPIO_O_DATA + (LCD_WR_PIN << 2)) = 0;
    HWREG(LCD_WR_BASE + GPIO_O_DATA + (LCD_WR_PIN << 2)) = 0;
    
    //
    // Deassert the write enable signal.
    //
    HWREG(LCD_WR_BASE + GPIO_O_DATA + (LCD_WR_PIN << 2)) = LCD_WR_PIN;
    HWREG(LCD_WR_BASE + GPIO_O_DATA + (LCD_WR_PIN << 2)) = LCD_WR_PIN;
    
    //
    // Set the DC signal high, indicating that following writes are data.
    //
    HWREG(LCD_DC_BASE + GPIO_O_DATA + (LCD_DC_PIN << 2)) = LCD_DC_PIN;
    
    HWREG(LCD_CS_BASE + GPIO_O_DATA + (LCD_CS_PIN << 2)) = LCD_CS_PIN;          // CS HIGH
}

//uint16_t Screen_K70::_readData16()
//{
//    uint16_t data16 = 0;
//    uint8_t red, blue, green;
//    uint8_t data8[64][5];
//
//    _writeCommand8(SSD1963_READ_MEMORY_CONTINUE);
//
//	// Switch to INPUT mode
//    GPIOPinTypeGPIOInput(LCD_DATAH_BASE, 0xff);
//
//	HWREG(LCD_CS_BASE + GPIO_O_DATA + (LCD_CS_PIN << 2)) = 0;                   // CS LOW
//
//    //
//    // Assert the read enable signal.  Do this three times to ensure that we
//    // meet the display timing requirements (20nS per instruction at 50MHz
//    // means we need to ensure at least 3 instructions between edges on WR)
//    //
//    // Assert the read enable signal. x10 = 200 ns
//    //
//    HWREG(LCD_RD_BASE + GPIO_O_DATA + (LCD_RD_PIN << 2)) = 0;
//    HWREG(LCD_RD_BASE + GPIO_O_DATA + (LCD_RD_PIN << 2)) = 0;
//    HWREG(LCD_RD_BASE + GPIO_O_DATA + (LCD_RD_PIN << 2)) = 0;
//    HWREG(LCD_RD_BASE + GPIO_O_DATA + (LCD_RD_PIN << 2)) = 0;
//    HWREG(LCD_RD_BASE + GPIO_O_DATA + (LCD_RD_PIN << 2)) = 0;
//    HWREG(LCD_RD_BASE + GPIO_O_DATA + (LCD_RD_PIN << 2)) = 0;
//    HWREG(LCD_RD_BASE + GPIO_O_DATA + (LCD_RD_PIN << 2)) = 0;
//    HWREG(LCD_RD_BASE + GPIO_O_DATA + (LCD_RD_PIN << 2)) = 0;
//
//    //
//    // Read the most significant byte of the data from the bus.
//    //
//    //    data16   = (GET_LCD_DATA() >>3) <<11;
//    red = GET_LCD_DATA();
//
//    //
//    // Deassert the read enable signal. x5 = 100 ns
//    //
//    HWREG(LCD_RD_BASE + GPIO_O_DATA + (LCD_RD_PIN << 2)) = LCD_RD_PIN;
//    HWREG(LCD_RD_BASE + GPIO_O_DATA + (LCD_RD_PIN << 2)) = LCD_RD_PIN;
//    HWREG(LCD_RD_BASE + GPIO_O_DATA + (LCD_RD_PIN << 2)) = LCD_RD_PIN;
//    HWREG(LCD_RD_BASE + GPIO_O_DATA + (LCD_RD_PIN << 2)) = LCD_RD_PIN;
//
//    //
//    // Assert the read enable signal. x10 = 200 ns
//    //
//    HWREG(LCD_RD_BASE + GPIO_O_DATA + (LCD_RD_PIN << 2)) = 0;
//    HWREG(LCD_RD_BASE + GPIO_O_DATA + (LCD_RD_PIN << 2)) = 0;
//    HWREG(LCD_RD_BASE + GPIO_O_DATA + (LCD_RD_PIN << 2)) = 0;
//    HWREG(LCD_RD_BASE + GPIO_O_DATA + (LCD_RD_PIN << 2)) = 0;
//    HWREG(LCD_RD_BASE + GPIO_O_DATA + (LCD_RD_PIN << 2)) = 0;
//    HWREG(LCD_RD_BASE + GPIO_O_DATA + (LCD_RD_PIN << 2)) = 0;
//    HWREG(LCD_RD_BASE + GPIO_O_DATA + (LCD_RD_PIN << 2)) = 0;
//    HWREG(LCD_RD_BASE + GPIO_O_DATA + (LCD_RD_PIN << 2)) = 0;
//
//    //
//    // Read the least significant byte of the data from the bus.
//    //
//    green = GET_LCD_DATA();
//
//    //
//    // Deassert the read enable signal. x5 = 100 ns
//    //
//    HWREG(LCD_RD_BASE + GPIO_O_DATA + (LCD_RD_PIN << 2)) = LCD_RD_PIN;
//    HWREG(LCD_RD_BASE + GPIO_O_DATA + (LCD_RD_PIN << 2)) = LCD_RD_PIN;
//    HWREG(LCD_RD_BASE + GPIO_O_DATA + (LCD_RD_PIN << 2)) = LCD_RD_PIN;
//    HWREG(LCD_RD_BASE + GPIO_O_DATA + (LCD_RD_PIN << 2)) = LCD_RD_PIN;
//
//    //
//    // Assert the read enable signal. x10 = 200 ns
//    //
//    HWREG(LCD_RD_BASE + GPIO_O_DATA + (LCD_RD_PIN << 2)) = 0;
//    HWREG(LCD_RD_BASE + GPIO_O_DATA + (LCD_RD_PIN << 2)) = 0;
//    HWREG(LCD_RD_BASE + GPIO_O_DATA + (LCD_RD_PIN << 2)) = 0;
//    HWREG(LCD_RD_BASE + GPIO_O_DATA + (LCD_RD_PIN << 2)) = 0;
//    HWREG(LCD_RD_BASE + GPIO_O_DATA + (LCD_RD_PIN << 2)) = 0;
//    HWREG(LCD_RD_BASE + GPIO_O_DATA + (LCD_RD_PIN << 2)) = 0;
//    HWREG(LCD_RD_BASE + GPIO_O_DATA + (LCD_RD_PIN << 2)) = 0;
//    HWREG(LCD_RD_BASE + GPIO_O_DATA + (LCD_RD_PIN << 2)) = 0;
//
//    //
//    // Read the least significant byte of the data from the bus.
//    //
//    //    data16   |= (GET_LCD_DATA() >>3);
//    blue = GET_LCD_DATA();
//
//    //
//    // Deassert the read enable signal.
//    //
//    HWREG(LCD_RD_BASE + GPIO_O_DATA + (LCD_RD_PIN << 2)) = LCD_RD_PIN;
//    HWREG(LCD_RD_BASE + GPIO_O_DATA + (LCD_RD_PIN << 2)) = LCD_RD_PIN;
//    HWREG(LCD_RD_BASE + GPIO_O_DATA + (LCD_RD_PIN << 2)) = LCD_RD_PIN;
//    HWREG(LCD_RD_BASE + GPIO_O_DATA + (LCD_RD_PIN << 2)) = LCD_RD_PIN;
//
//    HWREG(LCD_CS_BASE + GPIO_O_DATA + (LCD_CS_PIN << 2)) = LCD_CS_PIN;          // CS HIGH
//
//    // Return to OUTPUT mode
//    GPIOPinTypeGPIOOutput(LCD_DATAH_BASE, 0xff);
//    GPIOPadConfigSet(LCD_DATAH_BASE, LCD_DATAH_PINS, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD);
//    /*
//     Serial.print("(");
//     //    Serial.print(a, HEX);
//     //    Serial.print("\t");
//     //    Serial.print(b, HEX);
//     //    Serial.print("\t");
//     Serial.print(red, HEX);
//     Serial.print("\t");
//     Serial.print(green, HEX);
//     Serial.print("\t");
//     Serial.print(blue, HEX);
//     Serial.print(")\t");
//     */
//    data16 = (red >>3) <<11 | (green >>2) <<5 | blue >>3;
//    return data16;
//}


void Screen_K70::_writeRegister8(uint8_t command8, uint8_t data8)
{
    _writeCommand8(command8);
    _writeData8(data8);
}


void Screen_K70::_orientCoordinates(uint16_t &x1, uint16_t &y1)
{
    switch (_orientation)
    {
        case 0:                                                                 // ok
            y1 = screenSizeY()-1 - y1;
            _swap(x1, y1);
            break;
        case 1:                                                                 // ok
            x1 = screenSizeX() - x1 -1;
            y1 = screenSizeY() - y1 -1;
            break;
        case 2:                                                                 // ok
            x1 = screenSizeX() - x1 -1;
            _swap(x1, y1);
            break;
        case 3:                                                                 // ok
            break;
	}
}

void Screen_K70::_setPoint(uint16_t x1, uint16_t y1, uint16_t colour)           // compulsory
{
    _setWindow(x1, y1, x1, y1);
    
    uint8_t redColour8   =  (colour >>11) <<3;
    uint8_t greenColour8 =  (colour >> 5) <<2;
    uint8_t blueColour8  =   colour       <<3;
    
    HWREG(LCD_CS_BASE + GPIO_O_DATA + (LCD_CS_PIN << 2)) = 0;                   // CS LOW
    _writeData888(redColour8, greenColour8, blueColour8);
    HWREG(LCD_CS_BASE + GPIO_O_DATA + (LCD_CS_PIN << 2)) = LCD_CS_PIN;          // CS HIGH
    
    //    uint8_t highColour = highByte(colour);
    //    uint8_t lowColour  = lowByte(colour);
    //
    //    _writeData88(highColour, lowColour);
}

//void Screen_K70::_setCursor(uint16_t x1, uint16_t y1)
//{
//    _setWindow(x1, y1, x1, y1);
//}


void Screen_K70::_setWindow(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    _orientCoordinates(x1, y1);
    _orientCoordinates(x2, y2);
    if (x1 > x2) _swap(x1, x2);
    if (y1 > y2) _swap(y1, y2);
    
    _writeCommand8(SSD1963_SET_COLUMN_ADDRESS);
  	_writeData8(highByte(x1));
  	_writeData8(lowByte(x1));
  	_writeData8(highByte(x2));
  	_writeData8(lowByte(x2));
    
	_writeCommand8(SSD1963_SET_PAGE_ADDRESS);
  	_writeData8(highByte(y1));
  	_writeData8(lowByte(y1));
  	_writeData8(highByte(y2));
  	_writeData8(lowByte(y2));
    
    _writeCommand8(SSD1963_WRITE_MEMORY_START);
}

void Screen_K70::_closeWindow()
{
    _setWindow(0, 0, screenSizeX()-1, screenSizeY()-1);
}

//void Screen_K70::_setWindowRead(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
//{
//    _orientCoordinates(x1, y1);
//    _orientCoordinates(x2, y2);
//    if (x1 > x2) _swap(x1, x2);
//    if (y1 > y2) _swap(y1, y2);
//
//    _writeCommand8(SSD1963_SET_COLUMN_ADDRESS);
//  	_writeData8(highByte(x1));
//  	_writeData8(lowByte(x1));
//  	_writeData8(highByte(x2));
//  	_writeData8(lowByte(x2));
//
//	_writeCommand8(SSD1963_SET_PAGE_ADDRESS);
//  	_writeData8(highByte(y1));
//  	_writeData8(lowByte(y1));
//  	_writeData8(highByte(y2));
//  	_writeData8(lowByte(y2));
//
//    _writeCommand8(SSD1963_READ_MEMORY_START);
//}

inline uint16_t absDiff(uint16_t a, uint16_t b)
{
    return (a > b) ? a-b : b-a;
}

void Screen_K70::_fastFill(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t colour)
{
    _setWindow(x1, y1, x2, y2);
    
    HWREG(LCD_CS_BASE + GPIO_O_DATA + (LCD_CS_PIN << 2)) = 0;                   // CS low
    
    
    // required because of uint32_t, t>0 and t--
    if (x1 > x2) _swap(x1, x2);
    if (y1 > y2) _swap(y1, y2);
    
    // fastest
    if (colour == myColours.black)
    {
        SET_LCD_DATA(0);
        for (uint32_t t=(uint32_t)(y2-y1+1)*(x2-x1+1)*3; t>0; t--)
        {
            HWREG(LCD_WR_BASE + GPIO_O_DATA + (LCD_WR_PIN << 2)) = 0;
            HWREG(LCD_WR_BASE + GPIO_O_DATA + (LCD_WR_PIN << 2)) = LCD_WR_PIN;
        }
    } else {
        uint8_t redColour8   =  (colour >>11) <<3;
        uint8_t greenColour8 =  (colour >> 5) <<2;
        uint8_t blueColour8  =   colour       <<3;
        
        //    uint8_t redColour8, greenColour8, blueColour8;
        //    splitColour(colour, redColour8, greenColour8, blueColour8);
        
        for (uint32_t t=(uint32_t)(y2-y1+1)*(x2-x1+1)*3; t>0; t--)
        {
            _writeData888(redColour8, greenColour8, blueColour8);
        }
    }
    
    HWREG(LCD_CS_BASE + GPIO_O_DATA + (LCD_CS_PIN << 2)) = LCD_CS_PIN;          // CS high
}

// Touch
void Screen_K70::_getRawTouch(uint16_t &x0, uint16_t &y0, uint16_t &z0)
{
    uint16_t a, b, c, d;
    
    // Read x
    // xp = +Vref
    // xn = ground
    // yp = measure
    // yn = open
    pinMode(TOUCH_YP, INPUT);
    pinMode(TOUCH_YN, INPUT);
    digitalWrite(TOUCH_YP, LOW);
    digitalWrite(TOUCH_YN, LOW);
    
    pinMode(TOUCH_XP, OUTPUT);
    pinMode(TOUCH_XN, OUTPUT);
    digitalWrite(TOUCH_XP, HIGH);
    digitalWrite(TOUCH_XN, LOW);
    
    do {
        delay(1);
        a  = analogRead(TOUCH_YP);
        delay(1);
        b  = analogRead(TOUCH_YP);
    } while (absDiff(a, b) > 8);
    x0  = 4095 - a;
    
    // Read y
    // xp = measure
    // xn = open
    // yp = +Vref
    // yn = ground
    pinMode(TOUCH_XP, INPUT);
    pinMode(TOUCH_XN, INPUT);
    digitalWrite(TOUCH_XP, LOW);
    digitalWrite(TOUCH_XN, LOW);
    
    pinMode(TOUCH_YP, OUTPUT);
    pinMode(TOUCH_YN, OUTPUT);
    digitalWrite(TOUCH_YP, HIGH);
    digitalWrite(TOUCH_YN, LOW);
    
    do {
        delay(1);
        a  = analogRead(TOUCH_XP);
        delay(1);
        b  = analogRead(TOUCH_XP);
    } while (absDiff(a, b) > 8);
    y0  = 4095 - a;
    
    // Read z
    // xp = ground
    // xn = measure
    // yp = measure
    // yn = +Vref
    pinMode(TOUCH_XP, OUTPUT);
    digitalWrite(TOUCH_XP, LOW);
    pinMode(TOUCH_YN, OUTPUT);
    digitalWrite(TOUCH_YN, HIGH);
    
    digitalWrite(TOUCH_XN, LOW);
    pinMode(TOUCH_XN, INPUT);
    digitalWrite(TOUCH_YP, LOW);
    pinMode(TOUCH_YP, INPUT);
    delay(10);
    
    do {
        delay(1);
        a = analogRead(TOUCH_XN);
        delay(1);
        c = analogRead(TOUCH_YP);
        delay(1);
        b = analogRead(TOUCH_XN);
        delay(1);
        d = analogRead(TOUCH_YP);
    } while ((absDiff(a, b) >8) || (absDiff(c, d) >8));
    
    if (x0>0) z0 = (4095-(c-a));
    else z0 = 0;
    
    //    delay(10);
    //    if (z0>0) {
    //        Serial.print("xyz0\t");
    //        Serial.print(x0, DEC);
    //        Serial.print("\t");
    //        Serial.print(y0, DEC);
    //        Serial.print("\t");
    //        Serial.print(z0, DEC);
    //        Serial.println();
    //    }
}

void Screen_K70::feedArea(uint16_t colour)
{
    // The check has a -5% impact.
    if (_fsmArea & 0xf0)
    {
        
        HWREG(LCD_CS_BASE + GPIO_O_DATA + (LCD_CS_PIN << 2)) = 0;                   // CS low
        
        // Colour variables have no impact.
        //          _writeData888((colour >>11) <<3, (colour >> 5) <<2, colour       <<3);
        
        uint8_t redColour8   =  (colour >>11) <<3;
        uint8_t greenColour8 =  (colour >> 5) <<2;
        uint8_t blueColour8  =   colour       <<3;
        
        // The myColours.split has a -50% impact.
        //        uint8_t redColour8, greenColour8, blueColour8;
        //        myColours.split(colour, redColour8, greenColour8, blueColour8);
        
        _writeData888(redColour8, greenColour8, blueColour8);
        
        HWREG(LCD_CS_BASE + GPIO_O_DATA + (LCD_CS_PIN << 2)) = LCD_CS_PIN;          // CS high
    }
}

void Screen_K70::_setBacklight(bool state)
{
    // Doesn't work on the K70
    //        if (state)  _writeCommand8(SSD1963_SET_DISPLAY_ON);
    //        else        _writeCommand8(SSD1963_SET_DISPLAY_OFF);
}

void Screen_K70::_setIntensity(uint8_t intensity)
{
    ;
}

#endif // end __LM4F120H5QR__
