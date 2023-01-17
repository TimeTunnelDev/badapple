#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "key.h"
#include "lcd.h"
#include "usmart.h"  
#include "usart2.h"  
#include "timer.h" 
#include "ov2640.h" 
#include "dcmi.h" 
#include "stmflash.h"
#include "w25qxx.h" 
#include "spi.h"
#include "stdio.h"
#include "sdio_sdcard.h"
#include "malloc.h"	 
#include "fattester.h"
#include "exfuns.h"

__align(32) u8 mem[35];
const u8 t[]={"W25q128 FLASH TEST"};
#define temp sizeof(t)
#define size ((temp%4)?(temp/4+1):(temp/4))


int main(void)
{
	u8 res=0;	

//	u8 * p;//此种定义指针未初始化
	u32 p[size];
	u8 q[temp];
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);  //初始化延时函数
	uart_init(115200);		//初始化串口波特率为115200
	usart2_init(42,115200);		//初始化串口2波特率为115200
	LED_Init();					//初始化LED 
 	LCD_Init();					//LCD初始化  
 	KEY_Init();					//按键初始化 
	usmart_dev.init(84);		//初始化USMART
	TIM3_Int_Init(10000-1,8400-1);//10Khz计数,1秒钟中断一次
	W25QXX_Init();
	BRUSH_COLOR=RED;      //设置画笔颜色为红色	
	LCD_Clear(WHITE);
	my_mem_init(0);	//内部SRAM管理初始化
	while(SD_Init()) //SD卡初始化
	{
		 LCD_DisplayString(1,1,24,"SD_Init_ERROOR");
	}
	while(exfuns_init())
	{
			LCD_DisplayString(1,1,24,"exfuns_init_ERROOR");
	}	//
	f_mkfs("0:",1,4096);

	res = mf_mount("SD_CARD:",1);//挂载文件系统fs[2]到SD卡
	if(res==0x0D)
	{
		LCD_DisplayString(1,66,24,"f_mkfs");

		f_mkfs("0:",1,4096);
	}
	delay_ms(1000);
	mf_open("0:/test.txt",FA_OPEN_ALWAYS|FA_WRITE);
	mf_write((u8 *)t,temp);
	mf_close();
	mf_open("0:/test.txt",FA_READ);
	mf_read(temp);
	LCD_DisplayString(1,1,16,fatbuf);
	mf_close();
	
//		switch(SDCardInfo.CardType)
//	{
//		case SDIO_STD_CAPACITY_SD_CARD_V1_1:	LCD_DisplayString(1,1,24,"SDSC V1.1");break;
//		case SDIO_STD_CAPACITY_SD_CARD_V2_0:	LCD_DisplayString(1,1,24,"SDSC V2.0");break;
//		case SDIO_HIGH_CAPACITY_SD_CARD:	LCD_DisplayString(1,1,24,"SDHC V2.0");break;
//		case SDIO_MULTIMEDIA_CARD:	LCD_DisplayString(1,1,24,"MMC Card");break;
//	}	
//	LCD_DisplayNum(1,22,(u32)SDCardInfo.CardCapacity>>20,sizeof((u32)SDCardInfo.CardCapacity>>20),24,0);//显示容量
//	LCD_DisplayNum(1,44,SDCardInfo.CardBlockSize,sizeof(SDCardInfo.CardBlockSize),24,0);  //显示块大小
//	LCD_DisplayNum(1,66,(u32)SDCardInfo.RCA,sizeof((u32)SDCardInfo.RCA),24,0); //卡相对地址
	
// 	STMFLASH_Write(ADDR_FLASH_SECTOR_3,(u32 *)t,size);
//	STMFLASH_Read(ADDR_FLASH_SECTOR_3,(u32 *)p,size);
//	W25Q128
//	W25QXX_Write((u8 *)t,0,temp);
//	W25QXX_Read(q,0,temp);


	while(1)
	{
//		sprintf(q,"%d",sizeof(mem));
		LCD_DisplayString(1,88,24,"eee");
	}

}
