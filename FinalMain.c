/****************************************************************    
*    {Main.C}
*    Main Code file
*     Group 1
*        by Ryan Howard
*    PERFORMANCE:
*    Main program that runs through the entire State machine of 
*    the system. It uses all the header files to call functions 
*    and transition between all the statements.
*                                  **NOTE**
*     This program relies on many files to compile correctly.
*     Below is the command to compile the code. Refer to
*     documentation to download required files to run. 
*     gcc -o Main Main.c adcTesting.c pwm.c Initialization.c 
*     7_seg.c -lwiringPi -lwiringPiDev -lpthread
***************************************************************/
#include <stdio.h>
#include <wiringPi.h>
#include <stdlib.h>
#include <mcp23017.h>
#include <lcd.h>
#include "Initialization.h"
#include "adcTesting.h"
#include "7_seg.h"
 
//LCD-I2C-Pins--------------------------------------------------
#define BASE 100
#define E 113
#define RS 115
#define DB4 112
#define DB5 111
#define DB6 110
#define DB7 109
#define RW 114
#define BLUE 108
#define   DOWN 102
 
//GPIO-Inputs---------------------------------------------------
#define SprinklerOn 7
#define SensorMenu  19
#define ManualMode 4
#define SilentMode 8
  
//GPIO-Outputs--------------------------------------------------
#define ManualModeLED 22
#define SilentModeLED 18
#define SprinklerOnLED 24 //7
#define WaterLowLED 17
#define PowerOnLED 27
#define SoilDryLED 23
#define WaterLowBuzzer 20
 
//Sprinkler-Pump-PWM--------------------------------------------
#define Sprinkler 13
 
//LCD-Button----------------------------------------------------
#define   SELECT 103
 
//Counter-variable----------------------------------------------
int SensorButCounter;
  
