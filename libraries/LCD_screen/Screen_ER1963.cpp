//
// Screen_ER1963.cpp
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
// Copyright © Rei VILO, 2013-2014
// License All rights reserved
//
// See LCD_documentation.h and ReadMe.txt for references
//

// Library header
#include "Screen_ER1963.h"
#if defined(__LM4F120H5QR__) || defined(__TM4C1230C3PM__) || defined(__TM4C129XNCZAD__) || defined(__TM4C1294NCPDT__)
#include "inc/hw_gpio.h"
#include "driverlib/sysctl.h"
#else
#endif // end __LM4F120H5QR__
//

///
/// @name	SSD1963 constants
///
/// @{

#define SSD1963_NOP                     0x00
#define SSD1963_SOFT_RESET              0x01
#define SSD1963_GET_POWER_MODE          0x0a
#define SSD1963_GET_ADDRESS_MODE        0x0b
#define SSD1963_RESERVED_1              0x0c
#define SSD1963_GET_DISPLAY_MODE        0x0d
#define SSD1963_GET_TEAR_EFFECT_STATUS	0x0e
#define SSD1963_RESERVED_2              0x0f
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
#define SSD1963_RESERVED_3              0x3a
#define SSD1963_WRITE_MEMORY_CONTINUE	0x3c
#define SSD1963_READ_MEMORY_CONTINUE	0x3e
#define SSD1963_SET_TEAR_SCANLINE       0x44
#define SSD1963_GET_SCANLINE            0x45
#define SSD1963_READ_DDB                0xa1
#define SSD1963_RESERVED_4              0xa8
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
#define SSD1963_RESERVED_5              0xe8
#define SSD1963_RESERVED_6              0xe9
#define SSD1963_SET_PIXEL_DATA_INTERFACE	0xf0
#define SSD1963_GET_PIXEL_DATA_INTERFACE	0xf1
#define SSD1963_RESERVED                0xff


#ifdef RESOLUTION480x272
	// Hor front porch  Typc 2 -> 4
	#define   HPS        70
	// horizontal pulse width min 2 typ 41 -> 42
	#define   HPW        15
	// horizontal back porch not used on register coount on HT and LPS min 2 -> 4
	#define	  HBKP		4
	// line position display start, must be front + back porch + HPW (50)
	#define   LPS        0
	//HPS+HPW+HBKP
	// sub pixel clock adjust
	#define SUB_LPS		0
	// image display size -1
	#define   HDP        ER1963_WIDTH-1
	// horizontal line tot clock pixel Sync Fporch Display Front porch
	#define   HT	440
	//HPW+HBKP+HDP+HPS
	// Vert front Porch
	#define   VPS        11
	// Vertcal pulse width
	#define   VPW        7
	// Vert Back Porch not on Controller but used  for calculation of dsplay positon
	#define   VBKP        4
	// Vert frame display position, must be back + front + VPW (20)
	#define   FPS        0
	//VBKP+VPW+VPS
	// Vertical number of display lines -1
	#define   VDP        ER1963_HEIGHT -1
	// vertical total horizontal lines (291)
	#define   VT       328
	//VPW+VBKP+VDP+VPS
	// set PLL Frequency as Reference * Multiplier/Divider
	// mul div are less than 1 so 0=>1 1=>2 and so on
	// reference -> 10MHz (see xtal on ssd board
	// 250Mhz < DCO < 800 MHz
	// system clock max 110MHz
	// Lcd dot clock divider clk 10MHz @pll 100MHz
	// Multiplier 30 -> PLL 300MHz divider = 3 so clock -> 300/3 -> 100MHz
	#define		LCDC_PLLMULT	29
	//47
	#define		LCDC_PLLDVDR	2
	//3
	// dot clock parameter:
	// Pixel Clock FPR = (2^20*PixelFreq/PLLFreq)-1
	//
	// Pixel freq = PLLFreq*(FPR+1)  /2^20
	//
	// 10MHz Dot clock result in
	// 1048576 * 10 / 120 -> 104857.6 rounded to 104856
	// newhaven use FPR 671088 -> clock
	#define		LCDC_FPR	0xa3d70
	//87380
	// Panel mode for display:
	// A5-> 0 18bit 1 2 4BIt 24bit A4/A3-> DC 18Bit A4-> FRC on/off A3-> 1Dthering 0 FRC   A2->Dot clock 0 fall 1 raisng A1 Horizontal A0 Vertcal : 0 active low 1 actve High
	#define		LCDC_INTF	0x24
	// Panel type
	// B7 B6 0xc0 Serial RGB mode+dummy 0x80  Serial RGB mode 0x40 00 TFT RGB mode
	#define		LCDC_TYPE	0x80
	// mode interlace G5..G0 odd even line RGB sequence
	#define	LCDC_INLC	0
	// Screen flip scan mode, RGB/BGR send
	#define DEFAULT_ADDRESS_MODE	3
	//0b00001011
	// 0 8 bit, 1 12bit 2 16bitpacked 3 16 bit(565) 4 18 bit 5 24bit 6 9 bit other reserved
	#define	PIXEL_DATA_INTERFACE_MODE	0

#else
	//#ifdef RESOLUTION800x480
	// Hor front porch  Typc 2 -> 4 (0xb4 -> 180)
	#define   HPS        180
	// horizontal pulse width min 2 typ 41 -> 42 (0x59-> 89)
	#define   HPW        89
	// horizontal back porch not used on register coount on HT and LPS min 2 -> 4
	#define	  HBKP		4
	// line position display start, must be front + back porch + HPW (50) (0???)
	#define   LPS        HPS+HPW+HBKP
	// sub pixel clock adjust
	#define SUB_LPS		0
	// image display size -1 (0x31f->799)
	#define   HDP        ER1963_WIDTH-1
	// horizontal line tot clock pixel Sync Fporch Display Front porch (0x3f8 -> 1016)
	#define   HT         HPW+HBKP+HDP+HPS
	// Vert front Porch (0x14-> 20)
	#define   VPS        20
	// Vertcal pulse width (9)
	#define   VPW        9
	// Vert Back Porch not on Controller but used  for calculation of dsplay positon
	#define   VBKP        4
	// Vert frame display position, must be back + front + VPW (0???)
	#define   FPS        VBKP+VPW+VPS
	// Vertical number of display lines -1 (0x1df -> 479)
	#define   VDP        ER1963_HEIGHT -1
	// vertical total horizontal lines (0x1fb -> 507)
	#define   VT         VPW+VBKP+VDP+VPS
	// set PLL Frequency as Reference * Multiplier/Divider
	// mul div are less than 1 so 0=>1 1=>2 and so on
	// reference -> 10MHz (see xtal on ssd board
	// 250Mhz < DCO < 800 MHz
	// Lcd dot clock divider clk 10MHz @pll 120MHz
	// Multiplier 48 -> PLL 480MHz divider = 4 so clock -> 480/4 -> 120MHz
	#define		LCDC_PLLMULT	47
	#define		LCDC_PLLDVDR	3
	// dot clock parameter:
	// 2^20*PixelFreq/PLLFreq
	// 10MHz Dot clock result in
	// 1048576 * 10 / 120 -> 87381.333333333 rounded to 87380
	#define		LCDC_FPR	0x3ffff
/*
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
     _writeCommand8(SSD1963_SET_LCD_MODE);	//LCD SPECIFICATION
     _writeData8(0x20);//24
     _writeData8(0x00);
 */
#endif

/// @}

#if defined(__LM4F120H5QR__) || defined(__TM4C1230C3PM__) || defined(__TM4C129XNCZAD__)
//
// LCD control line GPIO definitions.
//
#define LCD_DATAH_PERIPH        SYSCTL_PERIPH_GPIOB
#define LCD_DATAH_BASE          GPIO_PORTB_BASE
#define LCD_DATAH_PINS          0xff                                            // PB_0..PB_7

#ifndef LCDC_CS_ISGROUNDED
	#define LCD_CS_PERIPH           SYSCTL_PERIPH_GPIOA
	#define LCD_CS_BASE             GPIO_PORTA_BASE
	#define LCD_CS_PIN              GPIO_PIN_7                                      // PA_7
