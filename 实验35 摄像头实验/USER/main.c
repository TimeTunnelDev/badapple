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
//ALIENTEK 探索者STM32F407开发板 实验39
//FATFS 实验 -库函数版本
//技术支持：www.openedv.com
//淘宝店铺：http://eboard.taobao.com
//广州市星翼电子科技有限公司    
//作者：正点原子 @ALIENTEK 
 

/*******变量定义*****/
FIL fil;
FRESULT res;
UINT bww;
char buf[100];

int main(void)
{        
 	u32 total,free;
	u8 t=0;	
	u8 res=0;	

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);  //初始化延时函数
	uart_init(115200);		//初始化串口波特率为115200
	LED_Init();					//初始化LED 
	usmart_dev.init(84);		//初始化USMART
 	LCD_Init();					//LCD初始化  
 	KEY_Init();					//按键初始化 
	W25QXX_Init();				//初始化W25Q128
	my_mem_init(SRAMIN);		//初始化内部内存池 
	my_mem_init(SRAMCCM);		//初始化CCM内存池
	
 	BRUSH_COLOR=RED;//设置字体为红色    
 	while(SD_Init())//检测不到SD卡
	{

	}
 	exfuns_init();							//为fatfs相关变量申请内存				 
  	res=f_mount(fs[0],"0:",1); 					//挂载SD卡 
// 	f_mount(fs[1],"1:",1); 				//挂载FLASH.	
//	if(res==0X0D)//FLASH磁盘,FAT文件系统错误,重新格式化FLASH
//	{
//		LCD_DisplayString(1,1,16,"Flash Disk Formatting...");	//格式化FLASH
//		res=f_mkfs("0:",1,4096);//格式化FLASH,1,盘符;1,不需要引导区,8个扇区为1个簇
//		if(res==0)
//		{
//			f_setlabel((const TCHAR *)"1:ALIENTEK");	//设置Flash磁盘的名字为：ALIENTEK
//			LCD_DisplayString(1,22,16,"Flash Disk Format Finish");	//格式化完成
//		}else LCD_DisplayString(1,22,16,"Flash Disk Format Error ");	//格式化失败
//		delay_ms(1000);
//	}													    
//	while(exf_getfree("0",&total,&free))	//得到SD卡的总容量和剩余容量
//	{
//		LCD_ShowString(30,150,200,16,16,"SD Card Fatfs Error!");
//		delay_ms(200);
//		LCD_Fill(30,150,240,150+16,WHITE);	//清除显示			  
//		delay_ms(200);
//	}													  			    
// 	POINT_COLOR=BLUE;//设置字体为蓝色	   
//	LCD_ShowString(30,150,200,16,16,"FATFS OK!");	 
//	LCD_ShowString(30,170,200,16,16,"SD Total Size:     MB");	 
//	LCD_ShowString(30,190,200,16,16,"SD  Free Size:     MB"); 	    
// 	LCD_ShowNum(30+8*14,170,total>>10,5,16);				//显示SD卡总容量 MB
// 	LCD_ShowNum(30+8*14,190,free>>10,5,16);					//显示SD卡剩余容量 MB		


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





