
#include "OLED.h"
#include "OLEDFont.h"

/***OLED12864/0.96'/SSD1306***/

/**
 * @description: 51延时函数 @11.0592MHz
 * @param {uint ms 毫秒} 
 * @return: void
 */
void Delay_ms(uint ms)
{
   	unsigned char i, j;
		while(ms--)
		{
			_nop_();
			_nop_();
			_nop_();
			i = 11;
			j = 190;
			do
			{
				while (--j);
			} while (--i);
		}
}

/**
 * @function: void OLED_Write_Byte(uchar WCommand,uchar cmdORdata)
 * @description: 
 * @param {uchar WData待写入的内容，uchar cmdORdata指明写入指令还是数据} 
 * @return: void
 */
void OLED_Write_Byte(uchar WData, uchar cmdORdata)
{
    I2C_Start();
    I2C_Write_Byte(OLED_Addr + 0); //写入器件地址(bit0=1读，bit0=0写)
    I2C_Wait_Ack();
    I2C_Write_Byte(cmdORdata); //指明写入指令还是数据
    I2C_Wait_Ack();
    I2C_Write_Byte(WData); //写入数据内容
    I2C_Wait_Ack();
    I2C_Stop();
}

/**
 * @function: void OLED_Fill(uchar Fill_Data)
 * @description: 全屏显示相同数据(主要用去清屏)
 * @param {uchar Fill_Data待全屏写入的数据} 
 * @return: void
 */
void OLED_Fill(uchar Fill_Data)
{
    uchar i, j;
    for (i = 0; i < 8; i++) //8页，共需要循环显示8次
    {
        OLED_Write_Byte(0xb0 + i, isCmd); //页地址0~7
        OLED_Write_Byte(0x10, isCmd);     //设置高位列地址
        OLED_Write_Byte(0x00, isCmd);     //设置低位列地址
        for (j = 0; j < OLED_Width; j++)  //每页有128列需要显示
            OLED_Write_Byte(Fill_Data, isData);
    }
}


/**
 * @function: void OLED_Set_Pos(uchar x,uchar y)
 * @description: 设置起始显示位置
 * @param {uchar x起始显示列0~127、uchar y起始显示页0~7} 
 * @return: void
 */
void OLED_Set_Pos(uchar x, uchar y)
{
    OLED_Write_Byte(0xb0 + y, isCmd);               //起始显示的页
    OLED_Write_Byte((x & 0xf0) >> 4 | 0x10, isCmd); //设置高位列地址
    OLED_Write_Byte(x & 0x0f, isCmd);               //设置低位列地址
}

/**
 * @function: void OLED_Init()
 * @description: OLED初始化函数
 * @param {void} 
 * @return: void
 */
void OLED_Init()
{
    Delay_ms(200);
    OLED_Write_Byte(0xae, isCmd); //关闭显示器 display off
    OLED_Write_Byte(0x00, isCmd); //设置低位列地址 set low column address
    OLED_Write_Byte(0x10, isCmd); //设置高位列地址 set high column address
    OLED_Write_Byte(0x40, isCmd); //设置起始行地址 set start line address
    OLED_Write_Byte(0xb0, isCmd); //设置页地址 set page address
    OLED_Write_Byte(0x81, isCmd); //设置对比度
    OLED_Write_Byte(0xff, isCmd); //对比度,数值越大对比度越高
    OLED_Write_Byte(0xa1, isCmd); //设置段重新映射 不左右翻转set segment remap
    OLED_Write_Byte(0xa6, isCmd); //设置正常/反向 normal / reverse
    OLED_Write_Byte(0xa8, isCmd); //设置多路复用比(1-64) set multiplex ratio(1 to 64)
    OLED_Write_Byte(0x3f, isCmd); //设定值1/32  1/32 duty
    OLED_Write_Byte(0xc8, isCmd); //扫描方向 不上下翻转Com scan direction

    OLED_Write_Byte(0xd3, isCmd); //设置显示偏移 set display offset
    OLED_Write_Byte(0x00, isCmd); //

    OLED_Write_Byte(0xd5, isCmd); //设置osc分区 set osc division
    OLED_Write_Byte(0x80, isCmd); //

    OLED_Write_Byte(0xd8, isCmd); //关闭区域颜色模式 set area color mode off
    OLED_Write_Byte(0x05, isCmd); //

    OLED_Write_Byte(0xd9, isCmd); //设置预充电期 Set Pre-Charge Period
    OLED_Write_Byte(0xf1, isCmd); //

    OLED_Write_Byte(0xda, isCmd); //设置com引脚配置 set com pin configuartion
    OLED_Write_Byte(0x12, isCmd); //

    OLED_Write_Byte(0xdb, isCmd); //设置vcomh set Vcomh
    OLED_Write_Byte(0x30, isCmd); //

    OLED_Write_Byte(0x8d, isCmd); //设置电源泵启用 set charge pump enable
    OLED_Write_Byte(0x14, isCmd); //

    OLED_Write_Byte(0xa4, isCmd); //设置全局显示开启；bit0，1开启，0关闭(白屏/黑屏)
    OLED_Write_Byte(0xa6, isCmd); //设置显示方式，bit0，1反相显示，0正常显示

    OLED_Write_Byte(0xaf, isCmd); //打开oled面板 turn on oled panel

    OLED_Fill(0x00); //清屏
}