#endif
#define LCD_DC_PERIPH           SYSCTL_PERIPH_GPIOA
#define LCD_DC_BASE             GPIO_PORTA_BASE
#define LCD_DC_PIN              GPIO_PIN_6                                      // PA_6
#define LCD_WR_PERIPH           SYSCTL_PERIPH_GPIOA
#define LCD_WR_BASE             GPIO_PORTA_BASE
#define LCD_WR_PIN              GPIO_PIN_5                                      // PA_5
#define LCD_RD_PERIPH           SYSCTL_PERIPH_GPIOA
#define LCD_RD_BASE             GPIO_PORTA_BASE
#define LCD_RD_PIN              GPIO_PIN_4                                      // PA_4

#define LCD_PE0_PERIPH           SYSCTL_PERIPH_GPIOA
#define LCD_PE0_BASE             GPIO_PORTE_BASE
#define LCD_PE0_PIN              GPIO_PIN_0                                     // PE_0

//#define LCD_BACKLIGHT_PERIPH    SYSCTL_PERIPH_GPIOF
//#define LCD_BACKLIGHT_BASE      GPIO_PORTF_BASE
//#define LCD_BACKLIGHT_PIN       GPIO_PIN_3
#elif  defined(__TM4C1294NCPDT__)
/*
 *
 * LCD control line GPIO definitions.
 * 1294 has no complete port B so due to pin mapping a split is done across two port
 *
 *
 *
 * Pin assignment:
 * data d0 d3: PD_0..3 d4..d7: PM4..7
 *  cs GND res PN_0 rs PN_1 rd PP_5 wr PP_4
 *
 * miso PQ mosi PQ clk PQ sdcs PQ tccs PL_3
 *
 */
/*RS  = PN_1;	//18;    // RS signal connected to Arduino digital pin 30
WR  = PP_4;	//12;    // /WR signal connected to Arduino digital pin 31
RD  = PP_5;	//11;    // /RD signal connected to Arduino digital pin 32
RES = PN_0;	//13;    // /RES signal connected to Arduino digital pin 33
// int CS  = 19;
// /CS signal tied to GND
SDCS = PQ_2;
TCCS = PL_3;
*/

#define LCD_DATAH_PERIPH        SYSCTL_PERIPH_GPIOM
#define LCD_DATAH_BASE          GPIO_PORTM_BASE
#define LCD_DATAH_PINS          0xf0                                            // PM_4..PM_7

#define LCD_DATAL_PERIPH        SYSCTL_PERIPH_GPIOD
#define LCD_DATAL_BASE          GPIO_PORTD_BASE
#define LCD_DATAL_PINS          0x0f                                            // PD_0..PD_3

#ifndef LCDC_CS_ISGROUNDED
	#define LCD_CS_PERIPH           SYSCTL_PERIPH_GPIOA
	#define LCD_CS_BASE             GPIO_PORTA_BASE
	#define LCD_CS_PIN              GPIO_PIN_7                                      // PA_7
#endif
#define LCD_RST_PERIPH           SYSCTL_PERIPH_GPION
#define LCD_RST_BASE             GPIO_PORTN_BASE
#define LCD_RST_PIN              GPIO_PIN_0                                      // PA_6
#define LCD_DC_PERIPH           SYSCTL_PERIPH_GPION
#define LCD_DC_BASE             GPIO_PORTN_BASE
#define LCD_DC_PIN              GPIO_PIN_1                                      // PA_6
#define LCD_WR_PERIPH           SYSCTL_PERIPH_GPIOP
#define LCD_WR_BASE             GPIO_PORTP_BASE
#define LCD_WR_PIN              GPIO_PIN_4                                      // PA_5
#define LCD_RD_PERIPH           SYSCTL_PERIPH_GPIOP
#define LCD_RD_BASE             GPIO_PORTP_BASE
#define LCD_RD_PIN              GPIO_PIN_5                                      // PA_4
/*
#define LCD_PE0_PERIPH           SYSCTL_PERIPH_GPIOA
#define LCD_PE0_BASE             GPIO_PORTE_BASE
#define LCD_PE0_PIN              GPIO_PIN_0                                     // PE_0
*/
//#define LCD_BACKLIGHT_PERIPH    SYSCTL_PERIPH_GPIOF
//#define LCD_BACKLIGHT_BASE      GPIO_PORTF_BASE
//#define LCD_BACKLIGHT_PIN       GPIO_PIN_3
#else
//
// LCD control line GPIO definitions.
//
#define LCD_DATAH_PORTIN		P6IN
#define LCD_DATAH_PORTOUT		P6OUT
#define LCD_DATAH_PORTDIR		P6DIR
#define LCD_DATAH_PINS			0xff                                            // PB_0..PB_7

#ifndef LCDC_CS_ISGROUNDED
#define LCD_CS_PORTOUT			P2OUT
#define LCD_CS_PORTDIR			P2DIR
#define LCD_CS_PIN              0X01                                      // PA_7 bit 0
#endif
#define LCD_DC_PORTOUT			P2OUT
#define LCD_DC_PORTDIR			P2DIR
#define LCD_DC_PIN              0X04                                      // PA_6 bit 2
#define LCD_WR_PORTOUT			P2OUT
#define LCD_WR_PORTDIR			P2DIR
#define LCD_WR_PIN              0X08                                      // PA_5 bit 3
#define LCD_RD_PORTOUT			P8OUT
#define LCD_RD_PORTDIR			P8DIR
#define LCD_RD_PIN              0X02                                      // PA_4 bit 1
#define LCD_RST_PORTOUT			P2OUT
#define LCD_RST_PORTDIR			P2DIR
#define LCD_RST_PIN              0X40                                      // PA_4 bit 6

#endif // end __LM4F120H5QR__


// Touch constants
#define TOUCH_X     0xd0
#define TOUCH_Y     0x90
#define TOUCH_Z1    0xb0
#define TOUCH_Z2    0xc0
#define TOUCH_TRIM  0x10 // Touch threshold

// Variables

///
/// @brief	Low level write to 8-bit parallel bus
/// @param	data8 byte to write
///
inline void SET_LCD_DATA(uint8_t data8)
{
#if defined(__LM4F120H5QR__) || defined(__TM4C1230C3PM__) || defined(__TM4C129XNCZAD__) || defined(__TM4C1294NCPDT__)
    HWREG(LCD_DATAH_BASE + GPIO_O_DATA + (LCD_DATAH_PINS << 2)) = (data8);
	#if  defined(__TM4C1294NCPDT__)
	HWREG(LCD_DATAL_BASE + GPIO_O_DATA + (LCD_DATAL_PINS << 2)) = (data8);
	#endif
#else
    LCD_DATAH_PORTOUT = data8;
#endif // end __LM4F120H5QR__
}
/*#define SET_LCD_DATA(ucByte)                                                  \
//{                                                                             \
//    HWREG(LCD_DATAH_BASE + GPIO_O_DATA + (LCD_DATAH_PINS << 2)) = (ucByte);   \
//}
*/

///
/// @brief	Low level read to 8-bit parallel bus
/// @return	value read, uint8_t
///
inline uint8_t GET_LCD_DATA()
{
#if defined(__LM4F120H5QR__) || defined(__TM4C1230C3PM__) || defined(__TM4C129XNCZAD__)
    return HWREG(LCD_DATAH_BASE + GPIO_O_DATA + (LCD_DATAH_PINS << 2));
#elif  defined(__TM4C1294NCPDT__)
    return HWREG(LCD_DATAH_BASE + GPIO_O_DATA + (LCD_DATAH_PINS << 2)) | HWREG(LCD_DATAL_BASE + GPIO_O_DATA + (LCD_DATAL_PINS << 2));
#else
    return (LCD_DATAH_PORTIN);
#endif // end __LM4F120H5QR__

}
//#define GET_LCD_DATA() HWREG(LCD_DATAH_BASE + GPIO_O_DATA + (LCD_DATAH_PINS << 2))

///
/// @name   Touch constants
///
/// @{

#define TOUCH_TRIM  0x10  ///< Touch threshold
//#define _pinTouchXR PE_4  ///< XP *analog
//#define _pinTouchYU PE_5  ///< YP *analog
//#define _pinTouchXL PA_2  ///< XN analog or *digital
//#define _pinTouchYD PA_3  ///< YN analog or *digital

