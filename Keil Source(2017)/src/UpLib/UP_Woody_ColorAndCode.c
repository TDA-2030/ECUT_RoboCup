#include "STM32Lib\\stm32f10x.h"
#include"UP_System.h"
#include "UP_USR232.h" 
#include "UP_UART.h"
#include "UP_Globle.h"
#include "UP_Woody_ColorAndCode.h"

/****************************************************
起停控制
*****************************************************/

/**************************************
函数名称;UP_Woody_StartUp
函数功能:启动woody的相应功能
被本函数调用的函数:UP_USR232_Putc?UP_UART5_Putc
调用本函数的函数：无
输入参数说明:u32 command:范围0-255(实际范围0-35,请查看启停命令控制字)
输出参数说明：无
函数返回值：无
其它说明：无
**************************************/
void UP_Woody_StartUp(u32 command){
	u32 sum=0x55+0xaa+0x06+0x02;
	UP_UART5_Putc(0x55);
	UP_UART5_Putc(0xaa);
	UP_UART5_Putc(0x06);
	UP_UART5_Putc(0x02);
	UP_UART5_Putc(command);
	sum+=command;
	UP_UART5_Putc(sum);
}
/******************************************************************
颜色识别模块 
******************************************************************/

/**************************************
函数名称；UP_Woody_ColorRecLetIssue
函数功能：颜色识别允许下发
被本函数调用的函数：UP_USR232_Putc或UP_UART5_Putc
调用本函数的函数：无
输入参数说明：无
输出参数说明：无
函数返回值：无
其它说明：无
**************************************/
void UP_Woody_ColorRecLetIssue(){
	char str[5]={0x55,0xAA,0x05,0x40,0x44};
	for(lab=0;lab<5;lab++){
		UP_UART5_Putc(str[lab]);
	}
	UP_delay_ms(DELAY_TIMES);
}
/**************************************
函数名称；UP_Woody_ColorRecLetIssue
函数功能：颜色识别允许下发
被本函数调用的函数：UP_USR232_Putc或UP_UART5_Putc
调用本函数的函数：无
输入参数说明：无
输出参数说明：无
函数返回值：无
其它说明：无
**************************************/
void UP_Woody_ColorRecForbidIssue(){
	char str[5]={0x55,0xAA,0x05,0x41,0x45};
	for(lab=0;lab<5;lab++){
		UP_UART5_Putc(str[lab]);
	}
	UP_delay_ms(DELAY_TIMES);
}
/**************************************
函数名称；UP_Woody_ColorRec_ID_Judge
函数功能：颜色识别返回的ID
被本函数调用的函数：UP_USR232_Putc或UP_UART5_Putc
调用本函数的函数：无
输入参数说明：无
输出参数说明：无
函数返回值：ID，十进制数值形式，范围0~255
其它说明：无
**************************************/
u16 UP_Woody_ColorRec_ID_Judge(void)
{
	ID=(UP_Woody_ReturnData[4]&0X0F)+((UP_Woody_ReturnData[4]&0XF0)>>4)*16;
	return ID;
}
/*************************************************
函数名称；UP_Woody_ColorRec_ID_Judge
函数功能：显示颜色识别返回的ID
被本函数调用的函数：无
调用本函数的函数：无
输入参数说明：无
输出参数说明：无
函数返回值：无
其它说明：无
*************************************************/
 void UP_Woody_Display_ColorID(void)
  {
	 printf("(Pixel)%lud\n",UP_Woody_ColorRec_ID_Judge());
	 UP_delay_ms(3000);
	 UP_LCD_ClearScreen();
	}
/******************************************************************
条码识别模块 
******************************************************************/

