#include <xc.h>
#include "main.h"

void main()
{
    init();
    while(1)
    {
        PORTA = 0x0;
        if (PORTFbits.RF4)
        {
            fan();
        }
        else if (PORTFbits.RF5)
        {
           shift();
        }
        else if(PORTFbits.RF3)
        {
            counter();
        }
        
//        make_sound();
    }
}

void counter()
{
    PORTA = 0x0;
    while(PORTFbits.RF3 && !PORTFbits.RF5 && !PORTFbits.RF4)
    {
        if (PORTDbits.RD15)
            PORTA--;
        else
            PORTA++;
        delay();
    }
}

void shift()
{
    PORTA = 0x1;
    while (PORTFbits.RF5 && !PORTFbits.RF4)
    {
        delay();
        if (PORTDbits.RD15)
            PORTA >>= 1;
        else
            PORTA <<= 1;
        
       if ((0xff & PORTA) == 0x00)
       {
           if (PORTDbits.RD15)
                PORTA = 0x80;           
           else
                PORTA = 0x1;
       }
    }
}

void fan()
{
    PORTA = 0x18;
    while(PORTFbits.RF4)
    {
        delay();
        if (PORTDbits.RD15)
            PORTA = (((PORTA & 0xf0) >> 1) & 0xf0) | (((PORTA & 0x0f) << 1) & 0x0f);
        else
            PORTA = ((PORTA & 0xf0) << 1) | ((PORTA & 0x0f) >> 1);
        
        if ((0xff & PORTA) == 0x0)
        {
            if (PORTDbits.RD15)
                PORTA = 0X81;
            else
                PORTA = 0x18;
        }                                                                           
//        0010 0100             0001 1000       0001 1000   0000 0100               0001 000
//        1111 0000 << 1        1111 0000       0000 1111   0010 0000               0001 0000
//        0000 1111 >> 1        0010 0000       0000 0100   0010 0100               0001 1000
    }
}

void make_sound()
{
    if (!PORTBbits.RB10) //SW6
        return;
    
    int pitch = PORTDbits.RD14 ? 0 : 5;
    int i, num = 0; 
    while(num++ < 10)
    {
        PORTBbits.RB14--;
        for(i = 0; i < 500 + pitch * 100; i++)
            ;
        PORTBbits.RB14++;
        for(i = 0; i < 500 + pitch * 100; i++)
            ;
    }
}

void delay()
{
    int interval = 80000;
    if (!PORTDbits.RD14)
        interval = 120000;
    
    int j;
    for (j = 0; j < interval; j++)
        ;
    
    make_sound();
    
    while (PORTBbits.RB11)  //  while sw5 is on, stop the iteration
        ;
}

void init()
{
    TRISA &= 0xff00;
    PORTA = 0x0;
     
    TRISFbits.TRISF3 = 1; // RF3 (SW0) configured as input
    TRISFbits.TRISF5 = 1; // RF5 (SW1) configured as input
    TRISFbits.TRISF4 = 1; // RF4 (SW2) configured as input
    TRISDbits.TRISD15 = 1; // RD15 (SW3) configured as input
    TRISDbits.TRISD14 = 1; // RD14 (SW4) configured as input
    TRISBbits.TRISB11 = 1; // RB11 (SW5) configured as input
    ANSELBbits.ANSB11 = 0; // RB11 (SW5) disabled analog
    TRISBbits.TRISB10 = 1; // RB10 (SW6) configured as input
    ANSELBbits.ANSB10 = 0; // RB10 (SW6) disabled analog
    TRISBbits.TRISB9 = 1; // RB9 (SW7) configured as input
    ANSELBbits.ANSB9 = 0; // RB9 (SW7) disabled analog
    TRISBbits.TRISB14 = 0; //speaker configured output
    ANSELBbits.ANSB14 = 0; //speaker disabled analog
}