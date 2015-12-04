/**
 *  UV Box Timer project
 *  @Auth Roberto Romano
 *  @Auth TheFwGuy
 *  November 2015
 *  @Brief Timer to control 12 UV fluorescent lamps with MSP430g2553
 *
 *  A LCD display, 16 x 2, is used to display information.
 *  The top line shows always the timer value.
 *  The bottom line shows menu' and indications.
 *
 */

#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>  // F Malpartida's NewLiquidCrystal library
#include "MspFlash.h"

/* Function prototypes */

void settingManagement();
void displaySetting();
void secondsToTime(int, short *, short *, short * );
void displayTime(short, short, short);
void displayMenu();
int timeToSeconds(int , int , int );
unsigned char waitingResetTimer(unsigned char);

/* 
 *  I2C display defines
 */
 
#define I2C_ADDR 0x3F
#define BACKLIGHT_PIN  3

#define Rs_pin  0
#define Rw_pin  1
#define En_pin  2
#define D4_pin  4
#define D5_pin  5
#define D6_pin  6
#define D7_pin  7

LiquidCrystal_I2C  lcd(I2C_ADDR,En_pin,Rw_pin,Rs_pin,D4_pin,D5_pin,D6_pin,D7_pin, BACKLIGHT_PIN, POSITIVE);

/*
 *  Generic defines
 */
#define  OFF  0
#define  ON  1

#define DISPLAY_TIME	/* Comment this out in order to display seconds countdwon */

#define MAX_PRESET	10  /* Max number of preset values handled */

#define BURNS_ENCODER    /* If defined the encoder used is the burns one (from Adafruit) - if not defined is the one from All Electronics */

/* Main state machine states defines */

#define IDLE  0
#define START 1
#define RUN   2
#define ALR   3
#define STOP  4
#define END   5

/* Setting state machine states defines */

#define READ_PRESETS  0
#define SET_SECONDS   1
#define SET_MINUTES   2
#define SET_HOURS     3
#define SAVE_PRESETS  4

/*
 *  I/O defines
 */

#define RELAY_LAMP 2		/* P1_0 */

#define STARTBTN   5        /* P1_3 */
#define STARTLED   6        /* P1_4 */
#define RELAY_FAN  7	    /* P1_5 */

#define BUZZER     8        /* P2_0 */

#define SELBTN     13       /* P2_5 */

// warning port name is supported by digital read but interrupt is not using it encoder is connected to pin 19 and 18
#define ENC_A   P2_4
#define ENC_B   P2_3

/*
 *  Macro defines
 */
 
#define KEYIsPressed(key) (!digitalRead(key))
#define KEYIsNotPressed(key) (digitalRead(key))

/*
 I2C mode 
 SDA P1_7 Pin 14
 SCL P1_6 Pin 13 
*/

/*
 *  Flash define
 *  Uses Segment D - 64 bytes - 2 bytes for a time - max allowed 32 presets
 *  Used only 10 presets
 */
#define flash SEGMENT_D 

/*
 *  Global variables
 */

long TimerTimeSec; // actual millis when timer elapse

int EncoderRead=0;
int oldEncoderRead=0;

int TimerSecCounter; 

short hour = 0;
short minute = 0;
short second = 0;

char buffer[20];

unsigned char saving_flag = 0;		        /* 0 - don't save - 1 - save */
unsigned char toggle_LED = 0;
unsigned char main_status = IDLE;
unsigned char set_status  = READ_PRESETS;
unsigned char force_display_setting = OFF;
unsigned char force_display_title = OFF;         /* 1 display the title */
unsigned char preset_index = 0;

const char time_menu[]   = "Time            ";
const char logo_menu[]   = "UVBox Timer 1.0 ";
const char preset_menu[] = "Preset #        ";
const char select_menu[] = "Set             ";
const char run_menu[]    = "Action:         ";


