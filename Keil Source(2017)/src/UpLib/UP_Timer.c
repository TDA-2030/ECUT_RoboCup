/******************************************************************/
//	Copyright (C), 2011-2013, 北京博创 
//  Author   	  	: 陈中元  
//  Reviser				: 乔潇楠
//  Update Date   : 2012/08/01
//  Version   	  : 1.3          
//  Description   : Transplant to v3.5 function library
/******************************************************************/ 

#include "STM32Lib\\stm32f10x.h"
#include "UP_Timer.h"
#include "UP_Globle.h"

u32 g_UP_iTimerCount = 0;				//计时器总计数值
u32 g_UP_iTimerTime[4] = {0};			//计时时间，us为单位
u16 g_UP_TimerCCRVal_L16[4] = {0};		//Timer比较通道比较值低16位
u16 g_UP_TimerCCRVal_H16[4] = {0};		//Timer比较通道比较值高16位

u16 g_UP_TempCCRVal_L16[4] = {0};		//Timer比较通道比较值低16位
u16 g_UP_TempCCRVal_H16[4] = {0};		//Timer比较通道比较值高16位

//初始化相关参数
void UP_Timer_InitParameters(void)
{

}

/*************************************************
  函数名称: UP_Timer_Init  
  函数功能: 定时器相关初始化
  被本函数调用的函数: 无       
  调用本函数的函数: 无    
  输入参数说明: 无         
  输出参数说明: 无       
  函数返回值: 无
  其它说明:无        
*************************************************/
void UP_Timer_Init(void)
{
	//Timer3的4个输出比较做4路计时器
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	UP_Timer_InitParameters();

	if(!g_UP_bTimerIT[0] && !g_UP_bTimerIT[1] && !g_UP_bTimerIT[2] && !g_UP_bTimerIT[3])
		return;
	//使能定时器时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	//Timer3基本设置   
	TIM_DeInit(TIM3);
	TIM_TimeBaseStructure.TIM_Prescaler = 71;						//72分频，1us
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;		//向上计数
	TIM_TimeBaseStructure.TIM_Period = 0xffff;						//计数值65535
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x0;
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);

	//比较通道设置
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Inactive;       	//输出比较非主动模式
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;		//极性为正

	TIM_OCInitStructure.TIM_Pulse = g_UP_iTimerTime[0]>65535?65535:g_UP_iTimerTime[0]; 	//比较时间
	if(g_UP_bTimerIT[0]) 
		TIM_OC1Init(TIM3,&TIM_OCInitStructure);							//通道1				
	TIM_OCInitStructure.TIM_Pulse = g_UP_iTimerTime[1]>65535?65535:g_UP_iTimerTime[1]; 	//比较时间
	if(g_UP_bTimerIT[1]) 
		TIM_OC2Init(TIM3,&TIM_OCInitStructure); 						//通道2
	TIM_OCInitStructure.TIM_Pulse = g_UP_iTimerTime[2]>65535?65535:g_UP_iTimerTime[2]; 	//比较时间
	if(g_UP_bTimerIT[2]) 
		TIM_OC3Init(TIM3,&TIM_OCInitStructure); 						//通道3
	TIM_OCInitStructure.TIM_Pulse = g_UP_iTimerTime[3]>65535?65535:g_UP_iTimerTime[3]; 	//比较时间
	if(g_UP_bTimerIT[3]) 
		TIM_OC4Init(TIM3,&TIM_OCInitStructure); 						//通道4

	//预先清除所有中断位
	TIM_ClearITPendingBit(TIM3, TIM_IT_CC1 | TIM_IT_CC2 | TIM_IT_CC3 | TIM_IT_CC4 | TIM_IT_Update);

	//比较中断使能
	if(g_UP_bTimerIT[0]) 
		TIM_ITConfig(TIM3, TIM_IT_CC1, ENABLE);
	if(g_UP_bTimerIT[1]) 
		TIM_ITConfig(TIM3, TIM_IT_CC2, ENABLE);
	if(g_UP_bTimerIT[2]) 
		TIM_ITConfig(TIM3, TIM_IT_CC3, ENABLE);
	if(g_UP_bTimerIT[3]) 
		TIM_ITConfig(TIM3, TIM_IT_CC4, ENABLE);	  
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
	//TIM3计数使能
	TIM_Cmd(TIM3,ENABLE);
}

