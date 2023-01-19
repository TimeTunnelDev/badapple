/*
 * @Author: 王辰�?
 * @Date: 2022-08-02 17:07:32
 * @Version: 1.0
 * @LastEditors: your name
 * @LastEditTime: 2022-08-02 17:56:44
 * @Description: 环形缓冲�?
 */

#include "RingBuff.h"

RingBuff_t ringBuff;//创建一个ringBuff的缓冲区

/**
 * @描述: 初始化缓冲区
 * @参数: �?
 * @返回�?: �?
 */
void RingBuff_Init(void)
{
   //初始化相关信�?
   ringBuff.Head = 0;
   ringBuff.Tail = 0;
   ringBuff.Lenght = 0;
}
/**
 * @描述: 向缓冲区写数�?
 * @参数: 写入数据
 * @返回�?: TRUE 写入成功 FALSE 写入失败
 */
u8 Write_RingBuff(Value_Type data)
{
   if(ringBuff.Lenght >= RINGBUFF_LEN) //判断缓冲区是否已�?
    {
      return FLASE;
    }
    ringBuff.Ring_Buff[ringBuff.Tail]=data;
    ringBuff.Tail = (ringBuff.Tail+1)%RINGBUFF_LEN;//防止越界非法访问
    ringBuff.Lenght++;
    return TRUE;
}

/**
 * @描述: 读缓冲区中的数据
 * @参数: �?
 * @返回�?: �?
 */
u8 Read_RingBuff(Value_Type *rData)
{
   if(ringBuff.Lenght == 0)//判断非空
    {
       return FLASE;
    }
   *rData = ringBuff.Ring_Buff[ringBuff.Head];//先进先出FIFO，从缓冲区头�?
   ringBuff.Head = (ringBuff.Head+1)%RINGBUFF_LEN;//防止越界非法访问
   ringBuff.Lenght--;
   return TRUE;
}
