/**
 *  @fn doEncoder
 *  @brief Interrupt service encoder function 
 *  The read has an upper limt to 80 and a lower limit of 0
 */

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
      if(EncoderRead<80)
         EncoderRead++;
   }
   else 
   {
      if(EncoderRead>0)
         EncoderRead--;
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
  lcd.setBacklight(ON);
  lcd.backlight();
    
  lcd.begin(16,2);               // initialize the lcd 

  lcd.clear();
  lcd.home ();                   // go home
  lcd.display();
  lcd.print(time_menu);  
  lcd.setCursor ( 0, 1 );        // go to the next line
  lcd.print (logo_menu);      
  
  displayTime(hour, minute, second);        

   /* 
    *  The BURNS encoder needs generates a rising signal for each dent, the AllElectronic encoder generates a change signal
    *  for each dent
    */
#if defined(BURNS_ENCODER)
  attachInterrupt(ENC_A,doEncoder,RISING);
#else
  attachInterrupt(ENC_A,doEncoder,CHANGE);
#endif  
}


void loop()
{
   short run_hour;
   short run_minute;
   short run_second;
   // put your main code here, to run repeatedly:
   // setup loop

   /*
    *  Main state machine
    *  Note that the pushbutton is active on the RELEASE not when pushed.
    */
   switch(main_status)
   {
      case IDLE:
         settingManagement();		/* Handle settings */
		 
         if(KEYIsPressed(STARTBTN) && !(hour==0 && minute == 0 && second == 0))
         {
            while(KEYIsPressed(STARTBTN));  /* Wait until the button is released */
            
            main_status = START;
            force_display_title = ON;
         }
         break;
         
      case START:   
         // timer loop
         TimerTimeSec=millis(); // timer value to actual time to display timer and decrement seconds
         TimerSecCounter=timeToSeconds(hour, minute,second)+1; // add one to account for first update

         digitalWrite(RELAY_LAMP,HIGH); // switch on
         digitalWrite(STARTLED,HIGH);   // Pushbutton LED on
         
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
         {
            while(KEYIsPressed(STARTBTN));  /* Wait until the button is released */

            digitalWrite(RELAY_LAMP,LOW); // switch off
            digitalWrite(STARTLED,LOW);   // Pushbutton LED off
                      
            main_status = STOP;
         }
         else if(TimerSecCounter == 0)
         {
            digitalWrite(RELAY_LAMP,LOW); // switch off
            digitalWrite(STARTLED,LOW);   // Pushbutton LED off
          
            main_status = ALR;
         }
         break;
             
      case ALR:
         tone(BUZZER, 550);            /* Emit tone */
         main_status = waitingResetTimer(main_status);
	 break;
      
      case STOP:
         main_status = waitingResetTimer(main_status);
         break;
         
      case END:      
         noTone(BUZZER);                  /* Stop tone */
         force_display_setting = 1;   
      
//         lcd.setCursor ( 0, 1 );        // go to the next line
//         lcd.print ("Action: END     ");       
   
         EncoderRead = second;      // Set default on last second value 
         oldEncoderRead = second;

         main_status = IDLE;
         set_status  = SET_SECONDS;
         
         displayTime(hour, minute, second);        
         break;
   }
   
   displayMenu();
}



/**
 *  @fn displayTime
 *  @brief Display the time on the first LCD line
 *  @param hours - value for hours
 *  @param minutes - value for minutes
 *  @param seconds - value for seconds
 */
void displayTime(short hours, short minutes, short seconds)
{
   lcd.setCursor ( 7, 0);        /* Go after the writing Time */
   lcd.print("     ");

#if defined(DISPLAY_TIME)
   lcd.setCursor ( 7, 0);        
   sprintf(buffer, "%02d:%02d:%02d", hours, minutes, seconds);
   lcd.print(buffer);         
#else
   lcd.setCursor ( 10, 1);        
   lcd.print(TimerSecCounter);     
#endif
}

/**
 *  @fn displayMenu
 *  @brief Display the menu on the second LCD line
 *  The function uses a flag to know when to display the title.
 */
