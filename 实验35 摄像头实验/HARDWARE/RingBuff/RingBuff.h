/*
 * @Author: 王辰昊
 * @Date: 2022-08-02 17:11:27
 * @Version: 1.0
 * @LastEditors: your name
 * @LastEditTime: 2022-08-02 17:34:06
 * @Description: 环形缓冲区
 */
#ifndef __RINGBUFF_H__
#define __RINGBUFF_H__

#include "sys.h"
#include "exfuns.h"    

#define RINGBUFF_LEN  (SingleWrite*2)//缓冲区最大长度
#define  FLASE   1 
#define  TRUE    0 


typedef uint8_t Value_Type;  //缓冲区数据类型
typedef uint32_t Maxlength_Type; //缓冲区最大长度所支持的数据类型

typedef struct
{
    Maxlength_Type Head;           
    Maxlength_Type Tail;
    Maxlength_Type Lenght;
    Value_Type Ring_Buff[RINGBUFF_LEN];
}RingBuff_t;

extern RingBuff_t ringBuff;

void RingBuff_Init(void);
u8 Write_RingBuff(Value_Type data);
u8 Read_RingBuff(Value_Type *rData);





#endif