/**
 * @function: void OLED_DisplayChar(uchar x, uchar y, uchar DisplayChar, uchar TextSize)
 * @description: 在OLED12864特定位置显示一个字符
 * @param {uchar x列, uchar y行, uchar DisplayChar显示的字符, uchar TextSize字体大小} 
 * @return: void
 */
void OLED_DisplayChar(uchar x, uchar y, uchar DisplayChar)
{
    uchar c = 0, i;
    c = DisplayChar - 32; //等同于减去个空格c = DisplayChar - ' '; 得到显示字符在库中的偏移地址
		OLED_Set_Pos(x, y); //开始显示位置
		for (i = 0; i < 6; i++)
				OLED_Write_Byte(F6X8[c][i], isData); //显示一个6X8字符
}

/**
 * @function: void OLED_DisplayStr(uchar x, uchar y, uchar *DisplayStr, uchar TextSize)
 * @description: 在OLED12864特定位置显示字符串
 * @param {uchar x列, uchar y行, uchar *DisplayStr待显示的字符串, uchar TextSize字体大小} 
 * @return: void
 */
void OLED_DisplayStr(uchar x, uchar y, uchar *DisplayStr) //(uchar *DisplayStr)改成(uchar DisplayStr[])也可
{
    uchar j = 0;
    while (DisplayStr[j] != '\0') //判断字符串是否显示完成
    {
        OLED_DisplayChar(x, y, DisplayStr[j]);
        x += 6;

        if (x > 122) //TextSize6x8如果一行不够显示了，从下一行继续显示
        {
            x = 0;
            y++;
        }
        j++;
    }
}

/**
 * @function: void OLED_DisplayBMP(uchar x0, uchar y0, uchar x1, uchar y1, uchar *BMP)
 * @description: 在OLED12864特定位置显示BMP图片
 * @param {uchar x0,uchar y0(x0,y0显示起点坐标x:0~127,y0:0~7)、uchar x1,uchar y1(x1,y1显示结束坐标x0:0~127,y0:0~7)、uchar *BMP待显示的图片数据} 
 * @return: void
 */
#if 0
void OLED_DisplayBMP(uchar x0, uchar y0, uchar x1, uchar y1, uchar *BMP)
{
    uint j = 0;
    uchar x, y;
    if (y1 % 8 == 0) //用于确定显示几页
        y = y1 / 8;
    else
        y = y1 / 8 + 1;
    for (y = y0; y < y1; y++) //从设置的起始页开始显示到结束页
    {
        OLED_Set_Pos(x0, y);                   //进入下一页显示起始位置
        for (x = x0; x < x1; x++)              //从设置开始列显示到结束列
            OLED_Write_Byte(BMP[j++], isData); //逐个传入显示数据
    }
}
#endif
