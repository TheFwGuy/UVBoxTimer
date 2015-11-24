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

void settingManagement();
void displaySetting();
void secondsToTime(int, short *, short *, short * );
void displayTime(short, short, short);
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
#define ALR   3
#define STOP  4
#define END   5

/* Setting state machine states */

#define SET_SECONDS   0
#define SET_MINUTES   1
#define SET_HOURS     2
#define SET_SAVINGS   3

LiquidCrystal_I2C  lcd(I2C_ADDR,En_pin,Rw_pin,Rs_pin,D4_pin,D5_pin,D6_pin,D7_pin, BACKLIGHT_PIN, POSITIVE);

/*
 *  I/O defines
 */

#define RELAY_LAMP 2		/* P1_0 */
#define RELAY_FAN  3	        /* P1_1 */

#define STARTLED   4            /* P1_2 */
#define STARTBTN   5            /* P1_3 */

#define BUZZER     6            /* P1_4 */

#define SELBTN     13           /* P2_5 */

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
int oldTimerTime=0;
unsigned char saving_flag = 0;		/* 0 - don't save - 1 - save */
int TimerSecCounter; 

short hour = 0;
short minute = 0;
short second = 0;

char buffer[20];

unsigned char main_status = IDLE;
unsigned char set_status  = SET_SECONDS;
unsigned char force_display_setting = 0;

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

  pinMode(BUZZER,OUTPUT);

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
  lcd.print("Time            ");  
  lcd.setCursor ( 0, 1 );        // go to the next line
  lcd.print ("UVBox Timer 1.0");      
  
  displayTime(hour, minute, second);        
  
  attachInterrupt(ENC_A,doEncoder,CHANGE);
}


void loop()
{
   short run_hour;
   short run_minute;
   short run_second;
   // put your main code here, to run repeatedly:
   // setup loop

   switch(main_status)
   {
      case IDLE:
         settingManagement();		/* Handle settings */
		 
         if(KEYIsPressed(STARTBTN) && !(hour==0 && minute == 0 && second == 0))
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
         lcd.print ("Action: RUN     ");       
         main_status = RUN;
         break;
      
      case RUN:  
         // test 1 second has elapsed 
         if(millis()>TimerTimeSec) // 1 second elapsed
         {
            TimerTimeSec=millis()+1000;
            TimerSecCounter--;

            secondsToTime(TimerSecCounter, &run_hour, &run_minute, &run_second);
            
            displayTime(run_hour, run_minute, run_second);        
         }
            
         if(KEYIsPressed(STARTBTN))
            main_status = STOP;
         if(TimerSecCounter == 0)
            main_status = ALR;
         break;
             
      case ALR:
         digitalWrite(RELAY_LAMP,LOW); // switch off
         digitalWrite(STARTLED,LOW);   // Pushbutton LED off
         lcd.setCursor ( 0, 1 );        // go to the next line
         lcd.print ("Action: END     ");       
         tone(BUZZER, 550);            /* Emit tone */
      
      case STOP:
         if(KEYIsPressed(STARTBTN))
         {
            while(KEYIsPressed(STARTBTN));  /* Wait until the button is released */
            digitalWrite(RELAY_LAMP,LOW); // switch off
            digitalWrite(STARTLED,LOW);   // Pushbutton LED off
            noTone(BUZZER);                  /* Stop tone */

            lcd.setCursor ( 0, 1 );        // go to the next line
            lcd.print ("Action: STOP    ");       
            while(KEYIsNotPressed(STARTBTN));  /* Wait until the button is pressed */
            while(KEYIsPressed(STARTBTN));  /* Wait until the button is released */

            main_status = END;
         }
         break;
         
      case END:      
         lcd.setCursor ( 0, 1 );        // go to the next line
         lcd.print ("Action: END     ");       
   
         TimerTime = second;      // Set default on last second value 
         oldTimerTime = second;

         main_status = IDLE;
         set_status  = SET_SECONDS;
         
         displayTime(hour, minute, second);        
         break;
   }
}


/*
 *  settingManagement function
 *  This function manage the setting state machine.
 *  This state machine is working only when the main state machine is in IDLE, otherwise it is never called.
 */
void settingManagement()
{
   displaySetting();

   if(KEYIsPressed(SELBTN))
    {
       while(KEYIsPressed(SELBTN));
                         
       force_display_setting = 1;   

       switch(set_status)
       {
         case SET_SECONDS:
            TimerTime = minute;
            oldTimerTime = minute;
            break;

         case SET_MINUTES:
            TimerTime = hour;
            oldTimerTime = hour;
            break;

         case SET_HOURS:
            TimerTime = 1;
            oldTimerTime = 1;
            saving_flag = 1;
            break;
 
        case SET_SAVINGS:
            TimerTime = second;
            oldTimerTime = second;
			
	    /* Button pushed in this state - if flag OK save value in eeprom */
            if(saving_flag)
	    {
	       /* TBD - save value in eeprom */
	       saving_flag = 0;
	    }
	    break;
       }     

       set_status++;
       if(set_status > SET_SAVINGS)
          set_status = SET_SECONDS;
	  
   }
}

/*
 *  displaySetting function
 *  This function manage the setting state machine.
 *  This state machine is working only when the main state machine is in IDLE, otherwise it is never called.
 */
void displaySetting()
{
   if(oldTimerTime != TimerTime || force_display_setting)
   {
      force_display_setting = 0;

      lcd.setCursor ( 0, 1 );        // go to the next line
      switch(set_status)
      {
         case SET_SECONDS:
            lcd.print ("Set Seconds     ");
            if(TimerTime > 59)
               TimerTime = 59;
            second = TimerTime;   
            break;

         case SET_MINUTES:
            lcd.print ("Set Minutes     ");
            if(TimerTime > 59)
               TimerTime = 59;
            minute = TimerTime;   
            break;

         case SET_HOURS:
            lcd.print ("Set Hours       ");
            if(TimerTime > 2)
               TimerTime = 2;
            hour = TimerTime;   
            break;

	case SET_SAVINGS:
	    switch(TimerTime)
            {
	       case 0:
                  lcd.print ("Save ?      No  ");
                  break;
	      case 1:
                  lcd.print ("Save ?      Yes ");
		  break;		  
	    }
			
            if(TimerTime > 1)
               TimerTime = 1;
            saving_flag = TimerTime;   
            break;
       }
     
      oldTimerTime=TimerTime;
      
      displayTime(hour, minute, second);
          
   }
}

void displayTime(short hours, short minutes, short seconds)
{
   lcd.setCursor ( 7, 0);        // go to Status
   lcd.print("     ");
   lcd.setCursor ( 7, 0);        // go to Status

   sprintf(buffer, "%02d:%02d:%02d", hours, minutes, seconds);
   lcd.print(buffer);         
   
//            lcd.setCursor ( 10, 1);        // go to countdown pos
//            lcd.print("     ");            // clean space space
//            lcd.setCursor ( 10, 1);        // go to countdown pos
//            lcd.print(TimerSecCounter);     
   
}

/*
 *  secondsToTime
 *  The function has the purpose to convert a single value in seconds into
 *  the more traditional hours/minutes/seconds display
 */
void secondsToTime(int raw_seconds, short *hours, short *minutes, short *seconds )
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


