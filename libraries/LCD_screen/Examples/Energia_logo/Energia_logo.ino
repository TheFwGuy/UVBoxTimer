///
/// @mainpage	Energia_logo
///
/// @details	<#details#>
/// @n
/// @n
/// @n @a	Developed with [embedXcode](http://embedXcode.weebly.com)
///
/// @author	Rei VILO
/// @author	embedXcode.weebly.com
/// @date	mai 04, 2013 15:41
/// @version	<#version#>
///
/// @copyright	© Rei VILO, 2013
/// @copyright	CC = BY NC SA
///
/// @see	ReadMe.txt for references
///


///
/// @file	Energia_logo.ino
/// @brief	Main sketch
///
/// @details	<#details#>
/// @n @a	Developed with [embedXcode](http://embedXcode.weebly.com)
///
/// @author	Rei VILO
/// @author	embedXcode.weebly.com
/// @date	mai 04, 2013 15:41
/// @version	<#version#>
///
/// @copyright	© Rei VILO, 2013
/// @copyright	CC = BY NC SA
///
/// @see	ReadMe.txt for references
/// @n
///


// Core library for code-sense
#if defined(WIRING) // Wiring specific
#include "Wiring.h"
#elif defined(MAPLE_IDE) // Maple specific
#include "WProgram.h"
#elif defined(MPIDE) // chipKIT specific
#include "WProgram.h"
#elif defined(DIGISPARK) // Digispark specific
#include "Arduino.h"
#elif defined(ENERGIA) // LaunchPad, FraunchPad and StellarPad specific
#include "Energia.h"
#elif defined(CORE_TEENSY) // Teensy specific
#include "WProgram.h"
#elif defined(ARDUINO) && (ARDUINO >= 100) // Arduino 1.0 and 1.5 specific
#include "Arduino.h"
#elif defined(ARDUINO) && (ARDUINO < 100) // Arduino 23 specific
#include "WProgram.h"
#else // error
#error Platform not defined
#endif

// Include application, user and local libraries


#define HY28A
#define LOGO 320 // 100, 220 or 320

#if defined(ILI9225B)
#include "ILI9225B_screen.h"
#elif defined(PicasoSPE)
#include "PicasoSPE_screen.h"
#elif defined(HY28A)
#include "HY28A_screen.h"
#elif defined(ST7735)
#include "ST7735_screen.h"
#else
#error Unknown screen
#endif

#if (LOGO == 100)
#include "Energia_logo_100_132.h"
#elif   (LOGO == 220)
#include "Energia_logo_220_96.h"
#elif   (LOGO == 320)
#include "Energia_logo_320_140.h"
#else
#error Wrong size
#endif

#include "SPI.h"

// Define variables and constants
///
/// @brief	Name of the LED
/// @details	Each board has a LED but connected to a different pin
///
uint8_t myLED;
uint32_t p;
uint16_t c, x00, y00;

#if defined(ILI9225B)
LCD_ILI9225B myLCD;
#elif defined(PicasoSPE)
#define mySerial Serial1
LCD_PicasoSPE myLCD(4, &mySerial);
#elif defined(HY28A)
LCD_HY28A myLCD;
#elif defined(ST7735)
LCD_ST7735 myLCD(ST7735R_RED_TAB);
#else
#error Unknown screen
#endif

void blink(uint8_t pin, uint8_t times, uint16_t ms) {
  for (uint8_t i=0; i<times; i++) {
    digitalWrite(pin, HIGH); 
    delay(ms >> 1);               
    digitalWrite(pin, LOW);  
    delay(ms >> 1);              
  }
}

