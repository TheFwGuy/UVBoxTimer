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

/* Function prototypes */

void displaySettingTime();
void secondsToTime(int, int *, int *, int * );

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

#define SELBTN     13
#define STARTBTN   5
#define STARTLED   4

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

int TimerTime=5;
int oldTimerTime=-1;
int TimerSecCounter; 

int hour;
int minute;
int second;

char buffer[20];

unsigned char status = IDLE;

void doEncoder() 
{
   /* Only in IDLE is allowed to set up */
   if(status != IDLE)
      return;

   /* If pinA and pinB are both high or both low, it is spinning
    * forward. If they're different, it's going backward.
    *
    * For more information on speeding up this process, see
    * [Reference/PortManipulation], specifically the PIND register.
    */
   if (digitalRead(ENC_A) == digitalRead(ENC_B)) 
   {
      if(TimerTime<7200)
         TimerTime += 5;
   }
   else 
   {
      if(TimerTime>0)
         TimerTime -= 5;
   }
}

void setup()
{
  // put your setup code here, to run once:
  pinMode(RELAY_LAMP,OUTPUT);
  digitalWrite(RELAY_LAMP,LOW); // switch off
  
  pinMode(STARTBTN,INPUT_PULLUP);
  pinMode(STARTLED,OUTPUT);

  pinMode(SELBTN,INPUT_PULLUP);
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
  lcd.print("Preset          ");  
  lcd.setCursor ( 0, 1 );        // go to the next line
  lcd.print ("UVBox Timer 1.0");      
  
  attachInterrupt(ENC_A,doEncoder,FALLING);
}


void loop()
{
   // put your main code here, to run repeatedly:
   // setup loop

   switch(status)
   {
      case IDLE:
         displaySettingTime();
         
         if(KEYIsPressed(SELBTN))
         {
            while(KEYIsPressed(SELBTN));
            lcd.setCursor ( 0, 1 );        // go to the next line
            lcd.print ("UVBox Timer 1.0");
         }
         
         if(KEYIsPressed(STARTBTN))
            status = START;
         break;
         
      case START:   
         while(KEYIsPressed(STARTBTN));  /* Wait until the button is released */

         // timer loop
         TimerTimeSec=millis(); // timer value to actual time to display timer and decrement seconds
         TimerSecCounter=TimerTime+1; // add one to account for first update
         digitalWrite(RELAY_LAMP,HIGH); // switch on
         
         lcd.setCursor ( 0, 1 );        // go to the next line
         lcd.print ("RUN             ");       
//         lcd.setCursor ( 13, 0);        // go to Status
//         lcd.print(" ON");
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
            lcd.print(TimerSecCounter);     
         }
            
//         updateSetting();
         
         if(KEYIsPressed(STARTBTN) || TimerSecCounter == 0)
            status = STOP;
         break;
         
      case STOP:      
         while(KEYIsPressed(STARTBTN));  /* Wait until the button is released */

         digitalWrite(RELAY_LAMP,LOW); // switch off

         lcd.setCursor ( 0, 1 );        // go to the next line
         lcd.print ("STOP            ");       

//         lcd.setCursor ( 13, 0 );        // go to status line
//         lcd.print("OFF");
         status = IDLE;
         break;
   }
}

void displaySettingTime()
{
   if(oldTimerTime != TimerTime)
   {
      oldTimerTime=TimerTime;
      
      secondsToTime(TimerTime, &hour, &minute, &second);
      
      lcd.setCursor ( 7, 0);        // go to Status
      lcd.print("     ");
      lcd.setCursor ( 7, 0);        // go to Status
      
      sprintf(buffer, "%02d:%02d:%02d", hour, minute, second);
      lcd.print(buffer);
      
//      lcd.print(TimerTime);     
   }
}

/*
 *  secondsToTime
 *  The function has the purpose to convert a single value in seconds into
 *  the more traditional hours/minutes/seconds display
 */
void secondsToTime(int raw_seconds, int *hours, int *minutes, int *seconds )
{
   int temp=0;

   *hours = raw_seconds/3600;
   temp = *hours * 3600;
   *minutes = (raw_seconds - temp)/60;
   temp += *minutes * 60;
   *seconds = raw_seconds - temp;
}


