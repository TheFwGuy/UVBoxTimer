//
// Screen_K35.cpp
// Library C++ code
// ----------------------------------
// Developed with embedXcode
// http://embedXcode.weebly.com
//
// Project LCD_screen Library Suite
//
// Created by Rei VILO, Jun 29, 2013
// embedXcode.weebly.com
//
//
// Copyright © Rei VILO, 2013-2014
// License All rights reserved
//
// See LCD_document.h and ReadMe.txt for references
//

#if defined(__LM4F120H5QR__)

// Library header
#include "screen_K35.h"
#include "inc/hw_gpio.h"
#include "driverlib/sysctl.h"

///
/// @name	SSD2119 constants
///
/// @{

#define SSD2119_DEVICE_CODE_READ_REG  0x00
#define SSD2119_OSC_START_REG         0x00
#define SSD2119_OUTPUT_CTRL_REG       0x01
#define SSD2119_LCD_DRIVE_AC_CTRL_REG 0x02
#define SSD2119_PWR_CTRL_1_REG        0x03
#define SSD2119_DISPLAY_CTRL_REG      0x07
#define SSD2119_FRAME_CYCLE_CTRL_REG  0x0B
#define SSD2119_PWR_CTRL_2_REG        0x0C
#define SSD2119_PWR_CTRL_3_REG        0x0D
#define SSD2119_PWR_CTRL_4_REG        0x0E
#define SSD2119_GATE_SCAN_START_REG   0x0F
#define SSD2119_SLEEP_MODE_REG        0x10
#define SSD2119_ENTRY_MODE_REG        0x11
#define SSD2119_GEN_IF_CTRL_REG       0x15
#define SSD2119_PWR_CTRL_5_REG        0x1E
#define SSD2119_RAM_DATA_REG          0x22
#define SSD2119_FRAME_FREQ_REG        0x25
#define SSD2119_VCOM_OTP_1_REG        0x28
#define SSD2119_VCOM_OTP_2_REG        0x29
#define SSD2119_GAMMA_CTRL_1_REG      0x30
#define SSD2119_GAMMA_CTRL_2_REG      0x31
#define SSD2119_GAMMA_CTRL_3_REG      0x32
#define SSD2119_GAMMA_CTRL_4_REG      0x33
#define SSD2119_GAMMA_CTRL_5_REG      0x34
#define SSD2119_GAMMA_CTRL_6_REG      0x35
#define SSD2119_GAMMA_CTRL_7_REG      0x36
#define SSD2119_GAMMA_CTRL_8_REG      0x37
#define SSD2119_GAMMA_CTRL_9_REG      0x3A
#define SSD2119_GAMMA_CTRL_10_REG     0x3B
#define SSD2119_V_RAM_POS_REG         0x44
#define SSD2119_H_RAM_START_REG       0x45
#define SSD2119_H_RAM_END_REG         0x46
#define SSD2119_X_RAM_ADDR_REG        0x4E
#define SSD2119_Y_RAM_ADDR_REG        0x4F

#define ENTRY_MODE_DEFAULT 0x6830
#define MAKE_ENTRY_MODE(x) ((ENTRY_MODE_DEFAULT & 0xFF00) | (x))

#define K35_WIDTH       320 // Vertical
#define K35_HEIGHT      240 // Horizontal

/// @}

//
// LCD control line GPIO definitions.
//
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

///
/// @brief	Low level write to 8-bit parallel bus
/// @param	data8 byte to write
///
inline void SET_LCD_DATA(uint8_t data8)
{
    HWREG(GPIO_PORTB_BASE + GPIO_O_DATA + (0xff << 2)) = (data8);
}

///
/// @brief	Low level read to 8-bit parallel bus
/// @return	value read, uint8_t
///
inline uint8_t GET_LCD_DATA()
{
    return HWREG(GPIO_PORTB_BASE + GPIO_O_DATA + (0xff << 2));
}

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
Screen_K35::Screen_K35() {
    ;
}

