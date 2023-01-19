#include "sys.h"
#include "usart2.h"	  
#include "exfuns.h"   
//////////////////////////////////////////////////////////////////////////////////	   
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F407开发板
//串口2初始化 
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2014/5/14
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved
//********************************************************************************
//修改说明
//无
////////////////////////////////////////////////////////////////////////////////// 	

//初始化IO 串口2
//pclk1:PCLK1时钟频率(Mhz)
//bound:波特率 
#include "RingBuff.h"

//char ReceiveBuffer1[RECEIVE_SIZE] = "0";
//char ReceiveBuffer2[RECEIVE_SIZE] = "0";
//// 0空闲且无数据，1写占用，2已满待读取
uint8_t ReceiveBuffer1_Status = 0;
uint8_t ReceiveBuffer2_Status = 0;

//uint16_t ReceiveBuffer1_WriteIndex = 0;
//uint16_t ReceiveBuffer2_WriteIndex = 0;

//// 0正常模式,1文件接收模式
//uint8_t USART1_Mode = 1;
u32 rcnt=0;
void usart2_init(u32 bound)
{  	 
  
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //使能GPIOA时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);//使能USART2时钟

	//USART1_TX   PA.2 PA.3
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3; //PA2,PA3,复用功能,上拉输出
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOA,&GPIO_InitStructure); //初始化PA2，PA3
	
 
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2); //GPIOA2复用为USART2
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2); //GPIOA3复用为USART2
  
	USART_InitStructure.USART_BaudRate = bound;//一般设置为9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode =   USART_Mode_Tx|USART_Mode_Rx;	// 发模式
  USART_Init(USART2, &USART_InitStructure); //初始化串口
  USART_Cmd(USART2, ENABLE);  //使能串口 
	
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启相关中断

	//Usart2 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;//串口1中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、
}


void USART2_IRQHandler(void)
{
		if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  //接收中断
	{
		Write_RingBuff((u8)USART2->DR);
		rcnt++;
		if(rcnt==SingleWrite)
		{
			rcnt=0;
			ReceiveBuffer1_Status=1;//buffer1装满			
		}
		

		
	}
	
	
}
//void File_Receiver(void)
//{
//	if(USART_GetITStatus(USART2, USART_IT_RXNE)!=RESET)
//	{
//		if(ReceiveBuffer1_Status == 1)
//		{
//			//写Buffer1
//			if(ReceiveBuffer1_WriteIndex < RECEIVE_SIZE)
//			{
//					ReceiveBuffer1[ReceiveBuffer1_WriteIndex ++] = USART_ReceiveData(USART2);
//			}
//			else
//			{
//				ReceiveBuffer1_Status = 2;
//				ReceiveBuffer1_WriteIndex = 0;
//			}
//		}
//		else if(ReceiveBuffer2_Status == 1)
//		{
//			//写Buffer2
//			if(ReceiveBuffer2_WriteIndex < RECEIVE_SIZE)
//			{
//					ReceiveBuffer2[ReceiveBuffer2_WriteIndex ++] = USART_ReceiveData(USART1);
//			}
//			else
//			{
//				ReceiveBuffer2_Status = 2;
//				ReceiveBuffer2_WriteIndex = 0;
//			}
//		}
//		else if(ReceiveBuffer1_Status == 0)
//		{
//			ReceiveBuffer1_Status = 1;
//			ReceiveBuffer1[ReceiveBuffer1_WriteIndex ++] = USART_ReceiveData(USART1);
//		}
//		else if(ReceiveBuffer2_Status == 0)
//		{
//			ReceiveBuffer2_Status = 1;
//			ReceiveBuffer2[ReceiveBuffer2_WriteIndex ++] = USART_ReceiveData(USART1);
//		}
////		else
////		{
////			USART_ReceiveData(USART1);
////		}
//	}
//}


