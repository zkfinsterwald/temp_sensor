//-----------------------------------------//
// ECE 372A Spring 2016
// Lab Group 201:
// Code Written by Zachary Finsterwald
//-----------------------------------------//

#include <xc.h>
#include "timer.h"
#include "lcd.h"

#define TRIS_D7 TRISEbits.TRISE1
#define TRIS_D6 TRISEbits.TRISE3
#define TRIS_D5 TRISEbits.TRISE5
#define TRIS_D4 TRISEbits.TRISE7

#define TRIS_RS TRISCbits.TRISC4
#define TRIS_E  TRISCbits.TRISC2

#define LAT_D7 LATEbits.LATE1
#define LAT_D6 LATEbits.LATE3
#define LAT_D5 LATEbits.LATE5
#define LAT_D4 LATEbits.LATE7

#define LAT_RS LATCbits.LATC4
#define LAT_E  LATCbits.LATC2

#define OUTPUT  0

#define true 1
#define false 0


void toggle_enable(unsigned int delay)
{
    LAT_E = 1;
    delay_us(500);
    LAT_E = 0;
    delay_us(delay);
}

void set_data(int db7, int db6, int db5, int db4)
{
    LAT_D7 = db7;
    LAT_D6 = db6;
    LAT_D5 = db5;
    LAT_D4 = db4;
}

void write_four_bits(unsigned char word, unsigned int command_type, unsigned int delay_after, unsigned int lower){

    LAT_RS  = command_type;  // SET RS

    set_data(0,0,0,0);       // Make pins zero
    if(lower == true) {
        LAT_D4  =  word&0x01;
        LAT_D5  = (word>>1)&0x01;
        LAT_D6  = (word>>2)&0x01;
        LAT_D7  = (word>>3)&0x01;
    } else {
        LAT_D4  = (word>>4)&0x01;
        LAT_D5  = (word>>5)&0x01;
        LAT_D6  = (word>>6)&0x01;
        LAT_D7  = (word>>7)&0x01;
    }
    toggle_enable(delay_after);
}

void write_lcd(unsigned char word, unsigned int command_type, unsigned int delay_after){
    write_four_bits(word, command_type, delay_after, 0);
    write_four_bits(word, command_type, delay_after, 1);
}

void print_char_lcd(char c) {
    write_lcd(c, 1, 500);
}

void init_lcd(void) {
    // DATA PORTS
    TRIS_D7 = OUTPUT;
    TRIS_D6 = OUTPUT;
    TRIS_D5 = OUTPUT;
    TRIS_D4 = OUTPUT;
    // RS AND ENABLE
    TRIS_RS = OUTPUT;
    TRIS_E  = OUTPUT;

    //----- BEGIN SEQUENCE -----//

    delay_ms(40);

    // SPECIFIC SEQUENCE
    write_lcd(0x03, 0, 4500);
    write_lcd(0x03, 0, 110);
    write_lcd(0x03, 0, 50);
    write_lcd(0x02, 0, 50);
    // END

    write_lcd(0x28, 0, 500); // FUNCTION SET
    write_lcd(0x0C, 0, 500); // DISPLAY ON, CURSOR OFF, BLINK OFF
    write_lcd(0x01, 0, 500); // CLEAR DISPLAY
    write_lcd(0x06, 0, 500); // ENTRY MODE, SET INCREMENT

    //----- END SEQUENCE -----//
}

void print_string_lcd(const char* s) {
    while(*s)
        print_char_lcd(*s++);
}

void clear_lcd(){
    unsigned char word = 0x01;
    write_lcd(word, 0, 1400); // CLEAR COMMAND
    move_cursor_lcd(0,1); // RESET POSITION
}

void move_cursor_lcd(unsigned char x, unsigned char y){
    unsigned char address;
    unsigned char row, col;
    row = x&(0x0f);
    col = y&(0x0f);
    address = (col - 1)*0x40 + (row);
    address = address + 0x80;
    write_lcd(address, 0, 500);
}

void bcd_lcd(unsigned char uc_digit, unsigned int ui_number)
{
    unsigned int ui_decimal[5] ={ 0 };
    move_cursor_lcd(0,2);
    
    //extract 5 single digit from ui_number
    ui_decimal[4] = ui_number/10000;            // obtain the largest single digit, digit4 
    ui_decimal[3] = ui_number%10000;            // obtain the remainder 
    ui_decimal[2] = ui_decimal[3]%1000;			
    ui_decimal[3] = ui_decimal[3]/1000;			// obtain the 2nd largest single digit, digit3 
    ui_decimal[1] = ui_decimal[2]%100;
    ui_decimal[2] = ui_decimal[2]/100;			// obtain the 3rd largest single digit, digit2 
    ui_decimal[0] = ui_decimal[1]%10;			// obtain the smallest single digit, digit0 
    ui_decimal[1] = ui_decimal[1]/10;			// obtain the 4th largest single digit, digit1 


    print_char_lcd(ui_decimal[4] + 0x30);	
    print_char_lcd(ui_decimal[3] + 0x30);	
    print_char_lcd(ui_decimal[2] + 0x30);
    print_char_lcd('.');	
    print_char_lcd(ui_decimal[1] + 0x30);	
    print_char_lcd(ui_decimal[0] + 0x30);			

}
