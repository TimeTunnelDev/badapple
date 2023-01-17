#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "lcd.h"
#include "key.h"  
//#include "sram.h"   
#include "malloc.h" 
#include "usmart.h"  
#include "sdio_sdcard.h"    
#include "malloc.h" 
#include "w25qxx.h"    
#include "ff.h"  
#include "exfuns.h"    
//ALIENTEK ̽����STM32F407������ ʵ��39
//FATFS ʵ�� -�⺯���汾
//����֧�֣�www.openedv.com
//�Ա����̣�http://eboard.taobao.com
//������������ӿƼ����޹�˾    
//���ߣ�����ԭ�� @ALIENTEK 
 

/*******��������*****/
FIL fil;
FRESULT res;
UINT bww;
char buf[100];

int main(void)
{        
 	u32 total,free;
	u8 t=0;	
	u8 res=0;	

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(168);  //��ʼ����ʱ����
	uart_init(115200);		//��ʼ�����ڲ�����Ϊ115200
	LED_Init();					//��ʼ��LED 
	usmart_dev.init(84);		//��ʼ��USMART
 	LCD_Init();					//LCD��ʼ��  
 	KEY_Init();					//������ʼ�� 
	W25QXX_Init();				//��ʼ��W25Q128
	my_mem_init(SRAMIN);		//��ʼ���ڲ��ڴ�� 
	my_mem_init(SRAMCCM);		//��ʼ��CCM�ڴ��
	
 	BRUSH_COLOR=RED;//��������Ϊ��ɫ    
 	while(SD_Init())//��ⲻ��SD��
	{

	}
 	exfuns_init();							//Ϊfatfs��ر��������ڴ�				 
  	res=f_mount(fs[0],"0:",1); 					//����SD�� 
// 	f_mount(fs[1],"1:",1); 				//����FLASH.	
//	if(res==0X0D)//FLASH����,FAT�ļ�ϵͳ����,���¸�ʽ��FLASH
//	{
//		LCD_DisplayString(1,1,16,"Flash Disk Formatting...");	//��ʽ��FLASH
//		res=f_mkfs("0:",1,4096);//��ʽ��FLASH,1,�̷�;1,����Ҫ������,8������Ϊ1����
//		if(res==0)
//		{
//			f_setlabel((const TCHAR *)"1:ALIENTEK");	//����Flash���̵�����Ϊ��ALIENTEK
//			LCD_DisplayString(1,22,16,"Flash Disk Format Finish");	//��ʽ�����
//		}else LCD_DisplayString(1,22,16,"Flash Disk Format Error ");	//��ʽ��ʧ��
//		delay_ms(1000);
//	}													    
//	while(exf_getfree("0",&total,&free))	//�õ�SD������������ʣ������
//	{
//		LCD_ShowString(30,150,200,16,16,"SD Card Fatfs Error!");
//		delay_ms(200);
//		LCD_Fill(30,150,240,150+16,WHITE);	//�����ʾ			  
//		delay_ms(200);
//	}													  			    
// 	POINT_COLOR=BLUE;//��������Ϊ��ɫ	   
//	LCD_ShowString(30,150,200,16,16,"FATFS OK!");	 
//	LCD_ShowString(30,170,200,16,16,"SD Total Size:     MB");	 
//	LCD_ShowString(30,190,200,16,16,"SD  Free Size:     MB"); 	    
// 	LCD_ShowNum(30+8*14,170,total>>10,5,16);				//��ʾSD�������� MB
// 	LCD_ShowNum(30+8*14,190,free>>10,5,16);					//��ʾSD��ʣ������ MB		


/********************start*************************/	
	res=f_open (&fil,"0:/message.txt", FA_CREATE_ALWAYS|FA_WRITE);	
	LCD_DisplayNum(1,1,(u32)(fil.fs)->drv,1,24,1);
	f_write (&fil, "alientek test", 14, &bww);
	
	f_close(&fil);
	
	res=f_open (&fil,"0:/message.txt", FA_READ);
    
    f_read (&fil, buf,14,&bww);	
	f_close(&fil);
	
	LCD_DisplayString(1,66,16,(u8 *)buf);
/********************end***************************/

	LCD_DisplayString(1,88,16,"11111");
	LCD_DisplayNum(1,44,SDCardInfo.CardBlockSize,4,24,4);

	
	while(1)
	{
		t++; 
		delay_ms(200);		 			   
		LED0=!LED0;
	} 
}