///
/// @brief	Setup
/// @details	Define the pin the LED is connected to
///
// Add setup code
void setup() {
    // myLED pin number
#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega2560__) || defined(__AVR_ATmega32U4__) || defined(__SAM3X8E__) // Arduino specific
    myLED = 13;
#elif defined(__PIC32MX__) // chipKIT specific
    myLED = 13;
#elif defined(__AVR_ATtinyX5__) // Digispark specific
    myLED = 1; // assuming model A
#elif defined(__AVR_ATmega644P__) // Wiring specific
    myLED = 15;
#elif defined(__MSP430G2452__) || defined(__MSP430G2553__) || defined(__MSP430G2231__) || defined(__MSP430FR5739__) // LaunchPad and FraunchPad specific
    myLED = RED_LED;
#elif defined(__LM4F120H5QR__) // StellarPad specific
    myLED = RED_LED;
#elif defined(MCU_STM32F103RB) || defined(MCU_STM32F103ZE) || defined(MCU_STM32F103CB) || defined(MCU_STM32F103RE) // Maple specific
    myLED = BOARD_LED_PIN;
#elif defined(__MK20DX128__) // Teensy 3.0 specific
    myLED = 13;
#endif
    
    pinMode(myLED, OUTPUT);
    blink(myLED, 3, 333);
    
#if defined(PicasoSPE)
    mySerial.begin(9600);
#endif

	myLCD.begin();
    myLCD.setOrientation(3);
    
    Serial.begin(9600);
    Serial.println();
    Serial.print("logo \t");

//    uint32_t chrono;
//    chrono = millis();
//    myLCD.logo();
//    Serial.println(millis() - chrono, DEC);
    
    //    myLCD.setSpeed((uint32_t)115200);
    //    mySerial.begin((uint32_t)115200);
    
    myLCD.clear(whiteColour);

//    chrono = millis();

#if (LOGO == 100)

    x00 = (myLCD.screenSizeX() - x_Energia_logo_100_132_bmp) / 2;
    y00 = (myLCD.screenSizeY() - y_Energia_logo_100_132_bmp) / 2;
    p = (uint32_t)x_Energia_logo_100_132_bmp * y_Energia_logo_100_132_bmp;
    
    for (uint16_t i=0; i<x_Energia_logo_100_132_bmp; i++) {
        for (uint16_t j=0; j<y_Energia_logo_100_132_bmp; j++) {
#if defined(ENERGIA)
            c = pic_Energia_logo_100_132_bmp[i*y_Energia_logo_100_132_bmp + j];
#else
            c = pgm_read_word(&pic_Energia_logo_100_132_bmp[j*y_Energia_logo_100_132_bmp + i]);
#endif
            myLCD.point(x00+i, y00+j, c);
        }
    }

#elif   (LOGO == 220)
    
    x00 = (myLCD.screenSizeX() - x_Energia_logo_220_96_bmp) / 2;
    y00 = (myLCD.screenSizeY() - y_Energia_logo_220_96_bmp) / 2;
    p = (uint32_t)x_Energia_logo_220_96_bmp * y_Energia_logo_220_96_bmp;
    
    for (uint16_t i=0; i<x_Energia_logo_220_96_bmp; i++) {
        for (uint16_t j=0; j<y_Energia_logo_220_96_bmp; j++) {
#if defined(ENERGIA)
            c = pic_Energia_logo_220_96_bmp[i*y_Energia_logo_220_96_bmp + j];
#else
            c = pgm_read_word(&pic_Energia_logo_220_96_bmp[j*y_Energia_logo_220_96_bmp + i]);
#endif
            myLCD.point(x00+i, y00+j, c);
        }
    }
    

    
#elif   (LOGO == 320)
    
    x00 = (myLCD.screenSizeX() - x_Energia_logo_320_140_bmp) / 2;
    y00 = (myLCD.screenSizeY() - y_Energia_logo_320_140_bmp) / 2;
    p = (uint32_t)x_Energia_logo_320_140_bmp * y_Energia_logo_320_140_bmp;
    
    for (uint16_t i=0; i<x_Energia_logo_320_140_bmp; i++) {
        for (uint16_t j=0; j<y_Energia_logo_320_140_bmp; j++) {
#if defined(ENERGIA)
            c = pic_Energia_logo_320_140_bmp[i*y_Energia_logo_320_140_bmp + j];
#else
            c = pgm_read_word(&pic_Energia_logo_320_140_bmp[i*y_Energia_logo_320_140_bmp + j]);
#endif
            myLCD.point(x00+i, y00+j, c);
        }
    }
#else
#error Wrong size
#endif // logo
//    Serial.println(millis() - chrono, DEC);

}

///
/// @brief	Loop
/// @details	Call blink
///
// Add loop code
void loop() {
    blink(myLED, 3, 333);
    delay(1000);
}


//// Communication, fast write
//#include "Energia_logo_320_140.h"
//
//void LCD_HY28A::logo()
//{
//    uint16_t x00 = (screenSizeX() - x_Energia_logo_320_140_bmp) / 2;
//    uint16_t y00 = (screenSizeY() - y_Energia_logo_320_140_bmp) / 2;
//    uint16_t colour;
//
//    _setWindow(x00, y00, x00+x_Energia_logo_320_140_bmp, y00+y_Energia_logo_320_140_bmp);
//    _writeIndex(0x0022);
//
//    digitalWrite(_pinScreenChipSelect, LOW);
//    SPI.transfer(SPI_START | SPI_WR | SPI_DATA); // write : RS = 1, RW = 0
//
//    for (uint16_t j=0; j<y_Energia_logo_320_140_bmp; j++) {
//        for (uint16_t i=0; i<x_Energia_logo_320_140_bmp; i++) {
//#if defined(ENERGIA)
//            colour = pic_Energia_logo_320_140_bmp[i*y_Energia_logo_320_140_bmp + y_Energia_logo_320_140_bmp-j-1];
//#else
//            colour = pgm_read_word(&pic_Energia_logo_320_140_bmp[j*y_Energia_logo_100_132_bmp + i]);
//#endif
//            SPI.transfer(highByte(colour)); // write D8..D15
//            SPI.transfer(lowByte(colour)); // write D0..D7
//        }
//    }
//
//    digitalWrite(_pinScreenChipSelect, HIGH);
//    _setWindow(0, 0, screenSizeX()-1, screenSizeY()-1);
//}