/*************************************************
  函数名称: UP_Timer_Pro  
  函数功能: 定时器3中断处理
  被本函数调用的函数: 无       
  调用本函数的函数: 无    
  输入参数说明: 无         
  输出参数说明: 无       
  函数返回值: 无
  其它说明:无        
*************************************************/
void UP_Timer_Pro(void)
{
	//定时器溢出中断
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
		g_UP_iTimerCount += 0xffff;
	}
	//比较通道1中断
	else if (TIM_GetITStatus(TIM3, TIM_IT_CC1) != RESET)
	{
	    TIM_ClearITPendingBit(TIM3, TIM_IT_CC1 );
		if(g_UP_TempCCRVal_H16[0] == 0 && g_UP_TempCCRVal_L16[0] == 0)
		{
			g_UP_TempCCRVal_L16[0] = g_UP_TimerCCRVal_L16[0];
			g_UP_TempCCRVal_H16[0] = g_UP_TimerCCRVal_H16[0];
			if(g_UP_Timer0ITAddress)
				(*((void(*)(u8))g_UP_Timer0ITAddress))(TIMER_CHANNEL0);
		}
		if(g_UP_TempCCRVal_H16[0] != 0)
		{
			TIM3->CCR1 += 65535;
			g_UP_TempCCRVal_H16[0]--;
		}
		else if(g_UP_TempCCRVal_L16[0] != 0)
		{
			TIM3->CCR1 += g_UP_TempCCRVal_L16[0];
			g_UP_TempCCRVal_L16[0] = 0;
		}
	}
	//比较通道2中断
	else if (TIM_GetITStatus(TIM3, TIM_IT_CC2) != RESET)
	{
	    TIM_ClearITPendingBit(TIM3, TIM_IT_CC2);
		if(g_UP_TempCCRVal_H16[1] == 0 && g_UP_TempCCRVal_L16[1] == 0)
		{
			g_UP_TempCCRVal_L16[1] = g_UP_TimerCCRVal_L16[1];
			g_UP_TempCCRVal_H16[1] = g_UP_TimerCCRVal_H16[1];
			if(g_UP_Timer1ITAddress)
				(*((void(*)(u8))g_UP_Timer1ITAddress))(TIMER_CHANNEL1);
		}
		if(g_UP_TempCCRVal_H16[1] != 0)
		{
			TIM3->CCR2 += 65535;
			g_UP_TempCCRVal_H16[1]--;
		}
		else if(g_UP_TempCCRVal_L16[1] != 0)
		{
			TIM3->CCR2 += g_UP_TempCCRVal_L16[1];
			g_UP_TempCCRVal_L16[1] = 0;
		}
	}
	//比较通道3中断
	else if (TIM_GetITStatus(TIM3, TIM_IT_CC3) != RESET)
	{
	    TIM_ClearITPendingBit(TIM3, TIM_IT_CC3);
		if(g_UP_TempCCRVal_H16[2] == 0 && g_UP_TempCCRVal_L16[2] == 0)
		{
			g_UP_TempCCRVal_L16[2] = g_UP_TimerCCRVal_L16[2];
			g_UP_TempCCRVal_H16[2] = g_UP_TimerCCRVal_H16[2];
			if(g_UP_Timer2ITAddress)
				(*((void(*)(u8))g_UP_Timer2ITAddress))(TIMER_CHANNEL2);
		}
		if(g_UP_TempCCRVal_H16[2] != 0)
		{
			TIM3->CCR3 += 65535;
			g_UP_TempCCRVal_H16[2]--;
		}
		else if(g_UP_TempCCRVal_L16[2] != 0)
		{
			TIM3->CCR3 += g_UP_TempCCRVal_L16[2];
			g_UP_TempCCRVal_L16[2] = 0;
		}
	}
	//比较通道4中断
	else if (TIM_GetITStatus(TIM3, TIM_IT_CC4) != RESET)
	{
	    TIM_ClearITPendingBit(TIM3, TIM_IT_CC4);
		if(g_UP_TempCCRVal_H16[3] == 0 && g_UP_TempCCRVal_L16[3] == 0)
		{
			g_UP_TempCCRVal_L16[3] = g_UP_TimerCCRVal_L16[3];
			g_UP_TempCCRVal_H16[3] = g_UP_TimerCCRVal_H16[3];
			if(g_UP_Timer3ITAddress)
				(*((void(*)(u8))g_UP_Timer3ITAddress))(TIMER_CHANNEL3);
		}
		if(g_UP_TempCCRVal_H16[3] != 0)
		{
			TIM3->CCR4 += 65535;
			g_UP_TempCCRVal_H16[3]--;
		}
		else if(g_UP_TempCCRVal_L16[3] != 0)
		{
			TIM3->CCR4 += g_UP_TempCCRVal_L16[3];
			g_UP_TempCCRVal_L16[3] = 0;
		}
	}
}

