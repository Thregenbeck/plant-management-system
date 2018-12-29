/*******************************************************************************
*    {adc.C}
*    Analog to Digital Code for the ADS1115
*        by Ryan Howard
*
*    PERFORMANCE:
*    Contains several functions related to the ADS1115 analog digital 
*    converter's initialization and measuring
*
*******************************************************************************/
#include "pwm.h"
#include "displayLCD.h"

int Initialization();

int Initialization()
{
	enablePWM();

	return 0;
}