// TOUCH_XP PE4 Resistor touch screen terminal (Left)
// TOUCH_YP PE5 Resistor touch screen terminal (Top)
// TOUCH_XN PA3 Resistor touch screen terminal (Right)
// TOUCH_YN PA2 Resistor touch screen terminal (Bottom)
//#define TOUCH_XP A8      // PE_5 no! // must be an analog pin, use "An" notation!
//#define TOUCH_YP A9      // PE_4 no! // must be an analog pin, use "An" notation!
//#define TOUCH_XN PA_2    // can be a digital pin
//#define TOUCH_YN PA_3    // can be a digital pin

/// @}



// Code
Screen_ER1963::Screen_ER1963(uint8_t pinScreenCS, uint8_t pinTouchCS)
{
#if defined(__LM4F120H5QR__) || defined(__TM4C1230C3PM__) || defined(__TM4C129XNCZAD__)
    _pinScreenCS   = pinScreenCS;
    _pinTouchCS    = pinTouchCS;
    
    _pinTouchMISO  = 34; // PC_7
    _pinTouchMOSI  = 35; // PC_6
    _pinTouchClock = 36; // PC_5
#elif  defined(__TM4C1294NCPDT__)
    /*RS  = PN_1;	//18;    // RS signal connected to Arduino digital pin 30
    WR  = PP_4;	//12;    // /WR signal connected to Arduino digital pin 31
    RD  = PP_5;	//11;    // /RD signal connected to Arduino digital pin 32
    RES = PN_0;	//13;    // /RES signal connected to Arduino digital pin 33
    */
    // int CS  = 19;
    // /CS signal tied to GND
//    SDCS = PQ_2;
//    TCCS = PL_3;

#ifndef LCDC_CS_ISGROUNDED
    _pinScreenCS   = -1; // pinScreenCS;
#else
    _pinScreenCS   = 19; // pinScreenCS;
#endif
    _pinTouchCS    = PL_3; // pinTouchCS;

    _pinTouchMISO  = PQ_3; // PC_7
    _pinTouchMOSI  = PQ_2; // PC_6
    _pinTouchClock = PQ_0; // PC_5
#else
#ifndef LCDC_CS_ISGROUNDED
    _pinScreenCS   = -1; // pinScreenCS;
#else
    _pinScreenCS   = 8; // pinScreenCS;
#endif
    _pinTouchCS    = 19; // pinTouchCS;

    _pinTouchMISO  = 14; // PC_7
    _pinTouchMOSI  = 15; // PC_6
    _pinTouchClock = 7; // PC_5
#endif // end __LM4F120H5QR__
}

