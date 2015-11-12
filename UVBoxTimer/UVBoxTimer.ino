/**
 *  UV Box Timer project
 *  @Auth Roberto Romano
 *  @Auth TheFwGuy
 *  November 2015
 *  @Brief Timer to control 12 UV fluorescent lamps with MSP430g2452
 *
 */

#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>  // F Malpartida's NewLiquidCrystal library

void updateSetting();

/* Define I2C Address  */
#define I2C_ADDR 0x3F
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

#define IDLE  0
#define START 1
#define RUN   2
#define STOP  3

LiquidCrystal_I2C  lcd(I2C_ADDR,En_pin,Rw_pin,Rs_pin,D4_pin,D5_pin,D6_pin,D7_pin, BACKLIGHT_PIN, POSITIVE);


/*
 *  I/O defines
 */

#define RELAY_LAMP 2				/* P1_0 */
#define RELAY_FAN  3				/* P1_1 */

//#define STOPKEY    5
#define STARTKEY   5

// warning port name is supported by digital read but interrupt is not using it encoder is connected to pin 19 and 18
#define ENC_A   P2_3
#define ENC_B   P2_4

#define KEYIsPressed(key) (!digitalRead(key))
#define KEYIsNotPressed(key) (digitalRead(key))

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


long TimerTimeSec; // actual millis when timer elapse

int TimerTime=100;
int oldTimerTime=-1;

int TimerSecCounter; 
char dspBuffer[20];

unsigned char status = IDLE;



void encoder_do(void)
{
   if(digitalRead(ENC_B))
   {
      if(TimerTime<10000)
         TimerTime++;
   }
   else
   {
      if(TimerTime>0)
         TimerTime--;
   }
}

void setup()
{
  // put your setup code here, to run once:
  pinMode(RELAY_LAMP,OUTPUT);
  digitalWrite(RELAY_LAMP,LOW); // switch off
  
  pinMode(STARTKEY,INPUT_PULLUP);
  pinMode(ENC_A,INPUT_PULLUP);
  pinMode(ENC_B,INPUT_PULLUP);

  lcd.setBacklightPin(BACKLIGHT_PIN, POSITIVE); // POSITIVE); //NEGATIVE);
  lcd.setBacklight(LED_ON);
  lcd.backlight();
    
  lcd.begin(16,2);               // initialize the lcd 

//  lcd.createChar (0, smiley);    // load character to the LCD
//  lcd.createChar (1, armsUp);    // load character to the LCD
//  lcd.createChar (2, frownie);   // load character to the LCD

  lcd.clear();
  lcd.home ();                   // go home
  lcd.print("Preset       SBY");  
  lcd.setCursor ( 0, 1 );        // go to the next line
  lcd.print (" Timer ->");      
  
  attachInterrupt(ENC_A,encoder_do,FALLING);
}


void loop()
{
   // put your main code here, to run repeatedly:
   // setup loop

   switch(status)
   {
      case IDLE:
         updateSetting();
         if(!KEYIsNotPressed(STARTKEY))
            status = START;
         break;
         
      case START:   
         // timer loop
         TimerTimeSec=millis(); // timer value to actual time to display timer and decrement seconds
         TimerSecCounter=TimerTime+1; // add one to account for first update
         digitalWrite(RELAY_LAMP,HIGH); // switch on
         lcd.setCursor ( 13, 0);        // go to Status
         lcd.print(" ON");
         status = RUN;
         break;
      
      case RUN:  
         // test 1 second has elapsed 
         if(millis()>TimerTimeSec) // 1 second elapsed
         {
            TimerTimeSec=millis()+1000;
       //   sprintf(dspBuffer,"%6l",TimerSecCounter);
            TimerSecCounter--;
            lcd.setCursor ( 10, 1);        // go to countdown pos
            lcd.print("     "); //dspBuffer);
            lcd.setCursor ( 10, 1);        // go to countdown pos
      
            sprintf(dspBuffer,"%d",TimerSecCounter);
            lcd.print(dspBuffer);
         }
            
         updateSetting();
         
         if(!KEYIsNotPressed(STARTKEY) || TimerSecCounter == 0)
            status = STOP;
         break;
         
      case STOP:      
         digitalWrite(RELAY_LAMP,LOW); // switch off
         lcd.setCursor ( 13, 0 );        // go to status line
         lcd.print("OFF");
         status = IDLE;
         break;
   }
}

void updateSetting()
{
   if(oldTimerTime != TimerTime)
   {
      oldTimerTime=TimerTime;
      lcd.setCursor ( 7, 0);        // go to Status
      lcd.print("     ");
      lcd.setCursor ( 7, 0);        // go to Status
      sprintf(dspBuffer,"%6d",TimerTime);
      lcd.print(dspBuffer);
   }
}
