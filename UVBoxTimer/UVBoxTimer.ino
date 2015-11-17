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
int timeToSeconds(int , int , int );

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

/* Main state machine states */

#define IDLE  0
#define START 1
#define RUN   2
#define STOP  3

/* Setting state machine states */

#define SET_SECONDS   0
#define SET_MINUTES   1
#define SET_HOURS     2

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
 I2C mode 
 SDA P1_7 Pin 14
 SCL P1_6 Pin 13 

*/


/*
 *  Global variables
 */

long TimerTimeSec; // actual millis when timer elapse

int TimerTime=0;
int oldTimerTime=-1;
int TimerSecCounter; 

short hour = 0;
short minute = 0;
short second = 5;

char buffer[20];

unsigned char main_status = IDLE;
unsigned char set_status  = SET_SECONDS;

/* Interrupt service encoder function */

void doEncoder() 
{
   /* Only in IDLE is allowed to set up */
   if(main_status != IDLE)
      return;

   /* If pinA and pinB are both high or both low, it is spinning
    * forward. If they're different, it's going backward.
    *
    * For more information on speeding up this process, see
    * [Reference/PortManipulation], specifically the PIND register.
    */
   if (digitalRead(ENC_A) == digitalRead(ENC_B)) 
   {
      if(TimerTime<80)
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
  
  pinMode(STARTBTN,INPUT_PULLUP);
  pinMode(STARTLED,OUTPUT);
  digitalWrite(STARTLED,LOW); // Pushbutton LED off

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
  lcd.display();
  lcd.print("Preset          ");  
  lcd.setCursor ( 0, 1 );        // go to the next line
  lcd.print ("UVBox Timer 1.0");      
  
  attachInterrupt(ENC_A,doEncoder,CHANGE);
}


void loop()
{
   // put your main code here, to run repeatedly:
   // setup loop

   switch(main_status)
   {
      case IDLE:
         displaySettingTime();

         if(KEYIsPressed(SELBTN))
         {
            while(KEYIsPressed(SELBTN));
            
            set_status++;
            if(set_status > SET_HOURS)
               set_status = SET_SECONDS;
               
           lcd.setCursor ( 0, 1 );        // go to the next line
           
           switch(set_status)
           {
               case SET_SECONDS:
                  lcd.print ("Set Seconds     ");
                  TimerTime = second;
                  oldTimerTime = second;
                  break;

               case SET_MINUTES:
                  lcd.print ("Set Minutes     ");
                  TimerTime = minute;
                  oldTimerTime = minute;
                  break;

               case SET_HOURS:
                  lcd.print ("Set Hours       ");
                  TimerTime = hour;
                  oldTimerTime = hour;
                  break;
                 
//               default:
//                  set_status = SET_SECONDS;
//                  break;   
            }      
         }
               
         if(KEYIsPressed(STARTBTN))
            main_status = START;
         break;
         
      case START:   
         while(KEYIsPressed(STARTBTN));  /* Wait until the button is released */

         // timer loop
         TimerTimeSec=millis(); // timer value to actual time to display timer and decrement seconds
         TimerSecCounter=timeToSeconds(hour, minute,second)+1; // add one to account for first update

         digitalWrite(RELAY_LAMP,HIGH); // switch on
         digitalWrite(STARTLED,HIGH);   // Pushbutton LED on
         
         lcd.setCursor ( 0, 1 );        // go to the next line
         lcd.print ("RUN             ");       
         main_status = RUN;
         break;
      
      case RUN:  
         // test 1 second has elapsed 
         if(millis()>TimerTimeSec) // 1 second elapsed
         {
            TimerTimeSec=millis()+1000;
       //   sprintf(dspBuffer,"%6l",TimerSecCounter);
            TimerSecCounter--;
            lcd.setCursor ( 10, 1);        // go to countdown pos
            lcd.print("     ");            // clean space space
            lcd.setCursor ( 10, 1);        // go to countdown pos
            lcd.print(TimerSecCounter);     
         }
            
         if(KEYIsPressed(STARTBTN) || TimerSecCounter == 0)
            main_status = STOP;
         break;
         
      case STOP:      
         while(KEYIsPressed(STARTBTN));  /* Wait until the button is released */

         digitalWrite(RELAY_LAMP,LOW); // switch off
         digitalWrite(STARTLED,LOW);   // Pushbutton LED off
 
         lcd.setCursor ( 0, 1 );        // go to the next line
         lcd.print ("STOP            ");       

         TimerTime = second;      // Set default on last second value 
         oldTimerTime = second;

         main_status = IDLE;
         set_status  = SET_SECONDS;
         break;
   }
}

void displaySettingTime()
{
   if(oldTimerTime != TimerTime)
   {
      switch(set_status)
      {
         case SET_SECONDS:
            if(TimerTime > 59)
               TimerTime = 59;
            second = TimerTime;   
            break;

         case SET_MINUTES:
            if(TimerTime > 59)
               TimerTime = 59;
            minute = TimerTime;   
            break;

         case SET_HOURS:
            if(TimerTime > 2)
               TimerTime = 2;
            hour = TimerTime;   
            break;
      }
     
      oldTimerTime=TimerTime;
      
 //     secondsToTime(TimerTime, &hour, &minute, &second);
      
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

/*
 *  timeToSeconds
 *  The function has the purpose to create a single value in seconds from hour/minutes/seconds
 */
int timeToSeconds(int hours, int minutes, int seconds )
{
   int raw_seconds;
   
   raw_seconds = (hours * 3600) + (minutes * 60) + seconds;
   
   return(raw_seconds);
}


