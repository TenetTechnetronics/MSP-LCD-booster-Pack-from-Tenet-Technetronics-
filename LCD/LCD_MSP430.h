//**************************************Tenet Technetronics*****************************//
//                                                                                       |
//  LCD_MSP430.h - This is the lib for 16x2 Liquid crystal display(LCD)                  |
//                                                                                       |
//                                                                                       |
//                MSP430F20xx                                                            |
//             -----------------                                                         |
//            |VCC           GND|                                                        |
//            |P1.0          XIN|                                                        | 
//            |P1.1         XOUT|   /|\                                                  |
//       RS<--|P1.2         TEST|     |                                                  |
//        E<--|P1.3          RST|------                                                  | 
//       D4<--|P1.4         P1.7|-->D7                                                   |
//       D5<--|P1.5         P1.6|-->D6                                                   |
                                                                                         |
//    R/W signal of the LCD_module is tied to GND                                        |
//    You must connect a pot to vary the voltage at the contrast pin of the LCD_module   |
                                                                                         |
//***************************************************************************************|
//   Connections to LCD_module                                                           |
//   Your LCD_module in all probabilty powers it's onboard controller off a 5V supply.   |
//   So U'll have to provide a 5V regulated supply seperately to the LCD_module          |
//   Remember MSP430 runs at 3.6 V (max) ...                                             |
//   in the setup we have tied R/W signal of the LCD_module to GND, also                 |
//   if your uC and LCD_module are running at different Vdd levels, it is advisable      |
//   to use a level converter while reading from the LCD_module.                         |
//**************************************************************************************** 
   





#ifndef __msp430x20x1_h__  // Change this header file to include the MSP430 uC you are using.
	#include <msp430x20x1.h> // Header must only be included if none of the files linking to 
	#define __msp430x20x1_h__ // this file include it already
#endif

// Global Variable declarations
char disp[32];  // Buffer to hold characters to be displayed on the LCD

// Global Function Declarations
void init_lcm(void);       // Function to initialize the LCD_module (LCM)
void putchar(char what);   // Function to print a char at current cursor location
void putstr(char *what);   // Function to print string from current cursor location
void clr_lcm(void);        // Function to clear the LCM display
void lcm_line2(void);      // Function to move cursor to start of line 2
void delay(char);          // Function to provide delay necessary for certain LCM operations
void strobe(void);         // Function to toggle the enable signal after suitable delay
void update_lcm(char *str);// Function to write disp[32] buffer onto the LCM display
                           // str is a 32 character array
void clr_disp_buffer(void);// Function to flush disp[32] to blank spaces
void tostr(int i,char *str); //Converts Integers to String, useful routine

void tostr(int i,char *s)	// Convert Interger to String
{
	char *p;
	p=s;
	
	if(i >= 200)
		p[0]= 2;
	else if(i >= 100)
		p[0]= 1;
	else
		p[0]= 0;
		
	p[2]=i%10;
	
	i-=p[2];
	i/=10;
	p[1]=i%10;
	
        p[3] = 0; // mark end of string
	p[2]+=0x30;
	p[1]+=0x30;
	p[0]+=0x30;
}
   

void clr_disp_buffer(void)
{ char x = 0;
	// Clears the internal message Buffer to contain blank spaces
  for(x=0;x<32;x++)
    disp[x] = ' ';
}

void strobe(void)
{ // This is a small function which pulls the Enable pin of the LCM low for some time
   P1OUT=P1OUT & 0xF7; //Enable Low
   delay(105); 
   P1OUT=P1OUT | 0x08; //Enable High
   delay(85); 
}
void init_lcm(void)
{ 
   P1DIR = 0xFF;
   P1OUT = 0xFF;
   // Set Interface length for the LCM
   P1OUT=P1OUT | 0x08; //Pull Enable pin of the LCM High
   P1OUT=P1OUT & 0xFB; //Set Command Mode
   P1OUT=0x2F;
   P1OUT=P1OUT & 0xFB; //Set Command Mode
   strobe(); 
   
   
	
  
   //Since we are now in the 4 bit mode
   //Byte wide transmission will now be broken down into 2 parts
   //We thus have to transmit the Higher Nibble (HN) first followed by the
   //Lower Nibble (LN)
   
    //Enable 2 line mode
   
   //HN
   P1OUT=0x2F;
   P1OUT=P1OUT & 0xFB; //Set Command Mode
   strobe();
	
   //LN
   P1OUT=0x8F;
   P1OUT=P1OUT & 0xFB; //Set Command Mode
   strobe();
   
	
   //Set Cursor parameters
   
   //HN
   P1OUT=0x0F;
   P1OUT=P1OUT & 0xFB; 
   strobe();
	
   //LN
   P1OUT=0xFF;
   P1OUT=P1OUT & 0xFB; 
   delay(65); 
   strobe();

   
   
   //Bring Cursor to Home Position
   //HN
   P1OUT=0x0F;
   P1OUT=P1OUT & 0xFB; 
   strobe();
   
   //LN
   P1OUT=0x3F;
   P1OUT=P1OUT & 0xFB; 
   strobe();
   
}
void lcm_line2(void)
{
   // Jump to line 2 of LCM
   //HN
   P1OUT=0xCF;
   P1OUT=P1OUT & 0xFB; 
   strobe();
	
   //LN
   P1OUT=0x0F;
   P1OUT=P1OUT & 0xFB; 
   strobe();
}

void update_lcm(char *str)
{ // write whatever is in the disp[32] message buffer onto the LCM
  char pos;
  // write the first 16 characters
  clr_lcm();
  for(pos = 0; pos <16; pos++ )
  { 
     putchar(str[pos]);
  }
  // write the next 16 characters
  lcm_line2();// newline ;)
  for(pos = 16; pos<32;pos++)
  {
    putchar(str[pos]);
  }
}
void clr_lcm(void)
{
   // Clear LCM, Blank Display
   //HN
   P1OUT=0x0F;
   P1OUT=P1OUT & 0xFB; 
   strobe();
	
   //LN
   P1OUT=0x1F;
   P1OUT=P1OUT & 0xFB; 
   strobe();
   
}
void putchar(char ascii)
{   // function to print the character passed, on the LCM at the current cursor position
	// HN
	P1OUT=0x0F | ascii ; 
	P1OUT=P1OUT | 0x01; //Character Mode
	delay(115); 
	strobe();
	// LN
	P1OUT=0x0F | (ascii<<4); // Remember that now we want to make available 
                              // the lower bits on our 4 bit data bus.
	strobe();
}

void putstr(char *str)
{
  while (*str)
  {
     putchar(*str++);
  }
}

void delay(char d)
{ 
	int i;// The Hallowed delay function
   for(i=0; i<2*d;i++);
}