int main(void)
{
     //ADC-Readings-Variables-----------------------------------
     float TempVoltage;
     int SoilVoltage;
     int LevelVoltage;  
     wiringPiSetupGpio();
     
     //declaring-Buttons-Switches-as-Inputs---------------------
     pinMode(SprinklerOn, INPUT);
     pinMode(ManualMode, INPUT);
     pinMode(SilentMode, INPUT);
     pinMode(SensorMenu, INPUT);
     
     //Declaring-LEDs-and-Buzzer-as-Output----------------------
     pinMode(ManualModeLED, OUTPUT);
     pinMode(SilentModeLED, OUTPUT);
     pinMode(SprinklerOnLED, OUTPUT);
     pinMode(WaterLowLED, OUTPUT);
     pinMode(PowerOnLED, OUTPUT);
     pinMode(SoilDryLED, OUTPUT);
 
     //declaring-buzzer-----------------------------------------
     pinMode(WaterLowBuzzer, OUTPUT);
     digitalWrite(WaterLowBuzzer, HIGH);
     
     //Intitalize-LCD-Settings----------------------------------
     int lcd;
     mcp23017Setup(BASE, 0x20);
     lcd = lcdInit(2,16,4,RS,E,DB4,DB5,DB6,DB7,0,0,0,0);
     lcdClear(lcd);
     pinMode(BLUE, OUTPUT);
     digitalWrite(BLUE, LOW); //enable blue backlight
     pinMode(RW, OUTPUT);
     digitalWrite(RW, HIGH);
     delay(0.5);
     digitalWrite(RW, LOW);
     pinMode(SELECT, INPUT);
     pullUpDnControl(SELECT, PUD_UP);
     pinMode(DOWN, INPUT);
     pullUpDnControl(DOWN, PUD_UP);
     
     //Turns-Off-All-LEDS-and-Buzzer-Except-Power-on------------
     //and-Program-Start----------------------------------------
     digitalWrite(ManualModeLED, HIGH);
     digitalWrite(SilentModeLED, HIGH);
     digitalWrite(SprinklerOnLED, HIGH);
     digitalWrite(WaterLowLED, HIGH);
     digitalWrite(PowerOnLED, LOW);
     digitalWrite(SoilDryLED, HIGH);
     digitalWrite(WaterLowBuzzer, LOW);
     Initialization();
     int display = 0;
     display = 1;
     wiringPiSetupGpio();
     
     int input;
     int prev_input = 0;
 
     while (1)
     {
          //Converts-ADC-Values-to-Variables------------------
          TempVoltage = TempRead();
          SoilVoltage = SoilRead()*10;
          LevelVoltage = LevelRead()*10;
	     
          //Turn-on-LED-if-Switch-is-Up-----------------------
          if (digitalRead(ManualMode) == HIGH)
          {
              digitalWrite(ManualModeLED, LOW);
          }
          else
          {
              digitalWrite(ManualModeLED, HIGH);
          }
          //Turn-on-LED-if-Switch-is-Up----------------------
          if (digitalRead(SilentMode) == LOW)
          {
              digitalWrite(SilentModeLED, LOW);
          }
          else
          {
              digitalWrite(SilentModeLED, HIGH);
          }

   //Turns-on-LED-and-PWM-if-Manual-Switch-is-on-and---------
   //Sprinkler-button is-pressed.----------------------------
  if (digitalRead(SprinklerOn) == LOW &&
   digitalRead(ManualMode) == HIGH)
          {
              digitalWrite(SprinklerOnLED, LOW);
              while (!digitalRead(SprinklerOn))
              {
              TogglePWM(1);
                   digitalWrite(RW, HIGH);
                   delay(0.5);
                   digitalWrite (RW, LOW);
              }
          }    
          else
          {
              digitalWrite(SprinklerOnLED, HIGH);
              TogglePWM(0);
          }
          //Turn-on-Soil-Low-LED-based-on-sensor-----------
          if (SoilVoltage < 20)
          {
              digitalWrite(SoilDryLED, LOW);
          }
          else
          {
              digitalWrite(SoilDryLED,HIGH);
          }
 
 	  //Turn-on-Water-Low-LED-Buzzer-and-7-seg-if-silent----
	  //mode-switch-off-and-water-level-sensor-reading-<-26-
          if (LevelVoltage < 26 &&digitalRead(SilentMode)==HIGH)
          {
              digitalWrite(WaterLowBuzzer, LOW);
              digitalWrite(WaterLowLED, LOW);
              Seven_Seg_Water_Level(3);
          }
          //Turn-on-7-seg-if-water-level-voltage->-26----------
          if (LevelVoltage >= 26)
          {
              Seven_Seg_Water_Level(1);
          }
          //turn-on-PWM-if-Water-level-high-enough-Manual-mode-----
          //active--and-soil-is-dry--------------------------------
	  if (LevelVoltage > 20 && SoilVoltage < 20 && digitalRead(ManualMode) == LOW )
          {
              digitalWrite(SprinklerOnLED, LOW);
              TogglePWM(1);
              delay(1500);
              TogglePWM(0);
          }
          else
          {
              digitalWrite(SprinklerOnLED, HIGH);
              TogglePWM(0);
          }

     	  //Rotates-through-3-screens-based-on-button-press-----
          int input;
          int prev_input = 0;
          input = !digitalRead(SELECT);
          if (input && input !=prev_input)
          {
              delay(300);
              SensorButCounter += 1;
              if (SensorButCounter == 4)
              {
                   SensorButCounter = 1;
              }
              lcdClear(lcd); 
          }
          prev_input = input;
          //first-press-of-button-------------------------------
          if (SensorButCounter == 1)
          {
              displayLCD(2);   
          }
          //second-press-of-button------------------------------
          else if (SensorButCounter == 2)
          {
              //display-if-reading->-26------------------------
              if (SoilVoltage > 26)
              {
                   //Soil-Wet-LCD------------------------------
                   displayLCD(3);
              }
              else
              {
                   //Soil-Dry-LCD------------------------------
                   displayLCD(4);     
              }
          }
          
          //third-button-press-lcd-screen-----------------------
          else if (SensorButCounter == 3)
          {
              //Full-Water-LCD---------------------------------
              if (LevelVoltage >= 26)
              {  
                   displayLCD(5);  
              }
              //Low-Water-LCD----------------------------------
              else if (LevelVoltage < 26)
              {
                   displayLCD(7);     
              }
          }
     }
     return 0;
}    