void Screen_K35::begin()
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
    
    //
    // Default values
    //
    GPIOPinWrite(LCD_CS_BASE, LCD_CS_PIN, LCD_CS_PIN);
    GPIOPinWrite(GPIO_PORTB_BASE, 0xff, 0x00);
    GPIOPinWrite(LCD_DC_BASE, LCD_DC_PIN, 0x00);
    GPIOPinWrite(LCD_RD_BASE, LCD_RD_PIN, LCD_RD_PIN);
    GPIOPinWrite(LCD_WR_BASE, LCD_WR_PIN, LCD_WR_PIN);
    GPIOPinWrite(LCD_CS_BASE, LCD_CS_PIN, 0);
    
    delay(2);
    
    //
    // Enter sleep mode
    //
    _writeCommand16(SSD2119_SLEEP_MODE_REG);
    _writeData16(0x0001);
    
    //
    // Power parameters
    //
    _writeCommand16(SSD2119_PWR_CTRL_5_REG);
    _writeData16(0x00BA);
    _writeCommand16(SSD2119_VCOM_OTP_1_REG);
    _writeData16(0x0006);
    
    //
    // Start oscillator
    //
    _writeCommand16(SSD2119_OSC_START_REG);
    _writeData16(0x0001);
    
    //
    // Set pixel format and basic display orientation
    //
    _writeCommand16(SSD2119_OUTPUT_CTRL_REG);
    _writeData16(0x30EF);
    _writeCommand16(SSD2119_LCD_DRIVE_AC_CTRL_REG);
    _writeData16(0x0600);
    
    //
    // Exit sleep mode
    //
    _writeCommand16(SSD2119_SLEEP_MODE_REG);
    _writeData16(0x0000);
    
    delay(30);
    
    //
    // Pixel color format
    //
    _writeCommand16(SSD2119_ENTRY_MODE_REG);
    _writeData16(ENTRY_MODE_DEFAULT);
    
    //
    // Enable  display
    //
    _writeCommand16(SSD2119_DISPLAY_CTRL_REG);
    _writeData16(0x0033);
    
    //
    // Set VCIX2 voltage to 6.1V
    //
    _writeCommand16(SSD2119_PWR_CTRL_2_REG);
    _writeData16(0x0005);
    
    //
    // Gamma correction
    //
    _writeCommand16(SSD2119_GAMMA_CTRL_1_REG);
    _writeData16(0x0000);
    _writeCommand16(SSD2119_GAMMA_CTRL_2_REG);
    _writeData16(0x0400);
    _writeCommand16(SSD2119_GAMMA_CTRL_3_REG);
    _writeData16(0x0106);
    _writeCommand16(SSD2119_GAMMA_CTRL_4_REG);
    _writeData16(0x0700);
    _writeCommand16(SSD2119_GAMMA_CTRL_5_REG);
    _writeData16(0x0002);
    _writeCommand16(SSD2119_GAMMA_CTRL_6_REG);
    _writeData16(0x0702);
    _writeCommand16(SSD2119_GAMMA_CTRL_7_REG);
    _writeData16(0x0707);
    _writeCommand16(SSD2119_GAMMA_CTRL_8_REG);
    _writeData16(0x0203);
    _writeCommand16(SSD2119_GAMMA_CTRL_9_REG);
    _writeData16(0x1400);
    _writeCommand16(SSD2119_GAMMA_CTRL_10_REG);
    _writeData16(0x0F03);
    
    //
    // Configure Vlcd63 and VCOMl
    //
    _writeCommand16(SSD2119_PWR_CTRL_3_REG);
    _writeData16(0x0007);
    _writeCommand16(SSD2119_PWR_CTRL_4_REG);
    _writeData16(0x3100);
    
    //
    // Display size and GRAM window
    //
    _writeCommand16(SSD2119_V_RAM_POS_REG);
    _writeData16((K35_HEIGHT-1) << 8);
    _writeCommand16(SSD2119_H_RAM_START_REG);
    _writeData16(0x0000);
    _writeCommand16(SSD2119_H_RAM_END_REG);
    _writeData16(K35_WIDTH-1);
    _writeCommand16(SSD2119_X_RAM_ADDR_REG);
    _writeData16(0x00);
    _writeCommand16(SSD2119_Y_RAM_ADDR_REG);
    _writeData16(0x00);
    
    // Standard
    setOrientation(1);
    
    _screenWidth  = K35_HEIGHT;
    _screenHeigth = K35_WIDTH;
    _screenDiagonal = 35;

    // Touch
    uint16_t x0, y0, z0;
    _getRawTouch(x0, y0, z0);
    
    _touchTrim = TOUCH_TRIM;
    _touchXmin = 2937;
    _touchXmax = 1009;
    _touchYmin = 3255;
    _touchYmax = 748;
    
    _penSolid  = false;
    _fontSolid = true;
    _flagRead  = true;
    _fsmArea   = true;
    _touchTrim = 10;
    clear();
}

