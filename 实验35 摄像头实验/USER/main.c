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
#include "RingBuff.h"
#include "usart2.h"	 
#include "exti.h"
#include "timer.h"



u8 test[20];
FIL fil ;
UINT bww;
/*******��������*****/

volatile u32 number=0;//д��SD������
volatile u8 Flag_WriteFinsh=0;
volatile u8 *p;
u16 temp_cnt=0;
/*һ��RGB565ͼƬռ�õĻ����С*/
#define PIC_SIZE 240*320*2
/*��LCD��X����*/
#define DISPLAY_START_X 0
/*��LCD��Y����*/
#define DISPLAY_START_Y 0
/*LCD��ȷֱ���*/
#define OLED_WIDTH   240
/*LCD�߶ȷֱ���*/
#define OLED_HEIGHT  320
/*������ʾ�Ļ�����*/
//uint8_t framebuffer[PIC_SIZE] = {1};
u16 framebuffer[2];
int sd_show_picture_bin(const char *path)
{
    UINT br;
//    FIL fil ;
    int offset = 0 ;
    int file_size ;
    FRESULT res ;
		int i;
		uint32_t cnt=0;
		uint16_t TEMP=0;
    /*1.�򿪴�������Fatfs�ļ�·��: path*/
    res = f_open(&fil, path, FA_READ);

    if(FR_OK != res)
		{
        return -1;			
		}


    /*2.��������ļ��Ĵ�С*/
    file_size = f_size(&fil);
		printf("�ļ��ߴ�Ϊ%d",file_size);
    /*3.��ʼ��ƫ�Ʊ���Ϊ0*/
    offset = 0 ;
		LCD_SetCursor(0,0); //���ù��λ�� 
		LCD_WriteGRAM();        //��ʼд��GRAM


    /*4.����bin�ļ���һ������������ͼƬ��Ȼ�󲻶ϵĸ�LCD������ʾ*/
    while(cnt<file_size/2)
    {
        res = f_read(&fil, (void *)framebuffer, 2, &br);

        if(FR_OK != res)
            return -2;

				LCD_DATA=*framebuffer;
        /*7.��ƫ�������PIC_SIZE/2*/
				cnt++;


        offset += 2;
        res = f_lseek(&fil, offset);

        if(FR_OK != res)
				{
            return -3;
					
				}
    }

    /*8.�ر��ļ�������*/
    res = f_close(&fil);

    if(FR_OK != res)
        return -4;

    return 0 ;
}




int main(void)
{        
 	u32 total,free;
	u8 t=0;	
	u8 res=0;	

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(168);  //��ʼ����ʱ����
	uart_init(115200);		//��ʼ�����ڲ�����Ϊ115200
	usart2_init(115200);
	LED_Init();
 	LCD_Init();					//LCD��ʼ��  	
	my_mem_init(SRAMIN);		//��ʼ���ڲ��ڴ�� 
	my_mem_init(SRAMCCM);		//��ʼ��CCM�ڴ��
	RingBuff_Init();
	EXTIX_Init();
	BEEP =0;
	TIM3_Int_Init(10000-1,8400-1);//10Khz����,1�����ж�һ��
 	BRUSH_COLOR=RED;//��������Ϊ��ɫ    
 	while(SD_Init())//��ⲻ��SD��
	{
		printf("SD_Init error\r\n");
	}
 	exfuns_init();							//Ϊfatfs��ر��������ڴ�				 
	p=fatbuf;
  res=f_mount(fs[0],"0:",1); 					//����SD�� 
//	framebuffer=(u16 *)mymalloc(SRAMIN,PIC_SIZE/2);//����һ֡ͼ��̫�󣬹�ÿ�δ�SD����ȡ��֡ͼ��

//	res=f_open (&fil,"0:/badapple.bin", FA_WRITE|FA_CREATE_ALWAYS		);
//	res=f_open (&fil,"0:/true_test.bin", FA_WRITE|FA_CREATE_ALWAYS		);
    

//********************end***************************/
//	while(Flag_WriteFinsh==0)
//	{

//		if(ReceiveBuffer1_Status==1) //ÿ����4096�ֽ����ݾͿ�ʼд��SD��
//		{
//			ReceiveBuffer1_Status=0;
//			while(p-fatbuf<SingleWrite)
//			{
//				Read_RingBuff(p);
//				p++;
//			}
//			p=fatbuf;
//			f_write(&fil,(const void *)fatbuf,SingleWrite,&bww);
//			number++;
//			printf("��%d��д��SD�����",number);
//		}
//		
//	}
//		f_close(&fil);

//		BEEP=1;
//		delay_ms(600);
//		BEEP=0;
		if(sd_show_picture_bin("0:/badapple.bin")!=0)
			printf ("show_picture error");
	
	while(1);
}





