/*
 * File:   main.c
 * Author: sathw
 *
 * Created on 21 March, 2024, 9:20 PM
 */


#include <xc.h> 
#include "main.h"
#include "digital_keypad.h"
#include "clcd.h"
#include "timers.h"

#pragma config WDTE = OFF

void init_config(void)
{
    init_digital_keypad();
    init_clcd();
    
    FAN_DDR = 0;
    
    BUZZER_DDR = 0;
    BUZZER = OFF;        
            
    init_timer2();
    
    PEIE = 1;
    GIE = 1;
}

unsigned char operation_mode =WASHING_PROGRAM_DISPLAY ;
unsigned char reset_mode = WASHING_PROGRAM_DISPLAY_RESET; 
unsigned char program_no=0, water_level_index = 0;
unsigned char *washing_prog[]={"Daily","Heavy","Delicates","Whites","stainwash","Ecocottons","Woolens","Beadsheets", "Rinse+Dry" , "Dry only",
"Wash only ", "Aqua store"};
unsigned char *water_level_option[] ={"Auto", "Low", "Medium", "High", "Max"};

unsigned char min , sec;
unsigned int rinse_time,wash_time,spin_time;

void main(void)
{ 
    init_config();
    unsigned char key=read_digital_keypad(STATE) ;
    
    clcd_print("press key 5 to", LINE1(1));
    clcd_print("power ON", LINE2(4));
    clcd_print("washing machine", LINE3(1)); 
    while(read_digital_keypad(STATE) != SW5)
    {
        for( unsigned int wait = 3000; wait--;);
    }
    power_on_screen();
    
    while(1)
    {
        
       key = read_digital_keypad(STATE);
       
       if(key == LSW4  && operation_mode == WASHING_PROGRAM_DISPLAY)
       {
           operation_mode = WATER_LEVEL;
           reset_mode = WATER_LEVEL_RESET; 
       }
       else if(key == LSW4  && operation_mode == WATER_LEVEL)
       {
           set_time();
           
          operation_mode = START_STOP_SCREEN;
          clear_screen(); 
          clcd_print("press switch",LINE1(1));
          clcd_print("SW5: START",LINE2(1));
          clcd_print("SW6: STOP",LINE3(1));
       }
           if (key == SW5 && operation_mode == PAUSE)
           {
               TMR2ON = 1;
               FAN = ON;
               operation_mode = START_PROGRAM;
           }
       
       
       switch (operation_mode)
       {
           case WASHING_PROGRAM_DISPLAY:
               washing_program_display(key);
               break;
           case WATER_LEVEL:
               water_level_display(key);
               break;
           case START_STOP_SCREEN:
               if(key == SW5)
               {
                  operation_mode = START_PROGRAM;
                  reset_mode = START_PROGRAM_RESET;
               }
               else if(key == SW6)
               {
                   operation_mode = WASHING_PROGRAM_DISPLAY;
                   reset_mode =WASHING_PROGRAM_DISPLAY_RESET; 
               }
               continue;
           case START_PROGRAM:
               run_program(key);
               break;
       }
       reset_mode = RESET_NOTHING;
       
       
    }
}
 void power_on_screen(void)
 {
    for (unsigned char i=0; i<16; i++)
    {
       clcd_putch(BLOCK,LINE1(i));
    }
    clcd_print("powering on",LINE2(2));
    clcd_print("washing machine",LINE3(1));
    for(unsigned char i=0; i<16; i++)
    {
       clcd_putch(BLOCK, LINE4(i));
    }
        
    __delay_ms(1000);
         
         clear_screen();
    
 }

 
void washing_program_display(unsigned char key)