String Screen_K35::WhoAmI()
{
    return "Kentec 3.5\" screen";
}

uint16_t Screen_K35::readPixel(uint16_t x1, uint16_t y1)
{
	_setCursor(x1, y1);
	return _readData16();
}

void Screen_K35::copyPasteArea(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t dx, uint16_t dy)
{
    uint16_t buffer[dx];
    
    _closeWindow();
    
    for (uint16_t y=0; y<dy; y++) {
        for (uint16_t x=0; x<dx; x++) {
            _setCursor(x1+x, y1+y);
            buffer[x] = _readData16();
        }
        
        _setWindow(x2, y2+y, x2+dx, y2+y);
        
        for (uint16_t x=0; x<dx; x++) {
            _writeData16(buffer[x]);
        }
    }
    _closeWindow();
}

void Screen_K35::_setOrientation(uint8_t orientation)
{
    //_orientation = orientation % 4;
//    LCD_screen_font::setOrientation(orientation);
    
    //
    // default = 0x6830 = 0x68 <<8 + 0b00110000
    switch (_orientation) {
        case 0:
            //                                                  hvO
            _writeRegister(SSD2119_ENTRY_MODE_REG, 0x6800 + 0b00101000);        // ok
            break;
        case 1:
            //                                                  hvO
            _writeRegister(SSD2119_ENTRY_MODE_REG, 0x6800 + 0b00000000);        // ok
            break;
        case 2:
            //                                                  hvO
            _writeRegister(SSD2119_ENTRY_MODE_REG, 0x6800 + 0b00011000);        // ok
            break;
        case 3:
            //                                                  hvO
            _writeRegister(SSD2119_ENTRY_MODE_REG, 0x6800 + 0b00110000);        // ok
            break;
    }
}

// Utilities
void Screen_K35::_writeData16(uint16_t data16)
{
    _writeData88(data16 >> 8, data16);
}