void Screen_ER1963::begin()
{
//    // Backlight PWM pin
//    pinMode(40, OUTPUT);
//    digitalWrite(40, HIGH);
// Backlight managed by controller
    
    // software SPI for touch
    pinMode(_pinTouchCS, OUTPUT);
    pinMode(_pinTouchClock, OUTPUT);
    pinMode(_pinTouchMOSI, OUTPUT);
    pinMode(_pinTouchMISO, INPUT);
    
#if defined(__LM4F120H5QR__) || defined(__TM4C1230C3PM__) || defined(__TM4C129XNCZAD__) || defined(__TM4C1294NCPDT__)
	//
    // Enable GPIOs
    //
	SysCtlPeripheralEnable(LCD_DATAH_PERIPH);
#if  defined(__TM4C1294NCPDT__)
	SysCtlPeripheralEnable(LCD_DATAL_PERIPH);
#endif
    SysCtlPeripheralEnable(LCD_RST_PERIPH);
    SysCtlPeripheralEnable(LCD_DC_PERIPH);
    SysCtlPeripheralEnable(LCD_RD_PERIPH);
    SysCtlPeripheralEnable(LCD_WR_PERIPH);
#ifndef LCDC_CS_ISGROUNDED
    SysCtlPeripheralEnable(LCD_CS_PERIPH);
#endif
    
    //
    // GPIOs as OUTPUT
    //
    GPIOPinTypeGPIOOutput(LCD_DATAH_BASE, LCD_DATAH_PINS);
#if  defined(__TM4C1294NCPDT__)
    GPIOPinTypeGPIOOutput(LCD_DATAL_BASE, LCD_DATAL_PINS);
#endif

    GPIOPinTypeGPIOOutput(LCD_RST_BASE, LCD_RST_PIN);
    GPIOPinTypeGPIOOutput(LCD_DC_BASE, LCD_DC_PIN);
    GPIOPinTypeGPIOOutput(LCD_RD_BASE, LCD_RD_PIN);
    GPIOPinTypeGPIOOutput(LCD_WR_BASE, LCD_WR_PIN);
#ifndef LCDC_CS_ISGROUNDED
    GPIOPinTypeGPIOOutput(LCD_CS_BASE, LCD_CS_PIN);
#endif
    
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
#if  defined(__TM4C1294NCPDT__)
    GPIOPadConfigSet(LCD_DATAL_BASE, LCD_DATAL_PINS, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD);
#endif
    //
    //    GPIODirModeSet(LCD_RST_BASE, LCD_RST_PIN, GPIO_DIR_MODE_OUT);
    GPIOPadConfigSet(LCD_RST_BASE, LCD_RST_PIN, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD);
    //    GPIODirModeSet(LCD_DC_BASE, LCD_DC_PIN, GPIO_DIR_MODE_OUT);
    GPIOPadConfigSet(LCD_DC_BASE, LCD_DC_PIN, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD);
    //    GPIODirModeSet(LCD_RD_BASE, LCD_RD_PIN, GPIO_DIR_MODE_OUT);
    GPIOPadConfigSet(LCD_RD_BASE, LCD_RD_PIN, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD);
    //    GPIODirModeSet(LCD_WR_BASE, LCD_WR_PIN, GPIO_DIR_MODE_OUT);
    GPIOPadConfigSet(LCD_WR_BASE, LCD_WR_PIN, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD);
#ifndef LCDC_CS_ISGROUNDED
    //    GPIODirModeSet(LCD_CS_BASE, LCD_CS_PIN, GPIO_DIR_MODE_OUT);
    GPIOPadConfigSet(LCD_CS_BASE, LCD_CS_PIN, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD);
    //    //            GPIODirModeSet(LCD_BACKLIGHT_BASE, LCD_BACKLIGHT_PIN, GPIO_DIR_MODE_OUT);
    //    //            GPIOPadConfigSet(LCD_BACKLIGHT_BASE, LCD_BACKLIGHT_PIN, GPIO_STRENGTH_8MA,
    //    //                             GPIO_PIN_TYPE_STD);
    
    //
    // Set the LCD control pins to their default values.
    //
    GPIOPinWrite(LCD_CS_BASE, LCD_CS_PIN, LCD_CS_PIN);
#endif
    GPIOPinWrite(LCD_DATAH_BASE, LCD_DATAH_PINS, 0x00);
	#if  defined(__TM4C1294NCPDT__)
    	GPIOPinWrite(LCD_DATAL_BASE, LCD_DATAL_PINS, 0x00);
	#endif
	GPIOPinWrite(LCD_RST_BASE, LCD_RST_PIN, LCD_RST_PIN);
    delay(10);
	GPIOPinWrite(LCD_RST_BASE, LCD_RST_PIN, 0x00);
    delay(10);
	GPIOPinWrite(LCD_RST_BASE, LCD_RST_PIN, LCD_RST_PIN);
	GPIOPinWrite(LCD_DC_BASE, LCD_DC_PIN, 0x00);
    GPIOPinWrite(LCD_RD_BASE, LCD_RD_PIN, LCD_RD_PIN);
    GPIOPinWrite(LCD_WR_BASE, LCD_WR_PIN, LCD_WR_PIN);
	#ifndef LCDC_CS_ISGROUNDED
    GPIOPinWrite(LCD_CS_BASE, LCD_CS_PIN, 0);
	#endif
#else

    LCD_RST_PORTDIR |= LCD_RST_PIN;
    LCD_RST_PORTOUT |= LCD_RST_PIN;
    delay(10);
    LCD_RST_PORTOUT &=~LCD_RST_PIN;
    delay(10);
    LCD_RST_PORTOUT |= LCD_RST_PIN;
    delay(10);
    //
    // GPIOs as OUTPUT
    //
    LCD_DATAH_PORTDIR = LCD_DATAH_PINS;
//
    LCD_DATAH_PORTOUT= 0x0;
/*    LCD_DATAH_PORTOUT= 1;
    LCD_DATAH_PORTOUT= 2;
    LCD_DATAH_PORTOUT= 4;
    LCD_DATAH_PORTOUT= 8;
    LCD_DATAH_PORTOUT= 0x10;
    LCD_DATAH_PORTOUT= 0x20;
    LCD_DATAH_PORTOUT= 0x40;
    LCD_DATAH_PORTOUT= 0x80;
    LCD_DATAH_PORTOUT= 0x0;
*/
    LCD_DC_PORTDIR |= LCD_DC_PIN;
    LCD_RD_PORTDIR |= LCD_RD_PIN;
    LCD_WR_PORTDIR |= LCD_WR_PIN;
#ifndef LCDC_CS_ISGROUNDED
    LCD_CS_PORTDIR |= LCD_CS_PIN;

    //
    // Set the LCD control pins to their default values.
    //
    LCD_CS_PORTOUT |= LCD_CS_PIN;
#endif
    LCD_DATAH_PORTOUT = 0x00;
    LCD_DC_PORTOUT &=~ LCD_DC_PIN;
    LCD_RD_PORTOUT |= LCD_RD_PIN;
    LCD_WR_PORTOUT |= LCD_WR_PIN;
	#ifndef LCDC_CS_ISGROUNDED
    LCD_CS_PORTOUT &=~LCD_CS_PIN;
	#endif
#endif // end __LM4F120H5QR__

    
    delay(10); // Delay 10mS
    

    _writeCommand8(SSD1963_SOFT_RESET);  // software reset
    delay(120); // Delay 1mS

    _writeCommand8(SSD1963_SET_PLL_MN);	//PLL multiplier, set PLL clock to 120M (480/4)
    _writeData8(LCDC_PLLMULT);				// DCO reference multiplier (0x2f);	    //N=0x36 for 6.5M, 0x23 for 10M crystal
    _writeData8(LCDC_PLLDVDR);				// DCO divider (0x03);
    _writeData8(0x04);						// dummy
    delay(1);
    _writeCommand8(SSD1963_SET_PLL);  // PLL enable
    _writeData8(0x01);
    delay(10); // Delay 1mS
    
    _writeCommand8(SSD1963_SET_PLL);
    _writeData8(0x03);
    _writeCommand8(SSD1963_SOFT_RESET);  // software reset
    delay(120); // Delay 1mS


    _writeCommand8(SSD1963_SET_LCD_MODE);	//LCD SPECIFICATION
    _writeData8(LCDC_INTF);		// Type of interface wdth and mode for pxel clock and synk polarity
    _writeData8(LCDC_TYPE);						// serial/Parallel mode TFT
    _writeData8(((HDP)>>8)&0x0FF);					//Set HDP 'hps??(0x03);  //Set HDP
    _writeData8((HDP)&0x0FF);						//(0x1F);
    _writeData8(((VDP)>>8)&0x0FF);					//Set VDP 'vps??(0x01);
    _writeData8((VDP)&0x0FF);						// (0xDF);
    _writeData8(LCDC_INLC);						// line interlace mode and RGB sequence

    _writeCommand8(SSD1963_SET_PIXEL_DATA_INTERFACE); //pixel data interface
    _writeData8(PIXEL_DATA_INTERFACE_MODE);
    delay(5); // Delay 5mS
    //	_writeData8(0x03);

    _writeCommand8(SSD1963_SET_ADDRESS_MODE); //rotation mirroring RGB/BGR
    _writeData8(DEFAULT_ADDRESS_MODE);


    _writeCommand8(SSD1963_SET_LSHIFT_FREQ);	//PLL setting for PCLK, depends on resolution
     _writeData8((LCDC_FPR>>16)&0xff);
     _writeData8(((LCDC_FPR)>>8)&0xFF);
     _writeData8(LCDC_FPR&0xFF);

     _writeCommand8(SSD1963_SET_HORI_PERIOD);		//HSYNC
     _writeData8(((HT)>>8)&0x0FF);					//Set HT(0x03);
     _writeData8((HT)&0x0FF);							//(0xF8);//F8
     _writeData8(((HPS)>>8)&0x0FF);					//Set HPS(0x00);
     _writeData8((HPS)&0x0FF);						//(0xB4);//64
     _writeData8(HPW);								//Set HPW//(0x59);//49
     _writeData8(((LPS)>>8)&0x0FF);					//Set LPS(0x00);
     _writeData8((LPS)&0x0FF);						//(0x00);
     _writeData8(SUB_LPS);

     _writeCommand8(SSD1963_SET_VERT_PERIOD);	//VSYNC
     _writeData8(((VT)>>8)&0x0FF);					//Set VT(0x01);
     _writeData8((VT)&0x0FF);							// (0xFB);
     _writeData8(((VPS)>>8)&0x0FF);					//Set VPS(0);
     _writeData8(VPS&0x0FF);						//(0x14);
     _writeData8(VPW);								//Set VPW(0x09);
     _writeData8(((FPS)>>8)&0x0FF);					//Set FPS(0);
     _writeData8((FPS)&0x0FF);						// (0);
    
    _writeCommand8(SSD1963_SET_GPIO_CONF);
    _writeData8(0x0F);    //set GPIO[3:0] as output
    _writeData8(0x01);    //GPIO0 normal
    
    _writeCommand8(SSD1963_SET_GPIO_VALUE);
    _writeData8(0x0B);    //set GPIO3(SHLR)=1, GPIO2(UPDN)=0, GPIO1=1, GPIO0(VGH_VGL)=1;
    
 /*
  *
    _writeCommand8(SSD1963_SET_PWM_CONF); //set PWM for B/L
    _writeData8(0x0e);//0e frequency
    _writeData8(0xff);//ff duty
    _writeData8(0x09);//09 bit[3]DBC enable,bit[0]PWM enable
    _writeData8(0xc0);//0xff 0xc0 DBC manual brightness
    _writeData8(0x10);//0x88 0x10 DBC minimum brightness
    _writeData8(0x00);//Brightness prescaler
    
    _writeCommand8(SSD1963_SET_DBC_TH); //DBC
    _writeData8(0x00);
    _writeData8(0x1c);
    _writeData8(0x20);
    _writeData8(0x00);
    _writeData8(0x46);
    _writeData8(0x50);
    _writeData8(0x00);
    _writeData8(0xa8);
    _writeData8(0xc0);
    
    _writeCommand8(SSD1963_SET_DBC_CONF);
    _writeData8(0b00101101);//DBC Aggressive mode
    //    _writeData8(0x00);//DBC disable
 */
    _writeCommand8(SSD1963_ENTER_NORMAL_MODE);
    _writeCommand8(SSD1963_EXIT_IDLE_MODE);
    _writeCommand8(SSD1963_SET_DISPLAY_ON); //display on
    
    _screenWidth  = ER1963_HEIGHT;
    _screenHeigth = ER1963_WIDTH;
    
    // Standard
    setOrientation(0);
    
    _screenWidth  = ER1963_HEIGHT;
    _screenHeigth = ER1963_WIDTH;
    _screenDiagonal = 70;
    
    // Touch
    _touchYmin = 78;
    _touchYmax = 1951;
    _touchXmin = 1894;
    _touchXmax = 189;
    
    _penSolid  = false;
    _fontSolid = true;
    _flagRead  = true;
    _flagEnergy = true;
    _flagIntensity = true;
    _touchTrim = TOUCH_TRIM;
    _fsmArea = 0x01;
    clear();
    
    delay(100);
}

String Screen_ER1963::WhoAmI()
{
//#if (SPI_MODE == FAST_SPI)
//    return "ER 5\" SSD1963 (fast)";
//#else
    return "ER 5\" SSD1963 screen";
//#endif
}

void Screen_ER1963::_setIntensity(uint8_t intensity)
{
    _writeCommand8(SSD1963_SET_PWM_CONF); //set PWM for B/L
    _writeData8(0x0e);      // 0e frequency
    _writeData8(0xff);      // ff duty
    _writeData8(0x09);      // 09 bit[3]DBC enable,bit[0]PWM enable
    _writeData8(intensity); // 0xff 0xc0 DBC manual brightness
    _writeData8(0x10);      // 0x88 0x10 DBC minimum brightness
    _writeData8(0x00);      // Brightness prescaler
}

void Screen_ER1963::_setBacklight(bool state)
{
    if (state)  _writeCommand8(SSD1963_SET_DISPLAY_ON);
    else        _writeCommand8(SSD1963_SET_DISPLAY_OFF);
}

