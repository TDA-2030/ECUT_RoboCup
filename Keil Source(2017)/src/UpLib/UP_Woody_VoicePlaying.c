#include"UP_System.h"
#include "UP_USR232.h" 
#include "UP_UART.h"
#include "UP_Globle.h"
#include "UP_Woody_VoicePlaying.h"
 /*************************************
 *语音播放
 **************************************/
 
 u32 w[2]={0x00,0x00};
 u32 data1;
/*************************************************
	函数名称: UP_Woody_PlayingMusic
  函数功能: 控制woody播放音乐
  被本函数调用的函数: void UP_USR232_Putc(unsigned char c);
  调用本函数的函数: 无    
  输入参数说明: u32 data1：范围0-255，播放对应歌曲id 0x00~0x31
  输出参数说明: 无       
  函数返回值: 无
  其它说明: 无  
*************************************************/
void UP_Woody_PlayingMusic(u32 data1)
{	 
	u32 sum=0x55+0xaa+0x07+0x20;
	w[0]=0x00;
	w[1]=data1;
	
	
	UP_UART5_Putc(0x55);
	UP_UART5_Putc(0xaa);
	UP_UART5_Putc(0x07);
	UP_UART5_Putc(0x20);
	UP_UART5_Putc(w[0]);
	UP_UART5_Putc(w[1]);
	sum+=data1;
	UP_UART5_Putc(sum);
	UP_delay_ms(DELAY_TIMES);
}