void Screen_K35::_writeData88(uint8_t dataHigh8, uint8_t dataLow8)
{
	//
    // Write the most significant byte of the data to the bus.
    //
    SET_LCD_DATA(dataHigh8);
    
	HWREG(LCD_CS_BASE + GPIO_O_DATA + (LCD_CS_PIN << 2)) = 0;                   // CS LOW
    
    //
    // Assert the write enable signal.  Do this three times to ensure that we
    // meet the display timing requirements (20nS per instruction at 50MHz
    // means we need to ensure at least 3 instructions between edges on WR)
    //
    HWREG(LCD_WR_BASE + GPIO_O_DATA + (LCD_WR_PIN << 2)) = 0;
    HWREG(LCD_WR_BASE + GPIO_O_DATA + (LCD_WR_PIN << 2)) = 0;
    
    //
    // Deassert the write enable signal.
    //
    HWREG(LCD_WR_BASE + GPIO_O_DATA + (LCD_WR_PIN << 2)) = LCD_WR_PIN;
    
    //
    // Write the least significant byte of the data to the bus.
    //
    SET_LCD_DATA(dataLow8);
    
    //
    // Assert the write enable signal.
    //
    HWREG(LCD_WR_BASE + GPIO_O_DATA + (LCD_WR_PIN << 2)) = 0;
    HWREG(LCD_WR_BASE + GPIO_O_DATA + (LCD_WR_PIN << 2)) = 0;
    
    //
    // Deassert the write enable signal.
    //
    HWREG(LCD_WR_BASE + GPIO_O_DATA + (LCD_WR_PIN << 2)) = LCD_WR_PIN;
    
    HWREG(LCD_CS_BASE + GPIO_O_DATA + (LCD_CS_PIN << 2)) = LCD_CS_PIN;          // CS HIGH
    
}

//*****************************************************************************
//
// Writes a command to the SSD2119.  This function implements the basic GPIO
// interface to the LCD display.
//
//*****************************************************************************
void Screen_K35::_writeCommand16(uint16_t command16)
{
    //
    // Write the most significant byte of the data to the bus. This is always
    // 0 since commands are no more than 8 bits currently.
    //
    SET_LCD_DATA(command16 >>8);
    
	HWREG(LCD_CS_BASE + GPIO_O_DATA + (LCD_CS_PIN << 2)) = 0;                   // CS LOW
    
    //
    // Assert the write enable and DC signals.  Do this 3 times to slow things
    // down a bit.
    //
    HWREG(LCD_WR_BASE + GPIO_O_DATA + ((LCD_WR_PIN | LCD_DC_PIN) << 2)) = 0;
    HWREG(LCD_WR_BASE + GPIO_O_DATA + ((LCD_WR_PIN | LCD_DC_PIN) << 2)) = 0;
    
    //
    // Deassert the write enable and DC signals. We need to leave WR high for
    // at least 50nS so, again, stick in dummy writes to pad the timing.
    //
    HWREG(LCD_WR_BASE + GPIO_O_DATA + (LCD_WR_PIN << 2)) = LCD_WR_PIN;
    HWREG(LCD_WR_BASE + GPIO_O_DATA + (LCD_WR_PIN << 2)) = LCD_WR_PIN;
    
    //
    // Write the least significant byte of the data to the bus.
    //
    SET_LCD_DATA(command16);
    
    //
    // Assert the write enable signal.  Again, do this three times to ensure
    // we meet the timing spec.
    //
    HWREG(LCD_WR_BASE + GPIO_O_DATA + (LCD_WR_PIN << 2)) = 0;
    HWREG(LCD_WR_BASE + GPIO_O_DATA + (LCD_WR_PIN << 2)) = 0;
    
    //
    // Deassert the write enable and DC signals.  Make sure we add padding here
    // too since some compilers inline this function.
    //
    HWREG(LCD_WR_BASE + GPIO_O_DATA + (LCD_WR_PIN << 2)) = LCD_WR_PIN;
    HWREG(LCD_WR_BASE + GPIO_O_DATA + (LCD_DC_PIN << 2)) = LCD_DC_PIN;
    
	HWREG(LCD_CS_BASE + GPIO_O_DATA + (LCD_CS_PIN << 2)) = LCD_CS_PIN;          // CS HIGH
}