/**************************************
函数名称；UP_Woody_BarCodeRecLetIssue
函数功能：条码识别允许下发
被本函数调用的函数：UP_USR232_Putc或UP_UART5_Putc
调用本函数的函数：无
输入参数说明：无
输出参数说明：无
函数返回值：无
其它说明：无
**************************************/
void UP_Woody_BarCodeRecLetIssue(){
	char str[5]={0x55,0xAA,0x05,0x50,0x54};
	for(lab=0;lab<5;lab++){
		UP_UART5_Putc(str[lab]);
	}
	UP_delay_ms(DELAY_TIMES);
}
/**************************************
函数名称；UP_Woody_BarCodeRecForbidIssue
函数功能：条码识别允许下发
被本函数调用的函数：UP_USR232_Putc或UP_UART5_Putc
调用本函数的函数：无
输入参数说明：无
输出参数说明：无
函数返回值：无
其它说明：无
**************************************/
void UP_Woody_BarCodeRecForbidIssue(){
	char str[5]={0x55,0xAA,0x05,0x51,0x55};
	for(lab=0;lab<5;lab++){
		UP_UART5_Putc(str[lab]);
	}
	UP_delay_ms(DELAY_TIMES);
}
/**************************************
函数名称；UP_Woody_CodeRec_ID_Judge
函数功能：条码识别返回的ID
被本函数调用的函数：UP_USR232_Putc或UP_UART5_Putc
调用本函数的函数：无
输入参数说明：无
输出参数说明：无
函数返回值：ID，十进制数值形式，范围0~255
其它说明：无
**************************************/
u16 UP_Woody_BarCodeRec_ID_Judge(void)
{
	ID=(UP_Woody_ReturnData[4]&0X0F)+((UP_Woody_ReturnData[4]&0XF0)>>4)*16;
	return ID;
}
/*************************************************
函数名称；UP_Woody_Display_BarCodeID
函数功能：显示条码识别返回的ID
被本函数调用的函数：无
调用本函数的函数：无
输入参数说明：无
输出参数说明：无
函数返回值：无
其它说明：无
*************************************************/
 void UP_Woody_Display_BarCodeID(void)
  {
	 printf("(Pixel)%lud\n",UP_Woody_BarCodeRec_ID_Judge());
	 UP_delay_ms(3000);
	 UP_LCD_ClearScreen();
	}

/******************************************************************
二维码识别模块 
******************************************************************/

/**************************************
函数名称；UP_Woody_QrCodeRecLetIssue
函数功能：条码识别允许下发
被本函数调用的函数：UP_USR232_Putc或UP_UART5_Putc
调用本函数的函数：无
输入参数说明：无
输出参数说明：无
函数返回值：无
其它说明：无
**************************************/
void UP_Woody_QrCodeRecLetIssue(){
	char str[5]={0x55,0xAA,0x05,0x60,0x64};
	for(lab=0;lab<5;lab++){
		UP_UART5_Putc(str[lab]);
	}
	UP_delay_ms(DELAY_TIMES);
}
/**************************************
函数名称；UP_Woody_QrCodeRecForbidIssue
函数功能：条码识别允许下发
被本函数调用的函数：UP_USR232_Putc或UP_UART5_Putc
调用本函数的函数：无
输入参数说明：无
输出参数说明：无
函数返回值：无
其它说明：无
**************************************/
void UP_Woody_QrCodeRecForbidIssue(){
	char str[5]={0x55,0xAA,0x05,0x61,0x65};
	for(lab=0;lab<5;lab++){
		UP_UART5_Putc(str[lab]);
	}
	UP_delay_ms(DELAY_TIMES);
}
/**************************************
函数名称；UP_Woody_QrCodeRec_ID_Judge
函数功能：条码识别返回的ID
被本函数调用的函数：UP_USR232_Putc或UP_UART5_Putc
调用本函数的函数：无
输入参数说明：无
输出参数说明：无
函数返回值：ID，十进制数值形式，范围0~255
其它说明：无
**************************************/
u16 UP_Woody_QrCodeRec_ID_Judge(void)
{
	ID=(UP_Woody_ReturnData[4]&0X0F)+((UP_Woody_ReturnData[4]&0XF0)>>4)*16;
	return ID;
}
/**************************************
函数名称；UP_Woody_Display_QrCodeID
函数功能：显示二维码识别返回的ID
被本函数调用的函数：无
调用本函数的函数：无
输入参数说明：无
输出参数说明：无
函数返回值：无
其它说明：无
*************************************************/
 void UP_Woody_Display_QrCodeID(void)
  {
	 printf("(Pixel)%lud\n",UP_Woody_BarCodeRec_ID_Judge());
	 UP_delay_ms(3000);
	 UP_LCD_ClearScreen();
	}