{
    if(reset_mode == WASHING_PROGRAM_DISPLAY_RESET)
    {
    clear_screen();
    program_no = 0;
    
    }
    
    if(key == SW4)
    {
        program_no++;
         clear_screen();
        if( program_no == 12)
        {
             program_no = 0;
        }
    }
    
    
    clcd_print("washing program",LINE1(0));
    clcd_putch('*',LINE2(0));
    
    if(program_no <= 9)
    {
    clcd_print(washing_prog[program_no],LINE2(2));
    clcd_print(washing_prog[program_no + 1],LINE3(2));
    clcd_print(washing_prog[program_no + 2],LINE4(2));
    }
    else if (program_no ==10 )
    {
    clcd_print(washing_prog[program_no],LINE2(2));
    clcd_print(washing_prog[program_no + 1],LINE3(2));
    clcd_print(washing_prog[0],LINE4(2));  
    }
    else if (program_no ==11 )
    {
    clcd_print(washing_prog[program_no],LINE2(2));
    clcd_print(washing_prog[0],LINE3(2));
    clcd_print(washing_prog[1],LINE4(2));  
    }
}
void water_level_display (unsigned char key)
{
  if (reset_mode == WATER_LEVEL_RESET ) 
  {
     water_level_index = 0;
     clear_screen(); 
  }
  if (key == SW4)
  {
     water_level_index++ ;
    if (water_level_index == 5)  
    {
        water_level_index = 0;
    }
     clear_screen(); 
  }
    
  clcd_print("water level:", LINE1(0));
  clcd_putch('*',LINE2(0));
  
  if (water_level_index <= 2)
  {
     clcd_print(water_level_option[water_level_index], LINE2(2));  
     clcd_print(water_level_option[water_level_index + 1], LINE3(2));  
     clcd_print(water_level_option[water_level_index + 2], LINE4(2));  
  }
  else if (water_level_index == 3)
  {
     clcd_print(water_level_option[water_level_index], LINE2(2));  
     clcd_print(water_level_option[water_level_index + 1], LINE3(2));  
     clcd_print(water_level_option[0], LINE4(2));   
  }
    else if (water_level_index == 4)
  {
     clcd_print(water_level_option[water_level_index], LINE2(2));  
     clcd_print(water_level_option[0], LINE3(2));  
     clcd_print(water_level_option[1], LINE4(2));   
  }
}

