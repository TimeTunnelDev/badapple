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
/*******变量定义*****/

volatile u32 number=0;//写入SD卡次数
volatile u8 Flag_WriteFinsh=0;
volatile u8 *p;
u16 temp_cnt=0;
/*一张RGB565图片占用的缓存大小*/
#define PIC_SIZE 240*320*2
/*在LCD的X坐标*/
#define DISPLAY_START_X 0
/*在LCD的Y坐标*/
#define DISPLAY_START_Y 0
/*LCD宽度分辨率*/
#define OLED_WIDTH   240
/*LCD高度分辨率*/
#define OLED_HEIGHT  320
/*用于显示的缓存区*/
//uint8_t framebuffer[PIC_SIZE] = {1};
u16 *framebuffer;
int sd_show_picture_bin(const char *path)
{
    UINT br;
//    FIL fil ;
    int offset = 0 ;
    int file_size ;
    FRESULT res ;
		int i;
		uint8_t cnt=0;
		uint16_t TEMP=0;
    /*1.打开传进来的Fatfs文件路径: path*/
    res = f_open(&fil, path, FA_READ);

    if(FR_OK != res)
        return -1;

    /*2.计算这个文件的大小*/
    file_size = f_size(&fil);
    /*3.初始化偏移变量为0*/
    offset = 0 ;

    /*4.计算bin文件里一共包含多少张图片，然后不断的给LCD进行显示*/
    for(i = 0 ; i < file_size /( PIC_SIZE /2 ) ; i++)
    {
        /*5.读取一张图片,一张图片的大小是PIC_SIZE，
        		将读取出来的图片存放到缓存区framebuffer数组里*/
        res = f_read(&fil, framebuffer, PIC_SIZE/2, &br);

        if(FR_OK != res)
            return -2;

        /*6.将整个数组赋值给显示接口，显示图片*/
        LCD_Draw_Picture(DISPLAY_START_X, DISPLAY_START_Y+TEMP, OLED_WIDTH, OLED_HEIGHT+(TEMP-OLED_HEIGHT/2), framebuffer);
        /*7.将偏移往后加PIC_SIZE/2*/
				cnt++;
				cnt=cnt%2; //cnt只取0和1
				TEMP=(cnt%2)?(OLED_HEIGHT/2):0;
				
        offset += PIC_SIZE/2;
        res = f_lseek(&fil, offset);

        if(FR_OK != res)
            return -3;
    }

    /*8.关闭文件描述符*/
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

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);  //初始化延时函数
	uart_init(115200);		//初始化串口波特率为115200
	usart2_init(115200);
	LED_Init();
 	LCD_Init();					//LCD初始化  	
	my_mem_init(SRAMIN);		//初始化内部内存池 
	my_mem_init(SRAMCCM);		//初始化CCM内存池
	RingBuff_Init();
	EXTIX_Init();
	BEEP =0;
	TIM3_Int_Init(10000-1,8400-1);//10Khz计数,1秒钟中断一次
 	BRUSH_COLOR=RED;//设置字体为红色    
 	while(SD_Init())//检测不到SD卡
	{

	}
 	exfuns_init();							//为fatfs相关变量申请内存				 
	p=fatbuf;
  res=f_mount(fs[0],"0:",1); 					//挂载SD卡 
	framebuffer=(u16 *)mymalloc(SRAMIN,PIC_SIZE/2);//由于一帧图像太大，故每次从SD卡读取半帧图像

//	res=f_open (&fil,"0:/badapple.bin", FA_WRITE|FA_CREATE_ALWAYS		);
//	res=f_open (&fil,"0:/true_test.bin", FA_WRITE|FA_CREATE_ALWAYS		);
    

//********************end***************************/
//	while(Flag_WriteFinsh==0)
//	{

//		if(ReceiveBuffer1_Status==1) //每接收4096字节数据就开始写入SD卡
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
//			printf("第%d次写入SD卡完成",number);
//		}
//		
//	}
//		f_close(&fil);

//		BEEP=1;
//		delay_ms(600);
//		BEEP=0;
		sd_show_picture_bin("0:/true_test.bin");
	 
	
	while(1);
}





