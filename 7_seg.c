/*******************************************************************************
*    {adc.C}
*    Analog to Digital Code for the ADS1115
*        by David Purdie
*        modified by Ryan Howard
*
*    PERFORMANCE:
*    Contains several functions related to the ADS1115 analog digital 
*    converter's initialization and measuring
*
*******************************************************************************/
#include <stdio.h>
#include <wiringPi.h>
#include <stdlib.h>

//7 Segment
#define SEGMENT_A 12
#define SEGMENT_B 5
#define SEGMENT_C 25
#define SEGMENT_D 6
#define SEGMENT_E 21
#define SEGMENT_F 16
#define SEGMENT_G 13
#define SEGMENT_ON  LOW  
#define SEGMENT_OFF HIGH  

int Seven_Seg_Water_Level(int WaterNumber);

int Seven_Seg_Water_Level(int WaterNumber)
{
	pinMode(SEGMENT_A, OUTPUT);
	pinMode(SEGMENT_B, OUTPUT);
	pinMode(SEGMENT_C, OUTPUT);
	pinMode(SEGMENT_D, OUTPUT);
	pinMode(SEGMENT_E, OUTPUT);
	pinMode(SEGMENT_F, OUTPUT);
	pinMode(SEGMENT_G, OUTPUT);
	if (WaterNumber == 1)
	{
		digitalWrite(SEGMENT_A, SEGMENT_ON);
		digitalWrite(SEGMENT_B, SEGMENT_ON);
		digitalWrite(SEGMENT_C, SEGMENT_ON);
		digitalWrite(SEGMENT_D, SEGMENT_ON);
		digitalWrite(SEGMENT_E, SEGMENT_ON);
		digitalWrite(SEGMENT_F, SEGMENT_ON);
		digitalWrite(SEGMENT_G, SEGMENT_OFF);
	}

	if (WaterNumber == 2)
	{

		digitalWrite(SEGMENT_A, SEGMENT_OFF);
		digitalWrite(SEGMENT_B, SEGMENT_ON);
		digitalWrite(SEGMENT_C, SEGMENT_ON);
		digitalWrite(SEGMENT_D, SEGMENT_ON);
		digitalWrite(SEGMENT_E, SEGMENT_ON);
		digitalWrite(SEGMENT_F, SEGMENT_ON);
		digitalWrite(SEGMENT_G, SEGMENT_OFF);
	}
	return 0;
}