uint16_t Screen_ER1963::readPixel(uint16_t x1, uint16_t y1)
{
	_setWindowRead(x1, y1, x1, y1);
	return _readData16();
}

void Screen_ER1963::copyPasteArea(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t dx, uint16_t dy)
{
    uint16_t buffer[dx];
    
    _closeWindow();
    
    for (uint16_t y=0; y<dy; y++) {
        
        _setWindowRead(x1, y1+y, x1+dx, y1+y);
        for (uint16_t x=0; x<dx; x++) {
            //            _setCursor(x1+x, y1+y);
            buffer[x] = _readData16();
        }
        
        _setWindow(x2, y2+y, x2+dx, y2+y);
        
        for (uint16_t x=0; x<dx; x++) {
            _writeData88(highByte(buffer[x]), lowByte(buffer[x]));
        }
    }
    _closeWindow();
}

void Screen_ER1963::_setOrientation(uint8_t orientation)
{
    //_orientation = orientation % 4;
    //    LCD_screen_font::setOrientation(orientation);
    
    //
    // default = 0x6830 = 0x68 <<8 + 0b00110000
    switch (_orientation) {
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
            _writeRegister8(SSD1963_SET_ADDRESS_MODE, 0b00000000 | DEFAULT_ADDRESS_MODE);              // ok
            break;
    }
}

// Utilities
void Screen_ER1963::_writeData8(uint8_t data8)
{
    // Writes a data word to the SSD2119.  This function implements the basic GPIO
    // interface to the LCD display.
#ifndef LCDC_CS_ISGROUNDED
#if defined(__LM4F120H5QR__) || defined(__TM4C1230C3PM__) || defined(__TM4C129XNCZAD__) || defined(__TM4C1294NCPDT__)
    HWREG(LCD_CS_BASE + GPIO_O_DATA + (LCD_CS_PIN << 2)) = 0;
#else
    LCD_CS_PORTOUT &=~LCD_CS_PIN;
#endif // end __LM4F120H5QR__
#endif
    
    //
    // Write the least significant byte of the data to the bus.
    //

    SET_LCD_DATA(data8);
    //
    // Assert the write enable signal.
    //
    // Deassert the write enable signal. TEHN CS
    //

#if defined(__LM4F120H5QR__) || defined(__TM4C1230C3PM__) || defined(__TM4C129XNCZAD__)  || defined(__TM4C1294NCPDT__)
    HWREG(LCD_WR_BASE + GPIO_O_DATA + (LCD_WR_PIN << 2)) = 0;
    HWREG(LCD_WR_BASE + GPIO_O_DATA + (LCD_WR_PIN << 2)) = LCD_WR_PIN;
//    HWREG(LCD_WR_BASE + GPIO_O_DATA + (LCD_WR_PIN << 2)) = LCD_WR_PIN;
    
#ifndef LCDC_CS_ISGROUNDED
    HWREG(LCD_CS_BASE + GPIO_O_DATA + (LCD_CS_PIN << 2)) = LCD_CS_PIN;          // CS HIGH
#endif
#else
    LCD_WR_PORTOUT &=~LCD_WR_PIN;
//    LCD_WR_PORTOUT &=~LCD_WR_PIN;
//    LCD_WR_PORTOUT |= LCD_WR_PIN;
    LCD_WR_PORTOUT |= LCD_WR_PIN;
#ifndef LCDC_CS_ISGROUNDED
    LCD_CS_PORTOUT |= LCD_CS_PIN;
#endif
#endif // end __LM4F120H5QR__

}

void Screen_ER1963::_writeData88(uint8_t dataHigh8, uint8_t dataLow8)
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
    
#ifndef LCDC_CS_ISGROUNDED
#if defined(__LM4F120H5QR__) || defined(__TM4C1230C3PM__) || defined(__TM4C129XNCZAD__)  || defined(__TM4C1294NCPDT__)
	HWREG(LCD_CS_BASE + GPIO_O_DATA + (LCD_CS_PIN << 2)) = 0;                   // CS LOW
#else
	LCD_CS_PORTOUT &=~LCD_CS_PIN;
#endif // end __LM4F120H5QR__
#endif

	_writeData888(redColour8, greenColour8, blueColour8);

#ifndef LCDC_CS_ISGROUNDED
#if defined(__LM4F120H5QR__) || defined(__TM4C1230C3PM__) || defined(__TM4C129XNCZAD__)  || defined(__TM4C1294NCPDT__)
	HWREG(LCD_CS_BASE + GPIO_O_DATA + (LCD_CS_PIN << 2)) = LCD_CS_PIN;          // CS HIGH
#else
	LCD_CS_PORTOUT |= LCD_CS_PIN;
#endif // end __LM4F120H5QR__
#endif

}

void Screen_ER1963::_writeData16(uint16_t data16)
{
    _writeData88(highByte(data16), lowByte(data16));
    
    //    // Fastest solution
    //    uint8_t redColour8   =    (data16 >> 8) & 0b11111000;
    //    uint8_t greenColour8 =   ((data16 >> 5) & 0b00111111) <<2;
    //    uint8_t blueColour8  =    (data16       & 0b00011111) <<3;
    //
    //    HWREG(LCD_CS_BASE + GPIO_O_DATA + (LCD_CS_PIN << 2)) = 0;                   // CS LOW
    //    _writeData888(redColour8, greenColour8, blueColour8);
    //    HWREG(LCD_CS_BASE + GPIO_O_DATA + (LCD_CS_PIN << 2)) = LCD_CS_PIN;          // CS HIGH
}

void Screen_ER1963::_writeData888(uint8_t dataRed8, uint8_t dataBlue8, uint8_t dataGreen8)
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
    // Deassert the write enable signal.
    //
#if defined(__LM4F120H5QR__) || defined(__TM4C1230C3PM__) || defined(__TM4C129XNCZAD__)  || defined(__TM4C1294NCPDT__)
    HWREG(LCD_WR_BASE + GPIO_O_DATA + (LCD_WR_PIN << 2)) = 0;
//    HWREG(LCD_WR_BASE + GPIO_O_DATA + (LCD_WR_PIN << 2)) = 0;
    HWREG(LCD_WR_BASE + GPIO_O_DATA + (LCD_WR_PIN << 2)) = LCD_WR_PIN;
//    HWREG(LCD_WR_BASE + GPIO_O_DATA + (LCD_WR_PIN << 2)) = LCD_WR_PIN;
#else
    LCD_WR_PORTOUT &=~LCD_WR_PIN;
    LCD_WR_PORTOUT |= LCD_WR_PIN;
#endif // end __LM4F120H5QR__
    
    //
    // Write the mid byte of the data to the bus.
    //
    SET_LCD_DATA(dataBlue8);
    
    //
    // Assert the write enable signal.  We need to do this 2 times to ensure
    // that we don't violate the timing requirements for the display.
    //
    // Deassert the write enable signal.
    //
#if defined(__LM4F120H5QR__) || defined(__TM4C1230C3PM__) || defined(__TM4C129XNCZAD__)  || defined(__TM4C1294NCPDT__)
    HWREG(LCD_WR_BASE + GPIO_O_DATA + (LCD_WR_PIN << 2)) = 0;
//    HWREG(LCD_WR_BASE + GPIO_O_DATA + (LCD_WR_PIN << 2)) = 0;
    HWREG(LCD_WR_BASE + GPIO_O_DATA + (LCD_WR_PIN << 2)) = LCD_WR_PIN;
//    HWREG(LCD_WR_BASE + GPIO_O_DATA + (LCD_WR_PIN << 2)) = LCD_WR_PIN;
#else
    LCD_WR_PORTOUT &=~LCD_WR_PIN;
    LCD_WR_PORTOUT |= LCD_WR_PIN;
#endif // end __LM4F120H5QR__
    
    //
    // Write the least significant byte of the data to the bus.
    //
    SET_LCD_DATA(dataGreen8);
    
    //
    // Assert the write enable signal.
    //
    // Deassert the write enable signal.
    //
#if defined(__LM4F120H5QR__) || defined(__TM4C1230C3PM__) || defined(__TM4C129XNCZAD__)  || defined(__TM4C1294NCPDT__)
    HWREG(LCD_WR_BASE + GPIO_O_DATA + (LCD_WR_PIN << 2)) = 0;
