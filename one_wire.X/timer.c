//-----------------------------------------//
// ECE 372A Spring 2016
// Lab Group 201:
// Code Written by Zachary Finsterwald
//-----------------------------------------//

#include <xc.h>
#include "timer.h"

#define PRESCALAR_256   3
#define PRESCALAR_8     1
#define PRESCALAR_1     0

void init_timer_1()
{
    TMR1            = 0;                // CLEAR TIMER
    T1CONbits.TCKPS = PRESCALAR_256;    // SET PRE-SCALAR TO 256
    T1CONbits.TCS   = 0;                // SELECT INTERNAL OSCILLATOR
    IFS0bits.T1IF   = 0;                // PUT DOWN FLAG
}

void init_timer_2()
{
    TMR2            = 0;                // CLEAR TIMER
    T2CONbits.TCKPS = PRESCALAR_8;      // SET PRE-SCALAR TO 8
    T2CONbits.TCS   = 0;                // SELECT INTERNAL OSCILLATOR
    IFS0bits.T2IF   = 0;                // PUT DOWN FLAG
}

void delay_us(unsigned int delay)
{
    TMR2                = 0;                // CLEAR TIMER
    PR2                 = delay - 1;        // CALCULATE WAIT COUNT
    IFS0bits.T2IF       = 0;                // PUT DOWN FLAG
    T2CONbits.ON        = 1;                // TURN ON TIMER
    while(
        IFS0bits.T2IF   == 0                // WAIT
    );
    T2CONbits.ON        = 0;                // TURN OFF TIMER
}

void delay_ms(unsigned int delay)
{
    TMR1                = 0;                // CLEAR TIMER
    PR1                 = delay * 31 - 1;// CALCULATE WAIT COUNT
    IFS0bits.T1IF       = 0;                // PUT DOWN FLAG
    T1CONbits.ON        = 1;                // TURN ON TIMER
    while(
        IFS0bits.T1IF   == 0                // WAIT
    );
    T1CONbits.ON        = 0;                // TURN OFF TIMER
}

