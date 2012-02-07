#include <msp430x20x3.h>
#include "LCD_MSP430.h"
 
int main(void)
{
  WDTCTL = 0x5A80; 
  int o;                    // Stop the Watchdog timer
  for(o=0;o<0xEF;o++)
  delay(145);                          // Startup delay
  init_lcm();
  clr_lcm();                   // Clear the disp[32] buffer
  putstr("16x2 LCD SHIELD");
  lcm_line2();
  putstr("BY TENET");
  
 
  P1DIR |= 0xFF;                        // Set P1.0 as o/p
  while(1)
  { // Dodo code that generates LED viewable PWM on P1.0
    // ;)
     
     P1OUT &= 0xFE;
     char j;
     char i =0;
     for(i=0;i<0xFF;i++)
     {
       for(j =0;j<=i;j++)
       {
         P1OUT |= 0x01;
         delay(10);
       }
       for(j=i;j<0xFF;j++)
       {
         P1OUT &= 0xFE;
         delay(10);
       }
     }
     
   }
  //return(0);

}