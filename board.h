
#ifndef __BOARD_H__
#define __BOARD_H__

sbit LED0=P1^0;
sbit LED1=P1^1;
sbit LED2=P1^6;

sbit BEEP=P5^4;

sbit SW1=P3^7;
sbit SW2=P3^6;
sbit SW3=P3^5;
sbit SW4=P3^4;
sbit SW5=P5^5;
sbit SW6=P1^7;

unsigned char Ramod(unsigned char mod);

#endif