uint16_t Screen_K35::_readData16()
{
    uint16_t data16 = 0;
    
	// Switch to INPUT mode
    GPIOPinTypeGPIOInput(GPIO_PORTB_BASE, 0xff);
    
	HWREG(LCD_CS_BASE + GPIO_O_DATA + (LCD_CS_PIN << 2)) = 0;                   // CS LOW
    
    //
    // Assert the read enable signal.  Do this three times to ensure that we
    // meet the display timing requirements (20nS per instruction at 50MHz
    // means we need to ensure at least 3 instructions between edges on WR)
    //
    // Assert the read enable signal. x10 = 200 ns
    //
    HWREG(LCD_RD_BASE + GPIO_O_DATA + (LCD_RD_PIN << 2)) = 0;
    HWREG(LCD_RD_BASE + GPIO_O_DATA + (LCD_RD_PIN << 2)) = 0;
    HWREG(LCD_RD_BASE + GPIO_O_DATA + (LCD_RD_PIN << 2)) = 0;
    HWREG(LCD_RD_BASE + GPIO_O_DATA + (LCD_RD_PIN << 2)) = 0;
    HWREG(LCD_RD_BASE + GPIO_O_DATA + (LCD_RD_PIN << 2)) = 0;
    HWREG(LCD_RD_BASE + GPIO_O_DATA + (LCD_RD_PIN << 2)) = 0;
    HWREG(LCD_RD_BASE + GPIO_O_DATA + (LCD_RD_PIN << 2)) = 0;
    HWREG(LCD_RD_BASE + GPIO_O_DATA + (LCD_RD_PIN << 2)) = 0;
    
    //
    // Read the dummy first sbyte of the data from the bus.
    //
    GET_LCD_DATA();
    
    //
    // Deassert the write enable signal. x5 = 100 ns
    //
    HWREG(LCD_RD_BASE + GPIO_O_DATA + (LCD_RD_PIN << 2)) = LCD_RD_PIN;
    HWREG(LCD_RD_BASE + GPIO_O_DATA + (LCD_RD_PIN << 2)) = LCD_RD_PIN;
    HWREG(LCD_RD_BASE + GPIO_O_DATA + (LCD_RD_PIN << 2)) = LCD_RD_PIN;
    HWREG(LCD_RD_BASE + GPIO_O_DATA + (LCD_RD_PIN << 2)) = LCD_RD_PIN;
    
    //
    // Assert the read enable signal. x10 = 200 ns
    //
    HWREG(LCD_RD_BASE + GPIO_O_DATA + (LCD_RD_PIN << 2)) = 0;
    HWREG(LCD_RD_BASE + GPIO_O_DATA + (LCD_RD_PIN << 2)) = 0;
    HWREG(LCD_RD_BASE + GPIO_O_DATA + (LCD_RD_PIN << 2)) = 0;
    HWREG(LCD_RD_BASE + GPIO_O_DATA + (LCD_RD_PIN << 2)) = 0;
    
    //
    // Read the most significant byte of the data from the bus.
    //
    data16   = GET_LCD_DATA() <<8;
    
    //
    // Deassert the read enable signal. x5 = 100 ns
    //
    HWREG(LCD_RD_BASE + GPIO_O_DATA + (LCD_RD_PIN << 2)) = LCD_RD_PIN;
    HWREG(LCD_RD_BASE + GPIO_O_DATA + (LCD_RD_PIN << 2)) = LCD_RD_PIN;
    HWREG(LCD_RD_BASE + GPIO_O_DATA + (LCD_RD_PIN << 2)) = LCD_RD_PIN;
    HWREG(LCD_RD_BASE + GPIO_O_DATA + (LCD_RD_PIN << 2)) = LCD_RD_PIN;
    
    //
    // Assert the read enable signal. x10 = 200 ns
    //
    HWREG(LCD_RD_BASE + GPIO_O_DATA + (LCD_RD_PIN << 2)) = 0;
    HWREG(LCD_RD_BASE + GPIO_O_DATA + (LCD_RD_PIN << 2)) = 0;
    HWREG(LCD_RD_BASE + GPIO_O_DATA + (LCD_RD_PIN << 2)) = 0;
    HWREG(LCD_RD_BASE + GPIO_O_DATA + (LCD_RD_PIN << 2)) = 0;
    
    //
    // Read the least significant byte of the data from the bus.
    //
    data16   |= GET_LCD_DATA();
    
    //
    // Deassert the read enable signal.
    //
    HWREG(LCD_RD_BASE + GPIO_O_DATA + (LCD_RD_PIN << 2)) = LCD_RD_PIN;
    HWREG(LCD_RD_BASE + GPIO_O_DATA + (LCD_RD_PIN << 2)) = LCD_RD_PIN;
    HWREG(LCD_RD_BASE + GPIO_O_DATA + (LCD_RD_PIN << 2)) = LCD_RD_PIN;
    HWREG(LCD_RD_BASE + GPIO_O_DATA + (LCD_RD_PIN << 2)) = LCD_RD_PIN;
    //    HWREG(LCD_RD_BASE + GPIO_O_DATA + (LCD_RD_PIN << 2)) = LCD_RD_PIN;
    
    HWREG(LCD_CS_BASE + GPIO_O_DATA + (LCD_CS_PIN << 2)) = LCD_CS_PIN;          // CS HIGH
    
    // Return to OUTPUT mode
    GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, 0xff);
    
    return data16;
}

