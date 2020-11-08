#include <xc.h>
#include "main.h"

void main()
{
    init();
    while(1)
    {
        reset_LEDs();
        
        if (PORTFbits.RF4)          //  if SW2 is ON
            fan();
        else if (PORTFbits.RF5)     //  if SW1 is ON
           shift();
        else if(PORTFbits.RF3)      //  if SW0 is ON
            counter();
        
        if(PORTBbits.RB9)           //  if SW7 is ON
            break;                  //  break from the loop and end program
        
        make_sound();
    }
    
    reset_LEDs();                   //  turn OFF the LEDs
}

void counter()
{
    //  while SW0 is ON and both SW1 and SW2 are OFF (if SW1 or SW2 is ON -> break from this function)
    //  if SW7 is ON -> BREAK
    while(PORTFbits.RF3 && !PORTFbits.RF5 && !PORTFbits.RF4 && !PORTBbits.RB9)
    {
        if (PORTDbits.RD15)     //  if SW3 is ON
            PORTA = (PORTA - 1) & 0xff;            //  count down
        else
            PORTA = (PORTA + 1) & 0xff;            //  otherwise, count up
        
        delay();                //  delay between each iteration
        make_sound();
    }
}

void shift()
{
    //  if SW1 is ON and SW2 is OFF (if SW2 is ON -> break from this function)
    //  if SW7 is ON -> BREAK
    while (PORTFbits.RF5 && !PORTFbits.RF4 && !PORTBbits.RB9)
    {
        if ((0xff & PORTA) == 0x00) //  if all the LEDs is OFF -> need to initiate them
       {
           if (PORTDbits.RD15)      //  if SW3 is turned ON
                PORTA = 0x80;       //  initiate with LED7 ON
           else
                PORTA = 0x1;        //  otherwise, initiate with LED0 ON
       }
        
        delay();                    //  delay between each iteration
        make_sound();               
        if (PORTDbits.RD15)         //  if SW3 is ON
            PORTA >>= 1;            //  shift the LEDs to the RIGHT
        else
            PORTA <<= 1;            //  otherwise, shift to the LEFT
        
    }
}

//      LEDs: 0010 0100                         LEDSs:  0010 0100                         
//      AND   1111 0000                         AND     0000 1111
//      RES:  0010 0000 shift << 1              RES:    0000 0100   shift >> 1
//            0100 0000            <-OR->               0000 0010
//                FINAL RESULT:   0100 0010
void fan()
{
//  while SW2 is ON
//  if SW7 is ON -> BREAK
    while(PORTFbits.RF4 && !PORTBbits.RB9)
    {
        if ((0xff & PORTA) == 0x0)  //  if all the LEDs is OFF -> need to initiate them
        {
            if (PORTDbits.RD15)     //  if SW3 is turned ON
                PORTA = 0X81;       //  initiate LEDs with  1000 0001
            else
                PORTA = 0x18;       //  otherwise, init as  0001 1000
        }  
        delay();                    //  delay between each iteration
        make_sound();
        
        if (PORTDbits.RD15)         //  if SW3 is ON, shift the LEDs towards INSIDE
        {   //  the 2nd & operator is to make sure only the 4 LEDs was shifted in each side
            PORTA = (((PORTA & 0xf0) >> 1) & 0xf0) | (((PORTA & 0x0f) << 1) & 0x0f);
        }
        else                        //  otherwise, shift the LEDs towards the OUTSIDE
            PORTA = ((PORTA & 0xf0) << 1) | ((PORTA & 0x0f) >> 1);                                                                                     
    }
}

void make_sound()
{
    //  if SW6 is OFF -> exit the function and do not make sound
    if (!PORTBbits.RB10) //SW6
        return;
    
    int i, num = 0; 
    while(num++ < 10)
    {
        PORTBbits.RB14--;
        for(i = 0; i < 500 ; i++)   // delay between the two beeps
            ;
        PORTBbits.RB14++;
        for(i = 0; i < 500; i++)   // delay between the two beeps
            ;
    }
    
    //  if either SW0, SW1 nor SW2 is ON, add delay
    if (!(PORTFbits.RF3 || PORTFbits.RF5 || PORTFbits.RF4))
        delay();
        
}

void delay()
{
    int interval = 80000;
    if (!PORTDbits.RD14)    //  if SW4 is OFF slower the speed
        interval = 120000;
    
    int j;
    for (j = 0; j < interval; j++)  //  the actual delay
        ;
    
    while (PORTBbits.RB11)  //  while SW5 is ON, pause the iteration
        ;
}

void reset_LEDs()
{
    PORTA = 0x0;
}

void init()
{
    TRISA &= 0xff00;        //  configure all the LEDs as output
     
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