void set_time(void)
{
    switch(program_no)
    {
        case  0:
        
          switch(water_level_index)
         {
            case 1:
                sec=33;
                min=0;
                break;
            case 0:
            case 2:
                    sec=41;
                    min=0;
                    break;
            case 3:
            case 4:
                    sec=45;
                    min=0;
                    break;
         }
        break;
        case 1:
        
            switch(water_level_index)
           {
            case 1:
                sec=43;
                min=0;
                break;
            case 0:
            case 2:
                sec=50;
                min=0;
                break;
            case 3:
            case 4:
                sec=57;
                min=0;
                break;
            }
        
          break;  
        case 2:
        {
            switch(water_level_index)
            {
            case 1:
            case 0:
            case 2:
                    sec=26;
                    min=0;
                    break;
            case 3:
            case 4:
                    sec=31;
                    min=0;
                    break;
            }
            break;
        }
        case 3:
        {
            
            sec = 16;
            min = 1;        
            break;
        }
        case 4:
        {
            
            sec = 36;
            min = 1;        
            break;
        }
        case 5:
        { 
            sec = 36;
            min = 0;        
            break;
        }
        case 6:
        {
            
            sec = 29;
            min = 0;        
            break;
        }
        case 7:
        {
            switch(water_level_index)
            {
             case 1:
                  sec=46;
                  min=0;
                  break;
            case 0:
            case 2:
                    sec=53;
                    min=0;
                    break;
            case 3:
            case 4:
                    sec=00;
                    min=1;
                    break;
                }
        break;
    }       
    case 8:
    {
     switch(water_level_index)
           {
            case 1:
                sec=18;
                min=0;
                break;
            case 0:
            case 2:
            case 3:
            case 4:
                    sec=20;
                    min=0;
                    break;
            }
        break;   
    }
    case 9:
    
            sec = 6;
            min = 0;        
            break; 
    
    case 10:
    case 11:
        switch(water_level_index)
           {
            case 1:
                sec=16;
                min=0;
                break;
            case 0:
            case 2:
                    sec = 21;
                    min = 0;
                    break;
            case 3:
            case 4:
                    sec=26;
                    min=0;
                    break;
                }
        break;   
    }
    
    
}
void run_program(unsigned char key)
{    
    door_status_check();
    static int total_time,time;
    if(reset_mode== START_PROGRAM_RESET)
    {
        clear_screen();
        clcd_print("prog:",LINE1(0));
        clcd_print(washing_prog[program_no], LINE1(6));
        clcd_print("time:",LINE2(0));
        
        clcd_putch((min/10)+'0',LINE2(6));
        clcd_putch((min%10)+'0',LINE2(7));
        clcd_putch(':',LINE2(8));
        clcd_putch((sec/10)+'0',LINE2(9));
        clcd_putch((sec%10)+'0',LINE2(10));
        clcd_print("(MM:SS)",LINE3(5));
        
        __delay_ms(2000);
        
        clear_screen();
        clcd_print("FUNCTION - ", LINE1(0));
        clcd_print("TIME: ",LINE2(0));
        clcd_print("5-START  6-PAUSE",LINE4(0));
        
        time = total_time = (min*60 )+ sec;
        wash_time = (int)total_time*(0.46);
        rinse_time =(int)total_time*(0.12);
        spin_time = total_time - wash_time - rinse_time;
        
        
        FAN = ON;
        
        TMR2ON = 1;
        
    }
    if (key == SW6)
    {
        FAN = OFF;
        TMR2ON = 0;
        operation_mode = PAUSE;
    }
    
    total_time = (min*60) + sec;
    if(program_no <= 7)
    {
        if(total_time >= (time - wash_time ))
        {
            clcd_print ("Wash ",LINE1(11));
            
        }
        else if (total_time >= (time - wash_time - spin_time))
        {
             clcd_print ("Rinse",LINE1(11));
        }
        else 
        {
             clcd_print ("Spin ",LINE1(11));
        }
            
    }
    else if (program_no == 8)
    {
        if (total_time >= (time - (0.40 *time)))
        {
           clcd_print ("Rinse",LINE1(11)); 
        }
        else 
        {
            clcd_print ("Spin ",LINE1(11));
        }
    }
    else if (program_no == 9)
    {
         clcd_print ("Spin ",LINE1(11));
    }
    else
    {
         clcd_print ("Wash ",LINE1(11));
    }
    clcd_putch((min/10)+'0',LINE2(6));
    clcd_putch((min%10)+'0',LINE2(7));
    clcd_putch(':',LINE2(8));
    clcd_putch((sec/10)+'0',LINE2(9));
    clcd_putch((sec%10)+'0',LINE2(10));
    
    if (( sec == 0 ) && (min == 0))
    {
        FAN = OFF;
        
        TMR2ON = OFF;
        
        BUZZER = ON ;
        clear_screen();
        
        clcd_print("Prog.completed", LINE1(0));
        clcd_print("Remove clothes",LINE2(0));
        __delay_ms(2000);
        
        BUZZER = OFF;
        
        operation_mode = WASHING_PROGRAM_DISPLAY;
        reset_mode =WASHING_PROGRAM_DISPLAY_RESET;
        clear_screen();
    }
    
}
void door_status_check(void)
{
    if (RB0 == 0)
    {
        FAN = OFF;
        TMR2ON = OFF;
        BUZZER = ON;
        clear_screen();
        clcd_print("Door:OPEN", LINE1(0));
        clcd_print("Please close",LINE3(0));
        while(RB0 == 0)
        {
         ;
        }
        clear_screen();
        clcd_print("FUNCTION - ", LINE1(0));
        clcd_print("TIME: ",LINE2(0));
        clcd_print("5-START  6-PAUSE",LINE4(0));
        
        FAN = ON;
        
        TMR2ON = ON;
        
        BUZZER = OFF;
       
    }
}
   
    

       
