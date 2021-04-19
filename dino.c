#include "STC8G.h"
#include "OLED.h"
#include "I2C.h"
#include "board.h"

code unsigned char xx2[32] = {0,0,0,0,0,0,0,0,
															2,3,4,5,6,7,8,9,
															12,14,16,18,20,22,24,26,
															30,32,34,36,36,36,36,36};

code unsigned char Cactus_12_24[36] ={0xC0,0xC0,0x00,0x00,0xFE,0xFF,0xFF,0xFE,0x00,0xE0,0xE0,0xC0,0x1F,0x3F,0x60,0x60,
																			0xFF,0xFF,0xFF,0xFF,0x60,0x7F,0x3F,0x1F,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,
																			0x00,0x00,0x00,0x00};

code unsigned char Cactus_8_16[2][8] = {0xF0,0xE0,0x00,0xFF,0xFF,0x80,0xFC,0x78,0x13,0x17,0x16,0xFF,0xFF,0x11,0x10,0x10};

	
code unsigned char Dino[2][16] = {0x40,0xC0,0x80,0x00,0x00,0x80,0x80,0xC0,0xFE,0xFF,0xFD,0xBF,0xAF,0x2F,0x0E,0x0C,
																	0x00,0x03,0x07,0x0F,0x1F,0xFF,0x9F,0x1F,0xFF,0x8F,0x07,0x00,0x01,0x00,0x00,0x00};

unsigned long s = 0;																
unsigned char dino_y = 0,speed = 15;
																	
short xr[4] = {100,150,200,250};

void OLED_Display_Dino(char y)
{
	char y_line=0,y_mode =0,i,j;
	
	y_line = 6 - y/8;
	y_mode = y%8;
	
	for(i=0; i<6; i++)
	{
		OLED_Set_Pos(0, i);
		for(j=0; j<16; j++)
		{
			OLED_Write_Byte(0, isData);
		}
	}
	
	if(y_mode) 
	{
		y_line--;
		
		OLED_Set_Pos(0, y_line);
		for(i=0; i<16; i++)
		{
			OLED_Write_Byte((Dino[0][i]  << (8-y_mode)), isData);
		}
		
		OLED_Set_Pos(0, y_line+1);
		for(i=0; i<16; i++)
		{
			OLED_Write_Byte((Dino[0][i]  >> (y_mode)) | (Dino[1][i]  << (8-y_mode)) , isData);
		}
		
		OLED_Set_Pos(0, y_line+2);
		for(i=0; i<16; i++)
		{
			OLED_Write_Byte((Dino[1][i]  >> y_mode), isData);
		}
	}
	else
	{
		OLED_Set_Pos(0, y_line);
		for(i=0; i<16; i++)
		{
			OLED_Write_Byte(Dino[0][i], isData);
		}
		
		OLED_Set_Pos(0, y_line+1);
		
		for(i=0; i<16; i++)
		{
			OLED_Write_Byte(Dino[1][i], isData);
		}
	}
}


void OLED_Display_Cactus(short x)
{
	char start=0,end=8,i;
	
	if(x < 0)
	{		
		start = 0-x;
		x=0;
	}
	if(x > 120) end = 128 - x;
	
	OLED_Set_Pos(x, 6);
	for(i=start; i<end; i++)
	{
		OLED_Write_Byte(Cactus_8_16[0][i], isData);
	}
	
	OLED_Set_Pos(x, 7);
	for(i=start; i<end; i++)
	{
		OLED_Write_Byte(Cactus_8_16[1][i], isData);
	}
}

void OLED_Line(unsigned char start, unsigned char len)
{
		unsigned char tmp;
	
		if(start >127) return;
	
		if((start + len) > 128)
		{
			tmp = 128 - start;
		}
		else
		{
			tmp=len;
		}

    OLED_Set_Pos(start, 6);
		len = tmp;
		while(len--)OLED_Write_Byte(0x00, isData);
	
		len = tmp;
		OLED_Set_Pos(start, 7);
		while(len--)OLED_Write_Byte(0x10, isData);
}

void Dino_Tick(void)
{
	static char jump = 1,cc = 0,dd =0;
	cc ++; dd++;
	
	if((SW5 == 0) && (jump))
	{
			if(cc > xx2[dino_y])
			{
				cc=0;
				dino_y++;
				if(dino_y == 30) jump =0;
			}
	}
	else
	{
		jump = 0;
		if(cc > xx2[dino_y])
		{
			cc=0;
			if(dino_y)dino_y--;
			
			if((dino_y==0) && (SW5)) jump =1;
		}
	}

	if(dd == speed)
	{
		LED0 =~LED0;
		dd=0;
		xr[0]--;
		xr[1]--;
		xr[2]--;
		xr[3]--;
		
		if(xr[0] < -5)
		{
			xr[0] = xr[1];
			xr[1] = xr[2];
			xr[2] = xr[3];
			
			xr[3] = xr[3] +50 + Ramod(50);
		}
		s++;
	}
}

void game_over(void)
{
	
	OLED_DisplayStr(35,3,"GAME OVER!");
	ET0=0;
	s=0;
	Delay_ms(1000);
	while(1)
	{
		if(SW5 == 0) 
		{
			OLED_Fill(0x00);
			xr[0] = 100;
			xr[1] = 150;
			xr[2] = 200;
			xr[3] = 250;
			ET0=1;
			break;
		}
	}
}
void Dino_Start(void)
{
		short i = 0;	
		unsigned char ss[8] = 0,c_t,d_t,l_t;
	
    while (1)
    {
			if((xr[0] > 0) && (dino_y > 4))
			{
				OLED_Line(0, xr[0]);
			}
			
			for(i=0; i<3; i++)
			{
				if(i==1) OLED_Display_Dino(dino_y);
				
				if(xr[i] < 126) 
				{
					OLED_Display_Cactus(xr[i]);
					OLED_Line(xr[i] + 8, xr[i+1] - xr[i]);
				}
			}
			
			if((xr[0] < 16) && ( dino_y < 16))
			{
				if(xr[0] > 0)
				{
					c_t = 0;
					d_t = xr[0];
					l_t = 16 - xr[0];
				}
				else
				{
					c_t = 0 - xr[0];
					d_t = 0;
					l_t = 8 + xr[0];
				}
				
				while(l_t--)
				{
					if(dino_y > 8)
					{
						if(Cactus_8_16[0][c_t] & (Dino[1][d_t] >> (dino_y - 8))) game_over();
					}
					else
					{
						if(Cactus_8_16[0][c_t] & ((Dino[0][d_t]  >> (dino_y)) | (Dino[1][d_t]  << (8-dino_y)) ))game_over();
					}
					c_t++;d_t++;
				}
			}
			
			ss[5] = 'm';
			ss[4] = 'K';
			ss[3] = (s/100)%10+48;
			ss[2] = (s/1000)%10+48;
			ss[1] = (s/10000)%10+48;
			ss[0] = (s/100000)%10+48;
			OLED_DisplayStr(92,0,ss);
			
			if(s>80000) speed = 7;
			else if(s>70000) speed = 8;
			else if(s>60000) speed = 9;
			else if(s>50000) speed = 10;
			else if(s>40000) speed = 11;
			else if(s>30000) speed = 12;
			else if(s>20000) speed = 13;
			else if(s>10000) speed = 14;
			Delay_ms(10);
    }
}