void Screen_K35::_writeRegister(uint8_t command8, uint16_t data16)
{
    _writeCommand16(command8);
    _writeData16(data16);
}


void Screen_K35::_orientCoordinates(uint16_t &x1, uint16_t &y1)
{
	switch (_orientation) {
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

void Screen_K35::_setPoint(uint16_t x1, uint16_t y1, uint16_t colour)           // compulsory
{
    _setCursor(x1, y1);
    _writeData16(colour);
}

void Screen_K35::_setCursor(uint16_t x1, uint16_t y1)
{
    _orientCoordinates(x1, y1);
    _writeRegister(SSD2119_X_RAM_ADDR_REG, x1);
    _writeRegister(SSD2119_Y_RAM_ADDR_REG, y1);
    
    _writeCommand16(SSD2119_RAM_DATA_REG);
}

void Screen_K35::_setWindow(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    _orientCoordinates(x1, y1);
    _orientCoordinates(x2, y2);
    
    _writeRegister(SSD2119_X_RAM_ADDR_REG, x1);
    _writeRegister(SSD2119_Y_RAM_ADDR_REG, y1);
    
    if (x1 > x2) _swap(x1, x2);
    if (y1 > y2) _swap(y1, y2);
    
    _writeCommand16(SSD2119_V_RAM_POS_REG);
    _writeData88(y2, y1);
    _writeRegister(SSD2119_H_RAM_START_REG, x1);
    _writeRegister(SSD2119_H_RAM_END_REG, x2);
    
    _writeCommand16(SSD2119_RAM_DATA_REG);
}

void Screen_K35::_closeWindow()
{
    _setWindow(0, 0, screenSizeX()-1, screenSizeY()-1);
}

inline uint16_t absDiff(uint16_t a, uint16_t b) { return (a > b) ? a-b : b-a; }


void Screen_K35::_fastFill(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t colour)
{
    if (x1 > x2) _swap(x1, x2);
    if (y1 > y2) _swap(y1, y2);
    
    uint8_t highColour = highByte(colour);
    uint8_t lowColour  = lowByte(colour);
    
    _setWindow(x1, y1, x2, y2);
    for (uint32_t t=(uint32_t)(y2-y1+1)*(x2-x1+1); t>0; t--) {
        _writeData88(highColour, lowColour);
    }
}

// Touch
void Screen_K35::_getRawTouch(uint16_t &x0, uint16_t &y0, uint16_t &z0)
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

void Screen_K35::_setBacklight(bool flag)
{
    if (flag)   _writeRegister(SSD2119_SLEEP_MODE_REG, 0);
    else        _writeRegister(SSD2119_SLEEP_MODE_REG, 1);
}

void Screen_K35::_setIntensity(uint8_t intensity)
{
    ;
}


#endif // end __LM4F120H5QR__
