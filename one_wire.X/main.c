#include <xc.h>
#include <sys/attribs.h>
#include "config.h"
#include "timer.h"
#include "lcd.h"
#include "1wire.h"

#include <stdio.h>
#include <stdlib.h>

#define SKIP_ROM        0xCC
#define CONVERT_T       0x44
#define READ_SCRATCHPAD 0xBE

#define TEMP_RESOLUTION 12
char *text = "000.0000";

void display_temperature_lcd(unsigned int temp2write) 
{
    const unsigned short RES_SHIFT = TEMP_RESOLUTION - 8;
    char temp_whole;
    unsigned int temp_fraction;

    // Check if temperature is negative
    if (temp2write & 0x8000) {
       text[0] = '-';
       temp2write = ~temp2write + 1;
       }

    // Extract temp_whole
    temp_whole = temp2write >> RES_SHIFT ;

    // Convert temp_whole to characters
    if (temp_whole/100)
       text[0] = temp_whole/100  + 48;
    else
       text[0] = '0';

    text[1] = (temp_whole/10)%10 + 48;             // Extract tens digit
    text[2] =  temp_whole%10     + 48;             // Extract ones digit

    // Extract temp_fraction and convert it to unsigned int
    temp_fraction  = temp2write << (4-RES_SHIFT);
    temp_fraction &= 0x000F;
    temp_fraction *= 625;

    // Convert temp_fraction to characters
    text[4] =  temp_fraction/1000    + 48;         // Extract thousands digit
    text[5] = (temp_fraction/100)%10 + 48;         // Extract hundreds digit
    text[6] = (temp_fraction/10)%10  + 48;         // Extract tens digit
    text[7] =  temp_fraction%10      + 48;         // Extract ones digit

    // Print temperature on LCD
    move_cursor_lcd(0,2);
    print_string_lcd(text);
}

float ds1820_read()
{
    int busy=0, temp1, temp2;
    signed int temp3;
    float result; 
    onewire_reset();
    OW_write_byte(SKIP_ROM);		
    OW_write_byte(CONVERT_T);
    
    while (busy == 0)
        busy = OW_read_byte();
    
    onewire_reset();
    OW_write_byte(SKIP_ROM);		
    OW_write_byte(READ_SCRATCHPAD);
    temp1 = OW_read_byte();
    temp2 = OW_read_byte();
    temp3 = ((unsigned int)temp2 << 8 ) + (unsigned int)temp1;
    result = (float) temp3 *6.25;   //Calculation for DS18S20 with 0.5 deg C resolution
    // result = (float) temp3 / 16.0;  //Calculation for DS18B20 with 0.1 deg C resolution
 
    delay_ms(200);
    return(result);
    
}

int main(void)
{
	unsigned temp;
	unsigned short tempL, tempH, fraction;
	unsigned int i = 0;	// declare a variable to store 
    float temperature; 
    //
    init_timer_1();
    init_timer_2();
	//Initialize LCD
	init_lcd();
    clear_lcd();
    //
    //CNPUGbits.CNPUG1 = 1;
	
	print_string_lcd("Cytron Tech");  //LCD display for the starting part
    delay_ms(1000);
    clear_lcd();
	move_cursor_lcd(0,1);
	print_string_lcd("DS18B20");
	delay_ms(1000);
    
     while (1)
     {
         temperature = ds1820_read();
         bcd_lcd(5,temperature);
     }

}
   
  