//    HWREG(LCD_WR_BASE + GPIO_O_DATA + (LCD_WR_PIN << 2)) = 0;
    
    HWREG(LCD_WR_BASE + GPIO_O_DATA + (LCD_WR_PIN << 2)) = LCD_WR_PIN;
//    HWREG(LCD_WR_BASE + GPIO_O_DATA + (LCD_WR_PIN << 2)) = LCD_WR_PIN;
    
    //    HWREG(LCD_CS_BASE + GPIO_O_DATA + (LCD_CS_PIN << 2)) = LCD_CS_PIN;          // CS HIGH
#else
    LCD_WR_PORTOUT &=~LCD_WR_PIN;
    LCD_WR_PORTOUT |= LCD_WR_PIN;
#endif // end __LM4F120H5QR__

}

//*****************************************************************************
//
// Writes a command to the SSD2119.  This function implements the basic GPIO
// interface to the LCD display.
//
//*****************************************************************************
void Screen_ER1963::_writeCommand8(uint8_t command8)
{
    //
    // Writes a command to the SSD1963.  This function implements the basic GPIO
    // interface to the LCD display.
    //
#ifndef LCDC_CS_ISGROUNDED
#if defined(__LM4F120H5QR__) || defined(__TM4C1230C3PM__) || defined(__TM4C129XNCZAD__)  || defined(__TM4C1294NCPDT__)
	HWREG(LCD_CS_BASE + GPIO_O_DATA + (LCD_CS_PIN << 2)) = 0;
#else
    LCD_CS_PORTOUT &=~LCD_CS_PIN;
#endif // end __LM4F120H5QR__
#endif
/*    //
    // Write the most significant byte of the data to the bus.  This is always
    // 0 since commands are no more than 8 bits.
    //
    SET_LCD_DATA(0);
*/
    //
    // Assert DC
    //
#if defined(__LM4F120H5QR__) || defined(__TM4C1230C3PM__) || defined(__TM4C129XNCZAD__)  || defined(__TM4C1294NCPDT__)
    HWREG(LCD_DC_BASE + GPIO_O_DATA + (LCD_DC_PIN << 2)) = 0;
#else
    LCD_DC_PORTOUT &=~LCD_DC_PIN;
#endif // end __LM4F120H5QR__
    
/*    //
    // Assert the write enable signal.  We need to do this 2 times to ensure
    // that we don't violate the timing requirements for the display.
    //
    // Deassert the write enable signal.
    //
#if defined(__LM4F120H5QR__) || defined(__TM4C1230C3PM__) || defined(__TM4C129XNCZAD__)  || defined(__TM4C1294NCPDT__)

    HWREG(LCD_WR_BASE + GPIO_O_DATA + (LCD_WR_PIN << 2)) = 0;
//    HWREG(LCD_WR_BASE + GPIO_O_DATA + (LCD_WR_PIN << 2)) = 0;

    HWREG(LCD_WR_BASE + GPIO_O_DATA + (LCD_WR_PIN << 2)) = LCD_WR_PIN;
//    HWREG(LCD_WR_BASE + GPIO_O_DATA + (LCD_WR_PIN << 2)) = LCD_WR_PIN;
#else
    LCD_WR_PORTOUT &=~LCD_WR_PIN;
    LCD_WR_PORTOUT |= LCD_WR_PIN;
#endif // end __LM4F120H5QR__
    
*/

    //
    // Write the least significant byte of the data to the bus.
    //
    SET_LCD_DATA(command8);
    
    //
    // Assert the write enable signal.
    //
    // Deassert the write enable signal.
    //
#if defined(__LM4F120H5QR__) || defined(__TM4C1230C3PM__) || defined(__TM4C129XNCZAD__)  || defined(__TM4C1294NCPDT__)
    HWREG(LCD_WR_BASE + GPIO_O_DATA + (LCD_WR_PIN << 2)) = 0;
//    HWREG(LCD_WR_BASE + GPIO_O_DATA + (LCD_WR_PIN << 2)) = 0;
    
    HWREG(LCD_WR_BASE + GPIO_O_DATA + (LCD_WR_PIN << 2)) = LCD_WR_PIN;
//    HWREG(LCD_WR_BASE + GPIO_O_DATA + (LCD_WR_PIN << 2)) = LCD_WR_PIN;
#else
    LCD_WR_PORTOUT &=~LCD_WR_PIN;
//    LCD_WR_PORTOUT &=~LCD_WR_PIN;
//    LCD_WR_PORTOUT &=~LCD_WR_PIN;
//    LCD_WR_PORTOUT |= LCD_WR_PIN;
    LCD_WR_PORTOUT |= LCD_WR_PIN;
#endif // end __LM4F120H5QR__

    
    //
    // Set the DC signal high, indicating that following writes are data.
    //
#if defined(__LM4F120H5QR__) || defined(__TM4C1230C3PM__) || defined(__TM4C129XNCZAD__)  || defined(__TM4C1294NCPDT__)

    HWREG(LCD_DC_BASE + GPIO_O_DATA + (LCD_DC_PIN << 2)) = LCD_DC_PIN;
	#ifndef LCDC_CS_ISGROUNDED
    	HWREG(LCD_CS_BASE + GPIO_O_DATA + (LCD_CS_PIN << 2)) = LCD_CS_PIN;          // CS HIGH
	#endif
#else
    LCD_DC_PORTOUT |= LCD_DC_PIN;
	#ifndef LCDC_CS_ISGROUNDED
    	LCD_CS_PORTOUT |= LCD_CS_PIN;
	#endif
#endif // end __LM4F120H5QR__
}