/*************************************************
  函数名称: UP_Timer_EnableIT
  函数功能: 使能计时器中断
  被本函数调用的函数: 无       
  调用本函数的函数: 无    
  输入参数说明: TimerChannel  Time_us     
  输出参数说明: 无       
  函数返回值: 无
  其它说明:无        
*************************************************/
void UP_Timer_EnableIT(u32 TimerChannel, u32 Time_us)
{
	if(TimerChannel > 3 || Time_us == 0)
		return;
	g_UP_bTimerIT[TimerChannel] = TRUE;
	g_UP_iTimerTime[TimerChannel] = Time_us;
	g_UP_TempCCRVal_L16[TimerChannel] = g_UP_TimerCCRVal_L16[TimerChannel] = Time_us;
	g_UP_TempCCRVal_H16[TimerChannel] = g_UP_TimerCCRVal_H16[TimerChannel] = Time_us>>16;
}

/*************************************************
  函数名称: UP_Timer_DisableIT
  函数功能: 禁能计时器中断
  被本函数调用的函数: 无       
  调用本函数的函数: 无    
  输入参数说明: TimerChannel     
  输出参数说明: 无       
  函数返回值: 无
  其它说明:无        
*************************************************/
void UP_Timer_DisableIT(u32 TimerChannel)
{
	if(TimerChannel > 3)
		return;
	g_UP_bTimerIT[TimerChannel] = FALSE;
	//比较中断禁能
	if(!g_UP_bTimerIT[0]) 
		TIM_ITConfig(TIM3, TIM_IT_CC1, DISABLE);
	if(!g_UP_bTimerIT[1]) 
		TIM_ITConfig(TIM3, TIM_IT_CC2, DISABLE);
	if(!g_UP_bTimerIT[2]) 
		TIM_ITConfig(TIM3, TIM_IT_CC3, DISABLE);
	if(!g_UP_bTimerIT[3]) 
		TIM_ITConfig(TIM3, TIM_IT_CC4, DISABLE);	
}

/*************************************************
  函数名称: UP_Timer_SetHadler
  函数功能: 设置定时器中断入口函数
  被本函数调用的函数: 无       
  调用本函数的函数: 无    
  输入参数说明: channel   ApplicationAddress    
  输出参数说明: 无       
  函数返回值: 无
  其它说明: 无        
*************************************************/
void UP_Timer_SetHadler(u32 channel,void (*ApplicationAddress)(u32))
{
	if(ApplicationAddress != 0)
	{
		//g_UP_TimerITAddress = (u32)ApplicationAddress;
		switch(channel)
		{
			case 0:
				g_UP_Timer0ITAddress = (u32)ApplicationAddress;
				break;
			case 1:
				g_UP_Timer1ITAddress = (u32)ApplicationAddress;
				break;		
			case 2:
				g_UP_Timer2ITAddress = (u32)ApplicationAddress;
				break;
			case 3:
				g_UP_Timer3ITAddress = (u32)ApplicationAddress;
				break;
			default:
				break;
		}
		
	}
}

/*************************************************
  函数名称: UP_Timer_GetCount
  函数功能: 获取计时器计数值（32位）
  被本函数调用的函数: 无       
  调用本函数的函数: 无    
  输入参数说明: ApplicationAddress    
  输出参数说明: 无       
  函数返回值: 无
  其它说明:无        
*************************************************/
u32 UP_Timer_GetCount(void)
{
	return (g_UP_iTimerCount+TIM3->CNT);
}

/*************************************************
  函数名称: UP_Timer_CleanCount
  函数功能: 清零计时器计数值
  被本函数调用的函数: 无       
  调用本函数的函数: 无    
  输入参数说明: 无 
  输出参数说明: 无       
  函数返回值: 无
  其它说明:无        
*************************************************/
void UP_Timer_CleanCount(void)
{
	g_UP_iTimerCount = 0;
	TIM3->CNT = 0;
}
														