void displayMenu()
{
   short tmp_value = 0;
   
   if(main_status == IDLE)
   {
      if(force_display_title)
      {
         lcd.setCursor ( 0, 1 );        /* Position on the second line */
         force_display_title = OFF;
         /* Display menu title */
         switch(set_status)
         {
            case READ_PRESETS:
            case SAVE_PRESETS:
               lcd.print (preset_menu);
               break;
            case SET_SECONDS:
            case SET_MINUTES:
            case SET_HOURS:
               lcd.print (select_menu);
	       break;
         }
      }   

      /* Display menu value */
      switch(set_status)
      {
         case READ_PRESETS:
            break;
         case SET_SECONDS:
            lcd.setCursor ( 4, 1 );        /* Position on the second line */
            lcd.print ("Seconds  ");
            break;
         case SET_MINUTES:
            lcd.setCursor ( 4, 1 );        /* Position on the second line */
            lcd.print ("Minutes  ");
            break;
         case SET_HOURS:
            lcd.setCursor ( 4, 1 );        /* Position on the second line */
            lcd.print ("Hours    ");
            break;
         case SAVE_PRESETS:
            lcd.setCursor ( 9, 1 );        /* Position on the second line */
            if(preset_index <= MAX_PRESET)
            {
               lcd.print(preset_index);
            }
            else
               lcd.print(" res");
            break;
      }
   }
   else
   {
      if(force_display_title)
      {
         lcd.setCursor ( 0, 1 );        /* Position on the second line */
         lcd.print (run_menu);
         force_display_title = OFF;
      }        
      
      lcd.setCursor ( 8, 1 );        /* Position on the second line */  
	   
      switch(main_status)
      {
         case STOP:
            lcd.print("STOP");
            break;
         case ALR:
            lcd.print("END ");
            break;
         default:		 
            lcd.print("RUN ");
            break;
      }
   }
}



/**
 *  @fn settingManagement
 *  @brief setting state machine management
 *  This state machine is working only when the main state machine is in IDLE, otherwise it is never called.
 */
void settingManagement()
{
   unsigned char enc_read = OFF;
   unsigned char btn_read = OFF;

   /* Reading selection push button status */
   if(KEYIsPressed(SELBTN))
   {
      while(KEYIsPressed(SELBTN));
	  btn_read = ON;
   }

   /* Determine if there is an encoder change value */   
   if(oldEncoderRead != EncoderRead )
   {
      enc_read = ON;	 
   }
   
   /* Continue only if there is an action */
   if(!enc_read && !btn_read)
      return;
	  
   switch(set_status)
   {
      case READ_PRESETS:
         force_display_title = ON;
         if(enc_read)
         {
            /*
             *  The preset_index meaning :
             *  0 - 10 -> preset index - 0 indicates no loading
             *  11     -> Set time
             */
            if(EncoderRead > MAX_PRESET+1)
               EncoderRead = MAX_PRESET+1;
            preset_index = EncoderRead;		 
        }
		 
        if(btn_read)
        {
	
           /* Execute action */
        }
        break;
		 
      case SET_SECONDS:
         if(enc_read)
         {
            if(EncoderRead > 59)
               EncoderRead = 59;
            second = EncoderRead;   
         }
		 
         if(btn_read)
         {
            /* 
             *  Button pressed - prepare for the setting of minutes
             */
            EncoderRead = minute;
            oldEncoderRead = minute;
            set_status = SET_MINUTES;
         }
         break;

      case SET_MINUTES:
         if(enc_read)
         {
            if(EncoderRead > 59)
               EncoderRead = 59;
            minute = EncoderRead;   
         }

         if(btn_read)
         {
            /* 
             *  Button pressed - prepare for the setting of hours
             */
            EncoderRead = hour;
            oldEncoderRead = hour;
	    set_status = SET_HOURS;
         }
         break;

      case SET_HOURS:
         if(enc_read)
         {
            if(EncoderRead > 2)
               EncoderRead = 2;
            hour = EncoderRead;   
         }
		 
         if(btn_read)
         {
            EncoderRead = preset_index;
            oldEncoderRead = preset_index;
            set_status = SAVE_PRESETS;
         }
         break;
		 
      case SAVE_PRESETS:
         if(enc_read)
         {
            /*
             *  The preset_index meaning :
             *  0 - 10 -> preset index - 0 indicates no saving and change state
             *  11     -> reset flash memory
             */
            if(EncoderRead > MAX_PRESET+1)
               EncoderRead = MAX_PRESET+1;
            preset_index = EncoderRead;		 
         }

         if(btn_read)
         {
            if(preset_index == 0)
            {
               /* Do nothing - change state */
               set_status = READ_PRESETS;
            }
            else if(preset_index >= 1 && preset_index <= MAX_PRESET)
            {
               /* Save current value in preset indexed - TBD */
			   
            }
            else
            {
              		   /* Reset flash memory */
            }
	 }
         break;
   }
}

