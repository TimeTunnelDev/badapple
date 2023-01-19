#include "exti.h"
#include "delay.h" 
//#include "led.h" 
#include "key.h"
#include "led.h"
extern volatile  u8 Flag_WriteFinsh;
//	   
//外部中断初始化程序
//初始化PE2~4,PA0为中断输入.
void EXTIX_Init(void)
{
	NVIC_InitTypeDef   NVIC_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;
	
	KEY_Init(); //按键对应的IO口初始化
 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//使能SYSCFG时钟
	
 
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOF, EXTI_PinSource9);
	
  /* 配置EXTI_Line0 */
  EXTI_InitStructure.EXTI_Line = EXTI_Line9;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断事件
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; 
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;//使能LINE0
  EXTI_Init(&EXTI_InitStructure);//配置

	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;//外部中断4
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;//抢占优先级1
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//子优先级2
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道
  NVIC_Init(&NVIC_InitStructure);//配置
	   
}



void EXTI9_5_IRQHandler(void)
{
	delay_ms(10);	//消抖
	if(KEY0==0)
	Flag_WriteFinsh=1;
	EXTI_ClearITPendingBit(EXTI_Line9);//清除LINE4上的中断标志位  
}








