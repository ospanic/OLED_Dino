#include "STC8G.h"
#include "OLED.h"
#include "I2C.h"
#include "board.h"
#include "dino.h"

void Pin_Init()
{
	P1M0 = 0x43; //LED OUT PUT
	P1M1 = 0x00;
	
	LED0 = LED1 = LED2 = 0; //Turn Off All Led
	
	P3M0 = 0x00;
	P3M1 = 0x00;
	
	P5M0 = 0x10;
	P5M1 = 0x00;
	
	BEEP = 0;
	
	SW1 = SW2 = SW3 = SW4 = SW5 = SW6 = 1;
}

unsigned char r =0;

void time0_irs(void) interrupt 1
{
	r++;
	Dino_Tick();
}

void Time_init()
{
	TMOD = 0;
	TL0 = 0x30;
	TH0 = 0xf8;
	TR0=1;
	ET0=1;
	
	EA=1;
}

unsigned char Ramod(unsigned char mod)
{
	return r%mod;
}

/*******************************************************************************
* 函 数 名       : main
* 函数功能		 : 主函数
* 输    入       : 无
* 输    出    	 : 无
*******************************************************************************/
void main()
{
		Pin_Init();
	
		OLED_Init();
		OLED_DisplayStr(100,0,"Game");
		Time_init();
	
		Dino_Start();

    while (1)
    {
			Delay_ms(1);
    }	
}