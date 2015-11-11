
// comment out for Parallel 4 bit mode
// uncomment for I2C mode
//#define I2CMODE_OPTION

#ifdef I2CMODE_OPTION
// code for I2C display
#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>  // F Malpartida's NewLiquidCrystal library

// 0x27 0x3f  // Define I2C Address 
#define I2C_ADDR 0x27
#define BACKLIGHT_PIN  3
#define Rs_pin  0
#define Rw_pin  1
#define En_pin  2
#define D4_pin  4
#define D5_pin  5
#define D6_pin  6
#define D7_pin  7
#define  LED_OFF  0
#define  LED_ON  1

LiquidCrystal_I2C  lcd(I2C_ADDR,En_pin,Rw_pin,Rs_pin,D4_pin,D5_pin,D6_pin,D7_pin);
// end <I2C section 
#else
// Code for 4 bit Display

//#include <LiquidCrystal_SR.h>
//#include <LiquidCrystal_SR3W.h>
#include <FastIO.h>
//#include <I2CIO.h>
#include <LiquidCrystal.h>
//#include <LiquidCrystal_I2C.h>
//#include <LiquidCrystal_SR2W.h>
//#include <LCD.h>

// if it doesn't compile add pins.arduino.h in directory where is FastIO on sketchbook/library

//#define   CONTRAST_PIN   9
//#define   BACKLIGHT_PIN  7
//#define   CONTRAST       110
// LiquidCrystal lcd(12, 11, 5, 4, 3, 2, BACKLIGHT_PIN, POSITIVE );
// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(P2_0, P2_1, P2_2, P2_3, P2_4, P2_5);
// End Display 4 bit Section
#endif

/*
  VCC LD1R Tx  Rx BTN
   1   2   3   4   5   6   7   8   9  10
  vcc p10 p11 p12 p13 p14 P15 P20 p21 p22
  gnd p27 p26 TST RST P17 P16 P25 P24 P23
  20  19  18  17  16  15  14  13  12  11
  GND         TST RST    LD2G  
*/

#define Relay 2
#define STOPKEY 7
#define STARTKEY 5
// warning port name is supported by digital read but interrupt is not using it encoder is connected to pin 19 and 18
#define ENC_A   P2_7
#define ENC_B   P2_6


/*
 Demonstrates the use a 16x2 LCD display.  The LiquidCrystal
 library works with all LCD displays that are compatible with the 
 Hitachi HD44780 driver. There are many of them out there, and you
 can usually tell them by the 16-pin interface.
 
 The circuit:
 =================================
 LCD pin              Connect to
 ---------------------------------
 01 - GND             GND, pot
 02 - VCC             +5V, pot
 03 - Contrast        Pot wiper
 04 - RS              Pin8 (P2.0)
 05 - R/W             GND
 06 - EN              Pin9 (P2.1)
 07 - DB0             GND
 08 - DB1             GND
 09 - DB2             GND
 10 - DB3             GND
 11 - DB4             Pin10 (P2.2)
 12 - DB5             Pin11 (P2.3)
 13 - DB6             Pin12 (P2.4)
 14 - DB7             Pin13 (P2.5)
 15 - BL+             +5V
 16 - BL-             GND
 =================================

 I2C mode 
 SDA P1_7 Pin 14
 SCL P1_6 Pin 13 

*/


// encoder service

volatile int TimerTime=100;

void encoder_do(void)
{
  if(digitalRead(ENC_B))
  { if(TimerTime<10000)
      TimerTime++;
  }
  else
  { if(TimerTime>0)
      TimerTime--;
  }
}

void setup()
{
  // put your setup code here, to run once:
  pinMode(Relay,OUTPUT);
  digitalWrite(Relay,LOW); // switch off
  pinMode(STARTKEY,INPUT_PULLUP);
  pinMode(STOPKEY,INPUT_PULLUP);
  pinMode(ENC_A,INPUT_PULLUP);
  pinMode(ENC_B,INPUT_PULLUP);
//  pinMode(); STOPKEY STARTKEY
//    pinMode(CONTRAST_PIN, OUTPUT);
//  analogWrite ( CONTRAST_PIN, CONTRAST );

  //lcd.setBacklightPin ( BACKLIGHT_PIN, POSITIVE );
  //lcd.setBacklight ( HIGH );
  //lcd.backlight();
    
  lcd.begin(16,2);               // initialize the lcd 

//  lcd.createChar (0, smiley);    // load character to the LCD
//  lcd.createChar (1, armsUp);    // load character to the LCD
//  lcd.createChar (2, frownie);   // load character to the LCD

#ifdef  I2CMODE_OPTION
  lcd.setBacklightPin(BACKLIGHT_PIN, POSITIVE); // POSITIVE); //NEGATIVE);
  lcd.setBacklight(LED_ON);
#endif
  lcd.home ();                   // go home
  lcd.print("Preset       SBY");  
  lcd.setCursor ( 0, 1 );        // go to the next line
  lcd.print (" Timer ->");      
  attachInterrupt(ENC_A,encoder_do,FALLING);
}

#define KEYIsPressed(key) (!digitalRead(key))
#define KEYIsNotPressed(key) (digitalRead(key))

/*int StopIsPressed(void)
{
  if(digitalRead(STOPKEY))
    return(FALSE);
  else
    return(TRUE);
}
*/

long TimerTimeSec; // actual millis when timer elapse
int oldTimerTime=-1;
int TimerSecCounter; 
char dspBuffer[40];

void loop()
{
  // put your main code here, to run repeatedly:
  // setup loop
  do
  {
    if(oldTimerTime != TimerTime)
    {
      oldTimerTime=TimerTime;
      lcd.setCursor ( 7, 0);        // go to Status
      lcd.print("     ");
      lcd.setCursor ( 7, 0);        // go to Status
      lcd.print(TimerTime);
    }
    delay(20);
  }
  while(KEYIsNotPressed(STARTKEY));

  // timer loop
  TimerTimeSec=millis(); // timer value to actual time to display timer and decrement seconds
  TimerSecCounter=TimerTime+1; // add one to account for first update
  digitalWrite(Relay,HIGH); // switch on
  lcd.setCursor ( 13, 0);        // go to Status
  lcd.print(" ON");
  do
  {
    // test 1 second has elapsed 
    if(millis()>TimerTimeSec) // 1 second elapsed
    {
      TimerTimeSec=millis()+1000;
 //     sprintf(dspBuffer,"%6l",TimerSecCounter);
      TimerSecCounter--;
      lcd.setCursor ( 10, 1);        // go to countdown pos
      lcd.print("     "); //dspBuffer);
      lcd.setCursor ( 10, 1);        // go to countdown pos
      lcd.print(TimerSecCounter); //dspBuffer);
    }
    // test also if timer setup has changed and refresh
    if(oldTimerTime != TimerTime)
    {
      oldTimerTime=TimerTime;
      lcd.setCursor ( 7, 0);        // go to Status
      lcd.print("     ");
      lcd.setCursor ( 7, 0);        // go to Status
      lcd.print(TimerTime);
    }
    delay(20); // wait a whyle
}
  while(KEYIsNotPressed(STOPKEY) && TimerSecCounter);
  digitalWrite(Relay,LOW); // switch off
  lcd.setCursor ( 13, 0 );        // go to status line
  lcd.print("OFF");
 }