/*-------------------- Utility functions ---------------------------------------*/

/*
 *  @fn waitingResetTimer
 *  @brief Wait for the push button pressing. Returns the next state, current one or END
 */
unsigned char waitingResetTimer(unsigned char status)
{
   /* Waiting to press the button */
         
   if(toggle_LED)
   {
      digitalWrite(STARTLED,LOW);   // Pushbutton LED off
      toggle_LED=OFF;
   }
   else
   {
      digitalWrite(STARTLED,HIGH);   // Pushbutton LED on
      toggle_LED=ON;
   }
   delay(200);
                
   if(KEYIsPressed(STARTBTN))
   {
      while(KEYIsPressed(STARTBTN));  /* Wait until the button is released */

      digitalWrite(STARTLED,LOW);   // Pushbutton LED off
      return(END);
   }
   return(status);
}


/**
 *  @fn secondsToTime
 *  @brief The function has the purpose to convert a single value in seconds into
 *  the more traditional hours/minutes/seconds display
 *  @param raw_seconds - number of seconds to convert
 *  @param hours - pointer to the location where to store the hour value
 *  @param minutes - pointer to the location where to store the minutes value
 *  @param seconds - pointer to the location where to store the seconds value
 *  No return value
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
 *  @fn timeToSeconds
 *  @brief The function has the purpose to create a single value in seconds from hour/minutes/seconds
 *  @param hours - value of hours, from 0 to 2
 *  @param minutes - value of minutes, from 0 to 59
 *  @param seconds - value of seconds, from 0 to 59
 *  Return the number of seconds
 */
int timeToSeconds(int hours, int minutes, int seconds )
{
   int raw_seconds;
   
   raw_seconds = (hours * 3600) + (minutes * 60) + seconds;
   
   return(raw_seconds);
}



/*
 * ----------- OLD functions ----- to be deleted -------------------------------- 
 */
#if 0 
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
            EncoderRead = minute;
            oldEncoderRead = minute;
            break;

         case SET_MINUTES:
            EncoderRead = hour;
            oldEncoderRead = hour;
            break;

         case SET_HOURS:
            EncoderRead = 1;
            oldEncoderRead = 1;
            saving_flag = 1;
            break;
 
        case READ_PRESETS:
        case SAVE_PRESETS:
            EncoderRead = second;
            oldEncoderRead = second;
			
	    /* Button pushed in this state - if flag OK save value in eeprom */
            if(saving_flag)
	    {
	       /* TBD - save value in eeprom */
	       saving_flag = 0;
	    }
	    break;
       }     

       set_status++;
       if(set_status > SAVE_PRESETS)
          set_status = READ_PRESETS;
	  
   }
}

/*
 *  displaySetting function
 *  This function manage the setting state machine.
 *  This state machine is working only when the main state machine is in IDLE, otherwise it is never called.
 */
void displaySetting()
{
   if(oldEncoderRead != EncoderRead || force_display_setting)
   {
      force_display_setting = 0;

      lcd.setCursor ( 0, 1 );        // go to the next line
      switch(set_status)
      {
         case SET_SECONDS:
            lcd.print ("Set Seconds     ");
            if(EncoderRead > 59)
               EncoderRead = 59;
            second = EncoderRead;   
            break;

         case SET_MINUTES:
            lcd.print ("Set Minutes     ");
            if(EncoderRead > 59)
               EncoderRead = 59;
            minute = EncoderRead;   
            break;

         case SET_HOURS:
            lcd.print ("Set Hours       ");
            if(EncoderRead > 2)
               EncoderRead = 2;
            hour = EncoderRead;   
            break;

	case SAVE_PRESETS:
	    switch(EncoderRead)
            {
	       case 0:
                  lcd.print ("Save ?      No  ");
                  break;+++
	      case 1:
                  lcd.print ("Save ?      Yes ");
		  break;		  
	    }
			
            if(EncoderRead > 1)
               EncoderRead = 1;
            saving_flag = EncoderRead;   
            break;
       }
     
      oldEncoderRead=EncoderRead;
      
      displayTime(hour, minute, second);
          
   }
}


#endif

