/******************************************************************/
//	Copyright (C), 2011-2012, 北京博创 
//  Author   	  	: 陈中元  
//  Reviser				: 乔潇楠
//  Update Date   : 2012/08/01
//  Version   	  : 1.3          
//  Description   : Transplant to v3.5 function library
/******************************************************************/ 

#ifndef SERVO_L
#define SERVO_L

//硬件初始化
extern void  UP_IOout_Init(void);																		//舵机相关初始化
extern void  UP_IOout_SetIO(u8 Channel, u8 Value);									//设置模拟舵机端口IO输出电平，通道0~7，值0或1
extern void  UP_IOout_SetALLIO(u8 Value);														//同时设置8路模拟舵机端口IO输出电平，值0x00~0xFF

#endif
