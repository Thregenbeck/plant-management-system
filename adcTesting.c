/************************************************************************************
*    {adcTesting.c}
* 	 Group 1
*        by Ryan Howard
*		    Chris Baker
* 			Dennis Dent 					
*
*    PERFORMANCE:
*	 Contains several functions related to the ADS1115 analog digital 
*    converter's initialization and measuring. Saves the readings into a file to
* 	 be able to use for interfacing
************************************************************************************/
//-included libraries----------------------------------------------------------------
#include <stdio.h>
#include <fcntl.h>     
#include <inttypes.h>  
#include <linux/i2c-dev.h> // I2C bus definitions
//-defined constants-----------------------------------------------------------------
#define adc_addr 0x48 //I2C address of ADC

//-functions within adc.c------------------------------------------------------------
float TempRead(void);
float SoilRead(void);
float LevelRead(void);


//-global variables------------------------------------------------------------------
int I2CFile;
uint8_t writeBuf[3];      
uint8_t readBuf[2];       
int16_t val;  
/************************************************************************************
*   TempRead function
*   determines voltage read from the ADS1115 registers and returns the value
* 	for the temperature sensor
************************************************************************************/
float TempRead() 
{
    // Open the I2C device-----------------------------------------------------------
    I2CFile = open("/dev/i2c-1", O_RDWR); 
       
    // Specify the address of the I2C Slave to communicate with----------------------
    ioctl(I2CFile, I2C_SLAVE, adc_addr);   

    //sets the pointer to the configuration registers--------------------------------
    writeBuf[0] = 1;
      
    //MSBs of the config register. Configures input pins, operation status, Mode
    writeBuf[1] = 0xC3; 
     
    //LSBs of the config register. Configures data rate(currently at 64 SPS) and
    //comparator mode (disabled)
    writeBuf[2] = 0x63;       

    // Initialize the buffer used to read data from the ADS1115 to 0-----------------
    readBuf[0]= 0;        
    readBuf[1]= 0;

    // Write writeBuf to the ADS1115 and begin a conversion
    write(I2CFile, writeBuf, 3);  

    // Wait for the conversion to complete-------------------------------------------
    while ((readBuf[0] & 0x80) == 0) 
    {
        //reads the config register to know when to move on the the next byte--------
        read(I2CFile, readBuf, 2);    
    }
    
    //set pointer to read from the convesion-----------------------------------------
    writeBuf[0] = 0;                 
    write(I2CFile, writeBuf, 1);
    
    // Read the contents of the conversion register into readBuf---------------------
    read(I2CFile, readBuf, 2);
      
    //shifts register to combine the two read registers into 2 bytes of data---------
    val = readBuf[0] << 8 | readBuf[1];
      
    //converts the value to volts based off the PGA value in bits 11-9 of the--------
    //config register and the resolution(16 bits / 2)
    return (float)val*4.096/32767.0;    
    //closes i2c file----------------------------------------------------------------
    close(I2CFile);
}


float SoilRead()
{
	// Open the I2C device-----------------------------------------------------------
	I2CFile = open("/dev/i2c-1", O_RDWR);
	// Specify the address of the I2C Slave to communicate with----------------------
	ioctl(I2CFile, I2C_SLAVE, adc_addr);

	//sets the pointer to the configuration registers--------------------------------
	writeBuf[0] = 1;
	//MSBs of the config register. Configures input pins, operation status, Mode
	writeBuf[1] = 0xD3;
	//LSBs of the config register. Configures data rate(currently at 64 SPS) and
	//comparator mode (disabled)
	writeBuf[2] = 0x63;

	// Initialize the buffer used to read data from the ADS1115 to 0-----------------
	readBuf[0] = 0;
	readBuf[1] = 0;

	// Write writeBuf to the ADS1115 and begin a conversion
	write(I2CFile, writeBuf, 3);

	// Wait for the conversion to complete-------------------------------------------
	while ((readBuf[0] & 0x80) == 0)
	{
		//reads the config register to know when to move on the the next byte--------
		read(I2CFile, readBuf, 2);
	}
	//set pointer to read from the convesion-----------------------------------------
	writeBuf[0] = 0;
	write(I2CFile, writeBuf, 1);
	// Read the contents of the conversion register into readBuf---------------------
	read(I2CFile, readBuf, 2);
	//shifts register to combine the two read registers into 2 bytes of data---------
	val = readBuf[0] << 8 | readBuf[1];
	//converts the value to volts based off the PGA value in bits 11-9 of the--------
	//config register and the resolution(16 bits / 2)
	return (float)val*4.096 / 32767.0;
	//closes i2c file----------------------------------------------------------------
	close(I2CFile);
}

float LevelRead()
{
	// Open the I2C device-----------------------------------------------------------
	I2CFile = open("/dev/i2c-1", O_RDWR);
	// Specify the address of the I2C Slave to communicate with----------------------
	ioctl(I2CFile, I2C_SLAVE, adc_addr);

	//sets the pointer to the configuration registers--------------------------------
	writeBuf[0] = 1;
	//MSBs of the config register. Configures input pins, operation status, Mode
	writeBuf[1] = 0xE3;
	//LSBs of the config register. Configures data rate(currently at 64 SPS) and
	//comparator mode (disabled)
	writeBuf[2] = 0x63;

	// Initialize the buffer used to read data from the ADS1115 to 0-----------------
	readBuf[0] = 0;
	readBuf[1] = 0;

	// Write writeBuf to the ADS1115 and begin a conversion
	write(I2CFile, writeBuf, 3);

	// Wait for the conversion to complete-------------------------------------------
	while ((readBuf[0] & 0x80) == 0)
	{
		//reads the config register to know when to move on the the next byte--------
		read(I2CFile, readBuf, 2);
	}
	//set pointer to read from the convesion-----------------------------------------
	writeBuf[0] = 0;
	write(I2CFile, writeBuf, 1);
	// Read the contents of the conversion register into readBuf---------------------
	read(I2CFile, readBuf, 2);
	//shifts register to combine the two read registers into 2 bytes of data---------
	val = readBuf[0] << 8 | readBuf[1];
	//converts the value to volts based off the PGA value in bits 11-9 of the--------
	//config register and the resolution(16 bits / 2)
	return (float)val*4.096 / 32767.0;
	//closes i2c file----------------------------------------------------------------
	close(I2CFile);
}