uint16_t Screen_ER1963::_readData16()
{
    uint16_t data16 = 0;
    uint8_t red, blue, green;
//    uint8_t data8[64][5];
    
    _writeCommand8(SSD1963_READ_MEMORY_CONTINUE);
    
	// Switch to INPUT mode
#if defined(__LM4F120H5QR__) || defined(__TM4C1230C3PM__) || defined(__TM4C129XNCZAD__)  || defined(__TM4C1294NCPDT__)
    GPIOPinTypeGPIOInput(LCD_DATAH_BASE, LCD_DATAH_PINS);
	#if  defined(__TM4C1294NCPDT__)
    	GPIOPinTypeGPIOInput(LCD_DATAL_BASE, LCD_DATAL_PINS);
	#endif
    
#ifndef LCDC_CS_ISGROUNDED
	HWREG(LCD_CS_BASE + GPIO_O_DATA + (LCD_CS_PIN << 2)) = 0;                   // CS LOW
#endif
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
#else
    LCD_DATAH_PORTDIR = 0x00;
#ifndef LCDC_CS_ISGROUNDED
    LCD_CS_PORTOUT &=~LCD_CS_PIN;
#endif
    LCD_RD_PORTOUT &=~LCD_RD_PIN;
    LCD_RD_PORTOUT &=~LCD_RD_PIN;
    LCD_RD_PORTOUT &=~LCD_RD_PIN;
    LCD_RD_PORTOUT &=~LCD_RD_PIN;
/*    LCD_RD_PORTOUT &=~LCD_RD_PIN;
    LCD_RD_PORTOUT &=~LCD_RD_PIN;
    LCD_RD_PORTOUT &=~LCD_RD_PIN;
    LCD_RD_PORTOUT &=~LCD_RD_PIN;
    LCD_RD_PORTOUT &=~LCD_RD_PIN; */
#endif // end __LM4F120H5QR__

    
    //
    // Read the most significant byte of the data from the bus.
    //
    //    data16   = (GET_LCD_DATA() >>3) <<11;
    red = GET_LCD_DATA();
    
    //
    // Deassert the read enable signal. x5 = 100 ns
    //
    // Assert the read enable signal. x10 = 200 ns
    //
#if defined(__LM4F120H5QR__) || defined(__TM4C1230C3PM__) || defined(__TM4C129XNCZAD__)  || defined(__TM4C1294NCPDT__)

    HWREG(LCD_RD_BASE + GPIO_O_DATA + (LCD_RD_PIN << 2)) = LCD_RD_PIN;
    HWREG(LCD_RD_BASE + GPIO_O_DATA + (LCD_RD_PIN << 2)) = LCD_RD_PIN;
    HWREG(LCD_RD_BASE + GPIO_O_DATA + (LCD_RD_PIN << 2)) = LCD_RD_PIN;
    HWREG(LCD_RD_BASE + GPIO_O_DATA + (LCD_RD_PIN << 2)) = LCD_RD_PIN;
    
    HWREG(LCD_RD_BASE + GPIO_O_DATA + (LCD_RD_PIN << 2)) = 0;
    HWREG(LCD_RD_BASE + GPIO_O_DATA + (LCD_RD_PIN << 2)) = 0;
    HWREG(LCD_RD_BASE + GPIO_O_DATA + (LCD_RD_PIN << 2)) = 0;
    HWREG(LCD_RD_BASE + GPIO_O_DATA + (LCD_RD_PIN << 2)) = 0;
    HWREG(LCD_RD_BASE + GPIO_O_DATA + (LCD_RD_PIN << 2)) = 0;
    HWREG(LCD_RD_BASE + GPIO_O_DATA + (LCD_RD_PIN << 2)) = 0;
    HWREG(LCD_RD_BASE + GPIO_O_DATA + (LCD_RD_PIN << 2)) = 0;
    HWREG(LCD_RD_BASE + GPIO_O_DATA + (LCD_RD_PIN << 2)) = 0;
#else
    LCD_RD_PORTOUT |= LCD_RD_PIN;
    LCD_RD_PORTOUT |= LCD_RD_PIN;
    LCD_RD_PORTOUT |= LCD_RD_PIN;
    LCD_RD_PORTOUT |= LCD_RD_PIN;

    LCD_RD_PORTOUT &=~LCD_RD_PIN;
    LCD_RD_PORTOUT &=~LCD_RD_PIN;
    LCD_RD_PORTOUT &=~LCD_RD_PIN;
    LCD_RD_PORTOUT &=~LCD_RD_PIN;
    LCD_RD_PORTOUT &=~LCD_RD_PIN;
/*    LCD_RD_PORTOUT &=~LCD_RD_PIN;
    LCD_RD_PORTOUT &=~LCD_RD_PIN;
    LCD_RD_PORTOUT &=~LCD_RD_PIN;
    LCD_RD_PORTOUT &=~LCD_RD_PIN;*/
#endif // end __LM4F120H5QR__
    
    //
    // Read the least significant byte of the data from the bus.
    //
    //    data16   |= (GET_LCD_DATA() >>2) <<5;
    green = GET_LCD_DATA();
    
    //
    // Deassert the read enable signal. x5 = 100 ns
    //
    // Assert the read enable signal. x10 = 200 ns
    //
#if defined(__LM4F120H5QR__) || defined(__TM4C1230C3PM__) || defined(__TM4C129XNCZAD__)  || defined(__TM4C1294NCPDT__)

    HWREG(LCD_RD_BASE + GPIO_O_DATA + (LCD_RD_PIN << 2)) = LCD_RD_PIN;
    HWREG(LCD_RD_BASE + GPIO_O_DATA + (LCD_RD_PIN << 2)) = LCD_RD_PIN;
    HWREG(LCD_RD_BASE + GPIO_O_DATA + (LCD_RD_PIN << 2)) = LCD_RD_PIN;
    HWREG(LCD_RD_BASE + GPIO_O_DATA + (LCD_RD_PIN << 2)) = LCD_RD_PIN;
    
    HWREG(LCD_RD_BASE + GPIO_O_DATA + (LCD_RD_PIN << 2)) = 0;
    HWREG(LCD_RD_BASE + GPIO_O_DATA + (LCD_RD_PIN << 2)) = 0;
    HWREG(LCD_RD_BASE + GPIO_O_DATA + (LCD_RD_PIN << 2)) = 0;
    HWREG(LCD_RD_BASE + GPIO_O_DATA + (LCD_RD_PIN << 2)) = 0;
    HWREG(LCD_RD_BASE + GPIO_O_DATA + (LCD_RD_PIN << 2)) = 0;
    HWREG(LCD_RD_BASE + GPIO_O_DATA + (LCD_RD_PIN << 2)) = 0;
    HWREG(LCD_RD_BASE + GPIO_O_DATA + (LCD_RD_PIN << 2)) = 0;
    HWREG(LCD_RD_BASE + GPIO_O_DATA + (LCD_RD_PIN << 2)) = 0;
#else
    LCD_RD_PORTOUT |= LCD_RD_PIN;
    LCD_RD_PORTOUT |= LCD_RD_PIN;
    LCD_RD_PORTOUT |= LCD_RD_PIN;
    LCD_RD_PORTOUT |= LCD_RD_PIN;

    LCD_RD_PORTOUT &=~LCD_RD_PIN;
    LCD_RD_PORTOUT &=~LCD_RD_PIN;
    LCD_RD_PORTOUT &=~LCD_RD_PIN;
    LCD_RD_PORTOUT &=~LCD_RD_PIN;
    LCD_RD_PORTOUT &=~LCD_RD_PIN;
/*    LCD_RD_PORTOUT &=~LCD_RD_PIN;
    LCD_RD_PORTOUT &=~LCD_RD_PIN;
    LCD_RD_PORTOUT &=~LCD_RD_PIN;
    LCD_RD_PORTOUT &=~LCD_RD_PIN; */
#endif // end __LM4F120H5QR__
    
    //
    // Read the least significant byte of the data from the bus.
    //
    //    data16   |= (GET_LCD_DATA() >>3);
    blue = GET_LCD_DATA();
    
    //
    // Deassert the read enable signal. x5 = 100 ns
    //
    // Assert the read enable signal. x10 = 200 ns
    //
#if defined(__LM4F120H5QR__) || defined(__TM4C1230C3PM__) || defined(__TM4C129XNCZAD__)  || defined(__TM4C1294NCPDT__)

    HWREG(LCD_RD_BASE + GPIO_O_DATA + (LCD_RD_PIN << 2)) = LCD_RD_PIN;
    HWREG(LCD_RD_BASE + GPIO_O_DATA + (LCD_RD_PIN << 2)) = LCD_RD_PIN;
    HWREG(LCD_RD_BASE + GPIO_O_DATA + (LCD_RD_PIN << 2)) = LCD_RD_PIN;
    HWREG(LCD_RD_BASE + GPIO_O_DATA + (LCD_RD_PIN << 2)) = LCD_RD_PIN;
#else
    LCD_RD_PORTOUT |= LCD_RD_PIN;
    LCD_RD_PORTOUT |= LCD_RD_PIN;
    LCD_RD_PORTOUT |= LCD_RD_PIN;
    LCD_RD_PORTOUT |= LCD_RD_PIN;
#endif // end __LM4F120H5QR__

    // DEASSERT CS & Return to OUTPUT mode
#if defined(__LM4F120H5QR__) || defined(__TM4C1230C3PM__) || defined(__TM4C129XNCZAD__)  || defined(__TM4C1294NCPDT__)
#ifndef LCDC_CS_ISGROUNDED
    HWREG(LCD_CS_BASE + GPIO_O_DATA + (LCD_CS_PIN << 2)) = LCD_CS_PIN;          // CS HIGH
#endif
    GPIOPinTypeGPIOOutput(LCD_DATAH_BASE, LCD_DATAH_PINS);
    GPIOPadConfigSet(LCD_DATAH_BASE, LCD_DATAH_PINS, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD);
	#if  defined(__TM4C1294NCPDT__)
		GPIOPinTypeGPIOOutput(LCD_DATAL_BASE, LCD_DATAL_PINS);
		GPIOPadConfigSet(LCD_DATAL_BASE, LCD_DATAL_PINS, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD);
	#endif
#else
#ifndef LCDC_CS_ISGROUNDED
    LCD_CS_PORTOUT |= LCD_CS_PIN;
#endif
    LCD_DATAH_PORTDIR = LCD_DATAH_PINS;
#endif // end __LM4F120H5QR__
    
    data16 = (red >>3) <<11 | (green >>2) <<5 | blue >>3;
    return data16;
}


void Screen_ER1963::_writeRegister8(uint8_t command8, uint8_t data8)
{
    _writeCommand8(command8);
    _writeData8(data8);
}


void Screen_ER1963::_orientCoordinates(uint16_t &x1, uint16_t &y1)
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

