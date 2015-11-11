///
/// @mainpage	LCD_clock_main
///
/// @details	<#details#>
/// @n
/// @n
/// @n @a	Developed with [embedXcode](http://embedXcode.weebly.com)
///
/// @author	Rei VILO
/// @author	embedXcode.weebly.com
/// @date	mai 19, 2013 22:18
/// @version	<#version#>
///
/// @copyright	© Rei VILO, 2013
/// @copyright	CC = BY NC SA
///
/// @see	ReadMe.txt for references
///


///
/// @file	LCD_screen_font_main.ino
/// @brief	Main sketch
///
/// @details	<#details#>
/// @n @a	Developed with [embedXcode](http://embedXcode.weebly.com)
///
/// @author	Rei VILO
/// @author	embedXcode.weebly.com
/// @date	mai 19, 2013 22:18
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
#define HY28A // ILI9225B HY28A ST7735 PicasoSPE PicasoSGC

#if defined(ILI9225B)
#include "ILI9225B_screen.h"
LCD_ILI9225B myScreen;

#elif defined(HY28A)
#include "HY28A_screen.h"
LCD_HY28A myScreen;

#elif defined(ST7735)
#include "ST7735_screen.h"
LCD_ST7735 myScreen(ST7735R_RED_TAB);

#elif defined(PicasoSPE)
#include "PicasoSPE_screen.h"
#define mySerial Serial1
LCD_PicasoSPE myScreen(20, &mySerial);

#elif defined(PicasoSGC)
#include "PicasoSGC_screen.h"
#define mySerial Serial1
LCD_PicasoSGC myScreen(20, &mySerial);

#else
#error Unkwon screen
#endif

#include "LCD_clock.h"
#include "SPI.h"

// Define variables and constants
uint8_t h, m, s;
gClock myClock;

// Add setup code
void setup() {
  Serial.begin(9600);
#if defined(PicasoSPE) || defined(PicasoSGC)
  mySerial.begin(9600);
#endif

  delay(10);
  Serial.println("*** LCD_clock_main");

  myScreen.begin();
#if defined(PicasoSPE) || defined(PicasoSGC)
  myScreen.setSpeed(38400); // 38400 max with Arduino mega2560
  mySerial.begin(38400);
#endif

  myScreen.setOrientation(1);

  myScreen.clear(grayColour);
  myClock.define(&myScreen, myScreen.screenSizeX()/2, myScreen.screenSizeY()/2, min(myScreen.screenSizeX()/2, myScreen.screenSizeY()/2)-20);

  h = 10;
  m = 10;
  s = 30;
}

void loop() {
  myClock.draw(h, m, s, "LCD_clock");
  s++;
  if (s > 59) {
    m++;
    s = 0;
  }
  if (m > 59) {
    h++;
    m = 0;
  }
  if (h > 23) {
    h = 0;
  }
  delay(333);
}

