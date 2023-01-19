#include "sys.h"
#include "usart2.h"	  
#include "exfuns.h"   
//////////////////////////////////////////////////////////////////////////////////	   
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//����2��ʼ�� 
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2014/5/14
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved
//********************************************************************************
//�޸�˵��
//��
////////////////////////////////////////////////////////////////////////////////// 	

//��ʼ��IO ����2
//pclk1:PCLK1ʱ��Ƶ��(Mhz)
//bound:������ 
#include "RingBuff.h"

//char ReceiveBuffer1[RECEIVE_SIZE] = "0";
//char ReceiveBuffer2[RECEIVE_SIZE] = "0";
//// 0�����������ݣ�1дռ�ã�2��������ȡ
uint8_t ReceiveBuffer1_Status = 0;
uint8_t ReceiveBuffer2_Status = 0;

//uint16_t ReceiveBuffer1_WriteIndex = 0;
//uint16_t ReceiveBuffer2_WriteIndex = 0;

//// 0����ģʽ,1�ļ�����ģʽ
//uint8_t USART1_Mode = 1;
u32 rcnt=0;
void usart2_init(u32 bound)
{  	 
  
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //ʹ��GPIOAʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);//ʹ��USART2ʱ��

	//USART1_TX   PA.2 PA.3
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3; //PA2,PA3,���ù���,�������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù��� 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOA,&GPIO_InitStructure); //��ʼ��PA2��PA3
	
 
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2); //GPIOA2����ΪUSART2
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2); //GPIOA3����ΪUSART2
  
	USART_InitStructure.USART_BaudRate = bound;//һ������Ϊ9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode =   USART_Mode_Tx|USART_Mode_Rx;	// ��ģʽ
  USART_Init(USART2, &USART_InitStructure); //��ʼ������
  USART_Cmd(USART2, ENABLE);  //ʹ�ܴ��� 
	
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//��������ж�

	//Usart2 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;//����1�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����
}


void USART2_IRQHandler(void)
{
		if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  //�����ж�
	{
		Write_RingBuff((u8)USART2->DR);
		rcnt++;
		if(rcnt==SingleWrite)
		{
			rcnt=0;
			ReceiveBuffer1_Status=1;//buffer1װ��			
		}
		

		
	}
	
	
}
//void File_Receiver(void)
//{
//	if(USART_GetITStatus(USART2, USART_IT_RXNE)!=RESET)
//	{
//		if(ReceiveBuffer1_Status == 1)
//		{
//			//дBuffer1
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
//			//дBuffer2
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