void Screen_ER1963::_setPoint(uint16_t x1, uint16_t y1, uint16_t colour)        // compulsory
{
    _setWindow(x1, y1, x1, y1);
    
    uint8_t redColour8   =  (colour >>11) <<3;
    uint8_t greenColour8 =  (colour >> 5) <<2;
    uint8_t blueColour8  =   colour       <<3;
    
#ifndef LCDC_CS_ISGROUNDED
	#if defined(__LM4F120H5QR__) || defined(__TM4C1230C3PM__) || defined(__TM4C129XNCZAD__)  || defined(__TM4C1294NCPDT__)
		HWREG(LCD_CS_BASE + GPIO_O_DATA + (LCD_CS_PIN << 2)) = 0;                   // CS LOW
		_writeData888(redColour8, greenColour8, blueColour8);
		HWREG(LCD_CS_BASE + GPIO_O_DATA + (LCD_CS_PIN << 2)) = LCD_CS_PIN;          // CS HIGH
	#else
		LCD_CS_PORTOUT &=~LCD_CS_PIN;
		_writeData888(redColour8, greenColour8, blueColour8);
		LCD_CS_PORTOUT |= LCD_CS_PIN;
	#endif // end __LM4F120H5QR__
#else
	_writeData888(redColour8, greenColour8, blueColour8);
#endif
    
    //    uint8_t highColour = highByte(colour);
    //    uint8_t lowColour  = lowByte(colour);
    //
    //    _writeData88(highColour, lowColour);
}

//void Screen_ER1963::_setCursor(uint16_t x1, uint16_t y1)
//{
//    _setWindow(x1, y1, x1, y1);
//}


void Screen_ER1963::_setWindow(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
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

void Screen_ER1963::_closeWindow()
{
    _setWindow(0, 0, screenSizeX()-1, screenSizeY()-1);
}

void Screen_ER1963::_setWindowRead(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
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
    
    _writeCommand8(SSD1963_READ_MEMORY_START);
}

inline uint16_t absDiff(uint16_t a, uint16_t b) { return (a > b) ? a-b : b-a; }


void Screen_ER1963::_fastFill(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t colour)
{
    _setWindow(x1, y1, x2, y2);

#ifndef LCDC_CS_ISGROUNDED
#if defined(__LM4F120H5QR__) || defined(__TM4C1230C3PM__) || defined(__TM4C129XNCZAD__)  || defined(__TM4C1294NCPDT__)

    HWREG(LCD_CS_BASE + GPIO_O_DATA + (LCD_CS_PIN << 2)) = 0;                   // CS low
#else
    LCD_CS_PORTOUT &=~LCD_CS_PIN;
#endif // end __LM4F120H5QR__
#endif
    
    // required because of uint32_t, t>0 and t--
    if (x1 > x2) _swap(x1, x2);
    if (y1 > y2) _swap(y1, y2);
    
    // fastest
    if (colour == myColours.black) {
        SET_LCD_DATA(0);
        for (uint32_t t=(uint32_t)(y2-y1+1)*(x2-x1+1)*3; t>0; t--) {
#if defined(__LM4F120H5QR__) || defined(__TM4C1230C3PM__) || defined(__TM4C129XNCZAD__)  || defined(__TM4C1294NCPDT__)
            HWREG(LCD_WR_BASE + GPIO_O_DATA + (LCD_WR_PIN << 2)) = 0;
            HWREG(LCD_WR_BASE + GPIO_O_DATA + (LCD_WR_PIN << 2)) = LCD_WR_PIN;
#else
			LCD_WR_PORTOUT &=~LCD_WR_PIN;
			LCD_WR_PORTOUT |= LCD_WR_PIN;
#endif // end __LM4F120H5QR__

        }
    } else {
        uint8_t redColour8   =  (colour >>11) <<3;
        uint8_t greenColour8 =  (colour >> 5) <<2;
        uint8_t blueColour8  =   colour       <<3;
        
        //    uint8_t redColour8, greenColour8, blueColour8;
        //    splitColour(colour, redColour8, greenColour8, blueColour8);
        
        for (uint32_t t=(uint32_t)(y2-y1+1)*(x2-x1+1)*3; t>0; t--) {
            _writeData888(redColour8, greenColour8, blueColour8);
        }
    }
#ifndef LCDC_CS_ISGROUNDED
	#if defined(__LM4F120H5QR__) || defined(__TM4C1230C3PM__) || defined(__TM4C129XNCZAD__)  || defined(__TM4C1294NCPDT__)
    	HWREG(LCD_CS_BASE + GPIO_O_DATA + (LCD_CS_PIN << 2)) = LCD_CS_PIN;          // CS high
#else
		LCD_CS_PORTOUT |= LCD_CS_PIN;
	#endif // end __LM4F120H5QR__
#endif
}


// Touch
void Screen_ER1963::_getOneTouch(uint8_t command8, uint8_t &a, uint8_t &b)
{
    digitalWrite(_pinTouchCS, LOW);
    
    // software SPI for touch
    shiftOut(_pinTouchMOSI, _pinTouchClock, MSBFIRST, command8);
    a = shiftIn(_pinTouchMISO, _pinTouchClock, MSBFIRST);
    b = shiftIn(_pinTouchMISO, _pinTouchClock, MSBFIRST);
    
    //    SPI.transfer(command8);
    //    a = SPI.transfer();
    //    b = SPI.transfer();
    
    digitalWrite(_pinTouchCS, HIGH);
}

void Screen_ER1963::_getRawTouch(uint16_t &x0, uint16_t &y0, uint16_t &z0)
{
//    uint16_ z1, z2;
    uint8_t a, aa, b, c, cc, d;
    
    delay(10);
    _getOneTouch(TOUCH_Z1, a, b); // 8-bit
    _getOneTouch(TOUCH_Z2, c, d);
    //    _getOneTouch(TOUCH_Z1 + 0x08 + 0x01, a, b); // 8-bit
    //    _getOneTouch(TOUCH_Z2 + 0x08 + 0x01, c, d);
    
    z0 = a + (0x7f - c);
    
    //    Serial.println(z0, DEC);
    
    if (z0 > _touchTrim) {
        do {
            //            _getOneTouch(TOUCH_X + 0x00 + 0x01, a, b);  // 12-bit
            //            _getOneTouch(TOUCH_Y + 0x00 + 0x01, c, d);
            _getOneTouch(TOUCH_X + 0x01, a, b);  // 8-bit
            _getOneTouch(TOUCH_Y + 0x01, c, d);
            delay(10);
            //            _getOneTouch(TOUCH_X + 0x00 + 0x01, aa, b); // 12-bit
            //            _getOneTouch(TOUCH_Y + 0x00 + 0x01, cc, d);
            _getOneTouch(TOUCH_X + 0x01, aa, b); // 12-bit
            _getOneTouch(TOUCH_Y + 0x01, cc, d);
        } while ( (a != aa) || (c != cc) );
        
        // In ER1963, x and y are swapped
        y0 = a <<4 | b >>4; // instead of normal x0
        x0 = c <<4 | d >>4; // instead of normal y0
    }
    
    delay(10);
}


void Screen_ER1963::feedArea(uint16_t colour)
{
    if (_fsmArea & 0xf0) {

#ifndef LCDC_CS_ISGROUNDED
	#if defined(__LM4F120H5QR__) || defined(__TM4C1230C3PM__) || defined(__TM4C129XNCZAD__) || defined(__TM4C1294NCPDT__)

        HWREG(LCD_CS_BASE + GPIO_O_DATA + (LCD_CS_PIN << 2)) = 0;               // CS low
	#else
        LCD_CS_PORTOUT &=~LCD_CS_PIN;
	#endif // end __LM4F120H5QR__
#endif

        uint8_t redColour8   =  (colour >>11) <<3;
        uint8_t greenColour8 =  (colour >> 5) <<2;
        uint8_t blueColour8  =   colour       <<3;
        
        //    uint8_t redColour8, greenColour8, blueColour8;
        //    myColours.split(colour, redColour8, greenColour8, blueColour8);
        
        _writeData888(redColour8, greenColour8, blueColour8);
#ifndef LCDC_CS_ISGROUNDED
	#if defined(__LM4F120H5QR__) || defined(__TM4C1230C3PM__) || defined(__TM4C129XNCZAD__) || defined(__TM4C1294NCPDT__)

        HWREG(LCD_CS_BASE + GPIO_O_DATA + (LCD_CS_PIN << 2)) = LCD_CS_PIN;      // CS high
	#else
		LCD_CS_PORTOUT |= LCD_CS_PIN;
	#endif // end __LM4F120H5QR__
#endif
    }
}
