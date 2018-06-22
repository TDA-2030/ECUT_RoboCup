#include "STM32Lib\\stm32f10x.h"
#include "UP_Beer.h"
#include "UP_Timer.h"
#include "UP_IOout.h"
u32 g_Timer2Count= 0;		//计时器2的计数值

/*定义Timer中断入口函数*/

void TimerHadler2(u32 timerchannel)
{
	UP_IOout_SetALLIO(0);
	g_Timer2Count++;	
}
void UP_Beer_Delay(u32 nCount){
	u16 TIMCounter = nCount;
  TIM_Cmd(TIM2, ENABLE);
  TIM_SetCounter(TIM2, TIMCounter);
  while (TIMCounter>1)
  {
    TIMCounter = TIM_GetCounter(TIM2);
  }
  TIM_Cmd(TIM2, DISABLE);
}
//Rate 值为要设置的时间间隔，channel 为io端口号范围0-5，Value 高低电平设置，
void UP_Beer_Set(u8 Channel,u32 Rate,u8 Value){
	UP_IOout_SetIO(Channel,Value);//设置对应IO的输出
	UP_Beer_Delay(Rate);
	UP_IOout_SetIO(Channel,0);//设置对应IO的输出为0
	
	//UP_Timer_EnableIT(TIMER_CHANNEL2,Rate);//使能计时器2，计时时间rate
}