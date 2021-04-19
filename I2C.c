
#include "I2C.h"

/**
 * @function: void Delay_5us()
 * @description: 5us延时
 * @param {void} 
 * @return: void
 */
void Delay_5us()
{
    _nop_();
    _nop_();
    _nop_();
    _nop_();
}

/**
 * @function: void I2C_Start()
 * @description: I2C开始信号
 * @param {void} 
 * @return: void
 */
void I2C_Start()
{
    I2C_SDA = 1;
    I2C_SCL = 1;
    Delay_5us();
    I2C_SDA = 0;
    Delay_5us();
    I2C_SCL = 0;
    Delay_5us();
}

/**
 * @function: void I2C_Stop()
 * @description: I2C结束信号
 * @param {void} 
 * @return: void
 */
void I2C_Stop()
{
    I2C_SCL = 1;
    I2C_SDA = 0;
    Delay_5us();
    I2C_SDA = 1;
    Delay_5us();
}

/**
 * @function: void I2C_Wait_Ack()
 * @description: I2C等待应答响应函数
 * @param {void} 
 * @return: return 1没有应答、return 0产生应答
 */
uint I2C_Wait_Ack()
{
    uint count = 0;
    I2C_SCL = 1;
    Delay_5us();
    while (I2C_SDA)
    {
        count++;
        if (count > 250)
        {
            I2C_Stop();
            return 1;
        }
    }
    I2C_SCL = 0;
    Delay_5us();
    return 0;
}

/**
 * @function: void I2C_Ack()
 * @description: 主机产生应答信号
 * @param {void} 
 * @return: void
 */
#if 0
void I2C_Ack()
{
    I2C_SDA = 0;
    _nop_();
    I2C_SCL = 1;
    _nop_();
    I2C_SCL = 0;
    _nop_();
    I2C_SDA = 1;
}
#endif

/**
 * @function: void I2C_Write_Byte(uchar WData)
 * @description: I2C写数据函数
 * @param {uchar WData待写入的数据} 
 * @return: void
 */
void I2C_Write_Byte(uchar WData)
{
    uchar i;
    for (i = 0; i < 8; i++)
    {
        I2C_SCL = 0;
        _nop_();
        _nop_();
        if (WData & 0x80) //先发送高字节数据，后发送低字节数据
            I2C_SDA = 1;
        else
            I2C_SDA = 0;
        _nop_();
        _nop_();
        I2C_SCL = 1; //数据写入总线
        _nop_();
        _nop_();
        I2C_SCL = 0;
        WData <<= 1; //准备写入下一位数据
    }
}

/**
 * @function: uchar I2C_Read_Byte()
 * @description: I2C读取数据函数
 * @param {void} 
 * @return: return RData读取的数据
 */
#if 0
uchar I2C_Read_Byte()
{
    uchar i, RData = 0;
    I2C_SDA = 1; //释放数据总线
    _nop_();
    _nop_();
    for (i = 0; i < 8; i++)
    {
        RData <<= 1; //准备读取下一位数据
        I2C_SCL = 0;
        _nop_();
        I2C_SCL = 1; //可以开始读取数据
        _nop_();
        _nop_();
        if (I2C_SDA)
            RData |= 0x01; //1写入RData此刻的最低位
        else
            RData &= 0xfe; //0写入RData此刻的最低位
    }
    I2C_SCL = 0;
    _nop_();
    _nop_();
    return RData;
}
#endif
