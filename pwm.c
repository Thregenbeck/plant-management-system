/***********************************************************************************
*    {PWM_C}
*        by Ryan Howard
*   Creates a software PWM pin using wiringpi software.
*   Sets up the GPIO 13 pin to the water pump with a max value of 200.
*   the speed of the motor is changed based on a counter from 1 to 48.
*   From 1-23 the speed is 50.
*   From 24-47 the speed is 100.
*   From 48 down to 1 the speed is 200(max).
************************************************************************************/
#include <stdio.h>
#include <wiringPi.h>
#include <softPwm.h>

//define-PWM-pin-number-------------------------------------------
#define PUMP 26
int enablePWM(void)
{

    //Creates-a-software-PWM-with=a-max---------------------------
    //value-of-200------------------------------------------------
	softPwmCreate(PUMP, 0, 200);

return 0;
}

int TogglePWM(int State)
{
	//Turns-On-Pump------------------------------------------
	if (State == 1)
	{	

		softPwmWrite(PUMP, 125);
		
	}
	//Turns-Off-Pump------------------------------------------
	if (State == 0)
	{
		softPwmWrite(PUMP, 0);
	}
return 0;
}
		
